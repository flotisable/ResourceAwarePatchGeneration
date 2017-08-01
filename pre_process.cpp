#include "ResourceAwarePatchGeneration.h"
#include <fstream>
#include <map>
#include <assert.h>


using namespace std;
extern "C"
{
//int    Cmd_CommandExecute( void * , char *  );
#include "base/cmd/cmd.h"
#include "misc/vec/vecPtr.h"
}


template <class T>
bool is_obj_in_vec ( T obj, vector<T> obj_vec)
{
	for (int i;i<obj_vec.size();i++)
	{
		if (obj_vec[i]==obj)
			return true;
	}
	return false;
}	



void ResourceAwarePatchGenerator:: pre_process()
{
    cout<<"[INFO] preprocessing ..."<<endl;
	//cout << gate_list.size() << endl;
	DP_reduce_base_function ();
	//cout << gate_list.size() << endl;

	convert_ntk_to_aig_with_base_func (false);
	//Abc_NtkMakeOnePo

	
	
    	
	
	
	
}

void ResourceAwarePatchGenerator::DP_reduce_base_function ()
{
	cout << "[INFO] DP reduce base function started" << endl;
	vector<Weight_gate*> new_weight_gate;

	Abc_Obj_t* iter_obj;
	int i;
	map<Abc_Obj_t*,int> DP_cost;
	Abc_NtkForEachPi(initial_F, iter_obj,i)			//add pi into cost array, assume all pi is in base function	
	{
		if (is_obj_in_vec(iter_obj,t_list))
		{
			DP_cost[iter_obj]=-10000;
			continue;
		}
		for (int j=0;j<gate_list.size();j++)
		{
			if (gate_list[j]->gate==iter_obj)
			{
				new_weight_gate.push_back(gate_list[j]);
				DP_cost[gate_list[j]->gate]=gate_list[j]->weight;
				break;
			}
				 
			if (j==gate_list.size()-1)
			{
				cout << Abc_ObjName(iter_obj) << endl;
				cout << "[WARNING] not all pi is in base function" << endl;
				
			}
			
		}
		
	}

	Vec_Ptr_t* DFS_ordered_gates;

	DFS_ordered_gates=Abc_NtkDfs2(initial_F);
	Abc_Obj_t* iter_dfs_obj;


	int count=0;
	int reduced_base_count=0;
	Vec_PtrForEachEntry(Abc_Obj_t*, DFS_ordered_gates,iter_dfs_obj,i)
	{
		int dp_weight=0;
		int j;
		Abc_ObjForEachFanin(iter_dfs_obj,iter_obj,j) 
		{
			dp_weight+=DP_cost[iter_obj];	
		}
		for (j=0;j<gate_list.size();j++)
		{
			if (gate_list[j]->gate==iter_dfs_obj)
			{
				count ++;
				if (gate_list[j]->weight>dp_weight)
				{
					reduced_base_count+=1;
				}
				else
				{
					new_weight_gate.push_back(gate_list[j]);
					dp_weight=gate_list[j]->weight;
				}
				break;
					
				
			}
		}
		DP_cost[iter_dfs_obj]=dp_weight;
	}
	cout << "[INFO] " << reduced_base_count << " base function reduced" << endl;
	gate_list=new_weight_gate;
}

void ResourceAwarePatchGenerator::convert_ntk_to_aig_with_base_func (bool delete_PO)
{
	cout<<"[INFO] add PO to base function and strash"<<endl;
	

	int initial_po_num=Abc_NtkPoNum(initial_F) ;
	initial_F_PO_num=initial_po_num;
	int i;
	//cout << initial_po_num << endl;
	Abc_Obj_t* iter_pi;
	Abc_Obj_t* iter_po;
	/*
	Abc_NtkForEachPo(initial_F,iter_po,i)
	{
		cout << Abc_ObjName(iter_po) <<endl;
	}
	*/
        for (i=0;i<gate_list.size();i++)
	{
		if (Abc_ObjIsPo(gate_list[i]->gate))
		{
			cout << "[Warning] base func is po" << endl;
		}
		Abc_Obj_t* base_po=Abc_NtkCreatePo(initial_F);
		Abc_ObjAddFanin (base_po,gate_list[i]->gate);
	}
	
	
	Abc_Ntk_t* strashed_F=Abc_NtkStrash(initial_F,0,1,0);
	for (i=0;i<gate_list.size();i++)
	{
		gate_list[i]->gate=Abc_NtkPo(strashed_F,i+initial_po_num);
	}
	
	//deal with target part
	for (i=0;i<t_list.size();i++)
	{
		t_list[i]=Abc_NtkPi(strashed_F,Abc_NtkPiNum(strashed_F)-t_list.size()+i);
	}
	
	//set pi, po copy and change t_pi_list,t_po_list to strashed_F (delete unused set?)
	Abc_NtkForEachPi(initial_F,iter_pi,i)
	{
		Abc_ObjSetCopy(iter_pi,Abc_NtkPi(strashed_F,i));
	}
	Abc_NtkForEachPo(initial_F,iter_po,i)
	{
		Abc_ObjSetCopy(iter_po,Abc_NtkPo(strashed_F,i));
	}


	for (i=0;i<t_list.size();++i)
	{
		set<Abc_Obj_t*>::iterator it;

		//cout << "pi " <<t_Pi_list[i].size() << endl;
		//cout << "po " <<t_Po_list[i].size() << endl;
		set<Abc_Obj_t*> new_t_pi_list;
		for (it=t_Pi_list[i].begin();it!=t_Pi_list[i].end();++it)
		{
			assert(Abc_ObjIsPi(*it));
			new_t_pi_list.insert(Abc_ObjCopy(*it));
		}


		set<Abc_Obj_t*> new_t_po_list;	
		for (it=t_Po_list[i].begin();it!=t_Po_list[i].end();++it)
		{
			assert(Abc_ObjIsPo(*it));
			new_t_po_list.insert(Abc_ObjCopy(*it));
		}

		t_Pi_list[i]=new_t_pi_list;
		t_Po_list[i]=new_t_po_list;
	}



 	//[Warning] Delete Po is dangerous
	//cout << Abc_NtkPoNum(strashed_F) << endl; ;
	if (delete_PO)
	{
        	for (i=0;i<gate_list.size();i++)
		{
			Abc_Obj_t* temp_po=gate_list[i]->gate;
			gate_list[i]->gate=Abc_ObjFanin0(gate_list[i]->gate);
			Abc_NtkDeleteObj(temp_po);
		
		}
	}
	//cout << Abc_NtkPoNum(strashed_F) << endl;

	//Abc_NtkDelete(initial_F);
	initial_F=strashed_F;

	//strash G part
    	Abc_Ntk_t* temp_Ntk;
	temp_Ntk = Abc_NtkStrash( initial_G, 0, 1, 0 );
    	//Abc_NtkDelete( initial_G );   	//delete?
    	initial_G = temp_Ntk;
    	temp_Ntk = NULL;


	
	//t_list
	//t_Pi_list
	//t_Po_list

}

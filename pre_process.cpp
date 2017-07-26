#include "ResourceAwarePatchGeneration.h"
#include <fstream>
#include <map>


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
	cout << gate_list.size() << endl;
	DP_reduce_base_function (initial_F, gate_list);
	cout << gate_list.size() << endl;

	Abc_Ntk_t* strashed_F = convert_ntk_to_aig_with_base_func (initial_F, gate_list);
	//Abc_NtkMakeOnePo

	
	
    	
	
	
	
}

void ResourceAwarePatchGenerator::DP_reduce_base_function (Abc_Ntk_t* ntk, vector <Weight_gate*>& base_func_list)
{
	cout << "[INFO] DP reduce base function started" << endl;
	vector<Weight_gate*> new_weight_gate;

	Abc_Obj_t* iter_obj;
	int i;
	map<Abc_Obj_t*,int> DP_cost;
	Abc_NtkForEachPi(ntk, iter_obj,i)			//add pi into cost array, assume all pi is in base function	
	{
		for (int j=0;j<base_func_list.size();j++)
		{
			if (base_func_list[j]->gate==iter_obj)
			{
				new_weight_gate.push_back(base_func_list[j]);
				DP_cost[base_func_list[j]->gate]=base_func_list[j]->weight;
				break;
			}
			if (j==base_func_list.size()-1)
				cout << "[WARNING] not all pi is in base function" << endl;
			
		}
		
	}

	Vec_Ptr_t* DFS_ordered_gates;

	DFS_ordered_gates=Abc_NtkDfs2(ntk);
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
		for (j=0;j<base_func_list.size();j++)
		{
			if (base_func_list[j]->gate==iter_dfs_obj)
			{
				count ++;
				if (base_func_list[j]->weight>dp_weight)
				{
					reduced_base_count+=1;
				}
				else
				{
					new_weight_gate.push_back(base_func_list[j]);
					dp_weight=base_func_list[j]->weight;
				}
				break;
					
				
			}
			if (j==base_func_list.size()-1)
				cout << "[WARNING] not all base func found in dfs" << endl;
		}
		DP_cost[iter_dfs_obj]=dp_weight;
	}
	cout << "[INFO] " << reduced_base_count << " base function reduced" << endl;
	base_func_list=new_weight_gate;
}

Abc_Ntk_t* ResourceAwarePatchGenerator::convert_ntk_to_aig_with_base_func (Abc_Ntk_t* initial_ntk, vector<Weight_gate*>& base_func_list)
{
	cout<<"[INFO] add PO to base function and strash"<<endl;
	

	int initial_po_num=Abc_NtkPoNum(initial_ntk) ;
	int i;
	cout << initial_po_num << endl;
        for (i=0;i<base_func_list.size();i++)
	{
		if (Abc_ObjIsPo(base_func_list[i]->gate))
			cout << "[Warning] base func is po" << endl;
		Abc_Obj_t* base_po=Abc_NtkCreatePo(initial_ntk);
		Abc_ObjAddFanin (base_po,base_func_list[i]->gate);
	}
	Abc_Ntk_t* strashed_F=Abc_NtkStrash(initial_ntk,0,1,0);
	for (i=0;i<base_func_list.size();i++)
	{
		base_func_list[i]->gate=Abc_NtkPo(strashed_F,i+initial_po_num);
	}
 	//[Warning] Delete Po is dangerous
	cout << Abc_NtkPoNum(strashed_F) << endl; ;
        for (i=0;i<base_func_list.size();i++)
	{
		Abc_Obj_t* temp_po=base_func_list[i]->gate;
		base_func_list[i]->gate=Abc_ObjFanin0(base_func_list[i]->gate);
		Abc_NtkDeleteObj(temp_po);
		
	}
	cout << Abc_NtkPoNum(strashed_F) << endl;
	return strashed_F;
	//t_list
	//t_Pi_list
	//t_Po_list

}

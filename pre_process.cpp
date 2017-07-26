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
    cout<<"[INFO] add PO to base function and strash"<<endl;
	

	Abc_Obj_t* iter_obj;
	int i;
	int initial_po_num=Abc_NtkPoNum(initial_F) ;
        for (i=0;i<gate_list.size();i++)
	{
		/*
		if (gate_list[i]->weight==10000)
			continue;
		*/
		Abc_Obj_t* base_po=Abc_NtkCreatePo(initial_F);
		Abc_ObjAddFanin (base_po,gate_list[i]->gate);
		//gate_list[i]->gate=base_po;
	}
	Abc_Ntk_t* strashed_F=Abc_NtkStrash(initial_F,0,1,0);
	for (i=0;i<gate_list.size();i++)
	{
		/*if (gate_list[i]->weight==10000)
		{
			continue;
		}
		*/
		gate_list[i]->gate=Abc_ObjFanin0(Abc_NtkPo(strashed_F,i+initial_po_num));
	}
	for (i=initial_po_num;i<Abc_NtkPoNum(strashed_F);i++)
	{
		Abc_NtkDeleteObjPo(Abc_NtkPo(strashed_F,i));
	}
	/*
        for (i=0;i<gate_list.size();i++)
	{
		Abc_Obj_t* temp_po=gate_list[i]->gate;
		gate_list[i]->gate=Abc_ObjFanin0(gate_list[i]->gate);
		Abc_NtkDeleteObjPo(temp_po);
		
		
		cout << gate_list[i]->name << endl;
		cout << gate_list[i]->gate << endl;
	}
	*/

	
	

	/*
	Abc_NtkForEachPi(initial_G, iter_obj,i)	
	{
		cout << Abc_ObjName(iter_obj) << endl;
	}
	*/
	//fstream weight_input;
	//weight_input.open( in_W_file.c_str(), ios::in );
	string base_function_name;
	int base_function_weight;
	/*
	while (weight_input>>base_function_name >> base_function_weight)	
	{
		 cout << endl;
		 gate_list.push_back(new Weight_gate());                                                                   
        	 Weight_gate* current_base=gate_list[gate_list.size()-1];                                                    
        	 current_base->name=base_function_name;                                                                   
        	 current_base->weight=base_function_weight;                                                              
        	 Abc_NtkForEachPi(initial_F,iter_obj,i)                                                                 
        	 {                                                                                                      
            		 if (string(Abc_ObjName(iter_obj))==base_function_name)                                        
                	 	cout << base_function_name <<endl;                                                           
			 else
				cout << "no" <<endl;
        	 }        
		
	}
	*/
	map<Abc_Obj_t*,int> DP_cost;
	//map<Abc_Obj_t*, vector<Abc_Obj_t*> > DP_optimal_base_node;
	Abc_NtkForEachPi(strashed_F, iter_obj,i)	
	{
		cout << iter_obj << endl;
		for (int j=0;j<gate_list.size();j++)
		{
			if (gate_list[j]->gate==iter_obj)
			{
				DP_cost[gate_list[j]->gate]=gate_list[j]->weight;
				break;
			}
			if (j==gate_list.size()-1)
				cout << "[WARNING] not all pi is in base function" << endl;
			
		}
		
	}
	Vec_Ptr_t* DFS_ordered_gates;
	DFS_ordered_gates=Abc_NtkDfs2(strashed_F);
	vector<Weight_gate*> new_weight_gate;
	Abc_Obj_t* iter_dfs_obj;


	cout << "[INFO] DP reduce base function started" << endl;
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
	//t_list
	//t_Pi_list
	//t_Po_list
	
	
	
}

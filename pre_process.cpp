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
	Abc_Obj_t* iter_obj;
	int i;
	Abc_NtkForEachObj(initial_F, iter_obj,i)	
	{
		cout << Abc_ObjName(iter_obj) << endl;
	}
	fstream weight_input;
	weight_input.open( in_W_file.c_str(), ios::in );
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
	Abc_NtkForEachPi(initial_F, iter_obj,i)	
	{
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
	DFS_ordered_gates=Abc_NtkDfs2(initial_F);
	vector<Weight_gate*> new_weight_gate;
	Abc_Obj_t* iter_dfs_obj;
	Vec_PtrForEachEntry(Abc_Obj_t*, DFS_ordered_gates,iter_dfs_obj,i)
	{
		bool is_weight_gate=false;
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
					break;
				else
				{
					new_weight_gate.push_back(gate_list[j]);
					dp_weight=gate_list[j]->weight;
				}
					
				
			}
		}
		DP_cost[iter_dfs_obj]=dp_weight;
	}
	gate_list=new_weight_gate;
	
	
	
}

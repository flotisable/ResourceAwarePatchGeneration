#include "ResourceAwarePatchGeneration.h"
#include <fstream>


using namespace std;
extern "C"
{
//int    Cmd_CommandExecute( void * , char *  );
#include "base/cmd/cmd.h"
}


void ResourceAwarePatchGenerator:: pre_process(char* W_file)
{
	Abc_Obj_t* iter_obj;
	int i;
	Abc_NtkForEachObj(initial_G, iter_obj,i)	
	{
		cout << Abc_ObjName(iter_obj) << endl;
	}
	fstream weight_input;
	weight_input.open( W_file, ios::in );
	string base_function_name;
	int base_function_weight;
	while (weight_input>>base_function_name >> base_function_weight)	
	{
		 gate_list.push_back(new Weight_gate());                                                                   
        	 Weight_gate* current_base=gate_list[gate_list.size()];                                                    
        	 current_base->name=base_function_name;                                                                   
        	 current_base->weight=base_function_weight;                                                              
        	 Abc_NtkForEachPi(initial_F,iter_obj,i)                                                                 
        	 {                                                                                                      
            		 if (Abc_ObjName(iter_obj)==base_function_name.c_str())                                        
                	 	cout << base_function_name <<endl;                                                           
        	 }        
		
	}

}

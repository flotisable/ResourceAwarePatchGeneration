#include "ResourceAwarePatchGeneration.h"
#include <fstream>


using namespace std;

<<<<<<< HEAD
extern "C"
{
int    Cmd_CommandExecute( void * pAbc, char * sCommand );
}


void ResourceAwarePatchGenerator:: pre_process(W_file)
{
	Abc_Obj_t* iter_obj;
	int i;
	Abc_NtkForEachObj(initial_G, iter_obj,i)	
	{
		cout << Abc_ObjName(iter_obj) << endl;
	}
	fstream weight_input;
	weight_input.open( W_file.c_str(), ios::in );
	string base_function_name;
	int base_function_weight;
	while (weight_input>>base_function_name >> base_function_weight)	
	{
		cout << base_function_name << " " << base_function_weight<< endl;
	}

}

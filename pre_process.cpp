#include "ResourceAwarePatchGeneration.h"


using namespace std;

extern "C"
{
int    Cmd_CommandExecute( void * pAbc, char * sCommand );
}


void ResourceAwarePatchGenerator:: pre_process()
{
	Abc_Obj_t* iter_obj;
	int i;
	Abc_NtkForEachObj(initial_G, iter_obj,i)	
	{
		cout << Abc_ObjName(iter_obj);
	}

}

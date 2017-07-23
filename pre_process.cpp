#include "ResourceAwarePatchGeneration.h"


using namespace std;

void ResourceAwarePatchGenerator:: pre_process()
{
    cout<<"[INFO] preprocessing ..."<<endl;
	
	Abc_Obj_t* iter_obj;
    int i;
    Abc_NtkForEachObj(initial_G, iter_obj,i)    
    {
        //cout << Abc_ObjName(iter_obj)<<endl;
    }

}

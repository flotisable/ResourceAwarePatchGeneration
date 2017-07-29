#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::replace_t_with_PI()
{
	cout<<"[INFO] replace_t_with_PI..."<<endl;

	for( int i = 0; i < t_list.size() ; ++i ){
		Abc_Obj_t* new_t = Abc_NtkCreatePi( initial_F );
		Abc_ObjReplace( t_list[i], new_t );//by abcFanio.c
		t_list[i] = new_t;
	}

}
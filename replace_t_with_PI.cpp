#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::replace_t_with_PI()
{
	cout<<"[INFO] replace_t_with_PI..."<<endl;

	for( int i = 0; i < t_list.size() ; ++i ){
		Abc_Obj_t* new_t = Abc_NtkCreatePi( initial_F );

		//set new object to t name
		Abc_ObjAssignName( new_t, Abc_ObjName( t_list[i] ), NULL );

		//replace new obj to t
		Abc_ObjReplace( t_list[i], new_t );//by abcFanio.c
		t_list[i] = new_t;

	}
	//Abc_Ntk_t* temp_F = Abc_NtkToNetlist( initial_F );
	//Abc_NtkToAig( temp_F );
	//Io_WriteVerilog( temp_F, "F_replace_t_with_PI.v" );
}
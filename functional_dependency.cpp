#include "ResourceAwarePatchGeneration.h"

#include <iostream>

void ResourceAwarePatchGenerator::functional_dependency()
{
  vector<Abc_Obj_t*>  baseFunctions;

  // setup base functions and target function
  cout << "po num " << Abc_NtkPoNum( target_function ) << "\n";
  for( int i = 0 ; i < gate_list.size() ; ++i )
    if( best_used_base[i] )
    {
      baseFunctions.push_back( gate_list[i]->gate );
      Abc_ObjAssignName( gate_list[i]->gate, const_cast<char*>( gate_list[i]->name.c_str() ), NULL );
    }
  Abc_ObjAssignName( Abc_NtkPo( target_function, 0 ), const_cast<char*>( "t_0" ), NULL );
  // end setup base functions and target function

  interpolation( target_function, Abc_NtkPo( target_function, 0 ), baseFunctions );
}

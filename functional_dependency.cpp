#include "ResourceAwarePatchGeneration.h"

#include <iostream>

void ResourceAwarePatchGenerator::functional_dependency()
{
  vector<Abc_Obj_t*>  baseFunctions;
  Abc_Obj_t           *po;
  int                 i;

  cout << "po num " << Abc_NtkPoNum( target_function ) << "\n";
  Abc_NtkForEachPo( target_function, po, i )
  {
    if( i == 0 ) continue;
    baseFunctions.push_back( po );
    cout << "po i" << i << "\n";
  }

  interpolation( target_function, Abc_NtkPo( target_function, 0 ), baseFunctions );
}

#include <iostream>
#include "ResourceAwarePatchGeneration.h"

using namespace std;

int main( int argc, char *argv[] )
{
  if( argc != 6 )
  {
    cerr << "wrong arguments number!\n";
    cerr << "arguments format:\n";
    cerr << "rpgen <F.v> <G.v> <weight.txt> <patch.v> <out.v>";
    return 1;
  }

  ResourceAwarePatchGenerator solver( argv[1], argv[2], argv[3], argv[4], argv[5] );
  
  solver.read_file();
  solver.delete_unused_PO();
  solver.read_weight();
  solver.pre_process();
  
  solver.construct_DLN();
  solver.functional_dependency();
  solver.write_patch();
  
  return 0;
}

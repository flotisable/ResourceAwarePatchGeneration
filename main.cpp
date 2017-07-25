#include <iostream>
#include "ResourceAwarePatchGeneration.h"

using namespace std;

int main( int argc, char *argv[] )
{
  if( argc != 6 )
  {
    cerr << "wrong arguments number!\n";
    cerr << "arguments format:\n";
    cerr << "rpgen <F.v> <G.v> <weight.txt> <patch.v> <out.v>\n";
    return 1;
  }

  string FFileName        = argv[1];  
  string GFileName        = argv[2];  
  string weightFileName   = argv[3];  
  string patchFileName    = argv[4];  
  string patchedFileName  = argv[5];  

  ResourceAwarePatchGenerator solver( FFileName, GFileName, weightFileName);
  
  solver.read_file();
  solver.delete_unused_PO();
  solver.read_weight();
  solver.pre_process();
  
  solver.construct_DLN();
  solver.functional_dependency();
  solver.write_patch( patchFileName, patchedFileName );
  
  return 0;
}

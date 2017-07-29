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
  solver.read_weight();
  solver.replace_t_with_PI();
  //solver.convert_ntk_to_aig_with_base_func();
  solver.DP_reduce_base_function();
  solver.delete_unused_PO();
  solver.convert_ntk_to_aig_with_base_func(false);
  //solver.construct_t(true);
  //solver.pre_process();
  
  solver.construct_DLN();
  solver.functional_dependency();
  //solver.write_patch( patchFileName, patchedFileName );
  
  return 0;
}

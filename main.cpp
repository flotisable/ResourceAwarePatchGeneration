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

<<<<<<< HEAD
  ResourceAwarePatchGenerator solver( argv[1], argv[2], argv[3], argv[4], argv[5] );
  cout<<"start to read_file"<<endl;
=======
  string FFileName        = argv[1];  
  string GFileName        = argv[2];  
  string weightFileName   = argv[3];  
  string patchFileName    = argv[4];  
  string patchedFileName  = argv[5];  

  ResourceAwarePatchGenerator solver( FFileName, GFileName, weightFileName);
  
>>>>>>> 2c9ba856bc41976b3fda8534b6ae66ebb46b9f4f
  solver.read_file();
<<<<<<< HEAD
  cout<<"start to delete_unused_PO"<<endl;
  solver.delete_unused_PO();
  cout<<"start to read_weight"<<endl;
  solver.read_weight();
  
  solver.pre_process();
=======
  solver.read_weight();
  solver.replace_t_with_PI();
  //solver.convert_ntk_to_aig_with_base_func();
  solver.DP_reduce_base_function();
  solver.delete_unused_PO();
  solver.convert_ntk_to_aig_with_base_func();
  //solver.pre_process();
>>>>>>> d377efb33693893b3b06bf920f0871dc4a5606a5
  
  solver.construct_DLN();
  solver.functional_dependency();
  //solver.write_patch( patchFileName, patchedFileName );
  
  return 0;
}

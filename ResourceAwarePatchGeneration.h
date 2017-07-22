#include <iostream>
#include <string>
#include <vector>
#include <set>

extern "C"
{
#include "base/main/mainInt.h"
#include "base/abc/abc.h"
}

using namespace std;

void trav_Po_add_to_set( Abc_Obj_t * pNode, set<Abc_Obj_t*> pSet );

class ResourceAwarePatchGenerator
{
  public:

      ResourceAwarePatchGenerator(string F_file, string G_file, string W_file, string o_p_file, string o_F_file);
      ~ResourceAwarePatchGenerator();

      void read_file();    // Input F_file, G_file, W_file, read to initial_F, initial_G, gate_list 
      void delete_unused_PO();             // direct delete on initial F
      void pre_process();                  // preprocessing on initial F
      void construct_t () {}                   // contrutct base on initial F
      void construct_DLN () {}          //transform initial circuit to DLN circuit 
      void transform_to_CNF () {}       //transform DLN circuit to CNF
      void sat_solve () {}          //solve CNF by sat
      void interpolation () {}      //construct t's circuit
      
      void functional_dependency() {}   //inculde above three step 
      void write_patch() {}         //write the result to file as the competition format
    
  private:

      Abc_Frame_t* pAbc;

	  class Weight_gate			//save weight and PO's address
	  {
	      Abc_Obj_t* gate;
	      int        weight; 
	  };

	  string out_Patch_file;
	  string out_F_file;
      string in_F_file;

	  Abc_Ntk_t*  initial_F;
	  Abc_Ntk_t*  initial_G;
	  vector <Weight_gate*> gate_list;
	  Abc_Ntk_t*  target_function;

	  Abc_Ntk_t*  DLN_circuit;
	  
      //??? CNF_formula; 
      //??? sat_result;
      //??? interpolation_result;
};

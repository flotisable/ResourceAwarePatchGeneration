#include <iostream>
#include <string>
#include <vector>
#include <set>

#define WEIGHT_MAX 10000

extern "C"
{
#include "base/main/mainInt.h"
#include "base/abc/abc.h"
}

using namespace std;

void trav_Po_add_to_set( Abc_Obj_t * pNode, set<Abc_Obj_t*> pSet );

class Weight_gate                     //save weight and PO's address
{
      public:
              Abc_Obj_t* gate;
              string name;
              int        weight;
};

class ResourceAwarePatchGenerator
{
  public:

      ResourceAwarePatchGenerator(string F_file, string G_file, string W_file);
      ~ResourceAwarePatchGenerator();

      void read_file();    // Input F_file, G_file, W_file, read to initial_F, initial_G, gate_list 
      void replace_t_with_PI() {}		//replace t with PI in initial F
      void delete_unused_PO();             // direct delete on initial F
      void pre_process();                  // preprocessing on initial F
      void read_weight();
      void construct_t (bool);                // contrutct base on initial F
      void construct_DLN () {}          //transform initial circuit to DLN circuit 
      void transform_to_CNF () {}       //transform DLN circuit to CNF
      void sat_solve () {}          //solve CNF by sat
      void interpolation ();       //construct t's circuit
      
      void functional_dependency() {}   //inculde above three step 
      void write_patch( const string patchedFileName, const string patchFileName );          //write the result to file as the competition format
    
      //pre_process sub-function
      
	void DP_reduce_base_function ();
	void convert_ntk_to_aig_with_base_func (bool);
  private:
      Abc_Frame_t* pAbc;


      string in_F_file;
      string in_G_file;
      string in_W_file;

      Abc_Ntk_t*  initial_F;
      int initial_F_PO_num;
      Abc_Ntk_t*  initial_G;
      vector <Weight_gate*> gate_list;
      Abc_Ntk_t*  target_function;

      Abc_Ntk_t*  DLN_circuit;
      
      //??? CNF_formula; 
      //??? sat_result;
      //??? interpolation_result;
};

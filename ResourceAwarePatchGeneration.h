#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#define WEIGHT_MAX 10000

extern "C"
{
#include "base/main/mainInt.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"


}

using namespace std;

void trav_Po_add_to_set( Abc_Obj_t * pNode, set<Abc_Obj_t*> pSet );

class Weight_gate                     //save weight and PO's address
{
      public:
        Weight_gate(Abc_Obj_t* pGate = NULL, string pName = string(), int pWeight = 0)
            :gate(pGate), name(pName), weight(pWeight){}
        Weight_gate(){}
        ~Weight_gate(){}
        Abc_Obj_t* gate;
        string name;
        int        weight;
};

class ResourceAwarePatchGenerator
{
  friend class TestResourceAwarePatchGeneration;

  public:

      ResourceAwarePatchGenerator(string F_file, string G_file, string W_file);
      ~ResourceAwarePatchGenerator(); 

      void read_file();    // Input F_file, G_file, W_file, read to initial_F, initial_G, gate_list 
      void replace_t_with_PI();		//replace t with PI in initial F
      void traverse_t_PI_and_PO();
      void delete_unused_PO();             // direct delete on initial F
      void pre_process();                  // preprocessing on initial F
      void read_weight();
      void construct_t (bool);                // contrutct base on initial F
      void construct_DLN () {}          //transform initial circuit to DLN circuit 
      void transform_to_CNF () {}       //transform DLN circuit to CNF
      void sat_solve ();         //solve CNF by sat
      void interpolation ( Abc_Ntk_t *dln, Abc_Obj_t *targetPo, const vector<Abc_Obj_t*> &baseFunctions );       //construct t's circuit
      
      void functional_dependency() {}   //inculde above three step 
      void write_patch( const string patchedFileName, const string patchFileName );          //write the result to file as the competition format

    //sub-function
      //construct pi and po list
      void recursive_trav_Po_add_to_set( Abc_Obj_t*, set<Abc_Obj_t*>& );
      void recursive_trav_Pi_add_to_set( Abc_Obj_t*, set<Abc_Obj_t*>& );
      //pre_process 
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

      vector<Abc_Obj_t*> t_list;
      vector< set<Abc_Obj_t*> > t_Pi_list, t_Po_list;

      Abc_Ntk_t*  target_function;

      Abc_Ntk_t*  DLN_circuit; 
      
      map< Abc_Obj_t*, set<Abc_Obj_t*> > Po_fanin_Pi;

      set<Abc_Obj_t*> depend_to_target_Po, depend_to_target_Pi;
      //??? CNF_formula; 
      //??? sat_result;
      Abc_Ntk_t *interpolant;
};

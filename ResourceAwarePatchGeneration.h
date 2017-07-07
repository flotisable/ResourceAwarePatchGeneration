#include <iostream>
#include "abc.h"
#include <string>
#include <vector>

using namespace std;


class ResourceAwarePatchGenerator
{
	ResourceAwarePatchGenerator(String F_file, String G_file, String W_file, String o_p_file, String o_F_file) {read_File(F_file,G_file,W_file);} 
	void read_file(String ,String , String);   // Input F_file, G_file, W_file, read to initial_F, initial_G, gate_list 
	void construct_DLN ();			//transform initial circuit to DLN circuit 
	void transform_to_CNF ();		//transform DLN circuit to CNF
	void sat_solve ();			//solve CNF by sat
	void interpolation ();		//construct t's circuit
	
	void functional_dependency(); 	//inculde above three step 
	void write_patch (); 		//write the result to file as the competition format
	
	
		
	



	String out_Patch_file;
	String out_F_file;
	Abc_Ntk_t*  initial_F;
	Abc_Ntk_t*  initial_G;
	class Weight_gate
	{
		Abc_Obj_t* gate;
		int        weight; 
	}
	vector <Weight_gate*> gate_list;
	Abc_Ntk_t*  DLN_circuit;
	//???	CNF_formula; 
	//???	sat_result;
	//???	interpolation_result;
	
	
	
	
	







}





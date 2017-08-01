#include <ResourceAwarePatchGeneration.h>
#include <iostream>
#include "SatAndInterpolation/InterpolationEngine.h"
#include <time.h>

using namespace std;



void ResourceAwarePatchGenerator::simulated_annealing ()
{
	cout << "[INFO] CNF construct started " << endl;
	cout << clock() << endl;
	
	NtkToCnfConverter* cnf_converter=new NtkToCnfConverter();	
	vector<Abc_Obj_t*> base;
	for (int i=0;i<gate_list.size();i++)
	{
		base.push_back(gate_list[i]->gate);
	}
	cnf_converter->setBaseFunctions(base);
	cnf_converter->setTargetFunction(Abc_NtkPo(target_function,0));
	cnf_converter->setCircuit(target_function);
	
	cnf_converter->convert();

	sat_solver* solver=sat_solver_new();
	sat_solver_setnvars   ( solver, cnf_converter->cnfOn()->nVars + cnf_converter->cnfOff()->nVars+gate_list.size() );
	addClause( solver, cnf_converter->cnfOn() );
	int lits[3];
	lits[0] = toLitCond( cnf_converter->literalsOn()[0], 0 );       
     	sat_solver_addclause( solver, lits, lits + 1 );

	lits[0] = toLitCond( cnf_converter->literalsOff()[0], 0 );
  	addClause( solver, cnf_converter->cnfOff() );
  	sat_solver_addclause( solver, lits, lits + 1 );
	
	int on_off_total_var_num=cnf_converter->cnfOn()->nVars+cnf_converter->cnfOff()->nVars;


	for( int i = 0 ; i < gate_list.size() ; ++i )
  	{
     		lits[0] = toLitCond( cnf_converter->literalsOn ()[i+1], 0 );
     		lits[1] = toLitCond( cnf_converter->literalsOff()[i+1], 1 );
		lits[2]=  toLitCond( on_off_total_var_num+i,1);

     		if( !sat_solver_addclause( solver, lits, lits + 3 ) )
       			std::cout << "clause add error!\n";

     		lits[0] = toLitCond( cnf_converter->literalsOn ()[i+1], 1 );
     		lits[1] = toLitCond( cnf_converter->literalsOff()[i+1], 0 );
		lits[2]=  toLitCond( on_off_total_var_num+i,1);

     		if( !sat_solver_addclause( solver, lits, lits + 3) )
       			std::cout << "clause add error!\n";
	}
	cout << "[INFO] CNF construct finished, SA started" << endl;

	vector<bool> current_solution(gate_list.size());
	for (int i=0;i<current_solution.size();++i)
	{
		current_solution[i]=true;
	}
	
	best_used_base=current_solution;

	
	
} 

bool sat_solve(void* solver,vector<bool>& base_selection , vector<Weight_gate*>& gate_list,int on_off_total_var_num)
{




	int count=0;
	for (int i=0;i<base_selection.size();++i)
	{
		if(base_selection[i]==true)
			count++; 
	}
	int base_alpha[count];
	count=0;
	for (int i=0;i<gate_list.size();++i)
	{
		if(base_selection[i]==true)
		{
			base_alpha[count]=toLitCond(on_off_total_var_num+i,0);
			count++;
		}
	}
	int status=sat_solver_solve((sat_solver*) solver,base_alpha,base_alpha+count,10000,0,0,0);
 	return status;	
	

	
	
	
	

}

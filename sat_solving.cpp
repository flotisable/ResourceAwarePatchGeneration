#include <ResourceAwarePatchGeneration.h>
#include <iostream>
#include "SatAndInterpolation/InterpolationEngine.h"

using namespace std;


void ResourceAwarePatchGenerator::sat_solve()
{
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

     		if( !sat_solver_addclause( solver, lits, lits + 2 ) )
       			std::cout << "clause add error!\n";

     		lits[0] = toLitCond( cnf_converter->literalsOn ()[i+1], 1 );
     		lits[1] = toLitCond( cnf_converter->literalsOff()[i+1], 0 );
		lits[2]=  toLitCond( on_off_total_var_num+i,1);

     		if( !sat_solver_addclause( solver, lits, lits + 2) )
       			std::cout << "clause add error!\n";
	}





	int base_alpha[gate_list.size()];
	for (int i=0;i<gate_list.size();i++)
	{
		base_alpha[i]=toLitCond(on_off_total_var_num+i,0);
	}
	int status=sat_solver_solve((sat_solver*) solver,base_alpha,base_alpha+gate_list.size(),10000,0,0,0);
	cout << status << endl;
	
	

	
	
	
	

}

#include <ResourceAwarePatchGeneration.h>
#include <iostream>
#include "SatAndInterpolation/InterpolationEngine.h"
#include <time.h>
#include <cstdlib>

using namespace std;


int find_neighborhood(const vector<Weight_gate*>& ,vector<bool>& , int& ,int );
int sat_solve(void* ,vector<bool>&  ,const int& );

void ResourceAwarePatchGenerator::simulated_annealing ()
{
	cout << "[INFO] CNF construct started " << endl;
	
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
	int current_weight=0;
        int best_weight;
	for (int i=0;i<current_solution.size();++i)
	{
		current_solution[i]=true;
		current_weight+=gate_list[i]->weight;
	}
	
	best_used_base=current_solution;
	
	best_weight=current_weight;
	cout << "initial_weight:" << best_weight << endl;
	float current_temp=100000;
	float temp_decrease_ratio=0.9; 
	int sec=600;
	cout << "[INFO] Simulated Annealing..  Timeout: "  << sec << " secs." <<endl;
	while ( float(clock())/CLOCKS_PER_SEC <sec)
	{
		for (int i=0;i<500;++i)
		{	
			int id=find_neighborhood(gate_list,current_solution,current_weight,current_temp);	
			if (sat_solve(solver,current_solution,on_off_total_var_num)==-1)
			{
				if (current_weight<best_weight)
				{
					best_used_base=current_solution;
					best_weight=current_weight;
				}	
			}
			else
			{
				if (!current_solution[id])
				{
					current_weight+=gate_list[id]->weight;
				}
				else
				{
					current_weight-=gate_list[id]->weight;
				}
				current_solution[id]=(!current_solution[id]);
			}
		}
		current_temp*=temp_decrease_ratio;
	}
	cout << "best_weight:" << best_weight << endl;
	

	
	
} 

int sat_solve(void* solver,vector<bool>& base_selection ,const int& on_off_total_var_num)
{




	int base_alpha[base_selection.size()];
	for (int i=0;i<base_selection.size();++i)
	{
		if(base_selection[i]==1)
		{
			base_alpha[i]=toLitCond(on_off_total_var_num+i,0);
			continue;
		}
		base_alpha[i]=toLitCond(on_off_total_var_num+i,1);
	}
	int status=sat_solver_solve((sat_solver*) solver,base_alpha,base_alpha+base_selection.size(),10000,0,0,0);
 	return status;	
	

	
	
	
	

}

int find_neighborhood(const vector<Weight_gate*>& gate_list,vector<bool>& current_solution, int& current_weight,int current_temp)
{
	int id;	
	float prob=0.2;
	while (1)
	{
		id=rand()%gate_list.size();
		if (current_solution[id])
		{
			current_solution[id]=0;	
			current_weight-=gate_list[id]->weight;
			break;
		}
		else if (rand()%RAND_MAX<prob)
		{
			current_solution[id]=1;	
			current_weight+=gate_list[id]->weight;
			break;
		}
	}
	return id;
	
} 

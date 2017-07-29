#include "InterpolationEngine.h"

#include <iostream>

extern "C"
{
#include "sat/bsat/satStore.h"
}

InterpolationEngine::InterpolationEngine()
{
  dln             = NULL;
  targetFunction  = NULL;
  mInterpolant    = NULL;
  satSolver       = NULL;
}

void InterpolationEngine::circuitToCnf()
{
  if( !dln || !targetFunction || baseFunctions.empty() ) return; // precondition

  converter.setCircuit       ( dln             );
  converter.setTargetFunction( targetFunction  );
  converter.setBaseFunctions ( baseFunctions   );

  converter.convert();
}

void InterpolationEngine::addClauseA()
{
  if( !converter.cnfOn() ) return; // precondition
  std::cout << "add clause A\n";

  // setup sat solver
  satSolver = sat_solver_new();

  sat_solver_store_alloc( satSolver );
  sat_solver_setnvars   ( satSolver, converter.cnfOn()->nVars + converter.cnfOff()->nVars );
  // end setup sat solver

  // add clause A
  addClause( satSolver, converter.cnfOn() );
  sat_solver_store_mark_clauses_a( satSolver );
  // end add clause A
}

void InterpolationEngine::addClauseB()
{
  if( !converter.cnfOff() || !satSolver ) return; // precondition
  std::cout << "add Clause B\n";

  int lits[3];

  // add clause B
  addClause( satSolver, converter.cnfOff() );
  // end add clause B

  // add common variables clause
  for( int i = 1 ; i < baseFunctions.size() ; ++i )
  {
     lits[0] = toLitCond( converter.literalsOn ()[i], 0 );
     lits[1] = toLitCond( converter.literalsOff()[i], 1 );

     if( !sat_solver_addclause( satSolver, lits, lits + 2 ) )
       std::cout << "clause add error!\n";

     lits[0] = toLitCond( converter.literalsOn ()[i], 1 );
     lits[1] = toLitCond( converter.literalsOff()[i], 0 );

     if ( !sat_solver_addclause( satSolver, lits, lits + 2 ) )
       std::cout << "clause add error!\n";
  }
  // end add common variables clause

  sat_solver_store_mark_roots( satSolver );
}

void InterpolationEngine::interpolation()
{
  if( !satSolver ) return; // precondition
  std::cout << "interpolation\n";

  lit         unitAssumption[3];
  Sto_Man_t   *proof;
  Inta_Man_t  *interMan;
  Vec_Int_t   *commonVariables;

  unitAssumption[0] = toLitCond( converter.literalsOn ()[0], 0 );
  unitAssumption[1] = toLitCond( converter.literalsOff()[0], 0 );

  if( sat_solver_solve( satSolver, unitAssumption, unitAssumption + 2, 0, 0, 0, 0  ) != l_False ) 
  {
    std::cout << "sat\n";
    return;
  };
  std::cout << "unsat\n";

  proof           = static_cast<Sto_Man_t*>( sat_solver_store_release( satSolver ) );
  commonVariables = Vec_IntAlloc( converter.literalsOn().size() - 1 );

  sat_solver_delete( satSolver );

  for( int i = 1 ; i < converter.literalsOn().size() ; ++i )
     Vec_IntPush( commonVariables, converter.literalsOn()[i] );

  satSolver     = NULL;
  interMan      = Inta_ManAlloc();
  mInterpolant  = static_cast<Aig_Man_t*>( Inta_ManInterpolate( interMan, proof, 0, commonVariables, 0 ) );

  Inta_ManFree( interMan        );
  Vec_IntFree ( commonVariables );
  Sto_ManFree ( proof           );
}

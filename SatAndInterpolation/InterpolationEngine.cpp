#include "InterpolationEngine.h"

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

  int lits[3];

  // add clause B
  addClause( satSolver, converter.cnfOff() );
  // end add clause B

  // add common variables clause
  for( int i = 0 ; i < baseFunctions.size() ; ++i )
  {
     lits[0] = toLitCond( converter.literalsOn ()[i+1], 0 );
     lits[1] = toLitCond( converter.literalsOff()[i+1], 1 );

     sat_solver_addclause( satSolver, lits, lits + 2 );

     lits[0] = toLitCond( converter.literalsOn ()[i+1], 1 );
     lits[1] = toLitCond( converter.literalsOff()[i+1], 0 );

     sat_solver_addclause( satSolver, lits, lits + 2 );
  }
  // end add common variables clause

  sat_solver_store_mark_roots( satSolver );
}

void InterpolationEngine::interpolation()
{
}


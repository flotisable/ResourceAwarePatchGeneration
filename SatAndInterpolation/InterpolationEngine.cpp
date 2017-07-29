#include "InterpolationEngine.h"

extern "C"
{
#include "base/abc/abc.h"
#include "aig/aig/aig.h"
#include "sat/cnf/cnf.h"
#include "sat/bsat/satSolver.h"
}

InterpolationEngine::InterpolationEngine()
{
  dln           = NULL;
  mInterpolant  = NULL;

  satSolver = NULL;
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

  int *begin;
  int *end;
  int i;

  // setup sat solver
  satSolver = sat_solver_new();

  sat_solver_store_alloc( satSolver );
  sat_solver_setnvars   ( satSolver, converter.cnfOn()->nVars + converter.cnfOff()->nVars );
  // end setup sat solver

  // add clause A
  Cnf_CnfForClause( converter.cnfOn(), begin, end, i )
  {
    sat_solver_addclause( satSolver, begin, end );
  }
  sat_solver_store_mark_clauses_a( satSolver );
  // end add clause A
}

void InterpolationEngine::addClauseB()
{
  if( !converter.cnfOff() || !satSolver ) return; // precondition

  int *begin;
  int *end;
  int i;
  int lits[3];

  // add clause B
  Cnf_CnfForClause( converter.cnfOff(), begin, end, i )
  {
    sat_solver_addclause( satSolver, begin, end );
  }
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


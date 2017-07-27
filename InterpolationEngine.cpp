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

  ntkA = ntkB = NULL;
  cnfA = cnfB = NULL;

  commonVariables = NULL;
  sat_solver      = NULL;
}

void InterpolationEngine::splitInterpolationAB()
{
  if( !dln || !targetFunction ) return; // precondition

  ntkB = dln;
  ntkA = Abc_NtkDup( ntkB );

  Abc_NodeComplement( targetFunction );
}

void InterpolationEngine::circuitToCnf()
{
  if( !ntkA || !ntkB ) return; // precondition

  Aig_Man_t *aigA = Abc_NtkToDar( ntkA, 0, 0 );
  Aig_Man_t *aigB = Abc_NtkToDar( ntkB, 0, 0 );

  cnfA = Cnf_DeriveSimple( Aig_ManCoNum( aigA ) );
  cnfB = Cnf_DeriveSimple( Aig_ManCoNum( aigB ) );

  Cnf_DataLift( cnfB, cnfA->nVars );
}

void InterpolationEngine::addClauseA()
{
  if( !cnfA ) return; // precondition

  int *begin;
  int *end;
  int i;

  satSolver = sat_solver_new();

  sat_solver_store_alloc( satSolver );
  sat_solver_setnvars   ( satSolver, cnfA->nVars + cnfB->nVars );

  Cnf_CnfForClause( cnfA, begin, end, i )
  {
    sat_solver_addclause( satSolver, begin, end );
  }
  sat_solver_store_mark_clauses_a( satSolver );
}

void InterpolationEngine::addClauseB()
{
}

void InterpolationEngine::interpolation()
{
}


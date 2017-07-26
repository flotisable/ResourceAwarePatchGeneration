#include "InterpolationEngine.h"

#include <algorithm>

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

  using std::find;

  Abc_Obj_t *node;
  int       i;

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
}

void InterpolationEngine::addClauseA()
{
}

void InterpolationEngine::addClauseB()
{
}

void InterpolationEngine::interpolation()
{
}


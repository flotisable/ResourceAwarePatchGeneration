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
}

void InterpolationEngine::circuitToCnf()
{
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


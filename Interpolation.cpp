#include "Interpolation.h"

extern "C"
{
#include "base/abc/abc.h"
#include "aig/aig/aig.h"
#include "sat/cnf/cnf.h"
#include "sat/bsat/satSolver.h"
}

Interpolation::Interpolation()
{
  dln             = NULL;
  mInterpolation  = NULL;

  ntkA = ntkB = NULL;
  cnfA = cnfB = NULL;

  commonVariables = NULL;
  sat_solver      = NULL;
}

void Interpolation::splitInterpolationAB()
{
}

void Interpolation::circuitToCnf()
{
}

void Interpolation::addClauseA()
{
}

void Interpolation::addClauseB()
{
}

void Interpolation::interpolation()
{
}


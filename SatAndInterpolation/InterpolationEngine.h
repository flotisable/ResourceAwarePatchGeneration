#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <iostream>
#include <queue>
using std::vector;

#include "NtkToCnfConverter.h"

extern "C"
{
#include "base/abc/abc.h"
#include "aig/aig/aig.h"
#include "sat/cnf/cnf.h"
#include "sat/bsat/satSolver.h"
}

class InterpolationEngine
{
  friend class TestInterpolationEngine;

  public:

    enum CircuitType
    {
      on,
      off
    };

    InterpolationEngine();

    inline void setCircuit        ( Abc_Ntk_t                 *circuit        , CircuitType type = on );
    inline void setBaseFunctions  ( const vector<Abc_Obj_t*>  &baseFunctions  , CircuitType type = on );
    inline void setTargetFunction ( Abc_Obj_t                 *targetFunction , CircuitType type = on );

    inline Abc_Ntk_t* interpolant();

    void circuitToCnf();
    void addClauseA();
    void addClauseB();
    void interpolation();

  private:

    Abc_Ntk_t*  convertAigToNtk ( Aig_Man_t *aig );
    void        createPi        ( Aig_Man_t *aig, Abc_Ntk_t *ntk, std::queue<Aig_Obj_t*> &box );
    void        buildCircuit    ( Aig_Man_t *aig, Abc_Ntk_t *ntk, std::queue<Aig_Obj_t*> &box );

    Abc_Ntk_t           *circuitOn;
    Abc_Ntk_t           *circuitOff;
    vector<Abc_Obj_t*>  basesOn;
    vector<Abc_Obj_t*>  basesOff;
    Abc_Obj_t           *targetOn;
    Abc_Obj_t           *targetOff;
    NtkToCnfConverter   converter;

    Abc_Ntk_t *mInterpolant;

    sat_solver *satSolver;
};

// inline non-member functions
inline void addClause( sat_solver *satSolver, Cnf_Dat_t *cnf );
// end inline non-member functions

// public inline member functions
inline void InterpolationEngine::setCircuit       ( Abc_Ntk_t *circuit, CircuitType type )
{
  if( type == on )  circuitOn   = circuit;
  else              circuitOff  = circuit;
}
inline void InterpolationEngine::setBaseFunctions ( const vector<Abc_Obj_t*> &baseFunctions, CircuitType type )
{
  if( type == on )  basesOn   = baseFunctions;
  else              basesOff  = baseFunctions;
}
inline void InterpolationEngine::setTargetFunction( Abc_Obj_t *targetFunction, CircuitType type )
{
  if( type == on )  targetOn  = targetFunction;
  else              targetOff = targetFunction;
}

inline Abc_Ntk_t* InterpolationEngine::interpolant() { return mInterpolant; }
// end public inline member functions

// inline non-member functions
inline void addClause( sat_solver *satSolver, Cnf_Dat_t *cnf )
{
  int *begin, *end, i;

  Cnf_CnfForClause( cnf, begin, end, i )
  {
    if( !sat_solver_addclause( satSolver, begin, end ) )
      std::cout << "clause add error!\n";
  }
}
// end inline non-member functions
#endif

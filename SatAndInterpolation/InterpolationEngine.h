#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
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
  public:

    InterpolationEngine();

    inline void setDln            ( Abc_Ntk_t                 *dln            );
    inline void setBaseFunctions  ( const vector<Abc_Obj_t*>  &baseFunctions  );
    inline void setTargetFunction ( Abc_Obj_t                 *targetFunction );

    inline Aig_Man_t* interpolant();

    void circuitToCnf();
    void addClauseA();
    void addClauseB();
    void interpolation();

  private:

    Abc_Ntk_t           *dln;          // dependency logic network
    vector<Abc_Obj_t*>  baseFunctions;
    Abc_Obj_t           *targetFunction;
    NtkToCnfConverter   converter;

    Aig_Man_t *mInterpolant;

    sat_solver *satSolver;
};

inline void addClause( sat_solver *satSolver, Cnf_Dat_t *cnf );

// public inline member functions
inline void InterpolationEngine::setDln           ( Abc_Ntk_t *dln )
{ this->dln = dln; }
inline void InterpolationEngine::setBaseFunctions ( const vector<Abc_Obj_t*> &baseFunctions )
{ this->baseFunctions = baseFunctions; }
inline void InterpolationEngine::setTargetFunction( Abc_Obj_t *targetFunction )
{ this->targetFunction = targetFunction; }

inline Aig_Man_t* InterpolationEngine::interpolant() { return mInterpolant; }
// end public inline member functions

inline void addClause( sat_solver *satSolver, Cnf_Dat_t *cnf )
{
  int *begin, *end, i;

  Cnf_CnfForClause( cnf, begin, end, i )
    sat_solver_addclause( satSolver, begin, end );
}
#endif

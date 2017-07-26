#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
using std::vector;

extern "C"
{
  struct Abc_Ntk_t;
  struct Abc_Obj_t;
  struct Aig_Man_t;
  struct Cnf_Dat_t;
  struct Vec_int_t;
  struct sat_solver;
}

class InterpolationEngine
{
  public:

    InterpolationEngine();

    inline void setDln          ( const Abc_Ntk_t *dln                    );
    inline void setBaseFunctions( const vector<Abc_Obj_t*> &baseFunctions );

    inline Aig_Man_t* interpolant();

    void splitInterpolationAB();
    void circuitToCnf();
    void addClauseA();
    void addClauseB();
    void interpolation();

  private:

    Abc_Ntk_t           *dln;          // dependency logic network
    vector<Abc_Obj_t*>  baseFunctions;

    Aig_Man_t *mInterpolant;

    Abc_Ntk_t *ntkA;
    Abc_Ntk_t *ntkB;

    Cnf_Dat_t *cnfA;
    Cnf_Dat_t *cnfB;
    Vec_Int_t *commonVariables;

    sat_solver *satSolver;
};

// public inline member functions
inline void InterpolationEngine::setDln          ( const Abc_Ntk_t *dln                    )
{ this->dln = dln; }
inline void InterpolationEngine::setBaseFunctions( const vector<Abc_Obj_t*> &baseFunctions )
{ this->baseFunctions = baseFunctions };

inline Aig_Man_t* InterpolationEngine::interpolant() { return mInterpolant; }
// end public inline member functions

#endif

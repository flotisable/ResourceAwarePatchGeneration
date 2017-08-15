#ifndef NTK_TO_CNF_CONVERTER_H
#define NTK_TO_CNF_CONVERTER_H

#include <vector>
using std::vector;

// abc related declaration
extern "C"
{
#include "base/abc/abc.h"
#include "sat/cnf/cnf.h"
#include "aig/aig/aig.h"
}
// end abc related declaration

/*
 * precondition:  base functions and target function are po type
 *
 * postcondition: literals on and off are in the same order of base functions
 *                first literal is correspond to target function
 *
 * note:          cnf on and off will be released at the NtkToCnfConverter's destruction
*/

class NtkToCnfConverter
{
  friend class TestNtkToCnfConverter;

  enum CircuitType
  {
    on,
    off
  };

  public:

    NtkToCnfConverter();
    ~NtkToCnfConverter();

    inline void setBaseFunctions  ( const vector<Abc_Obj_t*>  &baseFunctions  , CircuitType type = on );
    inline void setTargetFunction ( Abc_Obj_t                 *targetFunction , CircuitType type = on );
    inline void setCircuit        ( Abc_Ntk_t                 *circuit        , CircuitType type = on );

    inline const vector<int>& literalsOn  ();
    inline const vector<int>& literalsOff ();
    inline Cnf_Dat_t*         cnfOn       ();
    inline Cnf_Dat_t*         cnfOff      ();

    void convert();

  private:

    void createOnOffCircuit ();
    void circuitToCnf       ();

    vector<Abc_Obj_t*>  basesOn;
    vector<Abc_Obj_t*>  basesOff;
    Abc_Obj_t           *targetOn;
    Abc_Obj_t           *targetOff;
    Abc_Ntk_t           *circuitOn;
    Abc_Ntk_t           *circuitOff;

    vector<int> mLiteralsOn;
    vector<int> mLiteralsOff;
    Cnf_Dat_t   *mCnfOn;
    Cnf_Dat_t   *mCnfOff;

    // for test purpose
    Aig_Man_t *aigOn;
    Aig_Man_t *aigOff;
    // end for test purpose
};

// non-member functions
int findLiteral( Aig_Man_t *aig, Cnf_Dat_t *cnf, Abc_Obj_t *target );
// end non-member functions

// public inline member functions
inline void NtkToCnfConverter::setBaseFunctions  ( const vector<Abc_Obj_t*> &baseFunctions  , NtkToCnfConverter::CircuitType type )
{
  if( type == on )  basesOn   = baseFunctions;
  else              basesOff  = baseFunctions;
}
inline void NtkToCnfConverter::setTargetFunction ( Abc_Obj_t                *targetFunction , NtkToCnfConverter::CircuitType type )
{
  if( type == on )  targetOn  = targetFunction;
  else              targetOff = targetFunction;
}
inline void NtkToCnfConverter::setCircuit        ( Abc_Ntk_t                *circuit        , NtkToCnfConverter::CircuitType type )
{
  if( type == on )  circuitOn   = circuit;
  else              circuitOff  = circuit;
}

inline const vector<int>& NtkToCnfConverter::literalsOn () { return mLiteralsOn;  }
inline const vector<int>& NtkToCnfConverter::literalsOff() { return mLiteralsOff; }
inline Cnf_Dat_t*         NtkToCnfConverter::cnfOn      () { return mCnfOn;       }
inline Cnf_Dat_t*         NtkToCnfConverter::cnfOff     () { return mCnfOff;      }
// end public inline member functions

#endif

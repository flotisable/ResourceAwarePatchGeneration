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

  public:

    NtkToCnfConverter();
    ~NtkToCnfConverter();

    inline void setBaseFunctions  ( const vector<Abc_Obj_t*>  baseFunctions   );
    inline void setTargetFunction ( Abc_Obj_t                 *targetFunction );
    inline void setCircuit        ( Abc_Ntk_t                 *circuit        );

    inline vector<int>  literalsOn  ();
    inline vector<int>  literalsOff ();
    inline Cnf_Dat_t*   cnfOn       ();
    inline Cnf_Dat_t*   cnfOff      ();

    void convert();

  private:

    void createOnOffCircuit ();
    void circuitToCnf       ();

    vector<Abc_Obj_t*>  baseFunctions;
    Abc_Obj_t           *targetFunction;
    Abc_Ntk_t           *circuit;

    vector<int> mLiteralsOn;
    vector<int> mLiteralsOff;
    Cnf_Dat_t   *mCnfOn;
    Cnf_Dat_t   *mCnfOff;

    Abc_Ntk_t *ntkOn;
    Abc_Ntk_t *ntkOff;

    vector<Abc_Obj_t*>  baseCopy;
    Abc_Obj_t           *targetCopy;

    // for test purpose
    Aig_Man_t *aigOn;
    Aig_Man_t *aigOff;
    // end for test purpose
};

// non-member functions
int         findLiteral ( Aig_Man_t *aig, Cnf_Dat_t *cnf, Abc_Obj_t *target );
void        removePo    ( Aig_Man_t *aig, Aig_Obj_t *po );
void        insertPo    ( Aig_Man_t *aig, Aig_Obj_t *po, Aig_Obj_t *poFanin );
Cnf_Dat_t*  deriveCnf   ( Aig_Man_t *aig, Aig_Obj_t *output );
// end non-member functions

// public inline member functions
inline void NtkToCnfConverter::setBaseFunctions  ( const vector<Abc_Obj_t*> baseFunctions   )
{ this->baseFunctions   = baseFunctions; }
inline void NtkToCnfConverter::setTargetFunction ( Abc_Obj_t                *targetFunction )
{ this->targetFunction  = targetFunction; }
inline void NtkToCnfConverter::setCircuit        ( Abc_Ntk_t                *circuit        )
{ this->circuit         = circuit; }

inline vector<int>  NtkToCnfConverter::literalsOn  () { return mLiteralsOn;   }
inline vector<int>  NtkToCnfConverter::literalsOff () { return mLiteralsOff;  }
inline Cnf_Dat_t*   NtkToCnfConverter::cnfOn       () { return mCnfOn;        }
inline Cnf_Dat_t*   NtkToCnfConverter::cnfOff      () { return mCnfOff;       }
// end public inline member functions

#endif

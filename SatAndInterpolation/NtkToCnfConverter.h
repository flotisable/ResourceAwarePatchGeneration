#ifndef NTK_TO_CNF_CONVERTER_H
#define NTK_TO_CNF_CONVERTER_H

#include <vector>
using std::vector;

// abc related declaration
extern "C"
{
  struct Abc_Obj_t;
  struct Abc_Ntk_t;
  struct Cnf_Dat_t;
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
  public:

    NtkToCnfConverter();
    ~NtkToCnfConverter();

    inline void setBaseFuncions   ( const vector<Abc_Obj_t*>  baseFunctions   );
    inline void setTargetFunction ( const Abc_Obj_t           *targetFuction  );
    inline void setCircuit        ( const Abc_Ntk_t           *circuit        );

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
};

inline void NtkToCnfConverter::setBaseFuncions   ( const vector<Abc_Obj_t*>  baseFunctions   )
{ this->baseFunctions   = baseFunctions; }
inline void NtkToCnfConverter::setTargetFunction ( const Abc_Obj_t           *targetFuction  )
{ this->targetFunction  = targetFunction; }
inline void NtkToCnfConverter::setCircuit        ( const Abc_Ntk_t           *circuit        )
{ this->circuit         = circuit; }

inline vector<int>  NtkToCnfConverter::literalsOn  () { return mLiteralsOn;   }
inline vector<int>  NtkToCnfConverter::literalsOff () { return mLiteralsOff;  }
inline Cnf_Dat_t*   NtkToCnfConverter::cnfOn       () { return mCnfOn;        }
inline Cnf_Dat_t*   NtkToCnfConverter::cnfOff      () { return mCnfOff;       }

#endif

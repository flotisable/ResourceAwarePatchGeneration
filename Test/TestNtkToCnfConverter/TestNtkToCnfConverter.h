#ifndef TEST_NTK_TO_CNF_CONVERTER_H
#define TEST_NTK_TO_CNF_CONVERTER_H

#include <vector>
#include <string>

#include "SatAndInterpolation/NtkToCnfConverter.h"

extern "C"
{
#include "base/abc/abc.h"
}

class TestNtkToCnfConverter
{
  public:

    TestNtkToCnfConverter ();
    ~TestNtkToCnfConverter();

    void test();

  private:

    void testCreateOnOffCircuit();
    void testCircuitToCnf      ();

    NtkToCnfConverter converter;
};

Abc_Obj_t*  findPo      ( Abc_Ntk_t *circuit, const std::string &name );
void        writeCircuit( const std::string &outFile, Abc_Ntk_t *circuit );

#endif

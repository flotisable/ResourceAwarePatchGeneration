#ifndef TEST_NTK_TO_CNF_CONVERTER_H
#define TEST_NTK_TO_CNF_CONVERTER_H

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

void writeCircuit( const std::string &outFile, Abc_Ntk_t *circuit );

#endif

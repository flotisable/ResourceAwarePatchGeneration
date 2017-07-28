#include "SatAndInterpolation/NtkToCnfConverter.h"

class TestNtkToCnfConverter
{
  public:

    TestNtkToCnfConverter ();
    ~TestNtkToCnfConverter();

    void test();

  private:

    void testCreateOnOffCircuit();
    void testCircuitToCnf      ();
};

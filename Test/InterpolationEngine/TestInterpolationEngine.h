#ifndef TEST_INTERPOLATION_ENGINE_H
#define TEST_INTERPOLATION_ENGINE_H

#include <string>
#include <vector>

#include "SatAndInterpolation/InterpolationEngine.h"

extern "C"
{
#include "base/abc/abc.h"
}

class TestInterpolationEngine
{
  public:

    TestInterpolationEngine ();
    ~TestInterpolationEngine();

    void test();

  private:

    void read ( const std::string &file );
    void write( const std::string &file );

    InterpolationEngine engine;

    Abc_Ntk_t               *dln;
    Abc_Obj_t               *targetFunction;
    std::vector<Abc_Obj_t*> baseFunctions;

    Abc_Ntk_t *interpolant;
};

Abc_Obj_t* findPo( Abc_Ntk_t *circuit, const std::string &name );

#endif

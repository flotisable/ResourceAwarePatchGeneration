#ifndef TEST_INTERPOLATION_ENGINE_H
#define TEST_INTERPOLATION_ENGINE_H

#include "SatAndInterpolation/InterpolationEngine.h"

class TestInterpolationEngine
{
  public:

    TestInterpolationEngine ();
    ~TestInterpolationEngine();

    void test();

  private:

    InterpolationEngine engine;
};

#endif

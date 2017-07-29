#include "TestInterpolationEngine.h"

extern "C"
{
#include "base/main/main.h"
}

TestInterpolationEngine::TestInterpolationEngine()
{
  Abc_Start();
}

TestInterpolationEngine::~TestInterpolationEngine()
{
  Abc_Stop();
}

void TestInterpolationEngine::test()
{
}

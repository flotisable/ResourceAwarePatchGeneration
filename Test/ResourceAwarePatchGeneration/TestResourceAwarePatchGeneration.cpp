#include "TestResourceAwarePatchGeneration.h"

extern "C"
{
#include "base/main/main.h"
}

TestResourceAwarePatchGeneration::TestResourceAwarePatchGeneration()
{
  Abc_Start();
}

TestResourceAwarePatchGeneration::~TestResourceAwarePatchGeneration()
{
  Abc_Stop();
}

void TestResourceAwarePatchGeneration::test()
{
  testInterpolation();
}

void TestResourceAwarePatchGeneration::testReadFile()
{
}

void TestResourceAwarePatchGeneration::testReadWeight()
{
}

void TestResourceAwarePatchGeneration::testReplaceTWithPi()
{
}

void TestResourceAwarePatchGeneration::testTraverseTPiAndPo()
{
}

void TestResourceAwarePatchGeneration::testDeleteUnusedPo()
{
}

void TestResourceAwarePatchGeneration::testPreProcess()
{
}

void TestResourceAwarePatchGeneration::testConstructT()
{
}

void TestResourceAwarePatchGeneration::testTransformToCnf()
{
}

void TestResourceAwarePatchGeneration::testSatSolve()
{
}

void TestResourceAwarePatchGeneration::testInterpolation()
{
}

void TestResourceAwarePatchGeneration::testFunctionalDependency()
{
}

void TestResourceAwarePatchGeneration::testWritePatch()
{
}

void TestResourceAwarePatchGeneration::testRecursiveTravPoAddToSet()
{
}

void TestResourceAwarePatchGeneration::testRecursiveTravPiAddToSet()
{
}

void TestResourceAwarePatchGeneration::testDpReducedBaseFunction()
{
}

void TestResourceAwarePatchGeneration::testConvertNtkToAigWithBaseFunc()
{
}

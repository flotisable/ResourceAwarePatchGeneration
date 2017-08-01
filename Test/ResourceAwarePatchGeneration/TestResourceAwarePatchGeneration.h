#ifndef TEST_RESOURCE_AWARE_PATCH_GENERATION_H
#define TEST_RESOURCE_AWARE_PATCH_GENERATION_H

#include "ResourceAwarePatchGeneration.h"

class TestResourceAwarePatchGeneration
{
  public:

    TestResourceAwarePatchGeneration();
    ~TestResourceAwarePatchGeneration();

    void test();

  private:

  void testReadFile();
  void testReadWeight();
  void testReplaceTWithPi();
  void testTraverseTPiAndPo();
  void testDeleteUnusedPo();
  void testPreProcess();

  void testConstructT();
  void testTransformToCnf();
  void testSatSolve();
  void testInterpolation();

  void testFunctionalDependency();
  void testWritePatch();
  void testRecursiveTravPoAddToSet();
  void testRecursiveTravPiAddToSet();
  void testDpReducedBaseFunction();
  void testConvertNtkToAigWithBaseFunc();
};

#endif

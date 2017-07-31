#include "TestResourceAwarePatchGeneration.h"

#include <string>
#include <vector>
#include <iostream>

extern "C"
{
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"
}

Abc_Obj_t* findPo( Abc_Ntk_t *circuit, const std::string &name )
{
  Abc_Obj_t *po;
  int       i;

  Abc_NtkForEachPo( circuit, po, i )
    if( std::string( Abc_ObjName( po ) ) == name ) return po;

  return NULL;
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
  //testInterpolation();
  testWritePatch();
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
  using std::vector;
  using std::cout;

  const std::string inFile  = "test.v";
  const std::string outFile = "testOut.v";

  Abc_Ntk_t          *circuit          = Io_ReadVerilog( const_cast<char*>( inFile.c_str() ), 0 );
  Abc_Obj_t          *targetFunction;
  vector<Abc_Obj_t*> baseFunctions;

  ResourceAwarePatchGenerator generator( "", "", "" );

  circuit = Abc_NtkToLogic( circuit );
  circuit = Abc_NtkStrash ( circuit, 1, 1, 0 );

  targetFunction = findPo( circuit, "out" );

  if( !targetFunction )
  {
    cout << "can not find target function\n";
    return;
  }

  baseFunctions.push_back( findPo( circuit, "g1" ) );
  baseFunctions.push_back( findPo( circuit, "g3" ) );
  baseFunctions.push_back( findPo( circuit, "g4" ) );

  for( int i = 0 ; i < baseFunctions.size() ; ++i )
     if( !baseFunctions[i] )
     {
       cout << "can not find base function " << i << "\n";
       return;
     }

  generator.interpolation( circuit, targetFunction, baseFunctions );

  circuit = generator.interpolant;

  Io_WriteVerilog( circuit, const_cast<char*>( outFile.c_str() ) );
}

void TestResourceAwarePatchGeneration::testFunctionalDependency()
{
}

void TestResourceAwarePatchGeneration::testWritePatch()
{
  using std::vector;
  using std::cout;

  const std::string inFile      = "test.v";
  const std::string outFile     = "testOut.v";
  const std::string patchedFile = "out.v";

  Abc_Ntk_t          *circuit          = Io_ReadVerilog( const_cast<char*>( inFile.c_str() ), 0 );
  Abc_Obj_t          *targetFunction;
  vector<Abc_Obj_t*> baseFunctions;

  ResourceAwarePatchGenerator generator( inFile , "", "" );

  circuit = Abc_NtkToLogic( circuit );
  circuit = Abc_NtkStrash ( circuit, 1, 1, 0 );

  targetFunction = findPo( circuit, "out" );

  if( !targetFunction )
  {
    cout << "can not find target function\n";
    return;
  }

  baseFunctions.push_back( findPo( circuit, "g1" ) );
  baseFunctions.push_back( findPo( circuit, "g3" ) );
  baseFunctions.push_back( findPo( circuit, "g4" ) );

  for( int i = 0 ; i < baseFunctions.size() ; ++i )
     if( !baseFunctions[i] )
     {
       cout << "can not find base function " << i << "\n";
       return;
     }

  generator.interpolation( circuit, targetFunction, baseFunctions );
  generator.write_patch( outFile, patchedFile );
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

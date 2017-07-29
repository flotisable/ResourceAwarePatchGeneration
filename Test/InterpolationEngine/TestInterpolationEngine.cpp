#include "TestInterpolationEngine.h"

#include <string>

extern "C"
{
#include "base/main/main.h"
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
  using std::string;

  const string inFile   = "test.v";
  const string outFile  = "testOut.v";

  read( inFile );
}

void TestInterpolationEngine::read( const std::string &file )
{
  dln = Io_ReadVerilog( const_cast<char*>( file.c_str() ), 0 );
  dln = Abc_NtkStrash( dln, 1, 1, 0 );

  targetFunction = findPo( dln, "out" );

  baseFunctions.push_back( findPo( dln, "g1" ) );
  baseFunctions.push_back( findPo( dln, "g2" ) );
}

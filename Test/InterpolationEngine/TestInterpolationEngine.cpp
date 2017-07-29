#include "TestInterpolationEngine.h"

#include <string>

extern "C"
{
#include "base/main/main.h"
#include "base/io/ioAbc.h"
#include "aig/aig/aig.h"

Abc_Ntk_t* Abc_NtkFromDar( Abc_Ntk_t* , Aig_Man_t* );
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

  engine.setDln           ( dln             );
  engine.setTargetFunction( targetFunction  );
  engine.setBaseFunctions ( baseFunctions   );

  engine.circuitToCnf ();
  engine.addClauseA   ();
  engine.addClauseB   ();
  engine.interpolation();

  interpolant = Abc_NtkFromDar( dln, engine.interpolant() );

  write( outFile );
}

void TestInterpolationEngine::read( const std::string &file )
{
  dln = Io_ReadVerilog( const_cast<char*>( file.c_str() ), 0 );
  dln = Abc_NtkToLogic( dln );
  dln = Abc_NtkStrash( dln, 1, 1, 0 );

  targetFunction = findPo( dln, "out" );

  baseFunctions.push_back( findPo( dln, "g1" ) );
  baseFunctions.push_back( findPo( dln, "g2" ) );
}

void TestInterpolationEngine::write( const std::string &file )
{
  interpolant = Abc_NtkToNetlist( interpolant );

  Abc_NtkToAig( interpolant );
  Io_WriteVerilog( interpolant, const_cast<char*>( file.c_str() ) );
}

#include "TestInterpolationEngine.h"

#include <string>
#include <iostream>

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
  using std::cout;

  const string inFile   = "test.v";
  const string outFile  = "testOut.v";

  read( inFile );

  engine.setDln           ( dln             );
  engine.setTargetFunction( targetFunction  );
  engine.setBaseFunctions ( baseFunctions   );

  engine.circuitToCnf (); cout << "convert to cnf\n";
  Cnf_DataWriteIntoFile( engine.converter.cnfOn(), const_cast<char*>( "cnfOn.txt" ), 1, NULL, NULL );
  Cnf_DataWriteIntoFile( engine.converter.cnfOff(), const_cast<char*>( "cnfOff.txt" ), 1, NULL, NULL );
  engine.addClauseA   (); cout << "add clause A\n";
  sat_solver_store_write( engine.satSolver, const_cast<char*>( "satCnfA.txt" ) );
  engine.addClauseB   (); cout << "add clause B\n";
  sat_solver_store_write( engine.satSolver, const_cast<char*>( "satCnf.txt" ) );
  engine.interpolation(); cout << "interpolation\n";

  interpolant = engine.interpolant();

  write( outFile );
}

void TestInterpolationEngine::read( const std::string &file )
{
  dln = Io_ReadVerilog( const_cast<char*>( file.c_str() ), 0 );
  dln = Abc_NtkToLogic( dln );
  dln = Abc_NtkStrash( dln, 1, 1, 0 );

  targetFunction = findPo( dln, "out" );

  if( !targetFunction )
  {
    std::cout << "can not find po\n";
    return;
  }

  baseFunctions.push_back( findPo( dln, "g1" ) );
  baseFunctions.push_back( findPo( dln, "g3" ) );
  baseFunctions.push_back( findPo( dln, "g4" ) );

  if( !baseFunctions[0] || !baseFunctions[1] || !baseFunctions[2] )
    std::cout << "can not find base functions\n";
}

void TestInterpolationEngine::write( const std::string &file )
{
  interpolant = Abc_NtkToNetlist( interpolant );

  Abc_NtkToAig( interpolant );
  Io_WriteVerilog( interpolant, const_cast<char*>( file.c_str() ) );
}

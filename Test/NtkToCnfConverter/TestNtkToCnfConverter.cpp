#include "TestNtkToCnfConverter.h"

#include <iostream>
#include <fstream>
using namespace std;

extern "C"
{
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"
#include "base/cmd/cmd.h"
}

Abc_Obj_t* findPo( Abc_Ntk_t *circuit, const string &name )
{
  Abc_Obj_t *po;
  int       i;

  Abc_NtkForEachPo( circuit, po, i )
    if( string( Abc_ObjName( po ) ) == name )
      return po;

  return NULL;
}

void writeCircuit( const string &outFile, Abc_Ntk_t *circuit )
{
  Abc_Ntk_t *newCircuit = Abc_NtkToNetlist( circuit );

  Abc_NtkToAig( newCircuit );

  Io_WriteVerilog( newCircuit, const_cast<char*>( outFile.c_str() ) );
}

TestNtkToCnfConverter::TestNtkToCnfConverter()
{
  Abc_Start();
}

TestNtkToCnfConverter::~TestNtkToCnfConverter()
{
  Abc_Stop();
}

void TestNtkToCnfConverter::test()
{
  const string inFile = "test.v";

  Abc_Ntk_t           *circuit = Io_ReadVerilog( const_cast<char*>( inFile.c_str() ), 0 );
  Abc_Obj_t           *target;
  vector<Abc_Obj_t*>  bases;

  // setup circuit
  circuit = Abc_NtkToLogic( circuit );
  circuit = Abc_NtkStrash( circuit, 1, 1, 0 );
  // end setup circuit

  // find target function
  target = findPo( circuit, "t" );
  if( !target )
  {
    cout << "can not find target function\n";
    return;
  }
  cout << "target:" << Abc_ObjId( target ) << "\n";
  // end find target function

  // find base functions
  bases.push_back( findPo( circuit, "g1" ) );
  bases.push_back( findPo( circuit, "g2" ) );
  for( int i = 0 ; i < bases.size() ; ++i )
    if( !bases[i] )
    {
      cout << "can not find base functions\n";
      return;
    }
  cout << "base 1:" << Abc_ObjId( bases[0] ) << "\n";
  cout << "base 2:" << Abc_ObjId( bases[1] ) << "\n";
  // end find base functions

  // setup converter
  converter.setCircuit        ( circuit );
  converter.setTargetFunction ( target  );
  converter.setBaseFunctions  ( bases   );
  // end setup converter

  // test NtkToCnfConverter member functions
  testCreateOnOffCircuit();
  testCircuitToCnf      ();
  // end test NtkToCnfConverter member functions
}

void TestNtkToCnfConverter::testCreateOnOffCircuit()
{
  const string outFile = "testOut";

  converter.createOnOffCircuit();

  writeCircuit( outFile + "On.v",   converter.ntkOn  );
  writeCircuit( outFile + "Off.v",  converter.ntkOff );
}

void TestNtkToCnfConverter::testCircuitToCnf()
{
  const string aigLog = "aigCoLit.txt";
  const string litLog = "lit.txt";

  ofstream  file( aigLog.c_str() );
  Aig_Obj_t *co;
  int       i;

  converter.circuitToCnf();

  // write literal mapping of AIG node 
  Aig_ManForEachCo( converter.aigOn, co, i )
    file << converter.cnfOn()->pVarNums[co->Id] << "\n";

  file << "\n";

  Aig_ManForEachCo( converter.aigOff, co, i )
    file << converter.cnfOff()->pVarNums[co->Id] << "\n";

  file.close();
  // end write literal mapping of AIG node 

  // write literals
  file.open( litLog.c_str() );

  for( int i = 0 ; i < converter.literalsOn().size() ; ++i )
     file << converter.literalsOn()[i] << "\n";

  file << "\n";

  for( int i = 0 ; i < converter.literalsOff().size() ; ++i )
     file << converter.literalsOff()[i] << "\n";

  Cnf_DataWriteIntoFile( converter.cnfOn  (), const_cast<char*>( "cnfOn.txt"  ), 1, NULL, NULL );
  Cnf_DataWriteIntoFile( converter.cnfOff (), const_cast<char*>( "cnfOff.txt" ), 1, NULL, NULL );
  // end write literals
}

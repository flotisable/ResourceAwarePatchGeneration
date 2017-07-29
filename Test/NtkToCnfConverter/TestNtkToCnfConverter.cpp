#include "TestNtkToCnfConverter.h"

#include <iostream>
#include <fstream>

extern "C"
{
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"
#include "base/cmd/cmd.h"
}

Abc_Obj_t* findPo( Abc_Ntk_t *circuit, const std::string &name )
{
  Abc_Obj_t *po;
  int       i;

  Abc_NtkForEachPo( circuit, po, i )
    if( std::string( Abc_ObjName( po ) ) == name )
      return po;

  return NULL;
}

void writeCircuit( const std::string &outFile, Abc_Ntk_t *circuit )
{
  Abc_Ntk_t *newCircuit;

  newCircuit = Abc_NtkToNetlist( circuit );
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
  const std::string inFile = "test.v";

  Abc_Ntk_t               *circuit = Io_ReadVerilog( const_cast<char*>( inFile.c_str() ), 0 );
  Abc_Obj_t               *target;
  std::vector<Abc_Obj_t*> bases;

  circuit = Abc_NtkToLogic( circuit );
  circuit = Abc_NtkStrash( circuit, 1, 1, 0 );

  target = findPo( circuit, "out" );
  if( !target )
  {
    std::cout << "can not find target function\n";
    return;
  }
  std::cout << "target:" << Abc_ObjId( target ) << "\n";

  bases.push_back( findPo( circuit, "g1" ) );
  bases.push_back( findPo( circuit, "g2" ) );
  if( !bases[0] || !bases[1] )
  {
    std::cout << "can not find base functions\n";
    return;
  }
  std::cout << "base 1:" << Abc_ObjId( bases[0] ) << "\n";
  std::cout << "base 2:" << Abc_ObjId( bases[1] ) << "\n";

  converter.setCircuit        ( circuit );
  converter.setTargetFunction ( target  );
  converter.setBaseFunctions  ( bases   );

  testCreateOnOffCircuit();
  testCircuitToCnf      ();
}

void TestNtkToCnfConverter::testCreateOnOffCircuit()
{
  const std::string outFile = "testOut";

  converter.createOnOffCircuit();

  writeCircuit( outFile + "On.v",   converter.ntkOn  );
  writeCircuit( outFile + "Off.v",  converter.ntkOff );
}

void TestNtkToCnfConverter::testCircuitToCnf()
{
  const std::string aigLog = "aigCoLit.txt";
  const std::string litLog = "lit.txt";

  Aig_Obj_t *co;
  int       i;

  std::ofstream file;

  converter.circuitToCnf();

  file.open( aigLog.c_str() );

  Aig_ManForEachCo( converter.aigOn, co, i )
    file << converter.cnfOn()->pVarNums[co->Id] << "\n";

  file << "\n";

  Aig_ManForEachCo( converter.aigOff, co, i )
    file << converter.cnfOff()->pVarNums[co->Id] << "\n";

  file.close();

  file.open( litLog.c_str() );

  for( int i = 0 ; i < converter.literalsOn().size() ; ++i )
     file << converter.literalsOn()[i] << "\n";

  file << "\n";

  for( int i = 0 ; i < converter.literalsOff().size() ; ++i )
     file << converter.literalsOff()[i] << "\n";
}

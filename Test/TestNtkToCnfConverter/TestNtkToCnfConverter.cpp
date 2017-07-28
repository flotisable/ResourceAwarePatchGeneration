#include "TestNtkToCnfConverter.h"

#include <iostream>

extern "C"
{
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/io/ioAbc.h"
#include "base/cmd/cmd.h"
}

void writeCircuit( const std::string &outFile, Abc_Ntk_t *circuit )
{
  circuit = Abc_NtkToNetlist( circuit );
  Abc_NtkToAig( circuit );

  Io_WriteVerilog( circuit, const_cast<char*>( outFile.c_str() ) );
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
  testCreateOnOffCircuit();
}

void TestNtkToCnfConverter::testCreateOnOffCircuit()
{
  using std::string;
  using std::cout;

  string inFile   = "test.v";
  string outFile  = "testOut";

  Abc_Ntk_t *circuit  = Io_ReadVerilog( const_cast<char*>( inFile.c_str() ), 0 );
  Abc_Obj_t *target   = NULL;
  Abc_Obj_t *po;
  int       i;

  circuit = Abc_NtkToLogic( circuit );
  circuit = Abc_NtkStrash( circuit, 1, 1, 0 );

  Abc_NtkForEachPo( circuit, po, i )
  {
    if( string( Abc_ObjName( po ) ) == "out" )
    {
      target = po;
      break;
    }
  }
  if( !target )
  {
    cout << "can not find target function\n";
    return;
  }

  converter.setCircuit        ( circuit );
  converter.setTargetFunction ( target );
  converter.createOnOffCircuit();

  writeCircuit( outFile + "On.v",   converter.ntkOn  );
  writeCircuit( outFile + "Off.v",  converter.ntkOff );
}

void TestNtkToCnfConverter::testCircuitToCnf()
{
}

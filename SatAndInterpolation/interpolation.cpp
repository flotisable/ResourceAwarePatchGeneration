#include "ResourceAwarePatchGeneration.h"

#include "InterpolationEngine.h"

extern "C"
{
#include "base/main/main.h"
#include "base/cmd/cmd.h"
}

void ResourceAwarePatchGenerator::interpolation( Abc_Ntk_t *dln, Abc_Obj_t *targetPo, const vector<Abc_Obj_t*> &baseFunctions )
{
  const string libName      = "test.genlib";
  const string tempFileName = "testTemp.v";

  InterpolationEngine engine;
  Abc_Ntk_t           *circuit;

  // setup interpolation engine
  engine.setCircuit       ( dln           );
  engine.setTargetFunction( targetPo      );
  engine.setBaseFunctions ( baseFunctions );
  // end setup interpolation engine

  // do interpolation
  engine.circuitToCnf ();
  engine.addClauseA   ();
  engine.addClauseB   ();
  engine.interpolation();
  // end do interpolation

  // write interpolant as temporary file
  circuit = engine.interpolant();
  circuit = Abc_NtkToNetlist( circuit );

  Abc_NtkToAig( circuit );
  Io_WriteVerilog( circuit, const_cast<char*>( tempFileName.c_str() ) );
  // end write interpolant as temporary file

  // technology mapping
  Cmd_CommandExecute( pAbc, const_cast<char*>( string( "read " + tempFileName ).c_str() ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "strash" ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( string( "read_genlib " + libName ).c_str() ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "map" ) );
  // end technology mapping

  interpolant = Abc_NtkToNetlist( Abc_FrameReadNtk( pAbc ) );
}

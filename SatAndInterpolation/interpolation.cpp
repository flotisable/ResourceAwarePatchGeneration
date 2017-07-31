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

  engine.setDln           ( dln           );
  engine.setTargetFunction( targetPo      );
  engine.setBaseFunctions ( baseFunctions );

  engine.circuitToCnf ();
  engine.addClauseA   ();
  engine.addClauseB   ();
  engine.interpolation();

  circuit = engine.interpolant();
  circuit = Abc_NtkToNetlist( circuit );

  Abc_NtkToAig( circuit );
  Io_WriteVerilog( circuit, const_cast<char*>( tempFileName.c_str() ) );

  Cmd_CommandExecute( pAbc, const_cast<char*>( string( "read " + tempFileName ).c_str() ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "strash" ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( string( "read_genlib " + libName ).c_str() ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "map" ) );

  interpolant = Abc_NtkToNetlist( Abc_FrameReadNtk( pAbc ) );
}

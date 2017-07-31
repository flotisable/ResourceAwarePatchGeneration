#include "ResourceAwarePatchGeneration.h"

#include "InterpolationEngine.h"

extern "C"
{
#include "base/main/main.h"
#include "base/cmd/cmd.h"
}

void ResourceAwarePatchGenerator::interpolation( Abc_Ntk_t *dln, Abc_Obj_t *targetPo, const vector<Abc_Obj_t*> &baseFunctions )
{
  const string libName = "test.genlib";

  InterpolationEngine engine;

  engine.setDln           ( dln           );
  engine.setTargetFunction( targetPo      );
  engine.setBaseFunctions ( baseFunctions );

  engine.circuitToCnf ();
  engine.addClauseA   ();
  engine.addClauseB   ();
  engine.interpolation();

  /*
  Abc_FrameSetCurrentNetwork( pAbc, engine.interpolant() );

  Cmd_CommandExecute( pAbc, const_cast<char*>( string( "read_genlib " + libName ).c_str() ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "map" ) );
  */

  interpolant = engine.interpolant();//Abc_FrameReadNtk( pAbc );
}

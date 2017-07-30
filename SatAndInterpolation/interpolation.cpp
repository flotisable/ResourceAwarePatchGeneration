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

  Abc_FrameSetCurrentNetwork( pAbc, engine.interpolant() );

  Cmd_CommandExecute( pAbc, const_cast<char*>( "read_genlib " + libName ) );
  Cmd_CommandExecute( pAbc, const_cast<char*>( "map" ) );

  interpolant = Abc_FrameReadNtk( pAbc );
}

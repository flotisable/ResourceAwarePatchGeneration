#include "ResourceAwarePatchGeneration.h"

#include "InterpolationEngine.h"

extern "C"
{
#include "base/main/main.h"
#include "base/cmd/cmd.h"
#include "map/mio/mio.h"
  
  Abc_Ntk_t* Abc_NtkMap( Abc_Ntk_t*, double, double, double, float, float, float, int, int, int, int, int, int );
}

void ResourceAwarePatchGenerator::interpolation( Abc_Ntk_t *dln, Abc_Obj_t *targetPo, const vector<Abc_Obj_t*> &baseFunctions )
{
  const string libName = "test.genlib";

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
  Mio_UpdateGenlib( Mio_LibraryRead( const_cast<char*>( libName.c_str() ), NULL, NULL, 0 ) );

  circuit     = Abc_NtkMap( engine.interpolant(), -1, 0, 0, 0, 0, 250, 0, 1, 0, 0, 0, 0 );
  interpolant = Abc_NtkToNetlist( circuit );
}

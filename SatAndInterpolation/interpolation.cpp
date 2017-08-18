#include "ResourceAwarePatchGeneration.h"

#include "InterpolationEngine.h"

// abc related code
extern "C"
{
#include "base/main/main.h"
#include "base/cmd/cmd.h"
#include "map/mio/mio.h"
  
  Abc_Ntk_t* Abc_NtkMap( Abc_Ntk_t*, double, double, double, float, float, float, int, int, int, int, int, int );
}
// end abc related code

void ResourceAwarePatchGenerator::interpolation(
  Abc_Ntk_t *circuitOn,   Abc_Obj_t *targetOn,  const vector<Abc_Obj_t*> &basesOn,
  Abc_Ntk_t *circuitOff,  Abc_Obj_t *targetOff, const vector<Abc_Obj_t*> &basesOff )
{
  const string libName = "test.genlib";

  InterpolationEngine engine;
  Abc_Ntk_t           *circuit;

  // setup interpolation engine
  engine.setCircuit       ( circuitOn,  InterpolationEngine::on );
  engine.setTargetFunction( targetOn,   InterpolationEngine::on );
  engine.setBaseFunctions ( basesOn,    InterpolationEngine::on );

  if( circuitOff && targetOff && !basesOff.empty() && basesOn.size() == basesOff.size() )
  {
    engine.setCircuit       ( circuitOff, InterpolationEngine::off );
    engine.setTargetFunction( targetOff,  InterpolationEngine::off );
    engine.setBaseFunctions ( basesOff,   InterpolationEngine::off );
  }
  // end setup interpolation engine

  // do interpolation
  engine.circuitToCnf ();
  engine.addClauseA   ();
  engine.addClauseB   ();
  engine.interpolation();
  // end do interpolation

  Mio_UpdateGenlib( Mio_LibraryRead( const_cast<char*>( libName.c_str() ), NULL, NULL, 0 ) );

  circuit     = Abc_NtkMap( engine.interpolant(), -1, 0, 0, 0, 0, 250, 0, 1, 0, 0, 0, 0 );
  interpolant = Abc_NtkToNetlist( circuit );
}

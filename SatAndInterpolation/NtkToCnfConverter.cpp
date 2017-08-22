#include "NtkToCnfConverter.h"

#include <iostream>

extern "C"
{
#include "misc/vec/vecPtr.h"

  Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t*, int, int );
}

int findLiteral( Aig_Man_t *aig, Cnf_Dat_t *cnf, Abc_Obj_t *target )
{
  Aig_Obj_t *co;
  int       i;

  Aig_ManForEachCo( aig, co, i )
    if( Aig_ObjFanin0( co ) == reinterpret_cast<Aig_Obj_t*>( Abc_ObjCopy( Abc_ObjFanin0( target )  ) ) )
      return cnf->pVarNums[co->Id];

  return -1;
}

NtkToCnfConverter::NtkToCnfConverter()
{
  targetOn  = targetOff   = NULL;
  circuitOn = circuitOff  = NULL;

  mCnfOn  = mCnfOff = NULL;
  aigOn   = aigOff  = NULL;
}

NtkToCnfConverter::~NtkToCnfConverter()
{
  if( mCnfOn  ) Cnf_DataFree( mCnfOn  );
  if( mCnfOff ) Cnf_DataFree( mCnfOff );
  if( aigOn   ) Aig_ManStop ( aigOn   );
  if( aigOff  ) Aig_ManStop ( aigOff  );
}

void NtkToCnfConverter::convert()
{
  createOnOffCircuit();
  circuitToCnf      ();
}

void NtkToCnfConverter::createOnOffCircuit()
{
  if( !circuitOn || !targetOn ) return; // precondition
  if( circuitOff ) return;

  Abc_Obj_t *po;
  int       i;

  circuitOff  = Abc_NtkDup( circuitOn );

  // complement output of off circuit
  Abc_NtkForEachPo( circuitOn, po, i )
    if( po == targetOn )
    {
      Abc_Obj_t *outInv;

      targetOff = Abc_NtkPo( circuitOff, i );
      outInv    = Abc_ObjNot( Abc_ObjChild0( targetOff ) );

      Abc_ObjXorFaninC  ( targetOff, 0 );
      Abc_ObjDeleteFanin( targetOff, Abc_ObjFanin0( targetOff ) );
      Abc_ObjAddFanin   ( targetOff, outInv                     );
      break;
    }
  // end complement output of off circuit

  // save po in off circuit that is corresponding to base functions
  for( int i = 0 ; i < basesOn.size() ; ++i )
  {
     int j;

     Abc_NtkForEachPo( circuitOn, po, j )
       if( po == basesOn[i] )
       {
         basesOff.push_back( Abc_NtkPo( circuitOff, j ) );
         break;
       }
  }
  // end save po in off circuit that is corresponding to base functions
}

void NtkToCnfConverter::circuitToCnf()
{
  if( !circuitOn || !circuitOff ) return; // precondition

  Aig_Obj_t *fanout;
  int       i;

  // conver to cnf
  aigOn  = Abc_NtkToDar( circuitOn,  0, 0 );
  aigOff = Abc_NtkToDar( circuitOff, 0, 0 );

  mCnfOn  = Cnf_DeriveSimple( aigOn,  Aig_ManCoNum( aigOn   ) );
  mCnfOff = Cnf_DeriveSimple( aigOff, Aig_ManCoNum( aigOff  ) );

  Cnf_DataLift( mCnfOff, mCnfOn->nVars );
  // end conver to cnf

  // save literals
  mLiteralsOn.push_back ( findLiteral( aigOn,   mCnfOn,   targetOn  ) );
  mLiteralsOff.push_back( findLiteral( aigOff,  mCnfOff,  targetOff ) );

  for( int i = 0 ; i < basesOn.size() ; ++i )
     mLiteralsOn.push_back( findLiteral( aigOn, mCnfOn, basesOn[i] ) );

  for( int i = 0 ; i < basesOff.size() ; ++i )
     mLiteralsOff.push_back( findLiteral( aigOff, mCnfOff, basesOff[i] ) );
  // end save literals
}

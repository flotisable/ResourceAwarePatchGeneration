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
      return cnf->pVarNums[Aig_ObjFanin0( co )->Id];

  return -1;
}

void removePo( Aig_Man_t *aig, Aig_Obj_t *po )
{
  Vec_PtrRemove     ( aig->vCos, po );
  Aig_ObjDisconnect ( aig, po );
  aig->nObjs[AIG_OBJ_CO]--;
}

void insertPo( Aig_Man_t *aig, Aig_Obj_t *po, Aig_Obj_t *poFanin )
{
  Vec_PtrPush   ( aig->vCos, po );
  Aig_ObjConnect( aig, po, poFanin, NULL );
  aig->nObjs[AIG_OBJ_CO]++;
}

Cnf_Dat_t* deriveCnf( Aig_Man_t *aig, Aig_Obj_t *output )
{
  vector<Aig_Obj_t*> pos;
  vector<Aig_Obj_t*> poFanins;

  Cnf_Dat_t *cnf;
  Aig_Obj_t *po;
  int       i;

  std::cout << Aig_ManCoNum( aig ) << "\n";
  Aig_ManForEachCo( aig, po, i )
  {
    if( po == output ) continue;

    pos.push_back( po );
    poFanins.push_back( Aig_ObjFanin0( po ) );
  }
  for( int i = 0 ; i < pos.size() ; ++i )
     removePo( aig, pos[i] );
  std::cout << Aig_ManCoNum( aig ) << "\n";

  cnf = Cnf_DeriveSimple( aig, 0 );

  for( int i = 0 ; i < pos.size() ; ++i )
     insertPo( aig, pos[i], poFanins[i] );

  return cnf;
}

NtkToCnfConverter::NtkToCnfConverter()
{
  targetFunction  = NULL;
  circuit         = NULL;

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
  if( !circuit || !targetFunction ) return; // precondition

  Abc_Obj_t *po;
  int       i;

  ntkOn   = circuit;
  ntkOff  = Abc_NtkDup( ntkOn );

  // complement output of off circuit
  Abc_NtkForEachPo( ntkOn, po, i )
    if( po == targetFunction )
    {
      Abc_Obj_t *outInv;

      targetCopy  = Abc_NtkPo( ntkOff, i );
      outInv      = Abc_ObjNot( Abc_ObjFanin0( targetCopy ) );

      Abc_ObjDeleteFanin( targetCopy, Abc_ObjFanin0( targetCopy ) );
      Abc_ObjAddFanin   ( targetCopy, outInv                      );
      break;
    }
  // end complement output of off circuit

  // save po in off circuit that is corresponding to base functions
  for( int i = 0 ; i < baseFunctions.size() ; ++i )
  {
     int j;

     Abc_NtkForEachPo( ntkOn, po, j )
       if( po == baseFunctions[i] )
       {
         baseCopy.push_back( Abc_NtkPo( ntkOff, j ) );
         break;
       }
  }
  // end save po in off circuit that is corresponding to base functions
}

void NtkToCnfConverter::circuitToCnf()
{
  if( !ntkOn || !ntkOff ) return; // precondition

  Aig_Obj_t *fanout;
  int       i;

  // conver to cnf
  aigOn  = Abc_NtkToDar( ntkOn,  0, 0 );
  aigOff = Abc_NtkToDar( ntkOff, 0, 0 );

  Aig_ManForEachCo( aigOn, fanout, i )
    if( Aig_ObjFanin0( fanout ) == reinterpret_cast<Aig_Obj_t*>( Abc_ObjCopy( Abc_ObjFanin0( targetFunction ) ) ) )
    {
      mCnfOn = deriveCnf( aigOn, fanout );
      break;
    }
  Aig_ManForEachCo( aigOff, fanout, i )
    if( Aig_ObjFanin0( fanout ) == reinterpret_cast<Aig_Obj_t*>( Abc_ObjCopy( Abc_ObjFanin0( targetCopy ) ) ) )
    {
      mCnfOff = deriveCnf( aigOff, fanout );
      break;
    }

  Cnf_DataLift( mCnfOff, mCnfOn->nVars );
  // end conver to cnf

  // save literals
  mLiteralsOn.push_back ( findLiteral( aigOn,   mCnfOn,   targetFunction  ) );
  mLiteralsOff.push_back( findLiteral( aigOff,  mCnfOff,  targetCopy      ) );

  for( int i = 0 ; i < baseFunctions.size() ; ++i )
     mLiteralsOn.push_back( findLiteral( aigOn, mCnfOn, baseFunctions[i] ) );

  for( int i = 0 ; i < baseCopy.size() ; ++i )
     mLiteralsOff.push_back( findLiteral( aigOff, mCnfOff, baseCopy[i] ) );
  // end save literals
}

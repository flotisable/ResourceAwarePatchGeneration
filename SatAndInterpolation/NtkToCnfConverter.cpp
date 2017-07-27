#include "NtkToCnfConverter.h"

int findLiteral( Aig_Man_t *aig, Cnf_Dat_t *cnf, Abc_Obj_t *target )
{
  Aig_Obj_t *co;
  int       i;

  Aig_ManForEachCo( aig, co, j )
    if( co == Abc_ObjCopy( target ) )
      return cnf->pVarNums[co->Id];

  return -1;
}

NtkToCnfConverter::NtkToCnfConverter()
{
  targetFunction  = NULL;
  circuit         = NULL;

  mCnfOn = mCnfOff = NULL;
}

NtkToCnfConverter::~NtkToCnfConverter()
{
  if( mCnfOn  ) Cnf_DataFree( mCnfOn  );
  if( mCnfOff ) Cnf_DataFree( mCnfOff );
}

NtkToCnfConverter::convert()
{
}

NtkToCnfConverter::createOnOffCircuit()
{
  if( !circuit || !targetFunction ) return; // precondition

  Abc_Obj_t *po;
  int       i;

  ntkOn   = circuit;
  ntkOff  = Abc_NtkDup( ntkOff );

  // complement output of off circuit
  Abc_NtkForEachPo( ntkOn, po, i )
    if( po == targetFunction )
    {
      targetCopy = Abc_NtkPo( ntkOff, i );

      Abc_NodeComplement( Abc_ObjFanin0( targetCopy ) );
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

NtkToCnfConverter::circuitToCnf()
{
  if( !ntkOn || !ntkOff ) return; // precondition

  Aig_Obj_t *co;
  int       i;

  // conver to cnf
  Aig_Man_t *aigOn  = Abc_NtkToDar( ntkOn,  0, 0 );
  Aig_Man_t *aigOff = Abc_NtkToDar( ntkOff, 0, 0 );

  mCnfOn  = Cnf_DeriveSimple( Aig_ManCoNum( aigOn   ) );
  mCnfOff = Cnf_DeriveSimple( Aig_ManCoNum( aigOff  ) );

  Cnf_DataLift( cnfOff, cnfA->nVars );
  // end conver to cnf

  // save literals
  mLiteralsOn.push_back ( findLiteral( aigOn,   mCnfOn,   targetFunction  ) );
  mLiteralsOff.push_back( findLiteral( aigOff,  mCnfOff,  targetCopy      ) );

  for( int i = 0 ; i < baseFunctions.size() ; ++i )
     mLiteralsOn.push_back( findLiteral( aigOn, mCnfOn, baseFunctions[i] ) );

  for( int i = 0 ; i < baseCopy.size() ; ++i )
     mLiteralsOff.push_back( findLiteral( aigOff, mCnfOff, baseCopy[i] ) );
  // end save literals

  // release memory
  Aig_ManStop( aigOn  );
  Aig_ManStop( aigOff );
  // end release memory
}

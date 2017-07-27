#include "NtkToCnfConverter.h"

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

  Abc_NtkForEachPo( ntkOn, po, i )
  {
    if( po == targetFunction )
      Abc_NodeComplement( Abc_ObjFanin0( Abc_NtkPo( ntkOff, i ) ) );
  }
}

NtkToCnfConverter::circuitToCnf()
{
}

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
}

NtkToCnfConverter::circuitToCnf()
{
}

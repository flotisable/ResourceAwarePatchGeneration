#include "InterpolationEngine.h"

extern "C"
{
#include "base/abc/abc.h"
#include "aig/aig/aig.h"
#include "sat/cnf/cnf.h"
#include "sat/bsat/satSolver.h"
}

InterpolationEngine::InterpolationEngine()
{
  dln           = NULL;
  mInterpolant  = NULL;

  ntkA = ntkB = NULL;
  cnfA = cnfB = NULL;

  commonVariables = NULL;
  sat_solver      = NULL;
}

void InterpolationEngine::splitInterpolationAB()
{
  if( !dln || !targetFunction ) return; // precondition

  ntkA = dln;
  ntkB = Abc_NtkDup( ntkA );

  Abc_NodeComplement( Abc_ObjCopy( targetFunction ) );

  // save the copy of base function in networks B
  for( int i = 0 ; i < baseFunctions.size() ; ++i )
  {
     baseCopy.push_back( Abc_ObjCopy( baseFunctions[i] ) );
  }
  // end save the copy of base function in networks B
}

void InterpolationEngine::circuitToCnf()
{
  if( !ntkA || !ntkB ) return; // precondition

  Aig_Obj_t *node;
  int       i;

  Aig_Man_t *aigA = Abc_NtkToDar( ntkA, 0, 0 );
  Aig_Man_t *aigB = Abc_NtkToDar( ntkB, 0, 0 );

  cnfA = Cnf_DeriveSimple( Aig_ManCoNum( aigA ) );
  cnfB = Cnf_DeriveSimple( Aig_ManCoNum( aigB ) );

  Cnf_DataLift( cnfB, cnfA->nVars );

  // save common variables
  Aig_ManForEachObj( aigA, node, i )
  {
    for( int j = 0 ; j < baseFunctions.size() ; ++j )
    {
       if( node == baseFunctions[j] )
         goto matched;
    }
    continue;

matched:

    Vec_Int_Push( commonVariables, cnfA->pVarNums[node->Id] );
  }
  // end save common variables

  // save common variables copy
  Aig_ManForEachObj( aigB, node, i )
  {
    for( int j = 0 ; j < baseCopy.size() ; ++j )
    {
       if( node == baseCopy[j] )
         goto matchedCopy;
    }
    continue;

matchedCopy:

    Vec_Int_Push( commonCopy, cnfB->pVarNums[node->Id] );
  }
  // end save common variables
}

void InterpolationEngine::addClauseA()
{
  if( !cnfA ) return; // precondition

  int *begin;
  int *end;
  int i;

  // setup sat solver
  satSolver = sat_solver_new();

  sat_solver_store_alloc( satSolver );
  sat_solver_setnvars   ( satSolver, cnfA->nVars + cnfB->nVars );
  // end setup sat solver

  // add clause A
  Cnf_CnfForClause( cnfA, begin, end, i )
  {
    sat_solver_addclause( satSolver, begin, end );
  }
  sat_solver_store_mark_clauses_a( satSolver );
  // end add clause A
}

void InterpolationEngine::addClauseB()
{
  if( !cnfB || !satSolver ) return; // precondition

  int *begin;
  int *end;
  int i;
  int lits[3];

  // add clause B
  Cnf_CnfForClause( cnfB, begin, end, i )
  {
    sat_solver_addclause( satSolver, begin, end );
  }
  // end add clause B

  // add common variables clause
  for( int i = 0 ; i < baseFunctions.size() ; ++i )
  {
     lits[0] = toLitCond( Vec_IntEntry( commonVariables, i ), 0 );
     lits[1] = toLitCond( Vec_IntEntry( commonCopy,      i ), 1 );

     sat_solver_addclause( satSolver, lit, lit + 2 );

     lits[0] = toLitCond( Vec_IntEntry( commonVariables, i ), 1 );
     lits[1] = toLitCond( Vec_IntEntry( commonCopy,      i ), 0 );

     sat_solver_addclause( satSolver, lit, lit + 2 );
  }
  // end add common variables clause

  sat_solver_store_mark_roots( satSolver );

  // free the memory
  Aig_ManStop ( aigA );
  Aig_ManStop ( aigB );
  Cnf_DataFree( cnfA );
  Cnf_DataFree( cnfB );
  // end free the memory
}

void InterpolationEngine::interpolation()
{
}


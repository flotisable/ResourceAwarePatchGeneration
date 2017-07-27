#include "ResourceAwarePatchGeneration.h"

extern "C"
{
#include "sat/bsat/satStore.h"
#include "base/io/ioAbc.h"
#include "aig/aig/aig.h"
#include "sat/cnf/cnf.h"
#include "sat/bsat/satSolver.h"

extern Aig_Man_t* Abc_NtkToDar( Abc_Ntk_t*, int, int );
}

void ResourceAwarePatchGenerator::interpolation()
{
  sat_solver  *sat_result = NULL;
  Sto_Man_t   *cnf;
  Intb_Man_t  *intManager = Intb_ManAlloc();

  Abc_Ntk_t           *circuit = Io_ReadVerilog( const_cast<char*>( in_F_file.c_str() ), 0 );
  vector<Abc_Obj_t*>  bases;
  Abc_Obj_t           *po;
  int                 i;

  Abc_NtkForEachPo( circuit, po, i )
  {
    Abc_Obj_t *poNet  = Abc_ObjFanin0( po );
    string    poName  = Abc_ObjName( poNet );

    if( poName == "g01" || poName == "g02" )
    {
      bases.push_back( po );
    }
  }

  Abc_Ntk_t *circuitLogic   = Abc_NtkToLogic( circuit );
  Abc_Ntk_t *circuitStrash  = Abc_NtkStrash( circuitLogic, 1, 1, 1 );
  Aig_Man_t *aigMan         = Abc_NtkToDar( circuitStrash, 0, 0 );
  Cnf_Dat_t *cnfDat         = Cnf_DeriveSimple( aigMan, Aig_ManCoNum( aigMan ) );

  sat_result = sat_solver_new();
  sat_solver_store_alloc( sat_result );
  sat_result = static_cast<sat_solver*>( Cnf_DataWriteIntoSolverInt( sat_result, cnfDat, 1, 0 ) );

  if( sat_result )
  {
    Aig_Man_t *interpolant;
    Vec_Int_t *baseFunctions;
    Aig_Obj_t *co;
    lit       *begin  = ABC_ALLOC( lit, Aig_ManCoNum( aigMan ) );
    lit       *end    = begin + Aig_ManCoNum( aigMan );

    Aig_ManForEachCo( aigMan, co, i )
    {
      for( int j = 0 ;  j < bases.size() ; ++j )
      {
         if( reinterpret_cast<Aig_Obj_t*>( Abc_ObjChild0Copy( bases[j] ) ) == Aig_ObjChild0( co ) )
         {
           int varId = cnfDat->pVarNums[co->Id];

           Vec_IntPush( baseFunctions, varId );
         }
      }
      begin[i] = toLit( cnfDat->pVarNums[co->Id] );
    }

    
    sat_solver_solve( sat_result, begin, end, 0, 0, 0, 0 );
    cnf         = static_cast<Sto_Man_t*>( sat_solver_store_release( sat_result ) );
    interpolant = static_cast<Aig_Man_t*>( Intb_ManInterpolate( intManager, cnf, baseFunctions, 0 ) );

    Aig_ManDumpVerilog( interpolant, const_cast<char*>( "testOut" ) );
    ABC_FREE( begin );
  }

  sat_solver_store_free( sat_result );
  sat_solver_delete( sat_result );
  Intb_ManFree( intManager );
}

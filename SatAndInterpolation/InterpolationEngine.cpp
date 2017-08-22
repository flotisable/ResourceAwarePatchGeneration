#include "InterpolationEngine.h"

extern "C"
{
#include "sat/bsat/satStore.h"
}

InterpolationEngine::InterpolationEngine()
{
  circuitOn       = circuitOff  = NULL;
  targetOn        = targetOff   = NULL;
  mInterpolant    = NULL;
  satSolver       = NULL;
}

void InterpolationEngine::circuitToCnf()
{
  if( !circuitOn || !targetOn || basesOn.empty() ) return; // precondition

  // setup cnf converter
  converter.setCircuit       ( circuitOn, NtkToCnfConverter::on   );
  converter.setTargetFunction( targetOn,  NtkToCnfConverter::on   );
  converter.setBaseFunctions ( basesOn,   NtkToCnfConverter::on   );

  if( circuitOff && targetOff && !basesOff.empty() && basesOn.size() == basesOff.size() )
  {
    converter.setCircuit       ( circuitOff,  NtkToCnfConverter::off  );
    converter.setTargetFunction( targetOff,   NtkToCnfConverter::off  );
    converter.setBaseFunctions ( basesOff,    NtkToCnfConverter::off  );
  }
  // end setup cnf converter

  converter.convert();
}

void InterpolationEngine::addClauseA()
{
  if( !converter.cnfOn() ) return; // precondition
  std::cout << "add clause A\n";

  int lits[2];

  // setup sat solver
  satSolver = sat_solver_new();

  sat_solver_store_alloc( satSolver );
  sat_solver_setnvars   ( satSolver, converter.cnfOn()->nVars + converter.cnfOff()->nVars );
  // end setup sat solver

  // add clause A
  lits[0] = toLitCond( converter.literalsOn()[0], 0 );

  addClause( satSolver, converter.cnfOn() );
  sat_solver_addclause( satSolver, lits, lits + 1 );
  sat_solver_store_mark_clauses_a( satSolver );
  // end add clause A
}

void InterpolationEngine::addClauseB()
{
  if( !converter.cnfOff() || !satSolver ) return; // precondition
  std::cout << "add Clause B\n";

  int lits[3];

  // add clause B
  lits[0] = toLitCond( converter.literalsOff()[0], 0 );

  addClause( satSolver, converter.cnfOff() );
  sat_solver_addclause( satSolver, lits, lits + 1 );
  // end add clause B

  // add common variables clause
  for( int i = 0 ; i < basesOn.size() ; ++i )
  {
     lits[0] = toLitCond( converter.literalsOn ()[i+1], 0 );
     lits[1] = toLitCond( converter.literalsOff()[i+1], 1 );

     if( !sat_solver_addclause( satSolver, lits, lits + 2 ) )
       std::cout << "clause add error!\n";

     lits[0] = toLitCond( converter.literalsOn ()[i+1], 1 );
     lits[1] = toLitCond( converter.literalsOff()[i+1], 0 );

     if( !sat_solver_addclause( satSolver, lits, lits + 2 ) )
       std::cout << "clause add error!\n";
  }
  // end add common variables clause

  sat_solver_store_mark_roots( satSolver );
}

void InterpolationEngine::interpolation()
{
  if( !satSolver ) return; // precondition
  std::cout << "interpolation\n";

  Sto_Man_t   *proof;
  Inta_Man_t  *interMan;
  Vec_Int_t   *commonVariables;

  if( sat_solver_solve( satSolver, NULL, NULL, 0, 0, 0, 0  ) != l_False ) 
  {
    std::cout << "sat\n";
    return;
  };
  std::cout << "unsat\n";

  proof           = static_cast<Sto_Man_t*>( sat_solver_store_release( satSolver ) );
  commonVariables = Vec_IntAlloc( converter.literalsOn().size() - 1 );

  sat_solver_delete( satSolver );

  for( int i = 1 ; i < converter.literalsOn().size() ; ++i )
     Vec_IntPush( commonVariables, converter.literalsOn()[i] );

  satSolver     = NULL;
  interMan      = Inta_ManAlloc();
  mInterpolant  = convertAigToNtk( static_cast<Aig_Man_t*>( Inta_ManInterpolate( interMan, proof, 0, commonVariables, 0 ) ) );

  // release memory
  Inta_ManFree( interMan        );
  Vec_IntFree ( commonVariables );
  Sto_ManFree ( proof           );
  // end release memory
}

Abc_Ntk_t* InterpolationEngine::convertAigToNtk( Aig_Man_t *aig )
{
  Abc_Ntk_t *circuit = Abc_NtkAlloc( ABC_NTK_STRASH, ABC_FUNC_AIG, 1 );

  std::queue<Aig_Obj_t*> box;

  Abc_NtkSetName        ( circuit, const_cast<char*>( "patch" ) );
  Aig_ManIncrementTravId( aig );
  Aig_ManFanoutStart    ( aig );

  createPi    ( aig, circuit, box );
  buildCircuit( aig, circuit, box );

  Aig_ManFanoutStop( aig );
  return circuit;
}

void InterpolationEngine::createPi( Aig_Man_t *aig, Abc_Ntk_t *ntk, std::queue<Aig_Obj_t*> &box )
{
  for( int i = 0 ; i < Aig_ManCiNum( aig ) ; ++i )
  {
     Abc_Obj_t *pi = Abc_NtkCreatePi( ntk );
     Aig_Obj_t *ci = Aig_ManCi( aig, i );

     Abc_ObjAssignName( pi, Abc_ObjName( basesOn[i] ), NULL );
     Aig_ObjSetCopy( ci, reinterpret_cast<Aig_Obj_t*>( pi ) );
     Aig_ObjSetTravIdCurrent( aig, ci );
     box.push( ci );
  }
}

void InterpolationEngine::buildCircuit( Aig_Man_t *aig, Abc_Ntk_t *ntk, std::queue<Aig_Obj_t*> &box )
{
  Abc_Aig_t *manager = static_cast<Abc_Aig_t*>( ntk->pManFunc );

  // build circuit
  while( !box.empty() )
  {
    Aig_Obj_t *node = box.front();
    Aig_Obj_t *fanout;
    int       i;
    int       iFan;

    // build fanout
    Aig_ObjForEachFanout( aig, node, fanout, iFan, i )
    {
      if( Aig_ObjIsTravIdCurrent( aig, fanout ) ) continue;
      if( Aig_ObjIsCo( fanout ) )
      {
        Abc_Obj_t *po     = Abc_NtkCreatePo( ntk );
        Abc_Obj_t *fanin  = Abc_ObjRegular( reinterpret_cast<Abc_Obj_t*>( Aig_ObjCopy( node ) ) );

        Abc_ObjAddFanin   ( po, fanin );
        Abc_ObjAssignName ( po, Abc_ObjName( targetOn ), NULL );
        Aig_ObjSetTravIdCurrent( aig, fanout );
        continue;
      }
      if( !Aig_ObjCopy( Aig_ObjFanin0( fanout ) ) || !Aig_ObjCopy( Aig_ObjFanin1( fanout ) ) )
      {
        box.push( node );
        continue;
      }

      Abc_Obj_t *copy0    = reinterpret_cast<Abc_Obj_t*>( Aig_ObjChild0Copy( fanout ) );
      Abc_Obj_t *copy1    = reinterpret_cast<Abc_Obj_t*>( Aig_ObjChild1Copy( fanout ) );
      Abc_Obj_t *nodeNew  = Abc_AigAnd( manager, copy0, copy1 );

      Aig_ObjSetCopy( fanout, reinterpret_cast<Aig_Obj_t*>( nodeNew ) );
      Aig_ObjSetTravIdCurrent( aig, fanout );
      box.push( fanout );
    }
    // end build fanout

    box.pop();
  }
  // end build circuit

  Abc_AigCleanup( manager );
}

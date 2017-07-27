#include "ResourceAwarePatchGeneration.h"

//#define SHOW

using namespace std;

void ResourceAwarePatchGenerator::delete_unused_PO()
{
    cout<<"[INFO] delete unused PO ..."<<endl;
    
    set<Abc_Obj_t*> depend_to_target_Po;

    int i;
    Abc_Obj_t *pNode, *pPo;  
    char *name;
    ///////////////////////////////////////////////////////////////
	//                     remove Po on F                        //
    ///////////////////////////////////////////////////////////////	
    Abc_NtkForEachNode( initial_F, pNode, i ){
        if(Abc_NodeIsConst(pNode) == 1){
            name = Abc_ObjName( pNode );
            if( name[0] == 't' ){
                trav_Po_add_to_set( pNode, depend_to_target_Po );
            }
        }	
    }
	
	///////////////////////////////////////////////////////////////
	//                 remove the same Po on G                   //
    ///////////////////////////////////////////////////////////////
    //memorize all Po in G
	map< string, Abc_Obj_t* > g_Po_name_to_gate;
	Abc_NtkForEachPo( initial_G, pPo, i ){
		name = Abc_ObjName( pPo );
		g_Po_name_to_gate[ string(name) ] = pPo;
	}


    //delete unuse Po
    Abc_NtkForEachPo( initial_F, pPo, i ){
        set<Abc_Obj_t*>::iterator it = depend_to_target_Po.find( pPo );
        if( it == depend_to_target_Po.end() ){
            name = Abc_ObjName(pPo);
            Abc_NtkDeleteObj( g_Po_name_to_gate[ string(name) ] );//G
			Abc_NtkDeleteObj( pPo );//F

            //cout<<"this Po is not in t_i fanout cone"<<endl;
		}
        else{
            depend_to_target_Po.erase( it );//F

			//cout<<"this Po is in t_i fanout cone"<<endl;
		}
    }
	cout<<"G before strash "<< Abc_NtkObjNum(initial_G)<<endl;

    Abc_Ntk_t* temp_Ntk;
	temp_Ntk = Abc_NtkStrash( initial_G, 0, 1, 0 );
    Abc_NtkDelete( initial_G );
    initial_G = temp_Ntk;
    temp_Ntk = NULL;

	cout<<"G after strash "<< Abc_NtkObjNum(initial_G)<<endl;

    #ifdef SHOW
	  char Command[1000];
      sprintf( Command, "show" );
      if ( Cmd_CommandExecute( pAbc, Command ) )
      {
          fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
          return ;
      }
    #endif

	cout<<"Abc_NtkObjNum "<< Abc_NtkObjNum(initial_F)<<endl;
	cout<<"Abc_NtkObjNum G "<< Abc_NtkObjNum(initial_G)<<endl;
}

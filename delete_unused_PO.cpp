#include "ResourceAwarePatchGeneration.h"

#include <map>
//#define SHOW

using namespace std;

void ResourceAwarePatchGenerator::delete_unused_PO()
{
    cout<<"[INFO] delete unused PO ..."<<endl;

    int i;
    Abc_Obj_t *pNode, *pPo, *pPi;  
    char *name;
    

    //delete unuse Po on F and G
    Abc_NtkForEachPo( initial_F, pPo, i ){
        set<Abc_Obj_t*>::iterator it = depend_to_target_Po.find( pPo );
        if( it == depend_to_target_Po.end() ){ //it isn't t fanout Po
            //cout<<"Abc_ObjCopy: "<<Abc_ObjCopy(pPo)<<endl;
            Abc_NtkDeleteObj( Abc_ObjCopy( pPo ) ); //G
			Abc_NtkDeleteObj( pPo );//F

            //cout<<"this Po is not in t_i fanout cone"<<endl;
		}
        else{  //it is t fanout Po
            ///////////********************
            depend_to_target_Po.erase( it ); //F
            ///////////********************
			//cout<<"this Po is in t_i fanout cone"<<endl;
		}
    }

    //delete unuse Pi on G
    Abc_NtkForEachPi( initial_G, pPi, i ){
        set<Abc_Obj_t*>::iterator it = depend_to_target_Pi.find( pPo );
        if( it == depend_to_target_Pi.end() ){ //it isn't t fanout Po
            Abc_NtkDeleteObj( pPi ); //G
        }
        else{  //it is t fanout Po
            depend_to_target_Pi.erase( it ); //G
        }
    }


	cout<<"G before strash "<< Abc_NtkObjNum(initial_G)<<endl;

    Abc_Ntk_t* temp_Ntk;
	//temp_Ntk = Abc_NtkStrash( initial_G, 0, 1, 0 );
    //Abc_NtkDelete( initial_G );
    //initial_G = temp_Ntk;
    //temp_Ntk = NULL;

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
    

    Io_WriteBlifLogic( initial_F, "F_network.blif", 0 );
    Io_WriteBlifLogic( initial_G, "G_network.blif", 0 );
}

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
		//cout<<"18"<<endl;
        set<Abc_Obj_t*>::iterator it = depend_to_target_Po.find( pPo );
        if( it == depend_to_target_Po.end() ){ //it isn't t fanout Po
            //cout<<"Abc_ObjCopy: "<<Abc_ObjCopy(pPo)<<endl;
            Abc_NtkDeleteObj( Abc_ObjCopy( pPo ) ); //G
			Abc_NtkDeleteObj( pPo );//F

            //cout<<"  "<<Abc_ObjName(pPo)<<"Po is not in t_i fanout cone"<<endl;
		}
        else{  //it is t fanout Po
            ///////////********************
            depend_to_target_Po.erase( it ); //F
            ///////////********************
			//cout<<"  "<<Abc_ObjName(pPo)<<"Po is in t_i fanout cone"<<endl;
		}
    }
    //for(int i=0;i<gate_list.size();++i)
    //    cout<<"gate_list_print: "<<gate_list[i]->gate<<endl;
    //Abc_Ntk_t* temp_F = Abc_NtkToNetlist( initial_F );
    //Abc_NtkToAig( temp_F );
    //Io_WriteVerilog( temp_F, "F_delete_PO.v" );
    //Abc_Ntk_t* temp_G = Abc_NtkToNetlist( initial_G );
    //Abc_NtkToAig( temp_G );
    //Io_WriteVerilog( temp_G, "G_delete_PO.v" );



	//cout<<"34"<<endl;
	
    //delete unuse Pi on G
    /*Abc_NtkForEachPi( initial_G, pPi, i ){
		//cout<<"Pi="<<pPi<<endl;
        set<Abc_Obj_t*>::iterator it = depend_to_target_Pi.find( pPi );
		//cout<<"it="<<(*it)<<endl;
        if( it == depend_to_target_Pi.end() ){ //it isn't t fanout Po
            //cout<<"not find depend_to_target_Pi: "<<endl;

            Abc_NtkDeleteObj( pPi ); //G
        }
        else{  //it is t fanout Po
            //cout<<"find depend_to_target_Pi: "<<endl;
            //cout<<Abc_ObjName(*it)<<endl;
            depend_to_target_Pi.erase( it ); //G
        }
    }*/


	cout<<"  G before strash "<< Abc_NtkObjNum(initial_G)<<endl;
    Abc_Ntk_t* temp_G = Abc_NtkToNetlist( initial_G );
    Abc_NtkToAig( temp_G );
    //Io_WriteVerilog( temp_G, "G_before_strash.v" );
    //Abc_Ntk_t* temp_Ntk;
	initial_G = Abc_NtkStrash( initial_G, 0, 1, 0 );
    //Abc_NtkDelete( initial_G );
    //initial_G = temp_Ntk;
    //temp_Ntk = NULL;

	cout<<"  G after strash "<< Abc_NtkObjNum(initial_G)<<endl;

    #ifdef SHOW
	  char Command[1000];
      sprintf( Command, "show" );
      if ( Cmd_CommandExecute( pAbc, Command ) )
      {
          fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
          return ;
      }
    #endif

	cout<<"  Abc_NtkObjNum "<< Abc_NtkObjNum(initial_F)<<endl;
	//cout<<"  Abc_NtkObjNum G "<< Abc_NtkObjNum(initial_G)<<endl;
    
    /*Abc_Ntk_t* temp_F = Abc_NtkToNetlist( initial_F );
    Abc_NtkToAig( temp_F );
    Io_WriteVerilog( temp_F, "F_delete_PO_PI.v" );
    temp_G = Abc_NtkToNetlist( initial_G );
    Abc_NtkToAig( temp_G );
    Io_WriteVerilog( temp_G, "G_delete_PO_PI.v" );
*/
/*
    for(int i=0;i<t_list.size();++i){
        cout<<"t= "<<Abc_ObjName( t_list[i] )<<endl;

        cout<<"  t fanout po: size= "<<t_Po_list[i].size()<<endl;
        for(set<Abc_Obj_t*>::iterator j=t_Po_list[i].begin();j!=t_Po_list[i].end();++j)
            cout<<"      "<<Abc_ObjName( *j )<<',';
        cout<<endl;

        cout<<"  t fanout po fanin pi: size= "<<t_Pi_list[i].size()<<endl;
        for(set<Abc_Obj_t*>::iterator j=t_Pi_list[i].begin();j!=t_Pi_list[i].end();++j)
            cout<<"      "<<Abc_ObjName( *j )<<',';
        cout<<endl;
    }
    */
    
    //for(int i=0;i<gate_list.size();++i)
    //    cout<<"gate_list_print: "<<gate_list[i]->gate<<endl;
}

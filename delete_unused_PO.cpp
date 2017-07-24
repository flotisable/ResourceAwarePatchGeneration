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
	
	//cout <<"    F Po num:"<<Abc_NtkPoNum(initial_F) << endl;
    //cout <<"    F Obj num:"<<Abc_NtkObjNum(initial_F) << endl;
	
    //trav all target
	//cout<<"Abc_NtkNodeNum(initial_F)"<<Abc_NtkNodeNum(initial_F)<<endl;
	//Abc_NtkForEachNode( initial_F, pNode, i )
	//	cout<<"node type"<<pNode->Type<<endl;
	//cout<<"Abc_NtkObjNum(initial_F)"<<Abc_NtkObjNum(initial_F)<<endl;
	//Abc_NtkForEachObj( initial_F, pNode, i )
	//	cout<<"obj type"<<pNode->Type<<endl;
	
    Abc_NtkForEachNode( initial_F, pNode, i ){
		//cout<<"***i"<<i<<endl;
        if(Abc_NodeIsConst(pNode)==1){
            name = Abc_ObjName( pNode );
            if( name[0] == 't' ){
                //cout<<"name: "<<name<<" is start to trav. output"<<endl;
                trav_Po_add_to_set( pNode, depend_to_target_Po );
            }
        }	
    }
	
    //delete unuse Po
    Abc_NtkForEachPo( initial_F, pPo, i ){
        set<Abc_Obj_t*>::iterator it = depend_to_target_Po.find( pPo );
        if( it == depend_to_target_Po.end() )
            Abc_NtkDeleteObj( pPo );
        else
            depend_to_target_Po.erase( it );
    }
    
    #ifdef SHOW
	char Command[1000];
	
    sprintf( Command, "show" );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        return ;
    }
    #endif
}

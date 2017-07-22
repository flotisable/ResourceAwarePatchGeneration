#include "ResourceAwarePatchGeneration.h"

//#define SHOW

using namespace std;

void ResourceAwarePatchGenerator::delete_unused_PO()
{
    cout<<"delete unused PO ..."<<endl;
    
    
    set<Abc_Obj_t*> depend_to_target_Po;

    int i;
    Abc_Obj_t *pNode, *pPo;
    char *name;
    //trav all target
    Abc_NtkForEachNode( initial_F, pNode, i ){
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
    sprintf( Command, "show" );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        return ;
    }
    #endif
}

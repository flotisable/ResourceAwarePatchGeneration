#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::recursive_trav_Pi_add_to_set( Abc_Obj_t* pNode, set<Abc_Obj_t*>& pSet )
{
    Abc_Obj_t * pFanin;
    int i;
    //cout<<"recursive_trav_Pi_add_to_set..."<<endl;
    if( Abc_ObjIsPi(pNode) ){
        //cout<<"yyyyyyyyyyyyyaaaaaaaaaaaaaa"<<endl;
        pSet.insert(pNode);
        return;
    }
    //assert( !Abc_ObjIsNet(pNode) );
    // if this node is already visited, skip 
    if ( Abc_NodeIsTravIdCurrent( pNode ) ){
        //cout<<"what?!"<<endl;
        return;
    }
    // mark the node as visited
    Abc_NodeSetTravIdCurrent( pNode );
    //cout<<"name:                           fanin"<<Abc_ObjName(pNode)<<endl;
    //pNode = Abc_ObjFanin0(pNode);
    //cout<<"Abc_ObjName fanin0 node:         "<<Abc_ObjName(pNode)<<endl;
    Abc_ObjForEachFanin( pNode, pFanin, i )
        recursive_trav_Pi_add_to_set( pFanin, pSet );
}

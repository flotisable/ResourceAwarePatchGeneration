#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::recursive_trav_Po_add_to_set( Abc_Obj_t* pNode, set<Abc_Obj_t*> pSet )
{
    //cout<<"recursive_trav_Po_add_to_set..."<<endl;
    Abc_Obj_t * pFanout;
    int i;
    
    if( Abc_ObjIsPo(pNode) ){
        pSet.insert(pNode);
        return;
    }
    //assert( !Abc_ObjIsNet(pNode) );
    // if this node is already visited, skip
    if ( Abc_NodeIsTravIdCurrent( pNode ) )
        return;
    // mark the node as visited
    Abc_NodeSetTravIdCurrent( pNode );
    // skip the CI
    ////if ( Abc_ObjIsCo(pNode) )
    ////    return;
    //assert( Abc_ObjIsNode( pNode ) );
    // visit the transitive fanin of the node
    pNode = Abc_ObjFanout0(pNode);
    Abc_ObjForEachFanout( pNode, pFanout, i )
        recursive_trav_Po_add_to_set( pFanout, pSet );
}

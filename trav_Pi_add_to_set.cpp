#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::recursive_trav_Pi_add_to_set( Abc_Obj_t* pNode, set<Abc_Obj_t*> pSet )
{
    Abc_Obj_t * pFanin;
    int i;
    
    if( Abc_ObjIsPi(pNode) ){
        pSet.insert(pNode);
        return;
    }
    //assert( !Abc_ObjIsNet(pNode) );
    // if this node is already visited, skip
    if ( Abc_NodeIsTravIdCurrent( pNode ) )
        return;
    // mark the node as visited
    Abc_NodeSetTravIdCurrent( pNode );

    pNode = Abc_ObjFanin0(pNode);
    Abc_ObjForEachFanin( pNode, pFanin, i )
        recursive_trav_Pi_add_to_set( pFanin, pSet );
}

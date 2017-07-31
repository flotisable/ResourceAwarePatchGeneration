#include "ResourceAwarePatchGeneration.h"

using namespace std;

void ResourceAwarePatchGenerator::traverse_t_PI_and_PO()
{
	int i;
	Abc_Obj_t* pPo;

	cout<<"[INFO] traverse_t_PI_and_PO ..."<<endl;

	///////////////////////////////////////////////////////////////
	//              traverse all t fanout Po on F                //
    ///////////////////////////////////////////////////////////////	
    Abc_NtkIncrementTravId( initial_F );
    for( i = 0; i < t_list.size(); ++i){
        set<Abc_Obj_t*>& cur_t_Po_list = t_Po_list[i];						//cout<<"17"<<t_list[i]<<endl;
        recursive_trav_Po_add_to_set( t_list[i] , t_Po_list[i] );						//cout<<"18"<<endl;
        depend_to_target_Po.insert( t_Po_list[i].begin(), t_Po_list[i].end() );					//cout<<"19"<<endl;
    }
//cout<<"21"<<endl;
    //construct Po_fanin_Pi
    Abc_NtkIncrementTravId( initial_G );

    set<Abc_Obj_t*> current_Po_fanin_Pi_set;

    Abc_NtkForEachPo( initial_G, pPo, i ){
    	recursive_trav_Pi_add_to_set( pPo, current_Po_fanin_Pi_set );

        depend_to_target_Pi.insert( current_Po_fanin_Pi_set.begin(), current_Po_fanin_Pi_set.end() );                 //cout<<"19"<<endl;

    	Po_fanin_Pi[ pPo ] = current_Po_fanin_Pi_set;
        //cout<<"current_Po_fanin_Pi_set.size(): "<<current_Po_fanin_Pi_set.size()<<endl;
        current_Po_fanin_Pi_set.clear();
        //cout<<"33"<<endl;
    }
//cout<<"30"<<endl;

    ////////////////////////////////////////////////////////////////////////
	// traverse all t fanout Po on F and then find the fanin cone Pi on G //
    ////////////////////////////////////////////////////////////////////////	

    for( i = 0; i < t_list.size(); ++i){
    	set<Abc_Obj_t*>& cur_t_Po_list = t_Po_list[i];
    	set<Abc_Obj_t*>::iterator set_iter;
    	for( set_iter = cur_t_Po_list.begin(); set_iter != cur_t_Po_list.end(); ++set_iter){
    		t_Pi_list[i].insert( ( Po_fanin_Pi[ *set_iter ] ).begin(), ( Po_fanin_Pi[ *set_iter ] ).end() );
    	}
    }
//cout<<"43"<<endl;

}

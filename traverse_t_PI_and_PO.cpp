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
    
    
    for( i = 0; i < t_list.size(); ++i){
        Abc_NtkIncrementTravId( initial_F );
        set<Abc_Obj_t*>& cur_t_Po_list = t_Po_list[i];						//cout<<"17"<<t_list[i]<<endl;
        recursive_trav_Po_add_to_set( t_list[i] , t_Po_list[i] );						//cout<<"18"<<endl;
        depend_to_target_Po.insert( t_Po_list[i].begin(), t_Po_list[i].end() );					//cout<<"19"<<endl;
    }
//cout<<"21"<<endl;
    //construct Po_fanin_Pi
    

    set<Abc_Obj_t*> current_Po_fanin_Pi_set;

    Abc_NtkForEachPo( initial_G, pPo, i ){
        Abc_NtkIncrementTravId( initial_G );
        pair< Abc_Obj_t*, set<Abc_Obj_t*> > temp_pair;

    	temp_pair.first = pPo;

        recursive_trav_Pi_add_to_set( pPo, temp_pair.second );

        Po_fanin_Pi.insert( temp_pair );
        //cout<<"33"<<endl;

        //cout<<"temp_pair.size()"<<temp_pair.second.size()<<endl;
    }
    //cout<<"40"<<endl;
    //Abc_NtkForEachPo( initial_G, pPo, i )
        //cout<<"Po_fanin_Pi[pPo].size()"<<Po_fanin_Pi[pPo].size()<<endl;
//cout<<"30"<<endl;

    ////////////////////////////////////////////////////////////////////////
	// traverse all t fanout Po on F and then find the fanin cone Pi on G //
    ////////////////////////////////////////////////////////////////////////	
    map< Abc_Obj_t*, set<Abc_Obj_t*> >::iterator map_iter;

    for( i = 0; i < t_list.size(); ++i){
    	set<Abc_Obj_t*>& cur_t_Po_list = t_Po_list[i];
    	set<Abc_Obj_t*>::iterator set_iter;

        //cout<<"cur_t_Po_list: "<<cur_t_Po_list.size()<<endl;
    	for( set_iter = cur_t_Po_list.begin(); set_iter != cur_t_Po_list.end(); ++set_iter){
            map_iter = Po_fanin_Pi.find( Abc_ObjCopy(*set_iter) );
            //assert(map_iter!=Po_fanin_Pi.end());
    		t_Pi_list[i].insert( map_iter->second.begin(), map_iter->second.end() );
			depend_to_target_Pi.insert( map_iter->second.begin(), map_iter->second.end() );                 //cout<<"19"<<endl;

    	}
        //cout<<"depend_to_target_Pi.size: "<<depend_to_target_Pi.size()<<endl;
        //for( set_iter=cur_t_Po_list.begin();set_iter!= cur_t_Po_list.end();++set_iter)
        //    cout<<"  "<<Abc_ObjName(*set_iter)<<',';
        //cout<<endl;
    }
//cout<<"43"<<endl;

}

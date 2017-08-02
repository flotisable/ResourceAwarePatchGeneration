#include "ResourceAwarePatchGeneration.h"
#include <fstream>
#include <map> 

using namespace std;

void ResourceAwarePatchGenerator::read_weight()
{
	cout<<"[INFO] read_weight ..."<<endl;
	ifstream weight_file;
	string net_name;
	int net_weight;
	map< string, int > name_to_weight;
	///////////////////////////////////////////////////////////////
	//                    construct map                          //
	///////////////////////////////////////////////////////////////
	weight_file.open( in_W_file.c_str() );
	
	while( weight_file>>net_name>>net_weight ){
		name_to_weight[net_name] = net_weight;
		
		#ifdef DEBUG
		  cout<<"  net_name: "<<net_name<<endl;
		  cout<<"  net_weight: "<<net_weight<<endl;
		#endif
	}
	weight_file.close();
	
	
	///////////////////////////////////////////////////////////////
	//              connect weight and network node              //
	///////////////////////////////////////////////////////////////
	int i;
	Abc_Obj_t *pObj;
	
	Abc_NtkForEachObj( initial_F, pObj, i ){
		//visit all node in ntk
		//use name to check whether it is in the map
		if( Abc_ObjIsPo( pObj ) )//because output net will cause 2 obj
			continue;

		char *name = Abc_ObjName( pObj );
		
		map< string, int >::iterator iter;
	

		if( ( iter = name_to_weight.find( string(name) ) ) != name_to_weight.end()){    //this net weight is not defined
			Weight_gate* temp = new Weight_gate( pObj, string(name), iter->second );
			
			name_to_weight.erase( iter );
			gate_list.push_back( temp );
			
			#ifdef DEBUG
			  cout<<"  gate_list "<<i<<endl;
			  cout<<"    gate name: "<<Abc_ObjName( temp->gate )<<endl;
			  cout<<"    gate weight:"<<temp->weight<<endl;
			#endif
		}

		if( Abc_NodeIsConst( pObj )==1 && name[0] == 't' )
			t_list.push_back( pObj );
	}

	t_Pi_list.resize( t_list.size() );
	t_Po_list.resize( t_list.size() );

	//cout<<"gate_list.size()=="<<gate_list.size()<<endl;



}

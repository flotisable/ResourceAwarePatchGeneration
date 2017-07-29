#include "ResourceAwarePatchGeneration.h"
#include <map>

//#define DEBUG_READ_FILE

using namespace std;

void ResourceAwarePatchGenerator::read_file()
{   
    cout<<"[INFO] read_file ..."<<endl;

    char Command[1000];
    
    /////////////////////////////////////////////////////////////////
    //                           read G file                       //
    /////////////////////////////////////////////////////////////////
    sprintf( Command, "read %s", in_G_file.c_str() );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
    }
    initial_G = Abc_NtkDup(Abc_FrameReadNtk(pAbc));
    
	
    /////////////////////////////////////////////////////////////////
    //                           read F file                       //
    /////////////////////////////////////////////////////////////////
    sprintf( Command, "read %s", in_F_file.c_str() );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
    }
    
    initial_F = Abc_FrameReadNtk( pAbc );
	
	int i;
	Abc_Obj_t* pObj;
	map< string, Abc_Obj_t*> name_to_Pi, name_to_Po;
	map< string, Abc_Obj_t*>::iterator iter;
	
	
	Abc_NtkForEachPo( initial_G, pObj, i )
		name_to_Po[ string(Abc_ObjName(pObj)) ] = pObj;
	Abc_NtkForEachPi( initial_G, pObj, i )
		name_to_Pi[ string(Abc_ObjName(pObj)) ] = pObj;
		
	Abc_NtkForEachPo( initial_F, pObj, i )
		if( ( iter = name_to_Po.find( Abc_ObjName(pObj) ) ) != name_to_Po.end() ){ //find
			Abc_ObjSetCopy( iter->second, pObj );
			Abc_ObjSetCopy( pObj, iter->second );
			name_to_Po.erase( iter );
		}
		#ifdef DEBUG_READ_FILE
		  else
			  cout<<"F's output "<<Abc_ObjName(pObj)<<" is not found in G's Po"<<endl;
		#endif
		
	Abc_NtkForEachPi( initial_F, pObj, i )
		if( ( iter = name_to_Pi.find( Abc_ObjName(pObj) ) ) != name_to_Pi.end() ){ //find
			Abc_ObjSetCopy( iter->second, pObj );
			Abc_ObjSetCopy( pObj, iter->second );
			name_to_Pi.erase( iter );
		}
		#ifdef DEBUG_READ_FILE
		  else
			  cout<<"F's input "<<Abc_ObjName(pObj)<<" is not found in G's Pi"<<endl;
		#endif


}

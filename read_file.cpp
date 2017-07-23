#include "ResourceAwarePatchGeneration.h"

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
}
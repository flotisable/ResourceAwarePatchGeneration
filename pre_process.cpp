#include "ResourceAwarePatchGeneration.h"


using namespace std;


void ResourceAwarePatchGeneraor:: pre_process()
{
    char * pFileName;
    char Command[1000];
    sprintf( Command, "read %s", pFileName );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        return 1;
    }

}

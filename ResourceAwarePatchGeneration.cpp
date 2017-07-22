#include "ResourceAwarePatchGeneration.h"
#include <iostream>

#include "base/main/main.h"

using namespace std;

ResourceAwarePatchGenerator::ResourceAwarePatchGenerator(string F_file, string G_file, string W_file, string o_p_file, string o_F_file)
{
  Abc_Start();
  pAbc = Abc_FrameGetGlobalFrame();


  in_F_file       = F_file;
  out_Patch_file  = o_p_file;
  out_F_file      = o_F_file;


  read_file(F_file,G_file,W_file);
} 

ResourceAwarePatchGenerator::~ResourceAwarePatchGenerator()
{
  Abc_Stop();
}


void ResourceAwarePatchGenerator::read_file(string F_file,string G_file,string W_file)
{

    	char Command[1000];
    	sprintf( Command, "read %s", F_file.c_str() );
    	if ( Cmd_CommandExecute((Abc_Frame_t*) pAbc, Command ) )
    	{
        	fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
    	}
	initial_F = Abc_NtkDup(Abc_FrameReadNtk((Abc_Frame_t*)pAbc));
	cout << Abc_NtkPoNum(initial_F) << endl;
	
    	sprintf( Command, "read %s", G_file.c_str() );
    	if ( Cmd_CommandExecute((Abc_Frame_t*) pAbc, Command ) )
    	{
        	fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
	}
	initial_G = Abc_FrameReadNtk((Abc_Frame_t*)pAbc);
	
}



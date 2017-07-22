#include <iostream>
#include "ResourceAwarePatchGeneration"
#include <string>


using  namespace std;

ResourceAwarePatchGenerator::read_file(string F_file,string G_file,string W_file)
{

	int argc=1;
	char** argv[1];	
	argv[1]=F_file.c_str();
	IoCommandReadVerilog ( pAbc, argc, argv );
	







}

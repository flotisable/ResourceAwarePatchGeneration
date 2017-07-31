#include "ResourceAwarePatchGeneration.h"

#include <fstream>
#include <string>

extern "C"
{
#include "base/io/ioAbc.h"
#include "base/main/main.h"
}

void ResourceAwarePatchGenerator::write_patch( const string patchFileName, const string patchedFileName )
{
  using namespace std;

  const string patchName = "patch";

  // write patch circuit
  if( interpolant == NULL ) return;

  Io_WriteVerilog( interpolant, const_cast<char*>( patchFileName.c_str() ) );
  // end write patch circuit

  // write patched circuit
  ifstream  fileFv   ( in_F_file.c_str()        );
  ofstream  fileFvOut( patchedFileName.c_str()  );
  string    buffer;

  while( getline( fileFv, buffer ) )
  {
    if( buffer.find( "endmodule" ) != string::npos )
    {
      int       i;
      Abc_Obj_t *pObj;

      fileFvOut << patchName << " p0(";

      // write patch output name
      Abc_NtkForEachPo( interpolant, pObj, i )
      {
        Abc_Obj_t *pNet = Abc_ObjFanin0( pObj );

        fileFvOut << Abc_ObjName( pNet ) << ",";
      }
      // end write patch output name
      // write patch input name
      Abc_NtkForEachPi( interpolant, pObj, i )
      {
        Abc_Obj_t *pNet = Abc_ObjFanout0( pObj );

        fileFvOut << Abc_ObjName( pNet );
        if( i + 1 != Abc_NtkPiNum( interpolant ) )
          fileFvOut << ",";
      }
      // end write patch input name

      fileFvOut << ");\n";
    }
    fileFvOut << buffer << endl;
  }
  // end write patched circuit
}

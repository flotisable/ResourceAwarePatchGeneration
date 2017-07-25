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

  // write patch circuit
  Abc_Frame_t   *pAbc = Abc_FrameGetGlobalFrame();
  Abc_Ntk_t     *pNtk = Abc_FrameReadNtk( pAbc );
  const string  patchName = "patch";

  if( pNtk == NULL ) return;

  Abc_NtkSetName( pNtk, const_cast<char*>( patchName.c_str() ) );

  Io_WriteVerilog( pNtk, const_cast<char*>( patchFileName.c_str() ) );
  // end write patch circuit

  // write patched circuit
  ifstream  fileFv   ( in_F_file.c_str()   );
  ofstream  fileFvOut( patchedFileName.c_str()  );
  string    buffer;

  while( getline( fileFv, buffer ) )
  {
    if( buffer.find( "endmodule" ) != string::npos )
    {
      int       i;
      Abc_Obj_t *pObj;
      Abc_Obj_t *pNet;

      fileFvOut << patchName << " p0(";

      // write patch output name
      Abc_NtkForEachPo( pNtk, pObj, i )
      {
        pNet = Abc_ObjFanin0( pObj );

        fileFvOut << Abc_ObjName( pNet ) << ",";
      }
      // end write patch output name
      // write patch input name
      Abc_NtkForEachPi( pNtk, pObj, i )
      {
        pNet = Abc_ObjFanout0( pObj );

        fileFvOut << Abc_ObjName( pNet );
        if( i + 1 != Abc_NtkPiNum( pNtk ) )
          fileFvOut << ",";
      }
      // end write patch input name

      fileFvOut << ");\n";
    }
    fileFvOut << buffer << endl;
  }
  // end write patched circuit
}

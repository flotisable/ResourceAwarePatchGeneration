#include "ResourceAwarePatchGeneration.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cctype>

extern "C"
{
#include "base/io/ioAbc.h"
#include "base/main/main.h"
}

void formatPatch( const string &patchFileName, const string &patchTempFileName );

void ResourceAwarePatchGenerator::write_patch( const string patchFileName, const string patchedFileName )
{
  using namespace std;

  const string patchName          = "patch";
  const string patchTempFileName  = "patchTemp.v";

  // write patch circuit
  if( interpolant == NULL ) return;

  Io_WriteVerilog( interpolant, const_cast<char*>( patchTempFileName.c_str() ) );
  formatPatch( patchFileName, patchTempFileName );
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

void formatPatch( const string &patchFileName, const string &patchTempFileName )
{
  using namespace std;

  const string patchName = "patch";

  ifstream      inFile ( patchTempFileName.c_str() );
  ofstream      outFile( patchFileName.c_str()     );
  string        buffer;
  stringstream  stringStream;

  vector<string> outputSignals;
  vector<string> inputSignals;

  // find module keyword
  while( getline( inFile, buffer ) )
    if( buffer.find( "module" ) != string::npos )
    {
      int rpos;

      buffer = buffer.substr( buffer.find( '(' ) + 1 );

      while( ( rpos = buffer.find( ')' ) ) == string::npos )
      {
        stringStream << buffer;

        if( !getline( inFile, buffer ) ) break;
      }
      stringStream << buffer.substr( 0, rpos );
      break;
    }
  // end find module keyword

  // parse module arguments
  while( stringStream >> buffer )
  {
    for( int i = 1 ; i < buffer.size() ; ++i )
       if( !isalnum( buffer[i] ) )
       {
         buffer = buffer.substr( 0, i );
         break;
       }

    switch( buffer[0] )
    {
      case 'g': inputSignals.push_back( buffer );   break;
      case 't': outputSignals.push_back( buffer );  break;
      default:  inputSignals.push_back( buffer );   break;
    }
  }
  stringStream.clear();
  // end parse module arguments

  // output module line
  outFile << "module " << patchName << "(";
  for( int i = 0 ; i < outputSignals.size() ; ++i )
     outFile << outputSignals[i] << ", ";
  for( int i = 0 ; i < inputSignals.size() ; ++i )
  {
     outFile << inputSignals[i];
     if( i + 1 != inputSignals.size() ) outFile << ", ";
  }
  outFile << ");\n";

  outputSignals.clear();
  inputSignals.clear();
  // end output module line

  // find gate
  while( getline( inFile, buffer ) )
  {
    int lpos; // position before  substring  
    int rpos; // position after   substring

    // check if this line contain gate
    if( buffer.find( '(' ) == string::npos )
    {
      outFile << buffer << "\n";
      continue;
    }
    // end check if this line contain gate

    stringStream.str( buffer );

    // decide primitive gate
    stringStream >> buffer;

    if      ( buffer.find( "INV"  ) != string::npos ) outFile << "  not ";
    else if ( buffer.find( "BUF"  ) != string::npos ) outFile << "  buf ";
    else if ( buffer.find( "NOR"  ) != string::npos ) outFile << "  nor ";
    else if ( buffer.find( "NAND" ) != string::npos ) outFile << "  nand ";
    else if ( buffer.find( "OR"   ) != string::npos ) outFile << "  or ";
    else if ( buffer.find( "AND"  ) != string::npos ) outFile << "  and ";
    else if ( buffer.find( "XOR"  ) != string::npos ) outFile << "  xor ";
    else if ( buffer.find( "XNOR" ) != string::npos ) outFile << "  xnor ";
    else                                              outFile << "  " << buffer << " ";
    // end decide primitive gate

    // get gate arguments
    buffer  = stringStream.str();
    lpos    = buffer.find( '(' );
    rpos    = buffer.rfind( ')' );
    buffer  = buffer.substr( lpos + 1, rpos - lpos - 1 );
    // end get gate arguments

    stringStream.str( buffer );

    // parse gate arguments
    while( stringStream >> buffer )
    {
      string signalName;

      lpos        = buffer.find( '.' );
      rpos        = buffer.rfind( ')' );
      buffer      = buffer.substr( lpos + 1, rpos - lpos - 1 );
      signalName  = buffer.substr( buffer.find( '(' ) + 1 );

      if( buffer[0] == 'Y' )  outputSignals.push_back ( signalName );
      else                    inputSignals.push_back  ( signalName );
    }
    stringStream.clear();
    // end parse gate arguments

    // output gate line
    outFile << "(";

    for( int i = 0 ; i < outputSignals.size() ; ++i )
       outFile << outputSignals[i] << ", ";
    for( int i = 0 ; i < inputSignals.size() ; ++i )
    {
       outFile << inputSignals[i];
       if( i + 1 != inputSignals.size() ) outFile << ", ";
    }
    outFile << ");\n";

    outputSignals.clear();
    inputSignals.clear();
    // end output gate line
  }
  // end find gate
}

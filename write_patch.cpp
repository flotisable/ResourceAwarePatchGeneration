#include "ResourceAwarePatchGeneration.h"

// include standard library
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
// end include standard library

extern "C"
{
#include "base/io/ioAbc.h"
#include "base/main/main.h"
}

// local function declaration
void formatPatch        ( const string &patchFileName, const string &patchTempFileName );
void writePatchedCircuit( const string &patchedFileName, const string & referencedFileName, Abc_Ntk_t *circuit );

inline string parseName     ( const string &buffer    );
inline string gateType      ( const string &gateName  );
inline string getArgument   ( const string &buffer    );
void          printSignals  ( ostream & output, const vector<string> &inputSignals, const vector<string> &outputSignals );
inline void   initContainers( stringstream &stringStream, vector<string> &inputSignals, vector<string> &outputSignals );
// end local function declaration

void ResourceAwarePatchGenerator::write_patch( const string patchFileName, const string patchedFileName )
{
  if( !interpolant ) return; // precondition

  const string patchTempFileName = "patchTemp.v";

  // write patch circuit
  Io_WriteVerilog( interpolant, const_cast<char*>( patchTempFileName.c_str() ) );
  formatPatch( patchFileName, patchTempFileName );
  // end write patch circuit

  writePatchedCircuit( patchedFileName, in_F_file, interpolant );
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

      do
      {
        stringStream << buffer;

      }while( getline( inFile, buffer ) && ( rpos = buffer.find( ')' ) ) == string::npos );

      stringStream << buffer.substr( 0, rpos );
      break;
    }
  // end find module keyword

  // parse module arguments
  while( stringStream >> buffer )
  {
    buffer = parseName( buffer );

    // check the signal is output or input
    switch( buffer[0] )
    {
      case 'g': inputSignals.push_back( buffer );   break;
      case 't': outputSignals.push_back( buffer );  break;
      default:  inputSignals.push_back( buffer );   break;
    }
    // end check the signal is output or input
  }
  // end parse module arguments

  // output module line
  outFile << "module " << patchName;
  printSignals( outFile, inputSignals, outputSignals );
  // end output module line

  // find gate
  while( getline( inFile, buffer ) )
  {
    // check if this line contain gate
    if( buffer.find( '(' ) == string::npos )
    {
      outFile << buffer << "\n";
      continue;
    }
    // end check if this line contain gate

    initContainers( stringStream, inputSignals, outputSignals );

    outFile << gateType( buffer );

    stringStream.str( getArgument( buffer ) );

    // parse gate arguments
    while( stringStream >> buffer )
    {
      string signalName = getArgument( buffer );

      if( buffer[1] == 'Y' )  outputSignals.push_back ( signalName );
      else                    inputSignals.push_back  ( signalName );
    }
    // end parse gate arguments

    printSignals( outFile, inputSignals, outputSignals );
  }
  // end find gate
}

void writePatchedCircuit( const string &patchedFileName, const string & referencedFileName, Abc_Ntk_t *circuit )
{
  const string patchName = "patch";

  ifstream  refFile( referencedFileName.c_str() );
  ofstream  outFile( patchedFileName.c_str()    );
  string    buffer;

  while( getline( refFile, buffer ) )
  {
    // write patch line
    if( buffer.find( "endmodule" ) != string::npos )
    {
      int       i;
      Abc_Obj_t *pObj;

      outFile << patchName << " p0(";

      // write patch output name
      Abc_NtkForEachPo( circuit, pObj, i )
      {
        Abc_Obj_t *pNet = Abc_ObjFanin0( pObj );

        outFile << "." << Abc_ObjName( pNet ) << "(" << Abc_ObjName( pNet ) << "),";
      }
      // end write patch output name

      // write patch input name
      Abc_NtkForEachPi( circuit, pObj, i )
      {
        Abc_Obj_t *pNet = Abc_ObjFanout0( pObj );

        outFile << "." << Abc_ObjName( pNet ) << "(" << Abc_ObjName( pNet ) << ")";
        if( i + 1 != Abc_NtkPiNum( circuit ) )
          outFile << ",";
      }
      // end write patch input name

      outFile << ");\n";
    }
    // end write patch line
    outFile << buffer << endl;
  }
}

inline string parseName( const string &buffer )
{
  for( int i = 1 ; i < buffer.size() ; ++i )
     if( !isalnum( buffer[i] ) && buffer[i] != '_' )
       return buffer.substr( 0, i );
  return buffer;
}

inline string gateType( const string &gateName )
{
    if      ( gateName.find( "INV"  ) != string::npos ) return "  not ";
    else if ( gateName.find( "BUF"  ) != string::npos ) return "  buf ";
    else if ( gateName.find( "NOR"  ) != string::npos ) return "  nor ";
    else if ( gateName.find( "NAND" ) != string::npos ) return "  nand ";
    else if ( gateName.find( "OR"   ) != string::npos ) return "  or ";
    else if ( gateName.find( "AND"  ) != string::npos ) return "  and ";
    else if ( gateName.find( "XOR"  ) != string::npos ) return "  xor ";
    else if ( gateName.find( "XNOR" ) != string::npos ) return "  xnor ";
    else                                                return string( "  " + gateName + " " );
}

inline string getArgument( const string &buffer )
{
  int lpos, rpos;

  lpos = buffer.find( '(' );
  rpos = buffer.rfind( ')' );

  return buffer.substr( lpos + 1, rpos - lpos - 1 );
}

void printSignals( ostream & output, const vector<string> &inputSignals, const vector<string> &outputSignals )
{
  output << "(";
  for( int i = 0 ; i < outputSignals.size() ; ++i ) output << outputSignals[i] << ", ";
  for( int i = 0 ; i < inputSignals.size() ; ++i )
  {
     output << inputSignals[i];
     if( i + 1 != inputSignals.size() ) output << ", ";
  }
  output << ");\n";
}

inline void initContainers( stringstream &stringStream, vector<string> &inputSignals, vector<string> &outputSignals )
{
  stringStream.clear();
  outputSignals.clear();
  inputSignals.clear();
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsColumnSeparatedValuesConfigurationFile.cpp
 * @author Guillermo Paris
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <fstream>

#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/AbsColumnSeparatedValuesConfigurationFile.h>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{



AbsColumnSeparatedValuesConfigurationFile::AbsColumnSeparatedValuesConfigurationFile()
  : m_blErrors(false), m_nRecords(0)
{
}

bool AbsColumnSeparatedValuesConfigurationFile::loadList( const std::string & sFilename,
                                                          const char* pcFileUsage /* = NULL */)
{
  if( ! Utils::canAccess(sFilename) )
  {
    std::cout << "ERROR: Could not access configuration file " << sFilename << std::endl;
    m_blErrors = true;
    return false;
  }

  std::ifstream ifs( sFilename.c_str() );
  if( ! ifs.good() )
  {
    std::cout << "ERROR: Could not open filename " << sFilename << " for reading.\n";
    return (m_blErrors = false);
  }

  bool blOk = false;
  int iLineNo = 0;

  if( pcFileUsage == NULL ) pcFileUsage = "" ;

  while( ! ifs.eof() )
  {
    std::string sLine;

    ++iLineNo;
    std::getline(ifs, sLine);
    if( ! ifs.good() ) // file stream error
    {
      if( ifs.bad() || (ifs.fail() && !ifs.eof()) )
      {
        std::cout << "Error reading "  << pcFileUsage
                  << " table. Records read = " << m_nRecords
                  << " . Last line #" << iLineNo
                  << " read below:\n" << sLine << std::endl;
        m_blErrors = true;
      }
      break;
    }

    blOk = parseLine(sLine);
    if( !blOk )
    {
      std::cout << "ERROR: " << pcFileUsage << " file. Bad line " << iLineNo << std::endl;
    }
  } // end of while

  return (m_nRecords > 0 ? true : false);
}


int AbsColumnSeparatedValuesConfigurationFile::getFieldValue(std::string s)
{
  bool blHex = false;
  int uLen = s.length();

  if( uLen == 0 )
  {
    return -1; // bad input parameter
  }

  if( uLen == 1 )
  {
    return getDigitValue( false, s[0] );
  }

  if( uLen == 2 )
  {
    int ih = getDigitValue( false, s[0] );
    if( ih < 0 )
    {
      return -1; // bad first digit
    }

    int il = getDigitValue( false, s[1] );
    if( il < 0 )
    {
      return -1; // bad second digit
    }

    return ih * 10 + il;
  }

  blHex = ( s[0] == '0' && ( s[1] == 'x' || s[1] == 'X') );

  int ih = getDigitValue( blHex, blHex ? s[2] : s[0] );
  if( ih < 0)
  {
    return -1; // bad first digit
  }

  if( blHex && uLen == 3 )
  {
    return ih; // only one digit in this case
  }

  int il = getDigitValue( blHex, blHex ? s[3] : s[1] );
  if( il < 0)
  {
    return -1; // bad second digit
  }

  return ih * (blHex ? 16 : 10) + il;
}

int AbsColumnSeparatedValuesConfigurationFile::getDigitValue(bool blHex, char c)
{
  if( c > 'f' )      return -1;
  else if( c >= 'a') return ( blHex ? c - 'a' + 10 : -1 );
  else if( c > 'F' ) return -1;
  else if( c >= 'A') return ( blHex ? c - 'A' + 10 : -1 );
  else if( c > '9' ) return -1;
  else if( c >= '0') return ( c - '0' );
  else               return -1;
}



} // namespace spot

} // namespace communication

} // namespace gvr




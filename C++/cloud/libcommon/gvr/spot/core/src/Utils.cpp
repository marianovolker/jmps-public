/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file Utils.cpp
 * @author Guillermo Paris
 * @date Monday, March 03, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <sys/stat.h>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>

#include <gvr/communication/spot/Utils.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const char * Utils::CUp1RightNNThenEraseToEOL = "\033[1A\033[%dC\033[K";


bool Utils::isBlankChar( char c )
{
  if( c == ' ' ) return true; // Space
  if( c == '\t') return true; // H Tab
  if( c == '\n') return true; // End of line or Line feed
  if( c == '\r') return true; // Carriage return
  if( c == '\f') return true; // Form feed
  if( c == '\v') return true; // V Tab
  if( c == '\b') return true; // Delete char

  return false;
}

bool Utils::isBlankString( const char* pc)
{
  if( pc == NULL) return true;

  int len = strlen( pc );
  if( len == 0 ) return true;

  for( int i = 0; i < len; i++)
  {
    if( ! isBlankChar( pc[i] ) ) return false;
  }

  return true;
}

bool Utils::isBlankString( const std::string & s )
{
  if( s.empty() ) return true;

  return isBlankString( s.c_str() );
}

void Utils::getStringFromConsole( const std::string & sLabelPrompt, std::string & sOut )
{
  char acEraseInput[24];

  acEraseInput[0] = '\0';

  while( true )
  {
    std::cout << sLabelPrompt;

    getline(std::cin, sOut);
    if( isBlankString(sOut) )
    {
      sprintf( acEraseInput, CUp1RightNNThenEraseToEOL, 1 + sLabelPrompt.length() );
      std::cout << acEraseInput;
    }
    else // not a blank string has been entered.
    {
      break;
    }
  }
}

bool Utils::canAccess(const char* pc)
{
  if( pc == NULL ) return false;

  struct stat   statBuffer;
  return ( stat(pc, &statBuffer) == 0 );
}

bool Utils::canAccess(const std::string & s)
{
  return canAccess( s.c_str() );
}

const char* Utils::dumpBuffer( char* pcOutputBuffer,
                               unsigned const char* pbInputBuffer, ISpotMessage::SizeType length)
{
  *pcOutputBuffer = '\0';

  if( pcOutputBuffer == NULL ) return "(null)"; // bad input parameter

  if( length == 0 ) return pcOutputBuffer;

  unsigned int nOffset = 0;
  unsigned int outLength = 3 * length;

  for(int i = 0; i < length; i++)
  {
    nOffset = nOffset + snprintf(pcOutputBuffer + nOffset , outLength - nOffset, "%02X ", pbInputBuffer[i]);
  }

  return pcOutputBuffer;
}

const char* Utils::dumpBuffer(char* pcOutputBuffer, const ISpotMessage::BufferType& inputBuffer)
{
  *pcOutputBuffer = '\0';

  if( inputBuffer.empty() ) return ""; // bad input parameter

  unsigned int nOffset = 0;
  unsigned int length = inputBuffer.size();
  unsigned int outLength = 3 * length;

  for(int i = 0; i < length; i++)
  {
    nOffset = nOffset + snprintf(pcOutputBuffer + nOffset , outLength - nOffset, "%02X ", inputBuffer[i]);
  }

  return pcOutputBuffer;
}



} // namespace spot

} // namespace communication

} // namespace gvr



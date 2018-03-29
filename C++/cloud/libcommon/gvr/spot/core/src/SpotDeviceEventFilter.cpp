/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotDeviceEventFilter.cpp
 * @author Guillermo Paris
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <sstream>

#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotDeviceEventFilter.h>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{

const char *  SpotDeviceEventFilter::CpcConfigurationFile    = "cloud_events_filter.cfg";
const char *  SpotDeviceEventFilter::CapcCategories[eTotalCategories] =
{
  "status", "status-ex", "error", "job-status"
};

const char * SpotDeviceEventFilter::getCategoryName( EStatusCategory eCategory )
{
  if( eCategory == eInvalid )
    return "";
  else
    return CapcCategories[eCategory - 1];
}


SpotDeviceEventFilter::SpotDeviceEventFilter(std::string sConfFilename, const char* pcPlatform /*= NULL */)
  : m_pcPlatform(pcPlatform)
{
  if( Utils::isBlankString(sConfFilename) || ! Utils::canAccess(sConfFilename) )
  {
    sConfFilename = CpcConfigurationFile; // try with the default name instead
  }

  if( ! Utils::canAccess(sConfFilename) )
  {
    std::cout << "ERROR: Could not access configuration file " << sConfFilename << std::endl;
    m_blErrors = true;
    return;
  }

  if( pcPlatform == NULL )
  {
    m_pcPlatform = "m5";
  }

  loadList( sConfFilename );
}

bool SpotDeviceEventFilter::parseLine( const std::string & sLine )
{
  unsigned int ulen = sLine.length();

  if( ulen < CMinCharsInLine )             return true;// line too short or just a new-line character.
  else if( sLine[0] == '#' )               return true;// commented out line.
  else if( Utils::isBlankString( sLine ) ) return true;// blank line.
  else if( sLine[0] != '*' && sLine.substr(0,2).compare(m_pcPlatform) != 0 ) return true;// not for this platform
  else // valid line to process
  {
    int iAppId = 0, iDevCode = 0, iIterField = 0;
    EStatusCategory eCategory = eInvalid;
    std::vector<unsigned char> vByteFields;
    std::string sField1, sField2, sField3, sField4, sField5, sIterField, sKey;
    std::istringstream istr(sLine);

    istr >> sField1 >> sField2 >> sField3 >> sField4 >> sField5 >> sIterField;

    if( Utils::isBlankString(sField2) || sField2.find('=') == std::string::npos )
    {
      return false; // invalid line
    }

    if( Utils::isBlankString(sField3) ||
        Utils::isBlankString(sField4) || Utils::isBlankString(sField5) )
    {
      return false; // invalid line
    }

    if( ! validateFirstField(sField1, sKey) )
    {
      return false; // bad first part of the line, or duplicated key
    }

    if( (iAppId = getFieldValue(sField3)) < 0 )
    {
      return false; // invalid line
    }

    if( (iDevCode = getFieldValue(sField4)) < 0 )
    {
      return false; // invalid line
    }

    if( ( eCategory = intToCategoryEnum(getFieldValue(sField5)) ) == eInvalid )
    {
      return false; // invalid line
    }

    while( ! Utils::isBlankString(sIterField) )
    {
      if( (iIterField = getFieldValue(sIterField)) < 0 )
      {
        return false; // invalid byte field => invalid line
      }

      vByteFields.push_back( static_cast<unsigned char>(iIterField) ); // at least one element in the vector
      istr >> sIterField;
    }

    if( vByteFields.size() == 0 )
    {
      return false; // invalid line, the vector must have at least one element.
    }

    // Insert the new record into the map
    m_mapSupportedEvents[sField1] = DeviceEvent(iAppId, iDevCode, eCategory, vByteFields);
    m_nRecords++;

    std::cout << "Record #" << m_nRecords << ", Event \"" << sField1 << "\" App " << iAppId
              << ", Device " << iDevCode
              << ", Category " << getCategoryName(eCategory) << ", allows " << vByteFields.size() << " values.\n";

    return true; // Insert successful
  }
}

bool SpotDeviceEventFilter::validateFirstField(const std::string & sField, std::string & sKey)
{
  unsigned int uDotPos = 0, uUdsPos = 0;

  if( (uDotPos = sField.find('.')) > CMaxPlatformChars ) // find dot position
  {
    return false; // bad platform specification
  }

  if( (uUdsPos = sField.find_last_of('_')) == std::string::npos ) // find last underscore position
  {
    return false; // no category suffix
  }
  else if( (sField.length() - uUdsPos) < 6 )
  {
    return false; // category suffix must be longer or equal as "status" word
  }

  if( uUdsPos - uDotPos < CMinDeviceNameLength + 1)
  {
    return false; // device name must have at least 3 characters
  }

  sKey = sField.substr( uDotPos + 1 );
  if( Utils::isBlankString(sKey) || m_mapSupportedEvents.find(sKey) != m_mapSupportedEvents.end() )
  {
    return false;  // bad beginning of the line or repeated key
  }

  return true;
}

SpotDeviceEventFilter::EStatusCategory  SpotDeviceEventFilter::intToCategoryEnum( int i )
{
  switch( i )
  {
    case eStatus:
    return eStatus;

    case eStatusEx:
    return eStatusEx;

    case eError:
    return eError;

    case eJobStatus:
    return eJobStatus;

    default:
    return eInvalid;
  }
}

void SpotDeviceEventFilter::getComponentName( const std::string & sKeyname, std::string & sName )
{
  char c = 0;
  std::size_t u = 0, uMax = sKeyname.find_last_of('_');

  if( uMax == std::string::npos ) uMax = sKeyname.length();

  sName.clear();
  sName.reserve( 4 * (1 + uMax / 4) ); // just only one allocation whose capacity is multiple of 4.

  while( u < uMax )
  {
    if( (c = sKeyname[u]) == '_')
    {
      c = ' '; // replaces '_' by space ' '
    }
    sName += c;
  }
}


} // namespace spot

} // namespace communication

} // namespace gvr



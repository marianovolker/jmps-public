/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UploadableFileList.cpp
 * @author Guillermo Paris
 * @date Thursday, Jun 25, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/UploadableFileList.h>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


typedef std::map<std::string, UploadableFileList::FileAttrib>::iterator  SupportedFilesMapIterator;

const char *  UploadableFileList::CpcKeyNameSelectAll        = "ALL";
const char *  UploadableFileList::CpcKeyNameSelectAllCurrent = "ALL_CURRENT";
const char *  UploadableFileList::CpcAgeCurrent              = "CURR";
const char *  UploadableFileList::CpcAgeHistoric             = "HIST";
const char *  UploadableFileList::CpcAgeBoth                 = "BOTH";

const char *  UploadableFileList::CpcConfigurationFile       = "cloud-PH-logs.cfg";

UploadableFileList::UploadableFileList(std::string sConfFilename, const char* pcPlatform /*= NULL */)
  : m_blErrors(false), m_pcPlatform(pcPlatform), m_nRecords(0)
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

  /* Reading of the configuration filename sConfFilename is simulated below:

  m_mapSupportedFiles["PINPAD"]          = FileAttrib(eCurrent,  0x13, 0x00);
  m_mapSupportedFiles["PINPAD_HIST"]     = FileAttrib(eHistoric, 0x13, 0x80);
  m_mapSupportedFiles["SYSMANSERV"]      = FileAttrib(eCurrent,  0x13, 0x01);
  m_mapSupportedFiles["SYSMANSERV_HIST"] = FileAttrib(eHistoric, 0x13, 0x81);
  m_mapSupportedFiles["JOURNAL"]         = FileAttrib(eCurrent,  0x13, 0x03);
  m_mapSupportedFiles["JOURNAL_HIST"]    = FileAttrib(eHistoric, 0x13, 0x83);
  m_mapSupportedFiles["KERNEL"]          = FileAttrib(eBoth,     0x13, 0x05);
  m_mapSupportedFiles["FILTER"]          = FileAttrib(eCurrent,  0x13, 0x06);
  m_mapSupportedFiles["FILTER_HIST"]     = FileAttrib(eHistoric, 0x13, 0x86);
  m_mapSupportedFiles["OPT"]             = FileAttrib(eCurrent,  0x40, 0x00);
  m_mapSupportedFiles["OPT_HIST"]        = FileAttrib(eHistoric, 0x40, 0x80);
  */
}

bool UploadableFileList::loadList( const std::string & sFilename )
{
  std::ifstream ifs( sFilename.c_str() );
  if( ! ifs.good() )
  {
    std::cout << "ERROR: Could not open filename " << sFilename << " for reading.\n";
    return (m_blErrors = false);
  }

  bool blOk = false;
  int iLineNo = 0;

  while( ! ifs.eof() )
  {
    std::string sLine;

    ++iLineNo;
    std::getline(ifs, sLine);
    if( ! ifs.good() ) // file stream error
    {
      if( ifs.bad() || (ifs.fail() && !ifs.eof()) )
      {
        std::cout << "Error reading log configuration table. Records read = " << m_nRecords
                  << " . Last line #" << iLineNo
                  << " read below:\n" << sLine << std::endl;
        m_blErrors = true;
      }
      break;
    }

    blOk = parseLine(sLine);
    if( !blOk )
    {
      std::cout << "ERROR: Log configuration file. Bad line " << iLineNo << std::endl;
    }
  } // end of while

  return (m_nRecords > 0 ? true : false);
}

bool UploadableFileList::parseLine( const std::string & sLine )
{
  bool bl4Fields = true; // four data columns is assumed by default ( 3 fields + " = " )
  unsigned int uAge = 0, ulen = sLine.length();

  if( ulen < CMinCharsInLine )             return true;// line too short or just a new-line character.
  else if( sLine[0] == '#' )               return true;// commented out line.
  else if( Utils::isBlankString( sLine ) ) return true;// blank line.
  else if( sLine[0] != '*' && sLine.substr(0,2).compare(m_pcPlatform) != 0 ) return true;// not for this platform
  else // valid line to process
  {
    std::string sField1, sField2, sField3, sField4, sKey;
    std::istringstream istr(sLine);

    istr >> sField1 >> sField2 >> sField3 ;

    if( Utils::isBlankString(sField2) || sField2.find('=') == std::string::npos )
    {
      return false; // invalid line
    }

    if( sField3[0] == '/' )
    {
      bl4Fields = false; // just 3 columns, last one is a path.
    }
    else // 4 columns assumption was correct
    {
      istr >> sField4;
    }

    if( Utils::isBlankString(sField3) || ( bl4Fields && Utils::isBlankString(sField4) ) )
    {
      return false; // invalid line
    }

    if( ! validateFirstField(sField1, uAge, sKey) )
    {
      return false; // bad beginning of the line or duplicated key.
    }

    int iType = 0, iId = 0;
    const char * pcFilePath = NULL;

    if( bl4Fields )
    {
      if( (iType = getFieldValue(sField3)) < 0) return false;  // bad written file type.
      if( (iId   = getFieldValue(sField4)) < 0) return false;  // bad written file ID.
    }
    else // no attributes, just file system path
    {
      pcFilePath = sField3.c_str();
    }

    // Insert the new record into the map
    m_mapSupportedFiles[sKey] = FileAttrib(uAge, iType, iId, pcFilePath);
    m_nRecords++;

    std::cout << "Record #" << m_nRecords << ", Age " << uAge << ", Type " << iType
              << ", Id " << iId << ", file: " << sKey << std::endl;

    return true; // Insert successful
  }
}

bool UploadableFileList::validateFirstField( const std::string & sField,
                                             unsigned int & uAge, std::string & sKey)
{
  unsigned int uDotPos = 0, uUdsPos = 0;

  if( (uDotPos = sField.find('.')) > CMaxPlatformChars ) // find dot position
  {
    return false; // bad platform specification
  }

  if( (uUdsPos = sField.find('_')) == std::string::npos ) // find underscore position
  {
    return false; // bad age postfix
  }

  if( uUdsPos - uDotPos < CMinFileTypeNameLength + 1)
  {
    return false; // file type name must have at least 3 characters
  }

  if( sField.compare(++uUdsPos, 4, CpcAgeCurrent) == 0 )
  {
    uAge = eCurrent;
  }
  else if( sField.compare(uUdsPos, 4, CpcAgeHistoric) == 0 )
  {
    uAge = eHistoric;
  }
  else if( sField.compare(uUdsPos, 4, CpcAgeBoth) == 0 )
  {
    uAge = eBoth;
  }
  else
  {
    return false; // lack of age specification
  }

  sKey = sField.substr( uDotPos + 1, uUdsPos + 3 );
  if( Utils::isBlankString(sKey) || m_mapSupportedFiles.find(sKey) != m_mapSupportedFiles.end() )
  {
    return false;  // bad beginning of the line or repeated key
  }

  return true;
}

int UploadableFileList::getFieldValue(std::string s)
{
  bool blHex = ( s[0] == '0' && ( s[1] == 'x' || s[1] == 'X') );

  int ih = getDigitValue( blHex, blHex ? s[2] : s[0] );
  if( ih < 0)
  {
    return -1; // bad first digit
  }

  int il = getDigitValue( blHex, blHex ? s[3] : s[1] );
  if( il < 0)
  {
    return -1; // bad second digit
  }

  return ih * (blHex ? 16 : 10) + il;
}

int UploadableFileList::getDigitValue(bool blHex, char c)
{
  if( c > 'f' )      return -1;
  else if( c >= 'a') return ( blHex ? c - 'a' + 10 : -1 );
  else if( c > 'F' ) return -1;
  else if( c >= 'A') return ( blHex ? c - 'A' + 10 : -1 );
  else if( c > '9' ) return -1;
  else if( c >= '0') return ( c - '0' );
  else               return -1;
}

bool UploadableFileList::isSupported(std::string sFileTypeName)
{
  if( sFileTypeName.compare( CpcKeyNameSelectAllCurrent ) == 0 )
  {
    return true;
  }

  if( sFileTypeName.compare( CpcKeyNameSelectAll ) == 0 )
  {
    return true;
  }

  return (m_mapSupportedFiles.find(sFileTypeName) != m_mapSupportedFiles.end());
}

bool UploadableFileList::getSupportedFilenameList( std::vector<std::string> & vout )
{
  if( m_blErrors ) return false;

  SupportedFilesMapIterator it = m_mapSupportedFiles.begin(), itend = m_mapSupportedFiles.end();

  while( it != itend ) // iterates all map to collect all the supported file type names.
  {
    vout.push_back( it->first );
    ++it;
  }

  return true;
}


UploadableFileList::RequestedFilesVector &
UploadableFileList::getRequeriedFileList( const std::string & sFname,
                                          EAge eAge, int iFid, int iTyp,
                                          std::vector<FileNameAndAttrib> & vout )
{
  if( m_blErrors ) return vout;

  bool blSelected = false;
  bool blAllSelected = false;
  bool blAllCurrrentSelected = false;
  bool blNameFilter = false;
  SupportedFilesMapIterator it = m_mapSupportedFiles.begin(), itend = m_mapSupportedFiles.end();

  if( ! sFname.empty() )
  {
    if( sFname.compare( CpcKeyNameSelectAllCurrent ) == 0 ) // ALL_CURR "wildcard" means all the current files.
    {
      blAllCurrrentSelected = true;
    }
    else if( sFname.compare( CpcKeyNameSelectAll ) == 0 )   // ALL "wildcard" means all the file list.
    {
      blAllSelected = true;
      blSelected = true;
    }
    else // filter by file type name
    {
      blNameFilter = true;
    }
  }

  while( it != itend ) // iterate all map to see if some item meets the selection criteria
  {
    // Special key names ("ALL" and "ALL_CURRENT") are evaluated first
    if( ! blAllSelected )
    {
      if( blAllCurrrentSelected )
      {
        blSelected = ( static_cast<int>(it->second.b_age) == eCurrent ); // eBoth is currently ignored as of sprint 22.
      }
      else if( ( blNameFilter && sFname != it->first )      || // filter by name
               ( iFid >= 0 && iFid != it->second.b_fileId ) || // filter by file ID
               ( iTyp >= 0 && iTyp != it->second.b_type )      // filter by file type
             )
      {
        blSelected = false; // conditional filter really exist, but the condition is not met.
      }
      else
      {
        blSelected = true; // no filter conditions or the condition is met.
      }
    }

    if( blSelected ) // record selected for future file retrieval
    {
      vout.push_back(FileNameAndAttrib(it->first, it->second));
    }

    ++it;
  }

  return vout;
}




} // namespace spot

} // namespace communication

} // namespace gvr




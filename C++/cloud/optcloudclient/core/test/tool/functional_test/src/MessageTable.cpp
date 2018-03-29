/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MessageTable.cpp
 * @author gparis
 * @date Tuesday, May 04, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <sstream>
#include <fstream>

#include <Poco/ScopedLock.h>
#include <gvr/communication/spot/Utils.h>
#include <gvrlog/log.h>

#include "../include/MessageTable.h"

DEFINE_LOG_MODULE(msgtbl);

/// @brief Gilbarco Domain
namespace gvr
{
/// @brief Cloud Domain
namespace cloud
{


using gvr::communication::spot::Utils;

typedef Poco::ScopedLock<Poco::FastMutex>   ScopedLock; ///< @brief Automatic scope life mutex.

const char *  MessageTable::CDefaultPathName = "payloads/command_table.txt";


MessageTable::MessageTable()
  : m_blError(false), m_sFullFileName(CDefaultPathName)
{
}

MessageTable::MessageTable( const std::string & sFullFileName )
  : m_blError(false), m_sFullFileName(sFullFileName)
{
  if( Utils::isBlankString(m_sFullFileName) || ! Utils::canAccess(m_sFullFileName) )
  {
    m_sFullFileName = CDefaultPathName;
  }
}

MessageTable::~MessageTable (void)
{
}


void MessageTable::LoadTable()
{
  std::ifstream is( m_sFullFileName.c_str() );
  if( ! is.good() )
  {
    LOG_VError(msgtbl, "Could not open filename \"%s\" for reading.\n", m_sFullFileName.c_str());
    m_blError = true;
    return;
  }

  int iLineNo = 0, nRecords = 0;

  while( ! is.eof() )
  {
    std::string sLine;

    ++iLineNo;
    std::getline(is, sLine);
    if( ! is.good() ) // file stream error
    {
      LOG_VTrace( msgtbl, "gCount=%d, %s %s %s",
                  is.gcount(), is.eof() ? "EOF" : "", is.fail() ? "fail" : "", is.bad() ? "BAD" : "" );
      LOG_VDebug( msgtbl, "Error reading table. Records read = %d . Last line #%d read below:\n%s\n",
                  nRecords, iLineNo, sLine.c_str() );
      break;
    }

    unsigned int len = sLine.length(); // is.gcount() removed because ALWAYS returned 0 !

    if( len < CMinCharsInLine ) continue;// short line or just a new-line char.
    else if( Utils::isBlankString( sLine ) ) continue;// blank line
    else if( sLine[0] == '#' ) continue;// commented out line
    else
    {
      TableEntry oRecord;
      std::istringstream istr(sLine);

      istr >> oRecord.sCommandClass >> oRecord.sCommandOperation >> oRecord.iCommandFlag >> oRecord.sCommandReplyFilename;

      if( !Utils::isBlankString( oRecord.sCommandClass ) && !Utils::isBlankString( oRecord.sCommandOperation ) )
      {
        InsertRetPair rp = m_Map.insert( EntryPair(oRecord.sCommandClass+' '+oRecord.sCommandOperation, oRecord) );
        if( rp.second )
        {
          nRecords++; // Insert successfully
        }
      }
    }
  }

  LOG_VNotice(msgtbl, "Message table - %d records loaded.");

  if( nRecords == 0 )
  {
    m_blError = true; // no record could be read.
  }
}

bool MessageTable::hasError (void)
{
  return m_blError;
}

const MessageTable::TableEntry* MessageTable::getTableEntry(const std::string & sKey)
{
  if( m_blError ) return NULL;

  ScopedLock lock(m_Mutex);

  TableConstIterator constIt = m_Map.find(sKey);

  if( constIt == m_Map.end() ) return NULL; // key not found in the map.

  return & constIt->second;
}


} // Cloud Domain
} // Gilbarco Domain



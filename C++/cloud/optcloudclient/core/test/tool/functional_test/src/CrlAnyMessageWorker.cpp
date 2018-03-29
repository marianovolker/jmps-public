/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlAnyMessageWorker.cpp
 * @author gparis
 * @date Friday, May 8, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <fstream>
#include <sstream>

#include <gvrlog/log.h>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/crl/message/base/AbsCrlMessageInternal.h>
#include <gvr/communication/crl/message/CrlMessageAny.h>

#include "../include/MessageTable.h"
#include "../include/CrlAnyMessageWorker.h"

DEFINE_LOG_MODULE(crlwrk);

extern gvr::cloud::MessageTable  g_messageTable;///< @brief Global Message Table.

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    typedef Poco::ScopedLock<Poco::FastMutex>   ScopeLock; ///< @brief Automatic scope life mutex.
    using gvr::communication::spot::Utils;

    static const char*  COperationLabel  = AbsCrlMessageInternal::COperationName;

    const char * CrlAnyMessageWorker::CDefaultMessageReplyDir = "payloads/crl/response/"; // command reply dir

    CrlAnyMessageWorker::CrlAnyMessageWorker( CrlSessionClient * pSession)
      :  AbsOptCloudClientWorker("CrlAnyMessageWorker"),
         m_pSession(pSession), m_pcReplyMessageDir(CDefaultMessageReplyDir)
    {
      start();

      LOG_Debug(crlwrk, "Starting UCC any message worker.");
    }

    CrlAnyMessageWorker::~CrlAnyMessageWorker()
    {
      stop();

      LOG_Debug(crlwrk, "Stopping UCC any message worker.");
    }

    void CrlAnyMessageWorker::setReplyMessageDir (const char* pcReplyDir)
    {
      ScopeLock lock(m_mutex);

      m_pcReplyMessageDir = pcReplyDir;
    }

    void CrlAnyMessageWorker::sessionEvent( ICrlSessionListener::BufferType const & buffer, ICrlSessionListener::TypeId const & typeId, ICrlSessionListener::KeyType const & keyType)
    {
      LOG_Trace(crlwrk, "Enqueue a new message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void CrlAnyMessageWorker::sessionEvent( ICrlSessionListener::StatusType status )
    {
      LOG_Trace(crlwrk, "Enqueue a new status notification.");

      enqueueNotification ( status );
    }

    void CrlAnyMessageWorker::doSubscriptions( )
    {
      ScopeLock lock(m_mutex);

      // Here we need to subscribe to all server session events.
      LOG_Debug(crlwrk, "Subscribe to server session events.");

      m_pSession->subscribe(*this);
    }

    void CrlAnyMessageWorker::doUnsubscriptions( )
    {
      ScopeLock lock(m_mutex);

      // Here we need to unsubscribe to all server session events.
      LOG_Debug(crlwrk, "Un-subscribe to server session events.");

      m_pSession->unsubscribe(*this);
    }

    void CrlAnyMessageWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(crlwrk, "Process any CRL message type notification ...");

        doTheWork(data);
    }

    void CrlAnyMessageWorker::processNotification( StatusType status )
    {
      LOG_VTrace(crlwrk, "New status event %d has been received from CrlSession [%s].\n",
                         int(status), m_pSession->getStatusDescription().c_str());
    }

    void CrlAnyMessageWorker::doTheWork ( const AbsOptCloudClientWorker::BufferType& data )
    {
      ScopeLock lock(m_mutex);

      CrlMessageAny    crlMsgAny;

      if (! crlMsgAny.parse(data) )
      {
        LOG_VError(crlwrk, "Error parsing CrlMessageAny failed. Message Data:\n[%s].\n",
                            &data.front() + CBinaryHeaderSize);
        return;
      }

      unsigned int uFlag = crlMsgAny.getInternalFlag();
      const char * pcError = NULL;
      const char * pcProtocol = "CRL";
      std::string  sJson, sOp, sKey("crl ");

      if( crlMsgAny.getJson().get(COperationLabel).isString() )
      {
        sOp = crlMsgAny.getJson().get(COperationLabel).asString();
      }

      crlMsgAny.getJson().serialize(sJson);
      LOG_VNotice(crlwrk,
                 "\n\nReceived a valid command from the client.\nType: %d - %s\n%s\n\n",
                 uFlag, pcProtocol, sJson.c_str());

      std::cout << "\nMessage received:\n\n" << sJson.c_str() << std::endl << std::endl;


      // Hereinafter, this code will try to reply the received message, if it is possible.

      std::string::size_type uPos = 0;
      while( (uPos = sOp.find(' ',uPos)) != std::string::npos )
      {
        // If the operation field has spaces, then these must be changed by %20 (sOp can't have spaces).
        sOp.replace(uPos, 1, "%20");
        uPos += 3;
      }

      sKey += sOp;
      const MessageTable::TableEntry * pTableEntry = g_messageTable.getTableEntry(sKey);
      if( pTableEntry ) // A table entry was found having the same "crl <operation field>"
      {
        if( ! Utils::isBlankString(pTableEntry->sCommandReplyFilename) )
        {
          std::string sFullReplyFilename(m_pcReplyMessageDir);

          sFullReplyFilename += pTableEntry->sCommandReplyFilename;
          if( Utils::canAccess(sFullReplyFilename) )
          {
            std::ifstream fis( sFullReplyFilename.c_str() );

            if( fis.good() )
            {
              std::ostringstream  oss;
              CrlMessageAny       uccMsgAnyReply;
              gvr::utils::Json    json;

              oss << fis.rdbuf();
              fis.close();
              std::string sRspPayload( oss.str() );

              LOG_VDebug( crlwrk, "\nSuitable Reply found and loaded:\n\n%s\n\n", sRspPayload.c_str() );
              json.parse( sRspPayload );

              std::cout << "\nReply message to be sent:\n" << json.serialize() << std::endl;

              uccMsgAnyReply.setInternalFlag(uFlag);
              uccMsgAnyReply.setJson(json);
              int error = m_pSession->send(uccMsgAnyReply);

              if( error == CrlSession::ISessionError::CNoError )
              {
                  LOG_VDebug(crlwrk,
                             "\n\nA reply was sent to the client.\nType: %d - %s\n%s\n\n",
                             uFlag, pcProtocol, json.serialize().c_str());
              }
              else // error sending message
              {
                LOG_VCritical( crlwrk,
                               "Could not send a reply message to the server session. Error code: %d\n",
                               error );
              }
            }
            else // Could not load the response payload.
            {
              LOG_VError( crlwrk, "Could not load reply payload from %s.\n", sFullReplyFilename.c_str() );
            }
          }
          else // Could not access to the response payload.
          {
            LOG_VError( crlwrk, "Could not access to file %s.\n", sFullReplyFilename.c_str() );
          }
        }
        else // No response consigned.
        {
          LOG_VDebug( crlwrk, "No response consigned for operation \"%s\"\n.", sOp.c_str() );
        }
      }
      else // Operation not found.
      {
        LOG_VDebug( crlwrk, "Operation \"%s\" not found in message table\n.", sOp.c_str() );
      }
    }


  }  // Cloud Domain
}   // Gilbarco Domain




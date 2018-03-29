/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccAnyMessageWorker.h
 * @author gparis
 * @date Wednesday, April 29, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <fstream>
#include <sstream>

#include <gvrlog/log.h>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloud.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventRequest.h>
#include <gvr/communication/ucc/message/UccMessageAny.h>

#include "../include/MessageTable.h"
#include "../include/UccAnyMessageWorker.h"

DEFINE_LOG_MODULE(uccwrk);

extern gvr::cloud::MessageTable  g_messageTable;///< @brief Global Message Table.

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    typedef Poco::ScopedLock<Poco::FastMutex>   ScopeLock; ///< @brief Automatic scope life mutex.
    using gvr::communication::spot::Utils;

    static const char*  CMessageIdLabel  = AbsUccMessageCloud::CMessageIdName;
    static const char*  COperationLabel  = AbsUccMessageCloudEventRequest::COperationName;
    static const char*  CRegisterDeviceLabel  = "registerDevice";
    static const char*  CDeviceConfigLabel  = "deviceConfig";
    static const char*  CRegistrationOp  = "registration";

    const char * UccAnyMessageWorker::CProtocols[3] = {"Cloud", "UCC internal", "Registration"};

    const char * UccAnyMessageWorker::CDefaultMessageReplyDir = "payloads/ucc/response/"; // event reply dir

    UccAnyMessageWorker::UccAnyMessageWorker( UccSessionServer * pSession)
      :  AbsOptCloudClientWorker("UccAnyMessageWorker"),
         m_pServerSession(pSession), m_pcReplyMessageDir(CDefaultMessageReplyDir)
    {
      LOG_Debug(uccwrk, "Initializing UCC any message worker.");
    }

    UccAnyMessageWorker::~UccAnyMessageWorker()
    {
      stop();

      LOG_Debug(uccwrk, "Stopping UCC any message worker.");
    }

    void UccAnyMessageWorker::setReplyMessageDir (const char* pcReplyDir)
    {
      ScopeLock lock(m_mutex);

      m_pcReplyMessageDir = pcReplyDir;
    }

    void UccAnyMessageWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Trace(uccwrk, "Enqueue a new message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void UccAnyMessageWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Trace(uccwrk, "Enqueue a new status notification.");

      enqueueNotification ( status );
    }

    void UccAnyMessageWorker::doSubscriptions( )
    {
      ScopeLock lock(m_mutex);

      // Here we need to subscribe to all server session events.
      LOG_Debug(uccwrk, "Subscribe to server session events.");

      m_pServerSession->subscribe(*this);
    }

    void UccAnyMessageWorker::doUnsubscriptions( )
    {
      ScopeLock lock(m_mutex);

      // Here we need to unsubscribe to all server session events.
      LOG_Debug(uccwrk, "Un-subscribe to server session events.");

      m_pServerSession->unsubscribe(*this);
    }

    void UccAnyMessageWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(uccwrk, "Process any UCC message type notification ...");

      doTheWork(data);
    }

    void UccAnyMessageWorker::processNotification( StatusType status )
    {
      LOG_VTrace(uccwrk, "New status event %d has been received from UccSession [%s].\n",
                         int(status), m_pServerSession->getStatusDescription().c_str());
    }

    void UccAnyMessageWorker::doTheWork ( const AbsOptCloudClientWorker::BufferType& data )
    {
      ScopeLock lock(m_mutex);

      UccMessageAny    uccMsgAny;

      if (! uccMsgAny.parse(data) )
      {
        LOG_VError(uccwrk, "Error parsing UccMessageAny failed. Message Data:\n[%s].\n",
                            &data.front() + CBinaryHeaderSize);
        return;
      }

      bool blRegistration = false; // Set only in case of registration request message.
      unsigned int uFlag = uccMsgAny.getInternalFlag();
      const char * pcError = NULL;
      const char * pcProtocol = "";
      std::string  sMsgId, sJson, sOp, sKey("ucc ");

      if(uFlag == 2) // is a registration type ?
      {
        if( uccMsgAny.getJson().get(CDeviceConfigLabel).isObject() &&
            uccMsgAny.getJson().get(CRegisterDeviceLabel).isObject() )
        {
          blRegistration = true; // Set because it is a true registration request message.
        }
      }
      else // cloud or internal types
      {
        if( uccMsgAny.getJson().get(CMessageIdLabel).isString() )
        {
          sMsgId = uccMsgAny.getJson().get(CMessageIdLabel).asString();
        }

        if( uccMsgAny.getJson().get(COperationLabel).isString() )
        {
          sOp = uccMsgAny.getJson().get(COperationLabel).asString();
        }
      }

      if(uFlag < 3) pcProtocol = CProtocols[uFlag];

      uccMsgAny.getJson().serialize(sJson);
      LOG_VNotice(uccwrk,
                 "\n\nReceived a valid command from the client.\nType: %d - %s\nMessage Id: %s\n%s\n\n",
                 uFlag, pcProtocol, sMsgId.c_str(), sJson.c_str());

      std::cout << "\nMessage received:\n\n" << sJson.c_str() << std::endl << std::endl;


      // Hereinafter, this code will try to reply the received message, if it is possible.

      const MessageTable::TableEntry * pTableEntry = NULL;

      if(blRegistration) // registration command from cloud client (PH)
      {
        sOp = CRegistrationOp;
      }
      else // remaining of the requests.
      {
        std::string::size_type uPos = 0;
        while( (uPos = sOp.find(' ',uPos)) != std::string::npos )
        {
          // If the operation field has spaces, then these must be changed by %20 (sOp can't have spaces).
          sOp.replace(uPos, 1, "%20");
          uPos += 3;
        }
      }

      sKey += sOp;
      pTableEntry = g_messageTable.getTableEntry(sKey);
      if( pTableEntry ) // A table entry was found having the same "ucc <operation field>"
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
              UccMessageAny       uccMsgAnyReply;
              gvr::utils::Json    json;

              oss << fis.rdbuf();
              fis.close();
              std::string sRspPayload( oss.str() );

              LOG_VDebug( uccwrk, "\nSuitable Reply found and loaded:\n\n%s\n\n", sRspPayload.c_str() );
              json.parse( sRspPayload );

              if( ! Utils::isBlankString(sMsgId) )
              {
                json.set(CMessageIdLabel, sMsgId); // replicate the received message ID
              }

              std::cout << "\nReply message to be sent:\n" << json.serialize() << std::endl;

              uccMsgAnyReply.setInternalFlag(uFlag);
              uccMsgAnyReply.setJson(json);
              int error = m_pServerSession->send(uccMsgAnyReply);

              if( error == UccSession::ISessionError::CNoError )
              {
                  LOG_VDebug( uccwrk,
                              "\n\nA reply was sent to the client.\nType: %d - %s\nMessage Id: %s\n%s\n\n",
                              uFlag, pcProtocol, sMsgId.c_str(), json.serialize().c_str() );
              }
              else // error sending message
              {
                LOG_VCritical( uccwrk,
                               "Could not send a reply message to the server session. Error code: %d\n",
                               error );
              }
            }
            else // Could not load the response payload.
            {
              LOG_VError( uccwrk, "Could not load reply payload from %s.\n", sFullReplyFilename.c_str() );
            }
          }
          else // Could not access to the response payload.
          {
            LOG_VError( uccwrk, "Could not access to file %s.\n", sFullReplyFilename.c_str() );
          }
        }
        else // No response consigned.
        {
          LOG_VDebug( uccwrk, "No response consigned for operation \"%s\"\n.", sOp.c_str() );
        }
      }
      else // Operation not found.
      {
        LOG_VDebug( uccwrk, "Operation \"%s\" not found in message table\n.", sOp.c_str() );
      }
    }


  }  // Cloud Domain
}   // Gilbarco Domain



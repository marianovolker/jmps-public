/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ucc_tool.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvrucc_tool/ucc_tool.h"

#include <iostream>
#include <iomanip>

#include <string.h>
#include <stdlib.h>
#include <cstdlib>

#include <signal.h>
#include <unistd.h>

#include <Poco/Thread.h>
#include <gvr/communication/ucc/UccChannel.h>
#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/ucc/session/UccSessionClient.h>
#include <gvr/communication/ucc/message/UccMessageStatus.h>
#include <gvr/communication/ucc/message/UccMessageCloudReset.h>
#include <gvr/communication/ucc/message/UccMessageCloudResetResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>
#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>


bool bRunning           = true;
std::string ip          = "127.0.0.1";
int port                = 21300;
unsigned int timeout    = 30000;

/**
 * showUsage
 * @brief Shows the ucc session tool usage help text
 * */
void showUsage (void);

/**
 * interrupt
 * @brief Helper function to handle an interrupt signal.
 * */
void interrupt (int);

/**
 * parseCommandLineArguments
 * @brief Helper function to parse command line arguments.
 * */
bool parseCommandLineArguments (int, char **);

class UccSessionListenerClient : public gvr::communication::ucc::IUccSessionListener
{
  public:
    virtual ~UccSessionListenerClient()
    {
    }

    UccSessionListenerClient (gvr::communication::ucc::UccSessionClient & session)
     : m_session(session)
    {
    }

    void sessionEvent (BufferType const & buffer, TypeId const & typeId, KeyType const & keyType)
    {
      if (typeId == gvr::communication::ucc::UccMessageStatus::GetTypeId())
      {
        gvr::communication::ucc::UccMessageStatus message;
        if (message.parse(buffer))
        {
          std::string messageId = message.getMessageId();
          gvr::utils::Json json = message.getJson();
          std::string jsonMessage;
          json.serialize(jsonMessage);
          std::cout << "UccSessionListenerClient has received a message [ messageId = " << messageId << ", json = " << jsonMessage << "]." << std::endl;
        }
        else
        {
          std::cout << "UccSessionListenerClient has received a message but parsing failed." << std::endl;
        }
      }
      else if (typeId == gvr::communication::ucc::UccMessageCloudReset::GetTypeId())
      {
        gvr::communication::ucc::UccMessageCloudReset message;
        if (message.parse(buffer))
        {
          std::string messageId = message.getMessageId();
          gvr::utils::Json json = message.getJson();
          std::string jsonMessage;
          json.serialize(jsonMessage);
          std::cout << "UccSessionListenerClient has received a message [ messageId = " << messageId << ", json = " << jsonMessage << "]." << std::endl;

          {
            gvr::communication::ucc::UccMessageCloudResetResponse messageRs;
            messageRs.setMessageId(messageId);
            messageRs.setSuccess(true);
            m_session.send(messageRs);
          }
        }
        else
        {
          std::cout << "UccSessionListenerClient has received a message but parsing failed." << std::endl;
        }
      }
      else if (typeId == gvr::communication::ucc::UccMessageCloudFileDownload::GetTypeId())
      {
        gvr::communication::ucc::UccMessageCloudFileDownload message;
        if (message.parse(buffer))
        {
          std::string messageId = message.getMessageId();
          gvr::utils::Json json = message.getJson();
          std::string jsonMessage;
          json.serialize(jsonMessage);
          std::cout << "UccSessionListenerClient has received a message [ messageId = " << messageId << ", json = " << jsonMessage << "]." << std::endl;

          {
            gvr::communication::ucc::UccMessageCloudFileDownloadResponse messageRs;
            messageRs.setMessageId(messageId);
            messageRs.setSuccess(true);
            {
              messageRs.getJson().set(gvr::communication::ucc::UccMessageCloudFileDownloadResponse::CPayloadName).set(gvr::communication::ucc::UccMessageCloudFileDownloadResponse::CLocalUrlName) = "10.255.10.1/download/software/package/crind-1234.pkg";
            }
            m_session.send(messageRs);
          }
        }
        else
        {
          std::cout << "UccSessionListenerClient has received a message but parsing failed." << std::endl;
        }
      }
      else if (typeId == gvr::communication::ucc::AbsUccMessageRegister::GetTypeId())
      {
        gvr::communication::ucc::UccMessageRegisterRequest message;
        if (message.parse(buffer))
        {
          gvr::utils::Json json = message.getJson();
          std::string jsonMessage;
          json.serialize(jsonMessage);
          std::cout << "UccSessionListenerClient has received a message [ json = " << jsonMessage << "]." << std::endl;

          {
            gvr::communication::ucc::UccMessageRegisterResponse messageRs;
            messageRs.getJson().set(gvr::communication::ucc::UccMessageRegisterResponse::CSuccessName,false).set(gvr::communication::ucc::UccMessageRegisterResponse::CCodeName,"DEVICE_SERVICE_ERROR").set(gvr::communication::ucc::UccMessageRegisterResponse::CMessageName,"Device service error").set(gvr::communication::ucc::UccMessageRegisterResponse::CErrorNumberName,50002);
            m_session.send(messageRs);
          }
        }
        else
        {
          gvr::communication::ucc::UccMessageRegisterResponse messageRs;
          if (messageRs.parse(buffer))
          {
            gvr::utils::Json json = messageRs.getJson();
            std::string jsonMessage;
            json.serialize(jsonMessage);
            std::cout << "UccSessionListenerClient has received a message [ json = " << jsonMessage << "]." << std::endl;
          }
          else
          {
            std::cout << "UccSessionListenerClient has received a message but parsing failed." << std::endl;
          }
        }
      }
      else
      {
        std::cout << "UccSessionListenerClient has received an unknown message." << std::endl;
      }
    }

    void sessionEvent (StatusType status)
    {
      std::cout << "UccSessionListenerClient has received a new status [" << std::dec << static_cast<int>(status) << "]" << std::endl;
    }
  private:
    gvr::communication::ucc::UccSessionClient & m_session;
};


/**
 * @brief      Ucc Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the Ucc tool command line.
 * @param [in] argv List of arguments passed to the Ucc tool command line.
 * @return     Command line expected result code. Ucc Tool always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main (int argc, char **argv)
{
  signal(SIGINT, interrupt);

  if( !parseCommandLineArguments(argc, argv) )
  {
    return 1;
  }

  std::cout << "gvrucc_tool - ip = [" << ip << ":" << port << "]- connecting..." << std::endl;

  gvr::communication::ucc::UccSessionClient session;
  gvr::communication::ucc::UccChannel       channel(ip, port);
  UccSessionListenerClient clientListener(session);

  session.setTimeoutDefault(timeout);
  session.subscribe(clientListener,gvr::communication::ucc::UccMessageStatus::GetTypeId());
  session.subscribe(clientListener,gvr::communication::ucc::UccMessageCloudReset::GetTypeId());
  session.subscribe(clientListener,gvr::communication::ucc::UccMessageCloudFileDownload::GetTypeId());
  session.subscribe(clientListener,gvr::communication::ucc::UccMessageRegisterRequest::GetTypeId());

  //start the session
  session.start(channel);

  if (!session.hasError())
  {
    while( (!session.isLogged()) && (bRunning) ) // Wait until the login was done.
    {
      Poco::Thread::sleep(50);
    }

    while( bRunning ) // Wait until the user press Ctrl + c combinations of keys.
    {
      Poco::Thread::sleep(500);
    }

    std::cout << std::endl << "Terminating application." << std::endl;
  }
  else
  {
    std::cout << "Ucc Session Functional Tool - error starting ucc session. Error: [" << session.getErrorDescription() << "]." << std::endl;
  }

  //stop the session
  session.stop();
  std::cout << "Ucc Session Functional Tool - session closed. status:" << session.getStatusDescription() << std::endl;

  return 0;
}

void showUsage (void)
{
  std::cerr << "Usage: gvrucc_tool [option(s)]" << std::endl
            << "Options:" << std::endl
            << "\t-h\tShow this help message" << std::endl
            << "\t-i\tIP the ucc ip to start a session. If missing the default is -i127.0.0.1." << std::endl
            << "\t-p\tPORT the ucc ip port to start a session. If missing the default is -p5300." << std::endl
            << "\t-t\tTIMEOUT the ucc message timeout, measured in milliseconds, to consider a corresponding message response is missing. If missing the default is -t30000." << std::endl
            << "\t\tFor example: gvrucc_tool -i 10.28.120.7 -p 5300 -t 30000" << std::endl
            << "Description:"<< std::endl<< "\tStarts a session to the given ucc address."<< std::endl<<"\tCtrl+C to quit the session tool" << std::endl;
}

void interrupt (int sig)
{
  if( sig == SIGINT )  bRunning=false;
}

bool parseCommandLineArguments (int argc, char **argv)
{
  char *cvalue = NULL;
  int option;
  std::string sCommands;

  while( (option = getopt(argc, argv, "i:p:t:h?")) != -1 )
  {
    switch (option)
    {
      case 'i':
        ip = optarg;
      break;

      case 'p':
        port=atoi(optarg);
      break;

      case 't':
        timeout=atoi(optarg);
      break;

      case '?':
      case 'h':
      default:
        showUsage();
      return false;
    }
  }

  return true;
}

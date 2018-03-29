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
#include <sstream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <cstdlib>

#include <signal.h>
#include <unistd.h>

#include <Poco/Thread.h>
#include <gvr/communication/ucc/UccChannel.h>
#include <gvr/communication/ucc/session/UccSessionClient.h>
#include <gvr/communication/ucc/message/UccMessageAny.h>
#include <gvr/communication/ucc/IUccSessionListener.h>

#include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>
#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>

#include <gvr/communication/ucc/message/UccMessageServerUpdate.h>
#include <gvr/communication/ucc/message/UccMessageServerUpdateResponse.h>

# include <gvr/communication/ucc/message/UccMessageConnect.h>
# include <gvr/communication/ucc/message/UccMessageConnectResponse.h>


bool bRunning              = true;
std::string ip             = "127.0.0.1";
int port                   = 21310;
unsigned int timeout       = 30000;
unsigned int waittime      = 5000;
int flag                   = 1;
bool expectedResponse      = false;
bool listenerMode          = false;
bool sendRegistrationRspOk = false;
bool sendRegistrationRspNOk= false;
bool sendServerUpdateRspOk = false;
bool sendConnectionRspOk   = false;
std::string file           = "";

class UccSessionClientAny : public gvr::communication::ucc::UccSessionClient
{
public:
  virtual ~UccSessionClientAny (void)
  {
  }

  virtual void removeAllClassifiers(void)
  {
    m_classifiers.clear();
  }
};

class UccListener : public gvr::communication::ucc::IUccSessionListener
{
private:
  gvr::communication::ucc::UccSession & m_session;
  bool m_sendServerUpdateResponseOK;
  bool m_sendRegistrationResponseOK;
  bool m_sendRegistrationResponseNOK;
  bool m_sendConnectionResponseOK;

public:
  virtual ~UccListener (void)
  {}

  UccListener (gvr::communication::ucc::UccSession & session, bool sendRegistrationResponseOK, bool sendRegistrationResponseNOK, bool sendServerUpdateResponseOK, bool sendConnectionResponseOK):
    m_session(session),
    m_sendRegistrationResponseOK(sendRegistrationResponseOK),
    m_sendRegistrationResponseNOK(sendRegistrationResponseNOK),
    m_sendServerUpdateResponseOK(sendServerUpdateResponseOK),
    m_sendConnectionResponseOK(sendConnectionResponseOK)
  {}

  virtual void sessionEvent (BufferType const & buff, TypeId const & type, KeyType const & key)
  {
    gvr::communication::ucc::UccMessageAny msg;
    if (msg.parse(buff))
    {
      gvr::utils::Json json = msg.getJson();
      std::string str;
      json.serialize(str);
      std::cout << std::endl << "Ucc Session Functional Tool Received Message [ flag = "<< static_cast<unsigned int>(msg.getInternalFlag()) <<" ] response:" << std::endl << str  << std::endl << "." << std::endl;

      if( msg.getInternalFlag() == 0 ) // Cloud Messages;
      {
        // Nothing todo here for now....
      }
      else if( msg.getInternalFlag() == 1 ) // UCC Internal Messages;
      {
        gvr::utils::Json json = msg.getJson();

        if( json.get(gvr::communication::ucc::AbsUccMessageInternal::COperationName).asString() ==
            gvr::communication::ucc::UccMessageServerUpdate::CMessageOperation )
        {
          if(m_sendServerUpdateResponseOK)
          {
            sendServerUpdateResponse(json.get(gvr::communication::ucc::UccMessageServerUpdate::CMessageIdName).asString(),
                                     json.get(gvr::communication::ucc::UccMessageServerUpdate::CDeviceIdName).asString());
          }
        }
        else if( json.get(gvr::communication::ucc::AbsUccMessageInternal::COperationName).asString() ==
                 gvr::communication::ucc::UccMessageConnect::CMessageOperation )
        {
          if(m_sendConnectionResponseOK)
          {
            sendServerConnectionResponse(json.get(gvr::communication::ucc::UccMessageConnect::CMessageIdName).asString(),
                                         json.get(gvr::communication::ucc::UccMessageConnect::CDeviceIdName).asString());
          }
        }

      }
      else if( msg.getInternalFlag() == 2 )
      {
        // Always gvr::communication::ucc::UccMessageRegisterRequest;
        if(m_sendRegistrationResponseOK)
        {
          sendRegistrationResponse();
        }
        else if(m_sendRegistrationResponseNOK)
        {
          sendRegistrationResponse(false);
        }
      }
    }
    else
    {
      std::cout << std::endl << "Ucc Session Functional Tool Received Invalid Message." << std::endl;
    }
  }

  virtual void sessionEvent (StatusType)
  {
  }

  virtual void sendServerConnectionResponse (std::string messageId, std::string deviceId)
  {
    gvr::communication::ucc::UccMessageConnectResponse msgUccResp;
    gvr::utils::Json json;

    static char const * CUccConnectRsPayload = "{ \"success\": true }";

    json.parse(CUccConnectRsPayload);

    msgUccResp.setMessageId(messageId);
    msgUccResp.setDeviceId(deviceId);
    msgUccResp.setPayload(json);

    m_session.send(msgUccResp);
    if( m_session.getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
    {
      std::cout << std::endl << "UccSession Error: [" << m_session.getErrorDescription().c_str() << "], sending a message." << std::endl;
    }

    std::cout << std::endl << "UccSession the message UccMessageConnectResponse was sent!." << std::endl;
  }

  virtual void sendRegistrationResponse (bool Ok=true)
  {
    gvr::utils::Json json;
    gvr::communication::ucc::UccMessageRegisterResponse msgUccResp;

    static char const * CUccRegisterRsOk  = "{ \"success\": true, \"config\": { \"gvrSiteId\": 1234, \"deviceId\": \"32459\", \"connect\": { \"uri\": \"wss://device.gilbarco.com/socket/device/32459\", \"certFormat\": \"JKS\", \"cert\": \"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\", \"userid\": \"devices/32459\", \"pwd\": \"K45M4%5kB$KWj15P&dIt\" }, \"deviceList\": [ { \"deviceType\": \"FDT\", \"deviceId\": \"32459\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/32459\" }, { \"deviceType\": \"m5\", \"deviceId\": \"38910\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/38910\" }, { \"deviceType\": \"m5\", \"deviceId\": \"31935\", \"localId\": \"3\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/31935\" }, { \"deviceType\": \"E7000\", \"deviceId\": \"38912\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/38912\" } ] } }";
    static char const * CUccRegisterRsErr = "{ \"message\": \"Device service error\", \"code\": \"DEVICE_SERVICE_ERROR\", \"success\": false, \"errorNumber\": 50002 }";

    if(Ok)
    {
      json.parse(CUccRegisterRsOk);
    }
    else
    {
      json.parse(CUccRegisterRsErr);
    }

    msgUccResp.setJson(json);
    m_session.send(msgUccResp);
    if( m_session.getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
    {
      std::cout << std::endl << "UccSession Error: [" << m_session.getErrorDescription().c_str() << "], sending a message." << std::endl;
    }

    std::cout << std::endl << "UccSession the message UccMessageRegisterResponse was sent!." << std::endl;
  }

  virtual void sendServerUpdateResponse (std::string messageId, std::string deviceId)
  {
    gvr::communication::ucc::UccMessageServerUpdateResponse msgUccResp;
    gvr::utils::Json json;

    static char const * CUccServerUpdateRsPayload = "{  \"success\": true  }";

    json.parse(CUccServerUpdateRsPayload);

    msgUccResp.setMessageId(messageId);
    msgUccResp.setDeviceId(deviceId);
    msgUccResp.setPayload(json);

    {
      std::string collectedInfo;
      json = msgUccResp.getJson();
      json.serialize(collectedInfo);
      std::cout << std::endl << "[" << collectedInfo << "]" << std::endl;
    }


    m_session.send(msgUccResp);
    if( m_session.getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
    {
      std::cout << std::endl << "UccSession Error: [" << m_session.getErrorDescription().c_str() << "], sending a message." << std::endl;
    }

    std::cout << std::endl << "UccSession the message UccMessageServerUpdateResponse was sent!." << std::endl;
  }
};

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

  std::string jsonMessage;
  if (!file.empty())
  {
    std::ostringstream oss;
    if (file.compare("-") == 0)
    {
      oss << std::cin.rdbuf();
    }
    else
    {
      std::ifstream fin;
      fin.open(file.c_str());
      if (fin.is_open())
      {
        oss << fin.rdbuf();
        fin.close();
      }
      else
      {
        std::cout << "Error: File '" << file << "' cannot be opened." << std::endl;
        return 2;
      }
    }
    jsonMessage = oss.str();
  }

  std::cout << "gvrucc_sender - ip = [" << ip << ":" << port << "]- connecting..." << std::endl;

  UccSessionClientAny session;
  gvr::communication::ucc::UccChannel channel(ip, port);
  UccListener listener(session, sendRegistrationRspOk, sendRegistrationRspNOk, sendServerUpdateRspOk, sendConnectionRspOk);

  session.removeAllClassifiers();

  session.setTimeoutDefault(timeout);

  if (listenerMode)
  {
    session.subscribe(listener);
  }

  //start the session
  session.start(channel);

  if (!session.hasError())
  {
    while( (!session.isLogged()) && (bRunning) ) // Wait until the login was done.
    {
      Poco::Thread::sleep(50);
    }

    if (!jsonMessage.empty())
    {
      gvr::utils::Json json;
      gvr::communication::ucc::UccMessageAny anyMessage;

      if (json.parse(jsonMessage))
      {
        anyMessage.setInternalFlag(flag);
        anyMessage.setJson(json);
        std::cout << std::endl << "Sending Message." << std::endl;
        if (expectedResponse)
        {
          gvr::communication::ucc::UccMessageAny anyMessageResponse;
          session.sendSync(anyMessage,anyMessageResponse);
          gvr::utils::Json jsonRs = anyMessageResponse.getJson();
          std::string jsonRsMessage;
          jsonRs.serialize(jsonRsMessage);
          std::cout << std::endl << "Received Message response:" << std::endl << jsonRsMessage  << std::endl << "." << std::endl;
        }
        else
        {
          session.send(anyMessage);
        }
      }
      else
      {
        std::cout << std::endl << "Invalid Message." << std::endl;
      }
    }

    if (listenerMode)
    {
      while( (session.isLogged()) && (bRunning) )
      {
        Poco::Thread::sleep(50);
      }
    }

    std::cout << std::endl << "Terminating application." << std::endl;
  }
  else
  {
    std::cout << "Ucc Session Functional Tool Sender - error starting ucc session. Error: [" << session.getErrorDescription() << "]." << std::endl;
  }

  //stop the session
  session.stop();
  std::cout << "Ucc Session Functional Tool Sender - session closed. status:" << session.getStatusDescription() << std::endl;

  return 0;
}

void showUsage (void)
{
  std::cerr << "Usage: gvrucc_sender [option(s)]" << std::endl
            << "Options:" << std::endl
            << "\t-h\tShow this help message" << std::endl
            << "\t-i\tIP the ucc ip to start a session. If missing the default is -i127.0.0.1." << std::endl
            << "\t-p\tPORT the ucc ip port to start a session. If missing the default is -p5301." << std::endl
            << "\t-t\tTIMEOUT the ucc message timeout, measured in milliseconds, to consider a corresponding message response is missing. If missing the default is -t30000." << std::endl
            << "\t-w\tWAIT the ucc message wait time before sending the message, measured in milliseconds. If missing the default is -t5000." << std::endl
            << "\t-k\tFLAG the ucc message internal flag value to be used. If missing the default value is '1', internal message." << std::endl
            << "\t-l\tListen for ucc messages. If missign default value is false." << std::endl
            << "\t-r\tAn ucc message response is expected. If missing no response message is expected." << std::endl
            << "\t-f\tFILE the ucc message content in json format. If filename is \"-\" then message content is obtained from the standard input. If missing the default is empty." << std::endl
            << "\t-m\tSend a Server Connection response OK!." << std::endl
            << "\t-n\tSend a Server Update response OK!." << std::endl
            << "\t-o\tSend a registration response OK!." << std::endl
            << "\t-u\tSend a registration response FAIL!." << std::endl
            << "\t\tFor example: gvrucc_tool -i 10.28.120.7 -p 5301 -t 30000 -w 5000 -k 1 -l -r -f file.json" << std::endl
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

  while( (option = getopt(argc, argv, "i:p:t:w:k:rlmnouf:h?")) != -1 )
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

      case 'w':
        waittime=atoi(optarg);
      break;

      case 'k':
        flag=atoi(optarg);
      break;

      case 'r':
        expectedResponse = true;
      break;

      case 'n':
        sendServerUpdateRspOk = true;
      break;

      case 'o':
        sendRegistrationRspOk = true;
      break;

      case 'u':
        sendRegistrationRspNOk = true;
      break;

      case 'm':
        sendConnectionRspOk = true;
      break;

      case 'l':
        listenerMode = true;
      break;

      case 'f':
        file = optarg;
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

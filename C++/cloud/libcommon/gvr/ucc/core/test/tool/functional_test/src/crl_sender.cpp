/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file crl_tool.h
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
#include <gvr/communication/crl/CrlChannel.h>
#include <gvr/communication/crl/session/CrlSessionClient.h>
#include <gvr/communication/crl/message/CrlMessageAny.h>
#include <gvr/communication/crl/ICrlSessionListener.h>

bool bRunning           = true;
std::string ip          = "127.0.0.1";
int port                = 21301;
unsigned int timeout    = 30000;
unsigned int waittime   = 5000;
int flag                = 0;
bool expectedResponse   = false;
bool listenerMode       = false;
std::string file        = "";

class CrlSessionClientAny : public gvr::communication::crl::CrlSessionClient
{
public:
  virtual ~CrlSessionClientAny (void)
  {
  }

  virtual void removeAllClassifiers(void)
  {
    m_classifiers.clear();
  }
};

class CrlListener : public gvr::communication::crl::ICrlSessionListener
{
public:
  virtual ~CrlListener (void)
  {}

  virtual void sessionEvent (BufferType const & buff, TypeId const & type, KeyType const & key)
  {
    gvr::communication::crl::CrlMessageAny msg;
    if (msg.parse(buff))
    {
      gvr::utils::Json json = msg.getJson();
      std::string str;
      json.serialize(str);
      std::cout << std::endl << "Crl Session Functional Tool Received Message [ flag = "<< static_cast<unsigned int>(msg.getInternalFlag()) <<" ] response:" << std::endl << str  << std::endl << "." << std::endl;
    }
    else
    {
      std::cout << std::endl << "Crl Session Functional Tool Received Invalid Message." << std::endl;
    }
  }

  virtual void sessionEvent (StatusType)
  {
  }
};

/**
 * showUsage
 * @brief Shows the crl session tool usage help text
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
 * @brief      Crl Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the Crl tool command line.
 * @param [in] argv List of arguments passed to the Crl tool command line.
 * @return     Command line expected result code. Crl Tool always return 0, which means ended scrlessfully.
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

  std::cout << "gvrcrl_sender - ip = [" << ip << ":" << port << "]- connecting..." << std::endl;

  CrlSessionClientAny session;
  gvr::communication::crl::CrlChannel channel(ip, port);
  CrlListener listener;

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
      gvr::communication::crl::CrlMessageAny anyMessage;

      if (json.parse(jsonMessage))
      {
        anyMessage.setInternalFlag(flag);
        anyMessage.setJson(json);
        std::cout << std::endl << "Sending Message." << std::endl;
        if (expectedResponse)
        {
          gvr::communication::crl::CrlMessageAny anyMessageResponse;
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
    std::cout << "Crl Session Functional Tool Sender - error starting crl session. Error: [" << session.getErrorDescription() << "]." << std::endl;
  }

  //stop the session
  session.stop();
  std::cout << "Crl Session Functional Tool Sender - session closed. status:" << session.getStatusDescription() << std::endl;

  return 0;
}

void showUsage (void)
{
  std::cerr << "Usage: gvrcrl_sender [option(s)]" << std::endl
            << "Options:" << std::endl
            << "\t-h\tShow this help message" << std::endl
            << "\t-i\tIP the crl ip to start a session. If missing the default is -i127.0.0.1." << std::endl
            << "\t-p\tPORT the crl ip port to start a session. If missing the default is -p5301." << std::endl
            << "\t-t\tTIMEOUT the crl message timeout, measured in milliseconds, to consider a corresponding message response is missing. If missing the default is -t30000." << std::endl
            << "\t-w\tWAIT the crl message wait time before sending the message, measured in milliseconds. If missing the default is -t5000." << std::endl
            << "\t-k\tFLAG the crl message internal flag value to be used. If missing the default value is '1', internal message." << std::endl
            << "\t-l\tListen for crl messages. If missign default value is false." << std::endl
            << "\t-r\tAn crl message response is expected. If missing no response message is expected." << std::endl
            << "\t-f\tFILE the crl message content in json format. If filename is \"-\" then message content is obtained from the standard input. If missing the default is empty." << std::endl
            << "\t\tFor example: gvrcrl_tool -i 10.28.120.7 -p 5301 -t 30000 -w 5000 -k 1 -l -r -f file.json" << std::endl
            << "Description:"<< std::endl<< "\tStarts a session to the given crl address."<< std::endl<<"\tCtrl+C to quit the session tool" << std::endl;
}

void interrupt (int sig)
{
  if( sig == SIGINT )  bRunning=false;
}

bool parseCommandLineArguments (int argc, char **argv)
{
  char *cvalue = NULL;
  int option;

  while( (option = getopt(argc, argv, "i:p:t:w:k:rlf:h?")) != -1 )
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

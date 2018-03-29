/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file sender-receiver.cpp
 * @author gparis
 * @date Monday, April 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <Poco/Thread.h>

#include <gvrlog/log.h>

#include <gvr/communication/crl/CrlChannel.h>
#include <gvr/communication/crl/session/CrlSessionClient.h>
#include <gvr/communication/crl/message/CrlMessageAny.h>
#include <gvr/communication/crl/ICrlSessionListener.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloud.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventRequest.h>
#include <gvr/communication/ucc/message/UccMessageAny.h>
#include <gvr/utils/Json.h>
#include <gvr/communication/spot/Utils.h>

#include "../include/MessageTable.h"
#include "../include/CrlAnyMessageWorker.h"
#include "../include/UccServerManager.h"

using namespace gvr::communication;
using namespace gvr::cloud;
using gvr::communication::spot::Utils;



//----------------------  Application Constants  ------------------//

const unsigned int CuDefaultUCCPort = 21300;
const unsigned int CuDefaultCRLPort = 21301;
const char*        CpcDefaultPayloadTrunkDir = "payloads/";
const char*        CMessageIdLabel           = ucc::AbsUccMessageCloud::CMessageIdName;
const char*        COperationLabel           = ucc::AbsUccMessageCloudEventRequest::COperationName;
const char*        CUp1Right18ThenEraseToEOL = "\033[1A\033[18C\033[K";
const char*        g_CpcLogConfigurationPathName = "configuration/Log.ini";

//-----------------------------------------------------------------//



//---------------------- Global Application variables  ------------------//

DEFINE_LOG_MODULE(main);
DEFINE_LOG_MODULE(core); // for AbsOptCloudClientWorker use.

bool         blRunning  = true;
bool         blUCCmode  = false;
bool         blCRLmode  = false;

std::string  g_sIP       = "127.0.0.1";
         int g_iPort     = -1;
unsigned int g_uTimeout  = 7000;

MessageTable  g_messageTable;///< @brief Global Message Table.

//-----------------------------------------------------------------------//


void showCopyright (void)
{
  std::cout << "\nUCC/CRL Emulator and command/event Sender/Receiver.\n"
               "Copyright © 2015 Gilbarco Inc.\nFor Payment division internal use only.\n\n";
}

int showMenu (void)
{
  char cOpt = 0;
  std::string sDummy;
  std::cout << "\nTo send a message, please select one numeric option from the menu below:\n"
            << "\n  1) Pick up a file containing a payload.\n"
            << "\n  2) Paste a payload.\n"
            << "\n  3) Repeat the last sent payload.\n"
            << "\n  4) Change the request directory for payload files.\n"
            << "\n  0) Exit this tool.\n\n"
            << "Your election is: ";//<< "Your election is:\033[1;31 ";

  while( cOpt < '0' || cOpt > '4' )
  {
    std::cin  >> cOpt;
    std::cin.ignore(1000, '\n');
    std::cout << CUp1Right18ThenEraseToEOL;
  }

  std::cout << "\n\nYour selection is: " << cOpt << std::endl << std::endl;

  return cOpt - '0';
}

/**
 * showUsage
 * @brief Shows the ucc session tool usage help text
 * */
void showUsage (void)
{
  std::cerr << "\nUsage: gvrcloud_tool [ -C | -U ] [option(s)]\n"
            << "\t-C This tools is used as a  CRL Client.\n"
            << "\t-U This tools is used as an UCC Server.\n\n"
            << "Options:\n"
            << "\t-h\tShow this help message" << std::endl
            << "\t-i\tIP the IP address to start a session. If missing the default is 127.0.0.1.\n"
            << "\t-p\tPORT the IP port to start a session. If missing the default is: 21300 for UCC, 21301 for CRL.\n."
            << "\t-t\tTIMEOUT the ucc message timeout, measured in milliseconds, to consider a corresponding message response is missing. If missing the default is -t30000.\n"
            << "\t\tFor example: gvrcloud_tool -i 10.28.120.7 -p 5301 -t 30000\n"
            << "Description:\n\tStarts a session to the given address and port number.\n"
            << "\tCtrl+C is the last resource to quit this tool.\n\n";
}

/**
 * interrupt
 * @brief Helper function to handle an interrupt signal.
 * */
void interrupt (int sig)
{
  if( sig == SIGINT )  blRunning = false;
}

/**
 * parseCommandLineArguments
 * @brief Helper function to parse command line arguments.
 * */
bool parseCommandLineArguments (int argc, char **argv)
{
  char *cvalue = NULL;
  int option;

  while( (option = getopt(argc, argv, "CUi:p:t:h?")) != -1 )
  {
    switch (option)
    {
      case 'C':
        blCRLmode = true;
    break;

      case 'U':
        blUCCmode = true;
    break;

      case 'i':
        g_sIP = optarg;
      break;

      case 'p':
        g_iPort=atoi(optarg);
      break;

      case 't':
        g_uTimeout=atoi(optarg);
      break;

      case '?':
      case 'h':
      default:
        showUsage();
        return false;
    }
  }

  if( blCRLmode && blUCCmode)
  {
    std::cout << "This tool can't be configured as UCC server and CRL client simultaneously!!!\n\n";
    showUsage();
    return false;
  }
  else if( !blCRLmode && !blUCCmode)
  {
    std::cout << "This tool need to be configured as UCC server, or alternatively as CRL client.\n\n";
    showUsage();
    return false;
  }

  return true;
}

bool getPayloadFromFile( std::string & sDirectory, std::string & sPayload )
{
  std::string sFilename, sFullPath( sDirectory );

  Utils::getStringFromConsole( "Payload filename: ", sFilename );

  sFullPath += sFilename;
  sFullPath += ".json";

  std::ifstream fin;
  std::ostringstream oss;

  fin.open(sFullPath.c_str());
  if( fin.is_open() )
  {
    oss << fin.rdbuf();
    fin.close();
    sPayload = oss.str();
    return true;
  }
  else
  {
    std::cout << "Error opening file '" << sFullPath.c_str() << "' , please check the name and try again.\n";
    return false;
  }
}

bool getPayloadFromConsole( std::string & sPayload )
{
  std::ostringstream oss;

  std::cout << "Please paste payload: (Ctrl D to finish typing)\n";
  oss << std::cin.rdbuf();
  sPayload = oss.str();

  return ( ! Utils::isBlankString(sPayload) );
}

bool getPayloadRequestDir( std::string & sPayloadRequestDir )
{
  std::string sNewDir;

  Utils::getStringFromConsole( "Trunk Path for payloads: ", sNewDir );

  if( Utils::canAccess(sNewDir) )
  {
    int len = sNewDir.length();
    if( sNewDir[len] != '/')
    {
      sNewDir[len] = '/';
    }

    sPayloadRequestDir = sNewDir;
    return true;
  }
  else
  {
    std::cout << "Invalid entry. Can not access to directory \"" << sNewDir << '"'<< std::endl;
    return false;
  }
}

/**
 * @brief      Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the tool command line.
 * @param [in] argv List of arguments passed to the tool command line.
 * @return     Command line expected result code. This tool always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main(int argc, char** argv)
{
  showCopyright();

  if( ! gvr::log::init(g_CpcLogConfigurationPathName, gvr::log::CNORMAL_USE_OF_LOG_LIBRARY) )
  {
    std::cerr << "Logger initialization failed. "
                 "Please check the existence of log directory or the configuration file/directory.\n"
                 "Application abnormally terminated.\n\n";
    return 1;
  }

  LOG_VInfo(main, "CRL-UCC emulator is starting. %s", "This application emulates an UCC or CRL session.");

  if( !parseCommandLineArguments(argc, argv) )
  {
    return 1;
  }

  signal(SIGINT, interrupt);
  g_messageTable.LoadTable();

  bool blRet = false;
  int nOpt = -1;
  std::string sPayload;
  std::string sPayloadRootDir( CpcDefaultPayloadTrunkDir );
  std::string sPayloadRequestDir( CpcDefaultPayloadTrunkDir );
  std::string sPayloadResponseDir( CpcDefaultPayloadTrunkDir );
  Poco::SharedPtr<crl::CrlChannel>        qClientChannel;
  Poco::SharedPtr<StreamChannelServer>    qChannelServer;
  Poco::SharedPtr<crl::CrlSessionClient>  qClientSession;
  Poco::SharedPtr<UccServerManager>       qServerManager;
  Poco::SharedPtr<CrlAnyMessageWorker>    qClientListener;

  if( blUCCmode )
  {
    if(g_iPort == -1) // port was not supplied by command line
    {
      g_iPort = CuDefaultUCCPort;
    }

    qChannelServer = new StreamChannelServer( g_sIP, g_iPort);
    qServerManager = new UccServerManager(*qChannelServer);
    qChannelServer->start();
  }
  else if( blCRLmode )
  {
    if(g_iPort == -1) // port was not supplied by command line
    {
      g_iPort = CuDefaultCRLPort;
    }

    qClientSession = new crl::CrlSessionClient;
    qClientChannel = new crl::CrlChannel(g_sIP, g_iPort);
    qClientListener = new CrlAnyMessageWorker(qClientSession);

    qClientSession->setTimeoutDefault(g_uTimeout);
    qClientListener->doSubscriptions();

    //start the session
    qClientSession->start(*qClientChannel);
  }

  sPayloadRequestDir  += ( blUCCmode ? "ucc/request/" : "crl/request/" );
  sPayloadResponseDir += ( blUCCmode ? "ucc/response/" : "crl/response/" );
  std::cout << "Payload Directory: " << sPayloadRootDir << std::endl << std::endl;

  while ( blRunning && nOpt != 0 )
  {
      int nOpt = showMenu();
      if( nOpt == 0 ) // option to leave the application.
      {
        blRunning = false;
        if( blUCCmode )
        {
          qChannelServer->stop();
          exit(0);
        }
        else if( blCRLmode )
        {
          std::cout << "doUnsubscriptions(): " << std::endl;
          qClientListener->doUnsubscriptions();

          std::cout << "qClientListener->stop(): " << std::endl;
          qClientListener->stop();

          std::cout << "qClientSession->stop(): " << std::endl;
          qClientSession->stop();
        }
        Poco::Thread::sleep(500);
        return 0;
      }

      switch(nOpt)
      {
        case 1:
          blRet = getPayloadFromFile( sPayloadRequestDir, sPayload );
          if( ! blRet ) continue;
          break;

        case 2:
          blRet = getPayloadFromConsole( sPayload );
          if( ! blRet ) continue;
          break;

        case 4:
          blRet = getPayloadRequestDir( sPayloadRequestDir );
          if( ! blRet ) continue;
          else
          {
            std::cout << "\nNew payload request directory: " << sPayloadRequestDir << std::endl << std::endl;
          }
          continue;

        case 3:
        default:
          break; // re-send the last payload
      }

      std::cout << "\nPayload to send:\n\n" << sPayload << std::endl << std::endl;

      if( blUCCmode && qServerManager.get() != NULL )
      {
        gvr::utils::Json  json;
        UccMessageAny     msgAny;

        msgAny.setInternalFlag(0); // default value
        json.parse( sPayload.c_str() );
        LOG_VTrace( main, "\nPayload from Json:\n%s\n", json.serialize().c_str() );

        if( json.get(COperationLabel).isString() )// The message Json has a field named "operation-cmd".
        {
          std::string sKey("ucc ");

          sKey += json.get(COperationLabel).asString();
          LOG_VTrace( main, "Searching in table for key: \"%s\"\n", sKey.c_str() );
          const MessageTable::TableEntry * pTableEntry = g_messageTable.getTableEntry(sKey);
          if( pTableEntry ) // A table entry was found having the same "ucc <operation field>"
          {
            std::cout << "New flag obtained from table: " << pTableEntry->iCommandFlag << std::endl;
            msgAny.setInternalFlag(pTableEntry->iCommandFlag); // Appropriate value from the table.
          }
        }
        msgAny.setJson( json );

        int error = qServerManager->send(msgAny);
        if( error != UccSession::ISessionError::CNoError )
        {
          std::cout << "Error sending message to UCC client. Error Code = " << std::hex
                    << error << std::dec << std::endl;
        }
      }
      else if( blCRLmode )
      {
        gvr::utils::Json   json;
        crl::CrlMessageAny msgAny;

        msgAny.setInternalFlag(0); // default value
        json.parse( sPayload.c_str() );
        LOG_VTrace( main, "\nPayload from Json:\n%s\n", json.serialize().c_str() );
/*
        if( json.get(COperationLabel).isString() )// The message Json has a field named "operation".
        {
          std::string sKey("crl ");

          sKey += json.get(COperationLabel).asString();
          LOG_VTrace(main, "Searching in table for key: \"%s\"\n", sKey);
          const MessageTable::TableEntry * pTableEntry = g_messageTable.getTableEntry(sKey);
          if( pTableEntry ) // A table entry was found having the same "crl <operation field>"
          {
            msgAny.setInternalFlag(pTableEntry->iCommandFlag); // Appropriate value from the table.
          }
        }
*/      //TODO: Confirm with Knta the flag is always 0 in CRL

        msgAny.setJson( json );

        int error = qClientSession->send(msgAny);
        if( error != crl::CrlSession::ISessionError::CNoError )
        {
          std::cout << "Error sending message to CRL server. Error Code = " << std::hex
                    << error << std::dec << std::endl;
        }
      }

  } // while


  return 0;
}


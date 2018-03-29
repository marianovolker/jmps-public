/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file spot_session_tool.cpp
 * @author mgrieco
 * @date Friday, January 05, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvrspot_session_tool/spot_session_tool.h"

#include <iostream>
#include <iomanip>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>

#include <signal.h>
#include <unistd.h>

using namespace gvr::communication::spot;
using namespace std;


/**
 * @brief      Spot Session Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the spot tool command line.
 * @param [in] argv List of arguments passed to the spot tool command line.
 * @return     Command line expected result code. Spot Tool always return 0, which means ended successfully.
 * @see spot_session_tool.showUsage method for arguments detail.
 * @since 1.0.0
 */


bool bRunning             = true;
bool blHwrInfo            = false;
bool blSwrInfo            = false;
bool blPkgInfo            = false;
bool blDebInfo            = false;
bool blCmdReset           = false;
bool blPkgActiv           = false;
bool blPkgDownload        = false;
bool blResDownload        = false;
bool blSysStatusInfo      = false;
bool blDebDownload        = false;
bool blReleaseDownload    = false;

int  iPlatform            = 5;
std::string ip            = "127.0.0.1";
int port                  = 5200;
unsigned long clientId    = 0x86;
std::string sPackageDwlName;
std::string sPackageActName  = "";
std::string sResourceName = "";
unsigned int resTypeId    = 0x00;
unsigned int deviceId     = 0xFF;

bool parseCommandLineArguments( int argc, char **argv );
void interrupt(int sig);


int main (int argc, char **argv)
{
  signal(SIGINT, interrupt);

  if( !parseCommandLineArguments(argc, argv) )
  {
    return 1;
  }

  std::cout << "gvrspot_session_tool - ip = [" << ip << ":" << port << "] clientId [" << std::hex << clientId << "] platform [M" << iPlatform <<"]- connecting..." << std::endl;

  SpotSession::EDevicePlatform ePlatform = SpotSession::eM5;

  switch( iPlatform ) // This switch will be eliminated when session recognizes device platform
  {                   // automatically.
    case 3:
      ePlatform =  SpotSession::eM3;
      break;

    case 7:
      ePlatform =  SpotSession::eM7;
      break;

    case 5:

    default:
    break;
  }

  SpotSession session( ePlatform );
  SpotChannel channel(ip, port);

  //start the session
  session.start(channel, clientId, 0, 0);

  if (!session.hasError())
  {
    while( (!session.isLogged()) && (bRunning) ) // Wait until the login was done.
    {
      Poco::Thread::sleep(50);
    }

    if( (blHwrInfo) && (bRunning) )
    {
      SpotMessageHardwareInfo msgRequest;
      SpotMessageHardwareInfoResponse msgReply;
      session.sendSync(msgRequest, msgReply, 1000);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
    }

    if( (blSwrInfo) && (bRunning) )
    {
      SpotMessageSoftwareInfo msgRequest;
      SpotMessageSoftwareInfoResponse msgReply;
      session.sendSync(msgRequest, msgReply, 1000);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
    }

    if( (blPkgInfo) && (bRunning) )
    {
      SpotMessagePackageInfo msgRequest;
      SpotMessagePackageInfoResponse msgReply;
      session.sendSync(msgRequest, msgReply, 1000);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
    }

    if( (blDebInfo) && (bRunning) )
    {
      SpotMessageDebianInfo msgRequest;
      SpotMessageDebianInfoResponse msgReply;
      std::cout << std::endl << "sending a Debian Info Request"<< std::endl;
      session.sendSync(msgRequest, msgReply, 5000);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
      else
      {
        std::cout << std::endl << " Debian Info Response: " << msgReply.getInfoResponseData() << std::endl;
      }
    }

    if(blDebDownload && bRunning)
    {
      std::cout << "Debian Package Download: " << sPackageDwlName << std::endl << "Device Platform: M" << iPlatform << std::endl;

      SPOTFileDownload downloadFlow(session);
      bool blret = downloadFlow.downloadDebianPackage(sPackageDwlName);
      if( blret == false )
      {
        std::cout << "\n*** Debian Package Download or Process Error ***  Flow error = " << std::hex << downloadFlow.getLastOperationError() << std::dec << std::endl;
      }
      else
      {
        std::cout << "\nDebian Package " << sPackageDwlName << " downloaded OK! \n";
      }
    }

    if( (blReleaseDownload) && (bRunning) )
    {
      std::cout << "Release Download: " << sPackageDwlName << std::endl << "Device Platform: M" << iPlatform << std::endl;

      SPOTFileDownload downloadFlow(session);
      bool blret = downloadFlow.downloadBundle(sPackageDwlName);
      if( blret == false )
      {
        std::cout << "\n*** Release Download or Process Error ***  Flow error = " << std::hex << downloadFlow.getLastOperationError() << std::dec << std::endl;
      }
      else
      {
        std::cout << "\nRelease " << sPackageDwlName << " downloaded OK! \n";
      }
    }

    if( (blPkgDownload) && (bRunning) )
    {
      std::cout << "Package Download: " << sPackageDwlName << std::endl
                << "Device Platform: M" << iPlatform << std::endl;

      SPOTFileDownload downloadFlow(session);

      bool blret = downloadFlow.downloadSPOTPackage(sPackageDwlName);
      if( blret == false )
      {
        std::cout << "\n*** Package Download or Process Error ***  Flow error = " << std::hex
                  << downloadFlow.getLastOperationError() << std::dec << std::endl;
      }
      else
      {
        std::cout << "\nPackage " << sPackageDwlName << " downloaded OK! \n";
      }
    }

    if( (blResDownload) && (bRunning) )
    {
      std::cout << "Resource to download: " << sResourceName <<  " - " << std::hex << resTypeId << std::endl;
      SPOTFileDownload downloadFlow(session);
      bool blret = downloadFlow.downloadResourceFile(sResourceName, resTypeId);
      if( blret == false )
      {
        std::cout << "\n*** Resource Download Error ***  Flow error = " << std::hex
                  << downloadFlow.getLastOperationError() << std::dec << std::endl;
      }
      else
      {
        std::cout << "\nResource " << sResourceName << " downloaded OK! \n";
      }
    }

    if( (blPkgActiv) && (bRunning) )
    {
      std::cout << "Package Activation data: " << sPackageActName << std::endl;

        SpotMessagePackageActivation msgRequest;
        SpotMessagePackageActivationResponse msgReply;
        msgRequest.setVgdPlatform((iPlatform == 7));//is a vanguard test
        msgReply.setVgdPlatform((iPlatform == 7));//is a vanguard test

        /* Note: check the use of time activation parameter.
         * Time of automatic activation. ASCII format YYYY.MM.DD-hh:mm
         * 0000.00.00-00:00 means immediate activation
         * 0000.00.00-00:01 means Debian Packet activation PacketDeb[80] if Debian activation is the name of package to activate.
         * 0000.00.00-00:02 means Debian Packet activation only for test. Tries activation and reports output but does not install.
         * 0000.00.00-00:03 means activate all Debians.
         * 0000.00.00-00:04 means Debian Package delete of PacketDeb[80] if Debian activation is the name of package to delete.
         * */
        msgRequest.setPackageActivationName(sPackageActName);
        msgRequest.setPackageActivationTime("0000.00.00-00:00");
        if(!sPackageActName.empty())
        {
          msgRequest.setPackageActivationTime("0000.00.00-00:01");//means activate the package name below.
        }

        std::cout << std::endl << "sending a Package Activation"<< std::endl;
        session.sendSync(msgRequest, msgReply, 15000);
        if( session.getError() != SpotSession::SpotSessionError::CNoError )
        {
          std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
        }
        else
        {

          if(msgReply.getAckCode() == ISpotMessage::CErrorCodeNoError || msgReply.getAckCode() == ISpotMessage::CErrorCodeDebianActivation)
          {
            if(msgReply.getAckCode() == ISpotMessage::CErrorCodeDebianActivation)
            {
              std::cout << std::endl << "Debian Package Activation Ok" << std::endl;
            }
            else
            {
              std::cout << std::endl << "Package Activation Ok " << std::endl;
            }
          }
          else
          {
            if(msgReply.getAckCode() == 0x1B)
            {
              std::cout << std::endl << "Package Activation: no packages to update - " << std::endl;
            }
            else
            {
              std::cout << std::endl << "Package Activation Error - error code: "<< std::setfill('0') << std::hex << static_cast<int>(msgReply.getAckCode()) << std::endl;
            }
          }
        }
    }

    if( (blCmdReset) && (bRunning) )
    {
      SpotMessageReset msgRequest;
      session.send(msgRequest);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
    }

    if( (blSysStatusInfo) && (bRunning) && (session.isLogged()) )
    {
      // Wait for the unsolicited System Status Notification if the platform is m3 ...
      if( iPlatform == 3 )
      {
        Poco::Thread::sleep(1500);
      }

      SpotMessageSystemStatus msgRequest;
      SpotMessageSystemStatusResponse msgReply;
      std::cout << std::endl << "Sending a System Status Notification request for subsystem type [0x" << std::hex << deviceId << "]" << std::endl;

      msgRequest.setSubSystemStatusType(static_cast<SpotMessageSystemStatus::ESubsystemTypes>(deviceId));

      session.sendSync(msgRequest, msgReply, 5000);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        std::cout << std::endl << "Session Error: [" << session.getErrorDescription() << "], sending a message." << std::endl;
      }
      else
      {
        std::cout << "System Status Notification Info Response Qty of items: " << msgReply.getItemQty() << std::endl;
        std::cout << "System Status Notification Info Fields: " << std::endl;
        printf("Device: [0x%x], Status: 0x%x, StatusExt: 0x%x, ErrorCode: 0x%x\n\n\n", msgReply.getItemData(0).deviceCode,
                                                                                       msgReply.getItemData(0).status,
                                                                                       msgReply.getItemData(0).statusExt,
                                                                                       msgReply.getItemData(0).errorCode );
      }
    }

    while( bRunning ) // Wait until the user press Ctrl + c combinations of keys.
    {
      Poco::Thread::sleep(500);
    }

    std::cout << std::endl << "Terminating application." << std::endl;
  }
  else
  {
    std::cout << "Spot Session Functional Tool - error starting spot session. Error: [" << session.getErrorDescription() << "]." << std::endl;
  }

  //stop the session

  session.stop();

  std::cout << "Spot Session Functional Tool - session closed. status:" << session.getStatusDescription() << std::endl;

  return 0;
}


/**
 * showUsage
 * @brief Shows the spot session tool usage help text
 * */
static void showUsage()
{
  std::cerr << "Usage: spot_session_tool [option(s)]" << std::endl
            << "Options:" << std::endl
            << "\t-h\tShow this help message" << std::endl
            << "\t-i\tIP the spot ip to start a session. If missing the default is -i127.0.0.1." << std::endl
            << "\t-p\tPORT the spot ip port to start a session. If missing the default is -p5200." << std::endl
            << "\t-c\tCLIENTID the application client id to start a session. If missing the default is -c86." << std::endl
            << "\t-m\tOPT \tOPT is the opt platform number mode to be tested (m3, m5, m7). If missing the default is -m5." << std::endl
            << "\t-d\tAdditionally, sends one of the following commands: " << std::endl
            << "\t\t\t CmdReset"      << std::endl
            << "\t\t\t DebInfo"       << std::endl
            << "\t\t\t HwrInfo"       << std::endl
            << "\t\t\t SwrInfo"       << std::endl
            << "\t\t\t PkgInfo"       << std::endl
            << "\t\t\t PkgActiv"      << std::endl
            << "\t\t\t PkgDownload"   << std::endl
            << "\t\t\t DebDownload"   << std::endl
            << "\t\t\t SysStatusInfo" << std::endl
            << "\t\t\t ResDownload"   << std::endl
            << "\t\t\t ReleaseDownload"  << std::endl
            << "\t\tSome commands require a data parameter, according to the command send it as follow: " << std::endl
            << "\t\t-a\tActive-Package, the package name to be activated if it is a debian." << std::endl
            << "\t\t-w\tDownload-Package, the SPOT package name to be downloaded or the Release bundle for a complete download.\n"
            << "\t\t-r\tDownload-Resource, the SPOT resource name to be downloaded.\n"
            << "\t\t-t\tDownload-Resource-Id, the SPOT resource id to be downloaded.\n"
            << "\t\t-s\tSubSystemType, the device id to be queried according to the following list:\n"
            << "\t\t\t\t 0x01 The whole system.\n"
            << "\t\t\t\t 0x02 Security module subsystem.\n"
            << "\t\t\t\t 0x04 Display subsystem.\n"
            << "\t\t\t\t 0x08 Keyboard subsystem.\n"
            << "\t\t\t\t 0x10 Card reader subsystem.\n"
            << "\t\t\t\t 0x20 SAM interface-module subsystem.\n"
            << "\t\tIf you want to send more than one command you could create a list separate by a ',': " << std::endl
            << "\t\tFor example: gvrspot_session_tool -i 10.28.120.7 -p 5200 -d DebInfo,HwrInfo,PkgInfo,CmdReset -agvr-dvsdk" << std::endl
            << "Description:"<< std::endl<< "\tStarts a session to the given spot address."<< std::endl<<"\tCtrl+C to quit the session tool" << std::endl;
}

/**
 * interrupt
 * @brief Helper function to handle an interrupt signal.
 * */
void interrupt(int sig)
{
  if( sig == SIGINT )  bRunning=false;
}

/**
 * parseCommandLineArguments
 * @brief Helper function to parse command line arguments.
 * */
bool parseCommandLineArguments( int argc, char **argv )
{
  char *cvalue = NULL;
  int option;
  std::string sCommands;
  deviceId = SpotMessageSystemStatus::eSPOT_SYS_MAIN;

  while( (option = getopt(argc, argv, "a:c:d:f:i:m:p:r:s:t:w:h?")) != -1 )
  {
    switch (option)
    {
      case 'i':
        ip = optarg;
      break;

      case 'p':
        port=atoi(optarg);
      break;

      case 'c':
        clientId = strtoul(optarg, 0, 16);
      break;

      case 'd':
      {
        sCommands=optarg;
      }
      break;

      case 'w':
      {
        sPackageDwlName = optarg;
      }
      break;

      case 'a':
      {
        sPackageActName = optarg;
      }
      break;

      case 'r':
      {
        sResourceName = optarg;
      }
      break;

      case 's':
      {
        deviceId = strtoul(optarg, 0, 16);
        if( deviceId != SpotMessageSystemStatus::eSPOT_SYS_MAIN           &&
            deviceId != SpotMessageSystemStatus::eSPOT_SYS_SECURITYMODULE &&
            deviceId != SpotMessageSystemStatus::eSPOT_SYS_DISPLAY        &&
            deviceId != SpotMessageSystemStatus::eSPOT_SYS_KEYBOARD       &&
            deviceId != SpotMessageSystemStatus::eSPOT_SYS_CARDREADER     &&
            deviceId != SpotMessageSystemStatus::eSPOT_SYS_GEMCORE         )
        {
          deviceId = SpotMessageSystemStatus::eSPOT_SYS_MAIN;
        }
      }
      break;

      case 't':
      {
        resTypeId = strtoul(optarg, 0, 16);
      }
      break;

      case 'm':
        iPlatform = atoi(optarg);
        if(iPlatform != 3 && iPlatform != 5 && iPlatform != 7)
        {
          iPlatform  = 5;
        }
      break;

      case '?':
      case 'h':
      default:
        showUsage();
      return false;
    }
  }

  if( sCommands.find("CmdReset") != std::string::npos )
    blCmdReset=true;

  if( sCommands.find("DebInfo") != std::string::npos )
    blDebInfo=true;

  if( sCommands.find("HwrInfo") != std::string::npos )
    blHwrInfo=true;

  if( sCommands.find("SwrInfo") != std::string::npos )
    blSwrInfo=true;

  if( sCommands.find("PkgInfo") != std::string::npos )
    blPkgInfo=true;

  if( sCommands.find("PkgDownload") != std::string::npos )
    blPkgDownload=true;

  if( sCommands.find("DebDownload") != std::string::npos )
    blDebDownload=true;

  if( sCommands.find("PkgActiv") != std::string::npos )
    blPkgActiv=true;

  if( sCommands.find("ResDownload") != std::string::npos )
    blResDownload = true;

  if( sCommands.find("SysStatusInfo") != std::string::npos )
    blSysStatusInfo = true;

  if( sCommands.find("ReleaseDownload") != std::string::npos )
    blReleaseDownload = true;

  return true;
}



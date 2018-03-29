/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file cloud_main.cpp
 * @author pcantarini
 * @date Wednesday, September 29, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <exception>
#include <signal.h>
#include <unistd.h>

#include <cloud.h>
#include <CrlManager.h>
#include <CloudStatus.h>
#include <workers/OptCloudClientWorkerReset.h>
#include <workers/OptCloudClientWorkerFileActivation.h>
#include <workers/FileUploadWorker.h>
#include <workers/SpotReleaseDownloadWorker.h>
#include <workers/CloudStatusWorker.h>
#include <workers/SystemStatusNotificationWorker.h>
#include <workers/CloudReconnectWorker.h>
#include <workers/SecurityChangeWorker.h>

#include <gvrlog/log.h>

#include <gvr/communication/spot/SpotChannel.h>
#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/spot/SPOTFileDownloadFlow.h>
#include <gvr/communication/ucc/session/UccSessionClient.h>

#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>

using namespace gvr::communication::spot;
using namespace gvr::communication::ucc;

using Poco::Util::IniFileConfiguration;
using Poco::AutoPtr;

static const char * pcDefaultUloadableListFile = "/gilbarco/resources/cloud/cloud-PH-logs.cfg";

bool bRunning                 = true;
std::string spotIp            = "127.0.0.1";
std::string spotRealIp        = "127.0.0.1"; /**@brief spot ip variable to save the real value from the spot configuration*/
int spotPlatform              = 5;
int spotPort                  = 5200;
unsigned long spotClientId    = 0x86;
std::string uccIp             = "127.0.0.1";
int uccPort                   = 21300;
std::string crlIp             = "127.0.0.1";
int crlPort                   = 21301;
std::string spotReleaseFolder = "/gilbarco/releases";
std::string spotReleaseFile   = "release.zip";
std::string cloudUploadFolder = "/gilbarco/appsdata/CLOUD/upload";
int spotDefaultTimeout        = 15000;
int uccDefaultTimeout         = 90000;
int crlDefaultTimeout         = 90000;

void interrupt(int sig);

DEFINE_LOG_MODULE(main);

/**
 * @brief Helper function to parse command line arguments
 * @param[in] int argc number of arguments
 * @param[in] char** argv command line arguments
 * @return true if the args are ok, false otherwise
 */
bool parseCommandLineArguments (int, char **);

/**
 * @brief Helper function to show PH help/usage when it is called from the command line
 */
void showUsage ();

std::string const CLogConfigFilename = "logger.ini"; // logger config filename
std::string const CAppConfigFilename = "gvrcloud.ini"; // app config filename
std::string sConfigDir = "./"; // default root config dir
const char*  g_CpcAppSpotConfigurationPathName = "/etc/config/network";/** @brief default spot network configuration file*/

/**
 * @brief      Payload Handler main entry point.
 * @param [in] argc Amount of arguments passed to the cloud client command line.
 * @param [in] argv List of arguments passed to the cloud client command line.
 * @return     Command line expected result code. Cloud Client always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main (int argc, char** argv)
{
  signal(SIGINT, interrupt);

  if (!parseCommandLineArguments(argc, argv))
  {
	  return 1;
  }
  std::cout << " PH - root config dir: " << sConfigDir << std::endl;

  {
    std::string sLogConfigFullPath(sConfigDir + CLogConfigFilename);
    std::cout << " PH - logger config full path: " << sLogConfigFullPath << std::endl;
    if( !gvr::log::init(sLogConfigFullPath.c_str(), gvr::log::CAPPEND_DATE_AS_YYYYMMDD_TO_FILENAMES_OF_LOGS) )
    {
      std::cerr << "Logger initialization failed. "
                   "Please check the existence of log directory or the configuration file/directory.\n"
                   "Application abnormally terminated.\n\n";
      return 1;
    }
  }

  LOG_VInfo(main, "");
  LOG_VInfo(main, "------------------------------------------------------------------------");
  LOG_VInfo(main, "----------------------------- Payload Handler is starting...");
  LOG_VInfo(main, "------------------------------------------------------------------------");

  SpotSession::EDevicePlatform ePlatform = SpotSession::eM5;

  // Load configuration from .ini file type.
  AutoPtr<IniFileConfiguration> pConfigApp;
  std::string sAppConfigFullPath = sConfigDir + CAppConfigFilename;
  std::string sUloadableListFile;

  try
  {
    std::cout << " PH - app configuration file full path: " << sAppConfigFullPath << std::endl;
    LOG_VInfo(main, " PH - app configuration file full path: %s\n", sAppConfigFullPath.c_str());
    pConfigApp = new IniFileConfiguration(sAppConfigFullPath);

    spotIp        = pConfigApp->getString("SpotIp", "127.0.0.1");
    spotPort      = strtoul(pConfigApp->getString("SpotPort", "5200").c_str()  , 0, 10);
    spotPlatform  = strtoul(pConfigApp->getString("SpotPlatform", "5").c_str() , 0, 10);
    spotClientId  = strtoul(pConfigApp->getString("SpotClientId", "86").c_str(), 0, 16);
    uccIp         = pConfigApp->getString("UccIp", "127.0.0.1");
    uccPort       = strtoul(pConfigApp->getString("UccPort", "21300").c_str()   , 0, 10);
    crlIp         = pConfigApp->getString("CrlIp", "127.0.0.1");
    crlPort       = strtoul(pConfigApp->getString("CrlPort", "21301").c_str()   , 0, 10);

    spotReleaseFolder   = pConfigApp->getString("SpotReleaseFile", "spot-release.zip");
    cloudUploadFolder   = pConfigApp->getString("CloudUploadDir", "/gilbarco/appsdata/CLOUD/upload");
    sUloadableListFile  = pConfigApp->getString("UploadableLogFiles", pcDefaultUloadableListFile);

    if( spotPlatform == 3 )
    {
      ePlatform = SpotSession::eM3;
    }
    else if( spotPlatform == 5 )
    {
      ePlatform = SpotSession::eM5;
    }
    else if( spotPlatform == 7 )
    {
      ePlatform = SpotSession::eM7;
    }
  }
  catch(...)
  {
    LOG_VError(main, " PH cannot open configuration file [%s].", sAppConfigFullPath.c_str());
    LOG_Info(main, " PH is starting with default values.");
  }

  // Load configuration from .ini file type.
  AutoPtr<IniFileConfiguration> pConfigSpot;
  try
  {
    /* NOTE: IniFileConfiguration is read only now.
     * The spot configuration file inficated on g_CpcAppSpotConfigurationPathName should not be modified!
     * */
    pConfigSpot = new IniFileConfiguration(g_CpcAppSpotConfigurationPathName);
    spotRealIp        = pConfigSpot->getString("IP", "127.0.0.1");
    if ( spotRealIp.find('"') != std::string::npos )
    {
      spotRealIp.erase( 0, 1 ); // erase the first character
      spotRealIp.erase( spotRealIp.size() - 1 ); // erase the last character
    }
    pConfigSpot->release();
  }
  catch(...)
  {
    LOG_VError(main, " PH cannot open SPOT network configuration file [%s].", g_CpcAppSpotConfigurationPathName);
    LOG_Info(main, " PH is starting with default SPOT network configuration values.");
  }

  LOG_VInfo(main, "SpotSession - [%s:%d], %s, clientId [%x], platform [m%d] - connecting...\n", spotIp.c_str(), spotPort, spotRealIp.c_str(), spotClientId, spotPlatform);
  LOG_VInfo(main, "UccSession  - [%s:%d] - connecting...\n", uccIp.c_str(), uccPort);
  LOG_VInfo(main, "CrlManager  - [%s:%d] - waiting incoming connection...\n", crlIp.c_str(), crlPort);

  gvr::cloud::CloudStatus cloudStatus(pConfigApp);

  SpotChannel spotChannel(spotIp, spotPort);
  UccChannel uccChannel(uccIp, uccPort);

  SpotSession                          spotSession(ePlatform);
  UccSessionClient                     uccSession;
  gvr::cloud::CloudConfigDeviceChecker cloudConfigDeviceChecker(gvr::cloud::CloudConfigDeviceChecker::CDefaultCycleTimeout,
                                                                uccSession, spotSession, cloudStatus);
  gvr::cloud::CrlManager               crlManager(crlIp, crlPort, uccSession, spotSession, cloudStatus, cloudConfigDeviceChecker);

  spotSession.start(spotChannel, spotClientId, 0, 0);
  uccSession.setTimeoutDefault(uccDefaultTimeout);
  uccSession.start(uccChannel);

  if( !spotSession.hasError() )
  {
    if( !uccSession.hasError() )
    {
      while( !uccSession.isLogged() || !spotSession.isLogged() ) // Wait until the login was done.
      {
        if(!bRunning)
        {
          break;
        }

        Poco::Thread::sleep(50);
      }

      if(bRunning)
      {
        gvr::cloud::OptCloudClientWorkerReset           workerReset(uccSession, spotSession);
        gvr::cloud::SpotReleaseDownloadWorker           workerFileDownload(uccSession, spotSession, spotReleaseFolder, spotRealIp);
        gvr::cloud::FileUploadWorker                    workerFileUpload(uccSession, spotSession, cloudUploadFolder, spotRealIp, sUloadableListFile);
        gvr::cloud::OptCloudClientWorkerFileActivation  workerFileActivation(uccSession, spotSession);
        gvr::cloud::CloudStatusWorker                   workerCloudStatus(uccSession, cloudStatus, cloudConfigDeviceChecker);
        gvr::cloud::SystemStatusNotificationWorker      workerSystemStatusNotification(spotSession);
        gvr::cloud::CloudReconnectWorker                workerRconnect(uccSession, spotSession, cloudStatus);
        gvr::cloud::SecurityChangeWorker                workerSecurityChange(uccSession, spotSession, cloudStatus);

        if (cloudStatus.isRegistered())
        {
          cloudConfigDeviceChecker.start();
        }

        while( bRunning ) // Wait until the user press Ctrl + c combinations of keys.
        {
          Poco::Thread::sleep(500);
        }
      }
    }
    else
    {
      LOG_VInfo(main, "gvrcloud - error starting ucc session. Error: [%s]\n", uccSession.getErrorDescription().c_str() );
    }
  }
  else
  {
    LOG_VInfo(main, "gvrcloud - error starting spot session. Error: [%s]\n", spotSession.getErrorDescription().c_str() );
  }

  LOG_Info(main, "Terminating application...");
  uccSession.stop();
  spotSession.stop();
  LOG_Info(main, " Payload Handler finished.");

  return 0;
}

/**
 * interrupt
 * @brief Helper function to handle an interrupt signal.
 * */
void interrupt(int sig)
{
  if( sig == SIGINT ) bRunning=false;
}

bool parseCommandLineArguments (int argc, char **argv)
{
  int option(0);

  while( (option = getopt(argc, argv, "c:h?")) != -1 )
  {
    switch (option)
    {
      case 'c':
        sConfigDir = optarg;
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

void showUsage ()
{
  using namespace std;
  cout << "Usage: gvrcloud [OPTION...]" << endl << endl;
  cout << "       -c CONFIGDIR        Specify root configuration directory (default is . dir)" << endl;
  cout << "       -?, -h              Give this help list" << endl << endl;
}

/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file ConfigLabels.h
 * @author Guillermo Paris
 * @date Apr 11 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

#ifndef CONFIGLABELS_H_
#define CONFIGLABELS_H_

// Log configuration file constants
#ifdef WINDOWS_PLATFORM

#define PATH_SEPARATOR                          '\\'
#define RESOURCES_DEFAULT_PATHNAME              ".\\resources\\"
#define RESOURCES_PINPAD_COLOR_DEFAULT_PATHNAME "Pinpad\\DisplayScreen\\Color\\"

#define MAX_SIZE 2*1000*1000               //  little less than 2 MB.


#define IS_EXTERNAL_APP                         true
#define LOG_DEFAULT_PATHNAME                    ".\\"
#define LOG_DEFAULT_FILENAME                    "SPOTCheck.log"
#define INI_SPOTCHECK_CONFIG_DEFAULT_FILE       "SPOTCheck.ini"

#else // linux native or embedded

#define PATH_SEPARATOR                          '/'
#define RESOURCES_DEFAULT_PATHNAME              "./resources/"
#define RESOURCES_PINPAD_COLOR_DEFAULT_PATHNAME "Pinpad/DisplayScreen/Color/"

#define MAX_SIZE 2*1000*1000               //  little less than 2 MB.

#ifdef  SPOT_M3 // embedded linux for M3 SPOT
#define IS_EXTERNAL_APP                         false
#define LOG_DEFAULT_PATHNAME                    "/jffs2/logs/"
#define LOG_DEFAULT_FILENAME                    "DIAGNOSTIC.YYYYMMDD.000"
#define INI_SPOTCHECK_CONFIG_DEFAULT_FILE       "/etc/config/SPOTCheck.ini"
//#define RESOURCES_DEFAULT_PATHNAME              "/jffs2/storage/spotcheckfs/var/opt/spotcheck/resources/"    //for injected mode
#define RESOURCES_DEFAULT_PATHNAME              "/jffs2/storage/spotcheckfs/resources/"    //for injected mode
#define RESOURCES_PINPAD_COLOR_DEFAULT_PATHNAME "Pinpad/DisplayScreen/Color/"      //for injected mode

#endif
#ifdef  SPOT_M5 // embedded linux for M5 SPOT
#define IS_EXTERNAL_APP                         false
#define LOG_DEFAULT_PATHNAME                    "/gilbarco/logs/"
#define LOG_DEFAULT_FILENAME                    "DIAGNOSTIC.YYYYMMDD.000"
#define INI_SPOTCHECK_CONFIG_DEFAULT_FILE       "/etc/config/SPOTCheck.ini"
#define RESOURCES_DEFAULT_PATHNAME              "/gilbarco/spotcheckfs/resources/"    //for injected mode
#define RESOURCES_PINPAD_COLOR_DEFAULT_PATHNAME "Pinpad/DisplayScreen/Color/"      //for injected mode
#endif
#ifdef  LINUX_PLATFORM // native linux (build and run server)
#define IS_EXTERNAL_APP                         true
#define LOG_DEFAULT_PATHNAME                    "./"
#define LOG_DEFAULT_FILENAME                    "SPOTCheck.log"
#define INI_SPOTCHECK_CONFIG_DEFAULT_FILE       "SPOTCheck.ini"
#endif

/**
 * For simulation of the embedded mode in a Linux box, use the following definitions:
 *
#ifdef  LINUX_PLATFORM // native linux (build and run server)
#define IS_EXTERNAL_APP                         false
#define LOG_DEFAULT_PATHNAME                    "./"
#define LOG_DEFAULT_FILENAME                    "DIAGNOSTIC.YYYYMMDD.log"
#define INI_SPOTCHECK_CONFIG_DEFAULT_FILE       "/etc/config/SPOTCheck.ini"
#endif
 *
 */
#endif

#define RUNNING_MODE     "Mode"
#define ROMFS_NUMBER     "RomfsNumber"

#define pcRomfsDirM3          "/jffs2/packages/"
#define pcRomfsDirM5          "/gilbarco/packages/"
#define pcConfigPathM3        "/etc/config/"
#define pcTmpPathM3           "/tmp/"
#define pcSysmanKeyholder     "sysman-ROMfs%d.ini"
#define pcSpotCheckfsPathM3  "/jffs2/storage/spotcheckfs"
#define pcSpotCheckfsPathM5  "/gilbarco/spotcheckfs"


enum eRunningMode
{
	eExternal, eInjected, eEmbedded, eDummyEmbedded, eDummyInjected   //eDummyEmbedded, eDummyInjected will be used only for developers to run from native linux
};

#define DIAGNOSTIC_WORD         "DIAGNOSTIC"
#define DIAGNOSTIC_WORD_LENGTH  10
#define INI_CONFIG_DEFAULT_FILE "SPOTApplication.ini"
#define GENERAL_SECTION_NAME    "General.Log"
#define MODULE_SECTION_NAME     "Module.Log.Levels"
#define RESOURCES_SECTION_NAME  "Resources.SCR"
#define CONSOLELEVEL_KEY_NAME   "ConsoleLevel"
#define FILELEVEL_KEY_NAME      "FileLevel"
#define PATH_KEY_NAME           "LogPath"
#define FILE_KEY_NAME           "LogFile"
#define SPOT_IP_KEY_NAME        "SpotIP"
#define ASK_SPOT_IP_KEY_NAME    "AskIP"
#define DEFAULT_SPOT_IP         "10.5.48.71"
#define LOOPBACK_SPOT_IP        "127.0.0.1"
#define M3KEYMATRIX_SCR_KEY_NAME   "PinpadM3Keymatrix15"
#define SOFTKEYMATRIX_SCR_KEY_NAME "PinpadSoftkeys4x2"
#define AUXKEYMATRIX_SCR_KEY_NAME  "PinpadAuxkeys442"
#define SECUREAUDIO_SCR_KEY_NAME   "SecureAudio"


// Log level labels
#define LVL_DEBUG               "debug"
#define LVL_TRACE               "trace"
#define LVL_NOTICE              "notice"
#define LVL_INFO                "Info"
#define LVL_WARNING             "Warning"
#define LVL_ERROR               "ERROR"
#define LVL_FATAL               "FATAL"
#define LVL_SECURITY            "SECURITY"

// Log configuration file, keys and shortnames
#define KEY_PRINTER             "Printer"
#define LOG_DEV_PRINTER         "PRT"
#define KEY_TRIND               "Trind"
#define LOG_DEV_TRIND           "TRD"
#define KEY_BARCODE             "Barcode"
#define LOG_DEV_BARCODE         "BAR"
#define KEY_CARDREADER          "CardReader"
#define LOG_DEV_CARDREADER      "CR "
#define KEY_CONTACTLESSREADER   "ContactlessReader"
#define LOG_DEV_CLESSREADER     "CLR"
#define KEY_KEYPAD              "Keypad"
#define LOG_DEV_KEYPAD          "KP "
#define KEY_DISPLAY             "Display"
#define LOG_DEV_DISPLAY         "DY "
#define KEY_NOTEACCEPTOR        "NoteAcceptor"
#define LOG_DEV_NOTEACCEPTOR    "BNA"
#define KEY_POSINTERFACE        "POSInterface"
#define LOG_DEV_POSINTERFACE    "POI"
#define KEY_SMARTMERCH          "SmartMerch"
#define LOG_DEV_SMARTMERCH      "SM "
#define KEY_TIMER               "Timer"
#define LOG_DEV_TIMER           "TIM"
#define KEY_BIOS                "BIOS"
#define LOG_DEV_BIOS            "BIO"
#define KEY_SPOT                "SPOT"
#define LOG_DEV_SPOT            "SPT"
#define KEY_SPOTPROTOCOL        "SPOTProtocol"
#define LOG_DEV_SPOTPROTOCOL    "SPR"
#define KEY_PUMP                "Pump"
#define LOG_DEV_PUMP            "PMP"
#define KEY_CRINDTOCRIND        "CRINDtoCRIND"
#define LOG_DEV_CRINDTOCRIND    "C2C"
#define KEY_COMMON              "Common"
#define LOG_DEV_COMMON          "CMN"
#define KEY_MENUMANAGER         "MenuManager"
#define LOG_DEV_MENUMANAGER     "MMG"
#define KEY_WIN_TEMPLATE        "WinTemplate"
#define LOG_DEV_WIN_TEMPLATE    "WIN"
#define KEY_REPORT              "Report"
#define LOG_DEV_REPORT          "RPT"
#define KEY_CONFIG              "Config"
#define LOG_DEV_CONFIG          "CFG"
#define KEY_PASSTHRU            "PassThru"
#define LOG_DEV_PASSTHRU        "PTH"
#define KEY_UNCATEGORIZED       "UnCategorized"
#define LOG_DEV_UNCATEGORIZED   "   "
#define KEY_RESOURCES           "Resources"
#define LOG_DEV_RESOURCES       "RES"


#endif /* CONFIGLABELS_H_ */



/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file HardcodedLiterals.h
 * @brief Hardcoded literals for initializing static constant class data members.
 *
 * @note DO NOT USE THIS literals for initializing anything different than static constants.
 *
 * @author Guillermo Paris
 * @date Apr 11 2013
 * @copyright © 2013 2014 Gilbarco Inc. Confidential and Propietary
 *
 */

#ifndef HARDCODEDLITERALS_H_
#define HARDCODEDLITERALS_H_

// Numeric literals for static constant class data member initialization.
#define MAX_STATIC_LOGBUFFER_SIZE    (8*1024) // 8Kb for vnsprintf()
#define HEADER_BUFFER_SIZE                64
#define TIMESTAMP_BUFFER_SIZE             30
#define DIAGNOSTIC_WORD_LENGTH             5 // strlen("DIAGS");
#define DATEFORMAT_YYYYMMDD_LENGTH         8 // strlen("YYYYMMDD");
#define LOGFILENAME_DATE_MAXSIZE          32 // > strlen("YYYY-MM-DD HH:mm:SS.123456") == 26;
#define MAX_LOGFILE_SIZE        (2*1024*1024)// 2 MB.


// Log configuration string constants.
#define LOG_DEFAULT_PATHNAME              "./"
#define LOG_DEFAULT_FILENAME              "libgvrcommon.log"
#define INI_LOGGER_DEFAULT_CONFIG_FILE    "libgvrlog.ini"
#define PATH_SEPARATOR                    '/'
#define GENERAL_SECTION_NAME              "General.Log"
#define MODULE_SECTION_NAME               "Module.Log.Levels"
#define CONSOLELEVEL_KEY_NAME             "ConsoleLevel"
#define FILELEVEL_KEY_NAME                "FileLevel"
#define PATH_KEY_NAME                     "LogPath"
#define FILE_KEY_NAME                     "LogFile"


// Log level labels
#define LVL_DEBUG                         "debug"
#define LVL_TRACE                         "trace"
#define LVL_NOTICE                        "notice"
#define LVL_INFO                          "Info"
#define LVL_WARNING                       "Warning"
#define LVL_ERROR                         "ERROR"
#define LVL_FATAL                         "FATAL"


// Log configuration file, keys and shortnames
#define KEY_PRINTER                       "Printer"
#define LOG_DEV_PRINTER                   "PRT"
#define KEY_CARDREADER                    "CardReader"
#define LOG_DEV_CARDREADER                "CR "
#define KEY_KEYPAD                        "Keypad"
#define LOG_DEV_KEYPAD                    "KP "
#define KEY_DISPLAY                       "Display"
#define LOG_DEV_DISPLAY                   "DY "
#define KEY_SPOT                          "SPOT"
#define LOG_DEV_SPOT                      "SPT"
#define KEY_SPOTPROTOCOL                  "SPOTProtocol"
#define LOG_DEV_SPOTPROTOCOL              "SPR"
#define KEY_COMMON                        "Common"
#define LOG_DEV_COMMON                    "CMN"
#define KEY_CONFIG                        "Config"
#define LOG_DEV_CONFIG                    "CFG"
#define KEY_UNCATEGORIZED                 "UnCategorized"
#define LOG_DEV_UNCATEGORIZED             "   "


#endif /* HARDCODEDLITERALS_H_ */



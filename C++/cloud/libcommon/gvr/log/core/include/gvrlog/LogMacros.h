/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @brief Some macros to hide specific implementation of the logger.
 * @file LogMacros.h
 * @author gparis
 * @date Monday, December 22, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef LOGMACROS_H_
#define LOGMACROS_H_

#include <Poco/Logger.h>
#include <gvrlog/VariadicLogSupport.h>

//
// convenience macros for libgvrlog
//
//

// Example: If module is Session
// Then the macro below expands to: extern Logger* g_LoggerInstanceModuleSession
#define DECLARE_LOG_MODULE(module)   extern Poco::Logger* g_pLoggerInstanceModule##module

// Example: If module is Session
// Then the macro below expands to: Logger* g_LoggerInstanceModuleSession = NULL
#define DEFINE_LOG_MODULE(module)    Poco::Logger* g_pLoggerInstanceModule##module = NULL

#ifdef _NO_LOG_ // not logging at all, by symbol definition at client module.

#define LOG_Fatal(module, str)
#define LOG_VFatal(module, fmt,...)
#define LOG_Critical(module, str)
#define LOG_VCritical(module, fmt,...)
#define LOG_Error(module, str)
#define LOG_VError(module, fmt,...)
#define LOG_Warn(module, str)
#define LOG_VWarn(module, fmt,...)
#define LOG_Notice(module, str)
#define LOG_VNotice(module, fmt,...)
#define LOG_Info(module, str)
#define LOG_VInfo(module, fmt,...)
#define LOG_Debug(module, str)
#define LOG_VDebug(module, fmt,...)
#define LOG_Trace(module, str)
#define LOG_VTrace(module, fmt,...)

#else // YES_LOG !! :)

#define ANOTHER_DAY()\
  if( gvr::log::ifAnotherDay() )\
  {\
    gvr::log::cleanUp();\
    gvr::log::init();\
  }

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eFatal = Poco::Message::PRIO_FATAL priority. Module priority must be lower or equal than eFatal
 *         (priority number >= eFatal) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Fatal(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->fatal()) \
    gvr::log::VariadicLogSupport::LogFatal(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eFatal = Poco::Message::PRIO_FATAL priority. Module priority must be lower or equal than eFatal
 *         (priority number >= eFatal) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VFatal(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->fatal()) \
    gvr::log::VariadicLogSupport::LogFatal(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__); \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eCritical = Poco::Message::PRIO_CRITICAL priority. Module priority must be lower or equal than eCritical
 *         (priority number >= eCritical) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Critical(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->critical()) \
    gvr::log::VariadicLogSupport::LogCritical(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eCritical = Poco::Message::PRIO_CRITICAL priority. Module priority must be lower or equal than eCritical
 *         (priority number >= eCritical) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VCritical(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->critical()) \
    gvr::log::VariadicLogSupport::LogCritical(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0


/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eError = Poco::Message::PRIO_ERROR priority. Module priority must be lower or equal than eError
 *         (priority number >= eError) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Error(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->error()) \
    gvr::log::VariadicLogSupport::LogError(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eError = Poco::Message::PRIO_ERROR priority. Module priority must be lower or equal than eError
 *         (priority number >= eError) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VError(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->error()) \
    gvr::log::VariadicLogSupport::LogError(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eWarn = Poco::Message::PRIO_WARNING priority. Module priority must be lower or equal than eWarn
 *         (priority number >= eWarn) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Warn(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->warning()) \
    gvr::log::VariadicLogSupport::LogWarn(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eWarn = Poco::Message::PRIO_WARNING priority. Module priority must be lower or equal than eWarn
 *         (priority number >= eWarn) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VWarn(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->warning()) \
    gvr::log::VariadicLogSupport::LogWarn(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eNotice = Poco::Message::PRIO_NOTICE priority. Module priority must be lower or equal than eNotice
 *         (priority number >= eNotice) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Notice(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->notice()) \
    gvr::log::VariadicLogSupport::LogNotice(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eNotice = Poco::Message::PRIO_NOTICE priority. Module priority must be lower or equal than eNotice
 *         (priority number >= eNotice) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VNotice(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->notice()) \
    gvr::log::VariadicLogSupport::LogNotice(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__); \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eInfo = Poco::Message::PRIO_INFORMATION priority. Module priority must be lower or equal than eInfo
 *         (priority number >= eInfo) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Info(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->information()) \
    gvr::log::VariadicLogSupport::LogInfo(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eInfo = Poco::Message::PRIO_INFORMATION priority. Module priority must be lower or equal than eInfo
 *         (priority number >= eInfo) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VInfo(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->information()) \
    gvr::log::VariadicLogSupport::LogInfo(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eDebug = Poco::Message::PRIO_DEBUG priority. Module priority must be lower or equal than eDebug
 *         (priority number >= eDebug) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Debug(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->debug()) \
    gvr::log::VariadicLogSupport::LogDebug(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eDebug = Poco::Message::PRIO_DEBUG priority. Module priority must be lower or equal than eDebug
 *         (priority number >= eDebug) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VDebug(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->debug()) \
    gvr::log::VariadicLogSupport::LogDebug(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0

/**
 * @brief Tries to log the given string (into the channels defined in configuration file) in
 *         eTrace = Poco::Message::PRIO_TRACE priority. Module priority must be lower or equal than eTrace
 *         (priority number >= eTrace) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b str  The string (or char*) to be logged.
 *
 */
#define LOG_Trace(module, str) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->trace()) \
    gvr::log::VariadicLogSupport::LogTrace(g_pLoggerInstanceModule##module, __FILE__, __LINE__, 0, (str)); \
  else (void) 0

/**
 * @brief Tries to log the given message, as a printf() like output (variadic), into the channels defined in configuration file,
 *         in eTrace = Poco::Message::PRIO_TRACE priority. Module priority must be lower or equal than eTrace
 *         (priority number >= eTrace) to allow the log output.
 *
 * @param @b module is the NAME of the module WITHOUT " " quotation marks.
 *           That is modName instead of "modName".
 *
 * @param @b fmt  The format string (or char*) containing the message to be logged, and the format
 *                specifiers (% character) according to the type of the following parameters below.
 *
 * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
 *               format type specifiers given in the format string above.
 */
#define LOG_VTrace(module, fmt,...) \
  ANOTHER_DAY();\
  g_pLoggerInstanceModule##module = & Poco::Logger::get(#module); \
  if(g_pLoggerInstanceModule##module->trace()) \
    gvr::log::VariadicLogSupport::LogTrace(g_pLoggerInstanceModule##module, __FILE__, __LINE__, (fmt),##__VA_ARGS__);  \
  else (void) 0

#endif // _NO_LOG_

#endif /* LOGMACROS_H_ */



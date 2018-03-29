/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file VariadicLogSupport.h
 * @author gparis
 * @date Friday, December 19, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef VARIADICLOGSUPPORT_H_
#define VARIADICLOGSUPPORT_H_

#include <cstdarg>
#include <gvrlog/LogLevels.h>

/// @brief Gilbarco Domain
namespace gvr
{
/// @brief log domain
namespace log
{

class VariadicLogSupport
{
  public:

    static char* Format(unsigned int& nchars, char* pcBuffer, const char* pcFormat,...);

    static char* Format(unsigned int& nchars, char* pcBuffer, const char* pcFormat, va_list vlist);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eFatal = Poco::Message::PRIO_FATAL priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     *
     */
    static bool  LogFatal(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eFatal = Poco::Message::PRIO_FATAL priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogFatal(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eCritical = Poco::Message::PRIO_CRITICAL priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogCritical(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

   /**
    * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
    *         in eCritical = Poco::Message::PRIO_CRITICAL priority.
    *
    * @param @b pLogger is the pointer to the module's logger.
    *
    * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
    *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
    *
    * @param @b line is the literal line number in the source code filename that is calling the log action.
    *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
    *
    * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
    *           specifiers (% character) according to the types of the following parameters below.
    *
    * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
    *               format type specifiers given in the format string above.
    */
    static bool  LogCritical(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eError = Poco::Message::PRIO_ERROR priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogError(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eError = Poco::Message::PRIO_ERROR priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogError(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eWarn = Poco::Message::PRIO_WARNING priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogWarn(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eWarn = Poco::Message::PRIO_WARNING priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogWarn(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eNotice = Poco::Message::PRIO_NOTICE priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     *
     */
    static bool  LogNotice(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eNotice = Poco::Message::PRIO_NOTICE priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogNotice(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eInfo = Poco::Message::PRIO_INFORMATION priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogInfo(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eInfo = Poco::Message::PRIO_INFORMATION priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogInfo(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eDebug = Poco::Message::PRIO_DEBUG priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogDebug(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eDebug = Poco::Message::PRIO_DEBUG priority.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogDebug(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);

    /**
     * @brief Logs the given message, into the channels defined in configuration file,
     *         in eTrace = Poco::Message::PRIO_TRACE priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b mark Discarded! Used just as a mark parameter to break overloading ambiguity.
     *
     * @param @b pcMessage is the message string (or char*) to be logged.
     */
    static bool  LogTrace(Poco::Logger* pLogger, const char* pcCppFile, int line, int mark, const char* pcMessage);

    /**
     * @brief Logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         in eTrace = Poco::Message::PRIO_TRACE priority.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  LogTrace(Poco::Logger* pLogger, const char* pcCppFile, int line, const char* pcFormat,...);


private:

    /**
     * @brief This function is called internally by all the non variadic methods above.
     *         It is meant to provide a common treatment routine to the exception raising problematic.
     *         This logs the given message, into the channels defined in configuration file,
     *         using the parameter ELogLevel e priority.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b e Is the desired priority level as in enumeration gvr::log::ELogLevel in LogLevels.h
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcMessage is the string (or char*) containing the message to be logged.
     */
    static bool  Log(Poco::Logger* pLogger, ELogLevel e, const char* pcCppFile, int line, const char* pcMessage);

    /**
     * @brief  This function is called internally by all the variadic methods above.
     *         It is meant to provide a common treatment routine to the variadic problematic.
     *         This logs the given message, as a printf() like output (variadic), into the channels defined in configuration file,
     *         using the parameter ELogLevel e priority.
     *
     * @param @b pcCppFile is the literal name of the source code filename that is calling the log action.
     *           Usually is taken from the macro __FILE__ and then supplied at compilation time.
     *
     * @param @b e Is the desired priority level as in enumeration gvr::log::ELogLevel in LogLevels.h
     *
     * @param @b line is the literal line number in the source code filename that is calling the log action.
     *           Usually is taken from the macro __LINE__ and then supplied at compilation time.
     *
     * @param @b pLogger is the pointer to the module's logger.
     *
     * @param @b pcFormat is the format string (or char*) containing the message to be logged, and the format
     *           specifiers (% character) according to the types of the following parameters below.
     *
     * @param @b ... Ellipsis, the variable number of parameters whose types MUST BE in concordance with the
     *               format type specifiers given in the format string above.
     */
    static bool  Log(Poco::Logger* pLogger, ELogLevel e, const char* pcCppFile, int line, const char* pcFormat, va_list vlist);
};

} // end namespace log
} // end namespace gvr

#endif /* VARIADICLOGSUPPORT_H_ */

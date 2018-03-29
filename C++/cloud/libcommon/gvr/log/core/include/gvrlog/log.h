/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log.h
 * @author gparis
 * @date Tuesday, September 30, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>

// Macros using the new logging framework.
#include <gvrlog/LogMacros.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief log domain
    namespace log
    {
     static const bool CAPPEND_DATE_AS_YYYYMMDD_TO_FILENAMES_OF_LOGS = true;
     static const bool CNORMAL_USE_OF_LOG_LIBRARY = false;

     /**
      * @brief Performs log library initialization.
      * @note Calling this initialization function will result in a default
      *       configuration file called "libgvrlog.ini" .
      *
      * @return @b true  if Poco log framework is properly initialized.
      *         @b false if some error arose during initialization..
      *
      * @since 1.0.0
      */
      bool init(void);

     /**
      * @brief Performs log library initialization.
      *
      * @param @b pcFullPathNameToIniFile The full pathname to the configuration file.
      *        If NULL, the configuration file to be read is "libgvrlog.ini" .
      * @param @b bAppendDateToFilenamesOfLogs to specify if is needed to append the date to Logs files.
      *
      * @return @b true  if Poco log framework is properly initialized.
      *         @b false if some error arose during initialization..
      *
      * @since 1.0.0
      */
      bool init(const char* pcFullPathNameToIniFile, bool bAppendDateToFilenamesOfLogs);

     /**
      * @brief Performs log library clean up previous to exit the client application.
      * 
      * @since 1.0.0
      */
      void cleanUp(void);

     /**
      * @brief Set the log level of the specified logger (module) for the logfile stream.
      *
      * @param @b pcModule Name of the existing module logger to be configured.
      * @param @b iLevel Integer representing the level, in the range between eHighestLogLevel and eLowestLogLevel.
      *
      * @return @b true if iLevel and iModule are in the correct range and therefore the level could be set.
      *         @b false if iLevel, iModule or both doesn't fall into the specified ranges.
      *
      * @since 1.0.0
      */
      bool setModuleLevel(const char* pcModule, int iLevel);

     /**
      * @brief Set the log level of the specified logger (module) for the logfile stream.
      * 
      * @param @b pLogger pointer to the module logger to be configured.
      * @param @b iLevel integer representing the level, in the range between eHighestLogLevel and eLowestLogLevel.
      *
      * @return @b true if iLevel and iModule are in the correct range and therefore the level could be set.
      *         @b false if iLevel, iModule or both doesn't fall into the specified ranges.
      * 
      * @since 1.0.0
      */
      bool setLogLevel(Poco::Logger* pLogger, int iLevel);

      /**
       * @brief Check if is another day, this verification is to change the day of the logs.
       *
       * @param none.
       * @return @b true if is another day.
       *         @b false otherwise.
       *
       * @since 1.0.0
       */
      bool ifAnotherDay();

    } // end namespace log
  } // end namespace gvr

#endif // _LOG_H_


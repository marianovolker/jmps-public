/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @brief Indirect inclusion file to hide specific implementation of the logger.
 * @file LogLevels.h
 * @author gparis
 * @date Monday, December 22, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef LOGLEVELS_H_
#define LOGLEVELS_H_

#include <Poco/Message.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief log domain
  namespace log
  {
      typedef enum
      {
        eHighestLogLevel = Poco::Message::PRIO_FATAL, ///< 1
        eFatal    = Poco::Message::PRIO_FATAL,        ///< 1
        eCritical = Poco::Message::PRIO_CRITICAL,     ///< 2
        eError    = Poco::Message::PRIO_ERROR,        ///< 3
        eWarn     = Poco::Message::PRIO_WARNING,      ///< 4
        eNotice   = Poco::Message::PRIO_NOTICE,       ///< 5
        eInfo     = Poco::Message::PRIO_INFORMATION,  ///< 6
        eDebug    = Poco::Message::PRIO_DEBUG,        ///< 7
        eTrace    = Poco::Message::PRIO_TRACE,        ///< 8
        eLowestLogLevel = eTrace,                     ///< 8
        eAllLevels = eLowestLogLevel                  ///< 8

      } ELogLevel;

  } // end namespace log
} // end namespace gvr



#endif /* LOGLEVELS_H_ */

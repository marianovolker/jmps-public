/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log.h
 * @author gparis
 * @date Friday, October 03, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MODULESANDLEVELS_H_
#define _MODULESANDLEVELS_H_


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief log domain
  namespace log
  {
      typedef enum
      {
        eLowestLogLevel,
        eDebug = eLowestLogLevel,      ///< 0
        eTrace,                        ///< 1
        eNotice,                       ///< 2
        eInfo,                         ///< 3
        eWarning,                      ///< 4
        eError,                        ///< 5
        eFatal,                        ///< 6
        eHighestLogLevel = eFatal,
        eAllLevels                     ///< 7

      } eLogLevel;

      typedef enum
      {
        eFirstModule,
        ePrinter = eFirstModule,       ///< 0
        eCardReader,                   ///< 1
        eKeypad,                       ///< 2
        eDisplay,                      ///< 3
        eSpot,                         ///< 4
        eSpotProtocol,                 ///< 5
        eCommon,                       ///< 6
        eConfig,                       ///< 7
        eUnCategorized,                ///< 8
        eLastModule = eUnCategorized,
        eAllSubSystems                 ///< 9

      } eSubSystem;

  } // end namespace log
} // end namespace gvr

#endif // _MODULESANDLEVELS_H_



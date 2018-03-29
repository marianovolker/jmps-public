/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log_unittest.h
 * @author gparis
 * @date Tuesday, September 30, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _DUMMY_UNITTEST_H_
#define _DUMMY_UNITTEST_H_

#include <gvrlog/log.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief log domain
    namespace log
    {

      /**
       * @class GSPCLogUT
       * @brief Class to test the outside interface to the logger.
       *        Static function tested: init(), cleanup(), log(), setConsoleLevel(),
       *                                setLogfileLevel(), and setModuleLevel().
       * @author Guillermo Paris
       * @date   2014-09-30
       * @since  0.0.0
       */
      class GLogTestUT : public testing::Test
      {
        public:

          void SetUp();
          void TearDown();
          GLogTestUT();
          ~GLogTestUT();
      };

    } // end namespace log
  } // end namespace gvr

#endif

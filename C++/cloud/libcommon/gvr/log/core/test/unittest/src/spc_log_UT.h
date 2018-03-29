/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file spc_log_UT.h
 * @author gparis
 * @date Tuesday, October 07, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPC_LOG__UNITTEST_H_
#define _SPC_LOG_UNITTEST_H_

#include "../../../src/internals/spc_log/src/GLoggerConfig.h"

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief log domain
    namespace log
    {

      /**
       * @class GSPCLogUT
       * @brief Class to test objects of GLogger and GLoggerConfig classes.
       * @author Guillermo Paris
       * @date   2014-10-07
       * @since  0.0.0
       */
      class GSPCLogUT : public testing::Test
      {
        public:
          GSPCLogUT();     ///< Test Constructor. Test initialization.
          ~GSPCLogUT();    ///< Test Destructor. Test cleaning up.

          void SetUp();    ///< Initialization and configuration for every test.
          void TearDown(); ///< Cleaning up for every test..

          GLoggerConfig*   m_pLogConfig;  ///< GLoggerConfig object reference to also be tested.
      };

    } // end namespace log
  } // end namespace gvr


#endif // _SPC_LOG_UNITTEST_H_


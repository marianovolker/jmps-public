/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlWorker.cpp
 * @author mvolker
 * @date Monday, April 20, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/AbsCrlWorker.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    AbsCrlWorker::AbsCrlWorker( const std::string& threadName, CrlSessionServer & crlSession ):
      AbsOptCloudClientWorker(threadName),
      m_pCrlSession(&crlSession)
    {
    }

    AbsCrlWorker::~AbsCrlWorker()
    {
    }
  }
}

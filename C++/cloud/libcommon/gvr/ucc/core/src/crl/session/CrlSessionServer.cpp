/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlSessionServer.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/session/CrlSessionServer.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      CrlSessionServer::CrlSessionServer (void)
       : CrlSession()
      {
        addClassifier(m_serverClassifier);
      }

      CrlSessionServer::~CrlSessionServer (void)
      {
      }
    }
  }
}

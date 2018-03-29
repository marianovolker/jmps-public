/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccSessionServer.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/session/UccSessionServer.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      UccSessionServer::UccSessionServer (void)
       : UccSession()
      {
        addClassifier(m_registerClassifier);
        addClassifier(m_cloudServerClassifier);
        addClassifier(m_internalServerClassifier); /**@brief Classifier for ucc internal server messages*/
      }

      UccSessionServer::~UccSessionServer (void)
      {
      }
    }
  }
}

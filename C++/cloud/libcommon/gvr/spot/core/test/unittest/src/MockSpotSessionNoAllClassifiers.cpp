/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MockSpotSession.cpp
 * @author mgrieco
 * @date Friday, January 29, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvrspot_unittest/MockSpotSessionNoAllClassifiers.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      MockSpotSessionNoAllClassifiers::MockSpotSessionNoAllClassifiers()
      {
        removeClassifier(m_pingClassifier);
      }

      bool MockSpotSessionNoAllClassifiers::remainingBuffer()
      {
        return SpotSession::remainingBuffer();
      }
    }
  }
}

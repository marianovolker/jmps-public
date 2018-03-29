/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotSession.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvrspot_unittest/MockSpotSession.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      MockSpotSession::MockSpotSession ()
        : SpotSession()
      {
        addClassifier(m_pingClassifier);
        addClassifier(m_loginClassifier);
      }

      bool MockSpotSession::ping()
      {
        SpotMessagePing spotPingMessageRq;
        SpotMessagePingResponse spotPingMessageRs;
        StatusType result = sendSync(spotPingMessageRq, spotPingMessageRs,3000);
        StatusType ack = spotPingMessageRs.getAckCode();
        return isConnected();
      }

      bool MockSpotSession::login()
      {
        SpotMessageLogin spotLoginMessageRq;
        spotLoginMessageRq.setClientId(getClientId());
        spotLoginMessageRq.setModuloLen(getModuloLen());
        spotLoginMessageRq.setPingTimeout(getPingTimeout());
        SpotMessageLoginResponse spotLoginMessageRs;
        StatusType result = sendSync(spotLoginMessageRq, spotLoginMessageRs, 10000);
        StatusType ack = spotLoginMessageRs.getAckCode();
        return isConnected();
      }
    }
  }
}

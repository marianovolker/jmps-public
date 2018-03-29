/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MockSpotSessionCancelTimers.h
 * @author mvolker
 * @date Friday, Feb 06, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MOCKSPOTSESSION_CANCEL_TIMERS_H_
# define _MOCKSPOTSESSION_CANCEL_TIMERS_H_

#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/spot/SpotClassifyMessagePing.h>
#include <gvr/communication/spot/SpotClassifyMessageLogin.h>
#include <gvr/communication/spot/SpotClassifyMessagePingResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageLoginResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class MockSpotSessionCancelTimers : public SpotSession
        {
          public:

          /**
          * @brief Default Constructor.
          * @since 0.0.0
          */
          MockSpotSessionCancelTimers (void);

          /**
          * @brief sends the login and mocks the results expected
          * @return true if the send was successful.
          **/
          bool login();

          /**
          * @brief sends the ping and mocks the results expected
          * @return true if the send was successful.
          **/
          bool ping();

          /**
            * @brief Perform a verification if the SpotSession is ready to operate.
            * @return true if the session is ready.
            */
          bool isLogged(){ return isConnected(); };

          /**
          * @brief Perform a restart of the timer. You could not call this method from the callback timer,
          *        you will generate a deadlock.
          * @param The timer to restart.
          **/
          void restartSendLoginTimer(){ m_timerToSendLogin.stop(); };

          /**
          * @brief Perform a restart of the timer. You could not call this method from the callback timer,
          *        you will generate a deadlock.
          * @param The timer to restart.
          **/
          void restartRetryConnectionTimer(){ m_timerToRetryConnect.stop(); };

          /**
          * @brief Perform a restart of the timer. You could not call this method from the callback timer,
          *        you will generate a deadlock.
          * @param The timer to restart.
          **/
          void restartSendPingTimer(){ m_timerToSendPing.stop(); };
        };
      }
    }
  }
#endif
  

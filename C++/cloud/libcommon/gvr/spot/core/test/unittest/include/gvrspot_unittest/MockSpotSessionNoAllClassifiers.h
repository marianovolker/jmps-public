/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MockSpotSessionNoAllClassifiers.h
 * @author mgrieco
 * @date Friday, January 29, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MOCKSPOTSESSIONNOALLCLASSIFIERS_H_
# define _MOCKSPOTSESSIONNOALLCLASSIFIERS_H_

#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/Session.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class MockSpotSessionNoAllClassifiers : public SpotSession
        {
          public:
          /**
           * @brief Default Constructor.
           * @since 0.0.0
           */
          MockSpotSessionNoAllClassifiers();

          /**
          * @brief redefine the inherited type. remainingBuffer.
          * @see Session.h
          **/
          bool remainingBuffer();

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
  

/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MockSpotSession.h
 * @author mgrieco
 * @date Friday, January 29, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MOCKSPOTSESSION_H_
# define _MOCKSPOTSESSION_H_

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
        class MockSpotSession : public SpotSession
        {
          public:
            /**
            * @brief Default Constructor.
            * @since 0.0.0
            */
            MockSpotSession (void);

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
        };
      }
    }
  }
#endif
  

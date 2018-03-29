/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlSessionServer.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLSESSIONSERVER_H_
# define _CRLSESSIONSERVER_H_

# include <gvr/communication/crl/session/CrlSession.h>
# include <gvr/communication/crl/classifier/CrlClassifyMessageServer.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This class is the crl session client.
         * @see gvr.communication.Session
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class CrlSessionServer : public CrlSession
        {
          public:
            /**
            * @brief Default Constructor.
            * @since 0.0.0
            */
            CrlSessionServer (void);

            /**
             * @brief Destructor.
             * @since 0.0.0
             */
            virtual ~CrlSessionServer (void);

          protected:
            CrlClassifyMessageServer       m_serverClassifier;
        };
      }
    }
  }

#endif

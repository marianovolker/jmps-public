/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccSessionServer.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCSESSIONSERVER_H_
# define _UCCSESSIONSERVER_H_

# include <gvr/communication/ucc/session/UccSession.h>
# include <gvr/communication/ucc/classifier/UccClassifyMessageRegister.h>
# include <gvr/communication/ucc/classifier/UccClassifyMessageCloudServer.h>
# include <gvr/communication/ucc/classifier/UccClassifyMessageInternalServer.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /**
         * @brief This class is the ucc session client.
         * @see gvr.communication.Session
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccSessionServer : public UccSession
        {
          public:
            /**
            * @brief Default Constructor.
            * @since 0.0.0
            */
            UccSessionServer (void);

            /**
             * @brief Destructor.
             * @since 0.0.0
             */
            virtual ~UccSessionServer (void);

          protected:
            UccClassifyMessageRegister       m_registerClassifier;
            UccClassifyMessageCloudServer    m_cloudServerClassifier;
            UccClassifyMessageInternalServer m_internalServerClassifier;
        };
      }
    }
  }

#endif

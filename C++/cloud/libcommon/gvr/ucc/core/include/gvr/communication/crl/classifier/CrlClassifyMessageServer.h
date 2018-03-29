/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlClassifyMessageServer.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLCLASSIFYMESSAGESERVER_H_
# define _CRLCLASSIFYMESSAGESERVER_H_

# include <gvr/communication/crl/classifier/base/ICrlClassifyMessage.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /*
         * @brief This class is the implementation for server crl message classifier.
         * @see gvr.communication.crl.ICrlClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class CrlClassifyMessageServer : public ICrlClassifyMessage
        {
        public:

          /**
           * @brief Implements the inherited method. classify.
           * @see ICrlClassifyMessage::getKey()
           */
          ICrlClassifyMessage::IntegrityType classify (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &);

          /**
           * @brief default destructor.
           */
          virtual ~CrlClassifyMessageServer (void);
        };
      }
    }
  }

#endif

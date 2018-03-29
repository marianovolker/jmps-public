/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlClassifyMessageClient.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLCLASSIFYMESSAGECLIENT_H_
# define _CRLCLASSIFYMESSAGECLIENT_H_

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
         * @brief This class is the implementation for client crl message classifier.
         * @see gvr.communication.crl.ICrlClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class CrlClassifyMessageClient : public ICrlClassifyMessage
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
          virtual ~CrlClassifyMessageClient (void);
        };
      }
    }
  }

#endif

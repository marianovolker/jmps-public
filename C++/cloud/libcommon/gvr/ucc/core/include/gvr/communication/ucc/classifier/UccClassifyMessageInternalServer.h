/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageInternalServer.h
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCCLASSIFYMESSAGEINTERNALSERVER_H_
# define _UCCCLASSIFYMESSAGEINTERNALSERVER_H_

# include <gvr/communication/ucc/classifier/base/IUccClassifyMessage.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /*
         * @brief This class is the implementation for client ucc internal message classifier.
         * @see gvr.communication.ucc.IUccClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class UccClassifyMessageInternalServer : public IUccClassifyMessage
        {
        public:

          /**
           * @brief Implements the inherited method. classify.
           * @see IUccClassifyMessage::getKey()
           */
          IUccClassifyMessage::IntegrityType classify (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &);

          /**
           * @brief default destructor.
           */
          virtual ~UccClassifyMessageInternalServer (void);
        };
      }
    }
  }

#endif

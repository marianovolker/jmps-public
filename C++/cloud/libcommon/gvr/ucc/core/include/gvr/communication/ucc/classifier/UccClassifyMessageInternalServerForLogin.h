/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageInternalServerForLogin.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCCLASSIFYMESSAGEINTERNALSERVERFORLOGIN_H_
# define _UCCCLASSIFYMESSAGEINTERNALSERVERFORLOGIN_H_

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
         * @brief This class is the implementation for client ucc internal message classifier for login.
         * @see gvr.communication.ucc.IUccClassifyMessage
         * @since 0.0.0
         * @author pcantarini
         */
        class UccClassifyMessageInternalServerForLogin : public IUccClassifyMessage
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
          virtual ~UccClassifyMessageInternalServerForLogin (void);
        };
      }
    }
  }

#endif

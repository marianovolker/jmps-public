/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISpotClassifyMessage.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISPOTCLASSIFYMESSAGE_H_
# define _ISPOTCLASSIFYMESSAGE_H_

# include <gvr/communication/IClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class ISpotClassifyMessage : public IClassifyMessage<SpotMessageSpec>
        {

        public:

         /**
          * @brief The size of the total length file in SPOT protocol messages.
          * @see IClassifyMessage
          */
          static const unsigned int CMessageLengthFieldSize = 2;

          /**
           * @brief redefine the inherited type. Spec.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::Spec          Spec;

          /**
           * @brief redefine the inherited type. KeyType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::KeyType       KeyType;

          /**
           * @brief redefine the inherited type. BufferType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::BufferType    BufferType;

          /**
           * @brief redefine the inherited type. SizeType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::SizeType      SizeType;

          /**
           * @brief redefine the inherited type. TypeId.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::TypeId        TypeId;

          /**
           * @brief redefine the inherited type. IntegrityType.
           * @see IClassifyMessage
           */
          typedef IClassifyMessage<SpotMessageSpec>::IntegrityType IntegrityType;

          /**
           * @brief default destructor.
           */
          virtual ~ISpotClassifyMessage (void) = 0;
        };
      }
    }
  }

#endif

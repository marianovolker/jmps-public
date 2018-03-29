/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageEphemeralEncryptResponse.h
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEEPHEMERALENCRYPTRESPONSE_H_
# define _SPOTCLASSIFYMESSAGEEPHEMERALENCRYPTRESPONSE_H_

# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageEphemeralEncryptResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageEphemeralEncryptResponse : public ISpotClassifyMessage
        {
        public:
          /**
           * @brief Implements the inherited method. classify.
           * @see ISpotClassifyMessage::getKey()
           */
          ISpotClassifyMessage::IntegrityType classify (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &);

          /**
           * @brief default destructor.
           */
          ~SpotClassifyMessageEphemeralEncryptResponse (void);
        };
      }
    }
  }

#endif

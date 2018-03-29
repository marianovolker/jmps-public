/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageDebianInfoResponse.h
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEDEBIANINFORESPONSE_H_
# define _SPOTCLASSIFYMESSAGEDEBIANINFORESPONSE_H_

# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageDebianInfoResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageDebianInfoResponse : public AbsSpotClassifyMessageHeader
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
         ~SpotClassifyMessageDebianInfoResponse (void);
        };
      }
    }
  }
#endif

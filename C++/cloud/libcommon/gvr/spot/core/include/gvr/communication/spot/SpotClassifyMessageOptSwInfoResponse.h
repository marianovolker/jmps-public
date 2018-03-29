/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageOptSwInfoResponse.h
 * @author mgrieco
 * @date Monday, May 11, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEOPTSWINFORESPONSE_H_
# define _SPOTCLASSIFYMESSAGEOPTSWINFORESPONSE_H_

# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageOptSwInfoResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageOptSwInfoResponse : public ISpotClassifyMessage
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
         ~SpotClassifyMessageOptSwInfoResponse (void);
        };
      }
    }
  }
#endif

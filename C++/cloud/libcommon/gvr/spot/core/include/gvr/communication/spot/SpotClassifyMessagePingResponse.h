/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePingResponse.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEPINGRESPONSE_H_
# define _SPOTCLASSIFYMESSAGEPINGRESPONSE_H_

# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessagePingResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessagePingResponse : public ISpotClassifyMessage
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
          ~SpotClassifyMessagePingResponse (void);
        };
      }
    }
  }

#endif

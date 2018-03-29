/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageSystemStatusResponse.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGESYSTEMSTATUSRESPONSE_H_
# define _SPOTCLASSIFYMESSAGESYSTEMSTATUSRESPONSE_H_

# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageSystemStatusResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageSystemStatusResponse : public AbsSpotClassifyMessageHeader
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
          ~SpotClassifyMessageSystemStatusResponse (void);
        };
      }
    }
  }

#endif

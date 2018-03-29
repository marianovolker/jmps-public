/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileUpload.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEFILEUPLOAD_H_
# define _SPOTCLASSIFYMESSAGEFILEUPLOAD_H_

# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageFileUpload.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageFileUpload : public AbsSpotClassifyMessageHeader
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
          ~SpotClassifyMessageFileUpload (void);
        };
      }
    }
  }

#endif

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageActivation.h
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEPACKAGEACTIVATION_H_
# define _SPOTCLASSIFYMESSAGEPACKAGEACTIVATION_H_

# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessagePackageActivation.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessagePackageActivation : public AbsSpotClassifyMessageHeader
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
          ~SpotClassifyMessagePackageActivation (void);
        };
      }
    }
  }

#endif

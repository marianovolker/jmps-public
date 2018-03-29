/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileBlockUploadResponse.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCLASSIFYMESSAGEFILEBLOCKUploadRESPONSE_H_
# define _SPOTCLASSIFYMESSAGEFILEBLOCKUPLOADRESPONSE_H_

# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/ISpotClassifyMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageFileBlockUploadResponse.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotClassifyMessageFileBlockUploadResponse : public AbsSpotClassifyMessageHeader
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
         ~SpotClassifyMessageFileBlockUploadResponse (void);
        };
      }
    }
  }
#endif

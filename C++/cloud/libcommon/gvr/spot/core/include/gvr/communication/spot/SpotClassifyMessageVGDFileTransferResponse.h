/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageVGDFileDownloadTransferResponse.h
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEVGDFILETRANSFERRESPONSE_H_
#define SPOTCLASSIFYMESSAGEVGDFILETRANSFERRESPONSE_H_


#include <gvr/communication/spot/ISpotClassifyMessage.h>
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


class SpotClassifyMessageVGDFileTransferResponse : public ISpotClassifyMessage
{
  public:

   /**
    * @brief Implements the inherited method. classify.
    * @see ISpotClassifyMessage::getKey()
    */
    ISpotClassifyMessage::IntegrityType classify (ISpotClassifyMessage::BufferType const &,
                                                  ISpotClassifyMessage::OffsetType &,
                                                  ISpotClassifyMessage::SizeType &,
                                                  ISpotClassifyMessage::TypeId &,
                                                  ISpotClassifyMessage::KeyType &);

   /**
    * @brief default destructor.
    */
    ~SpotClassifyMessageVGDFileTransferResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr

#endif // SPOTCLASSIFYMESSAGEVGDFILETRANSFERRESPONSE_H_


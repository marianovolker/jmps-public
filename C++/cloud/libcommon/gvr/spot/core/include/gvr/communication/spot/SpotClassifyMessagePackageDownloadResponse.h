/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageDownloadResponse.h
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEPACKAGEDOWNLOADRESPONSE_H_
#define SPOTCLASSIFYMESSAGEPACKAGEDOWNLOADRESPONSE_H_

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessagePackageDownloadResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessagePackageDownloadResponse : public AbsSpotClassifyMessageHeader
{
  public:

   /**
    * @brief Implements the inherited method classify.
    * @see ISpotClassifyMessage::getKey()
    */
    ISpotClassifyMessage::IntegrityType classify (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &);

   /**
    * @brief default destructor.
    */
    ~SpotClassifyMessagePackageDownloadResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTCLASSIFYMESSAGEPACKAGEDOWNLOADRESPONSE_H_


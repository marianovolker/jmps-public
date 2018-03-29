/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageDownload.h
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEPACKAGEDOWNLOAD_H_
#define SPOTCLASSIFYMESSAGEPACKAGEDOWNLOAD_H_


# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessagePackageDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessagePackageDownload : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessagePackageDownload (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr

#endif // SPOTCLASSIFYMESSAGEPACKAGEDOWNLOAD_H_


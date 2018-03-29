/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageBlockDownload.h
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEPACKAGEBLOCKDOWNLOAD_H_
#define SPOTCLASSIFYMESSAGEPACKAGEBLOCKDOWNLOAD_H_


# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessagePackageBlockDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessagePackageBlockDownload : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessagePackageBlockDownload (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr

#endif // SPOTCLASSIFYMESSAGEPACKAGEBLOCKDOWNLOAD_H_


/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageInfoResponse.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEPACKAGEINFORESPONSE_H_
#define SPOTCLASSIFYMESSAGEPACKAGEINFORESPONSE_H_

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessagePackageInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessagePackageInfoResponse : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessagePackageInfoResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTCLASSIFYMESSAGEPACKAGEINFORESPONSE_H_


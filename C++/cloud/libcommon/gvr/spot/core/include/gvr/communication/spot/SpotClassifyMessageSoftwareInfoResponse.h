/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageSoftwareInfoResponse.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGESOFTWAREINFORESPONSE_H_
#define SPOTCLASSIFYMESSAGESOFTWAREINFORESPONSE_H_

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessageSoftwareInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessageSoftwareInfoResponse : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessageSoftwareInfoResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTCLASSIFYMESSAGESOFTWAREINFORESPONSE_H_


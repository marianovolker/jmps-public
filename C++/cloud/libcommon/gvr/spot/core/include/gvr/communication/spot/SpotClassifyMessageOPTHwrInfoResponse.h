/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageOPTHwrInfoResponse.h
 * @author Guillermo Paris
 * @date Thursday, May 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEOPTHWRINFORESPONSE_H_
#define SPOTCLASSIFYMESSAGEOPTHWRINFORESPONSE_H_

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessageOPTHwrInfoResponse : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessageOPTHwrInfoResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTCLASSIFYMESSAGEOPTHWRINFORESPONSE_H_


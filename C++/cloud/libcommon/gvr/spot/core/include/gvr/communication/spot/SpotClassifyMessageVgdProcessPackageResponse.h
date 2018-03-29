/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageVgdProcessPackageResponse.h
 * @author Guillermo Paris
 * @date Tuesday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGEVGDPROCESSPACKAGERESPONSE_H_
#define SPOTCLASSIFYMESSAGEVGDPROCESSPACKAGERESPONSE_H_

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackageResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessageVgdProcessPackageResponse : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessageVgdProcessPackageResponse (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr


#endif // SPOTCLASSIFYMESSAGEVGDPROCESSPACKAGERESPONSE_H_


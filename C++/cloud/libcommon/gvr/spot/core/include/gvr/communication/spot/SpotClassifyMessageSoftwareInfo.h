/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageSoftwareInfo.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTCLASSIFYMESSAGESOFTWAREINFO_H_
#define SPOTCLASSIFYMESSAGESOFTWAREINFO_H_


# include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>
# include <gvr/communication/spot/SpotMessageSpec.h>
# include <gvr/communication/spot/SpotMessageSoftwareInfo.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


class SpotClassifyMessageSoftwareInfo : public AbsSpotClassifyMessageHeader
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
    ~SpotClassifyMessageSoftwareInfo (void);
};

} // namespace spot

} // namespace communication

} // namespace gvr

#endif // SPOTCLASSIFYMESSAGESOFTWAREINFO_H_


/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVgdProcessPackageResponse.h
 * @author Guillermo Paris
 * @date Tuesday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEVGDPROCESSPACKAGERESPONSE_H_
#define SPOTMESSAGEVGDPROCESSPACKAGERESPONSE_H_

#include <gvr/communication/spot/TagLengthValueVector.h>
#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>

 /// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


/**
 * @brief This class is the implementation of a handler of vanguard spot process package response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-10
 * @version 0.0.0, 2015-02-10
 * @author Guillermo Paris
 */
class SpotMessageVgdProcessPackageResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /**@brief default message size*/
    static const int CMessageSize = 7; // constant message size for this message.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CVgdProcessPackageCategoryId = 0x60;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CVgdProcessPackageCommandId = 0x05;

    /** @brief Item Quantity byte. Offset inside message. */
    static const ISpotMessage::SizeType CMessageAckCodeCodeOffset = 0x06;
    /**@brief Result - Package activation in VGD SPOT was successful.*/
    static const unsigned char CVGDPkgActivationOK = 0x00;

    /**
     * @brief Following methods Retrieves the current instance Message class type identification.
     *
     * @return The current Message class type identification.
     *
     * @exception Throws no exception.
     *
     * @see gvr.communication.IMessage.TypeId
     *
     * @since 0.0.0
     */
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageVgdProcessPackageResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessageVgdProcessPackageResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageVgdProcessPackageResponse (void);

    bool hasBeenProcessedOK() const;

    /**
    * @brief Serializes the hardware info response message (only used for tests).
    * @note This method serializes just a typical hard-coded reply for unit testing.
    *       This is because a SPOT response message is never sent to SPOT !!
    * @see IMessage::serialize()
    */
    ISpotMessage::BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT Hardware Info Response message inside it.
    * @see IMessage::parse()
    */
    bool parse (ISpotMessage::BufferType const &);
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEVGDPROCESSPACKAGERESPONSE_H_ */


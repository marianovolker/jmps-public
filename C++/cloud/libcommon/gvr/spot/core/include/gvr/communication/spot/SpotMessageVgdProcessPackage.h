/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVgdProcessPackage.h
 * @author Guillermo Paris
 * @date Tuesday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEVGDPROCESSPACKAGE_H_
#define SPOTMESSAGEVGDPROCESSPACKAGE_H_


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
 * @brief This class is the implementation of a handler of vanguard spot process package request messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-10
 * @version 0.0.0, 2015-02-10
 * @author Guillermo Paris
 */
class SpotMessageVgdProcessPackage : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /**@brief default message size*/
    static const int CMessageSize = 7; // constant message size for this message.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CVgdProcessPackageCommandId = 0x60;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CVgdProcessPackageSubCommandId = 0x05;

    /** @brief Item Quantity byte. Offset inside message. */
    static const ISpotMessage::SizeType CMessageCommandCodeOffset = 0x06;

    /**@brief Command - only Verify package.*/
    static const unsigned char CVerify = 0x00;
    /**@brief Command - Save package into SPOT.*/
    static const unsigned char CSave = 0x01;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageVgdProcessPackage>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


   /**
    * @brief default Constructor
    */
    SpotMessageVgdProcessPackage(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageVgdProcessPackage (void);

   /**
    * @brief Serializes the hardware info request message to a byte vector.
    * @see IMessage::serialize()
    */
    BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT Hardware Info Request message inside it.
    * @brief This parsing of a request is only used for tests.
    * @see IMessage::parse()
    */
    bool parse (BufferType const &);

   /**
    * @brief Gets the current command operation.
    *
    * @return @b byte . Values: 0x00 for "Verify", or 0x01 for "Save".
    */
    unsigned char getCommandOperation(void) const;

   /**
    * @brief Set package verification against currently active one, as the current operation.
    */
    void setVerifyOperation(void);

   /**
    * @brief Set package storage for later activation, as the current operation.
    * @note  This is set by default after message construction.
    */
    void setSaveOperation(void);


  private:

   /**
    * @brief Command, verify or save.
    * @note  Set to CSave (download) by construction.
    */
    unsigned char    m_bCommand;

};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEVGDPROCESSPACKAGE_H_ */


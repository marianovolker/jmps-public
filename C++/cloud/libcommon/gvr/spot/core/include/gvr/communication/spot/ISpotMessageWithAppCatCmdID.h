/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISpotMessageWithAppCatCmdID.h
 * @author Guillermo Paris
 * @date Monday, February 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISPOTMESSAGEWITHAPPCATCMDID_H_
#define _ISPOTMESSAGEWITHAPPCATCMDID_H_

#include <gvr/communication/spot/ISpotMessage.h>

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
 * @brief This class is the base interface for all spot messages having six byte header,
 *        that is, the header bytes are these: sizeH, sizeL, appID, ssk, catID, cmdID.
 * @see gvr.communication.MessageSpec
 * @since 0.0.0, 2015-02-09
 * @version 0.0.0, 2015-02-09
 * @author Guillermo Paris
 */
class ISpotMessageWithAppCatCmdID : public ISpotMessage
{
  public:

    /**@brief default message header size*/
    static const int CHeaderSize = 6; // sizeH, sizeL, appID, ssk, catID, cmdID

    static const unsigned int CRequestMessageAppmask = 0x7F; /// @brief Mask for setting bit 7 to 0, forcing request mode.

    static const unsigned int CResponseMessageAppmask = 0x80; /// @brief Mask for setting bit 7 to 1 forcing response mode.

    /** @brief Higher byte of the 16 bit total length. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageLengthOffsetH = 0x00;
    /** @brief Lower byte of the 16 bit total length. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageLengthOffsetL = 0x01;
    /** @brief Application ID code. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageAppIdOffset = 0x02;
    /** @brief SSK byte. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageSSKOffset = 0x03;
    /** @brief Category (command) code. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageCatIdOffset = 0x04;
    /** @brief Command (sub-command) code. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageCmdIdOffset = 0x05;
    /** @brief ACK byte code. Offset inside all kind of messages. */
    static const ISpotMessage::SizeType CMessageACKOffset = 0x06;

    /** ACK code OK - no errors in response. */
    static const unsigned char CMessageResponseAckOK = 0x00;

    /** 1 item - only one information item requested or in the response. */
    static const unsigned char CMessageOnlyOneItem = 0x01;


   /**
    * @brief Retrieves the spot application sub-command ID ("command code") for the message.
    * @return The message spot sub-command ID.
    * @exception Throws no exception.
    * @since 0.0.0
    */
    virtual unsigned char getSubCommand(void) const = 0;

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~ISpotMessageWithAppCatCmdID(void) = 0;
};

} // namespace gvr

} // namespace communication

} // namespace spot


#endif // _ISPOTMESSAGEWITHAPPCATCMDID_H_

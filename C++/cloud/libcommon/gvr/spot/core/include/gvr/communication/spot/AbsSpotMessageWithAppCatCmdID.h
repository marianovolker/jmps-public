/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotMessageWithAppCatCmdID.h
 * @author Guillermo Paris
 * @date Monday, February 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSSPOTMESSAGEWITHAPPCATCMDID_H_
#define _ABSSPOTMESSAGEWITHAPPCATCMDID_H_

#include <gvr/communication/spot/ISpotMessageWithAppCatCmdID.h>

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
 * @brief This class is the base abstract class with basic common functionality for all spot messages
 *        having six bytes header: sizeH, sizeL, appID, ssk, catID, cmdID.
 * @note catID is commonly called "command ID". Similarly, cmdID is commonly called "sub-command ID".
 * @see gvr.communication.spot.ISpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-09
 * @version 0.0.0, 2015-02-09
 * @author Guillermo Paris
 */
class AbsSpotMessageWithAppCatCmdID : public ISpotMessageWithAppCatCmdID
{
  public:

   /**
    * @brief Constructor forcing request/response definition.
    * @param blReply boolean @b true if this message will be used as a reply, @b false if will be a request.
    * @param bAppID @b byte , SPOT Application ID to talk with.
    * @param bCatID @b byte , command Category ID, or "command code".
    * @param bCmdID @b byte , Command ID, or "sub-command code".
    *
    * @since 0.0.0
    */
    AbsSpotMessageWithAppCatCmdID (bool blReply, unsigned char bAppID,
                                   unsigned char bCatID, unsigned char bCmdID);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~AbsSpotMessageWithAppCatCmdID (void) = 0;


   /**
    * @brief Says if this object is a request or a response message.
    * @return boolean @b true if this message is used as a reply, @b false if it is a request.
    * @since 0.0.0
    */
    bool isReply();

   /**
    * @brief Retrieves the SPOT Application ID to talk with.
    * @return The SPOT Application ID .
    * @exception Throws no exception.
    * @since 0.0.0
    */
    virtual unsigned char getSpotAppId( void ) const;

   /**
    * @brief Retrieves the ssk message field.
    * @return The ssk message field.
    * @exception Throws no exception.
    * @since 0.0.0
    */
    virtual unsigned char getSsk (void) const;

   /**
    * @brief Retrieves the command code.
    * @return The command Category ID or "command code".
    * @exception Throws no exception.
    * @since 0.0.0
    */
    virtual unsigned char getCommand( void ) const;

   /**
    * @brief Retrieves the sub-command code.
    * @return The Command ID or "sub-command code".
    * @exception Throws no exception.
    * @since 0.0.0
    */
    virtual unsigned char getSubCommand( void ) const;

   /**
    * @brief  Gets the acknowledge response code.
    * @return The acknowledge response byte code.
    * @exception Throws no exception.
    * @since 0.0.0
    */
    unsigned char getAckCode (void) const;

   /**
    * @brief Sets the acknowledge response code.
    * @param bAckCode. The acknowledge response byte code.
    * @exception Throws no exception.
    * @since 0.0.0
    */
    void setAckCode (unsigned char const bAckCode);

    /**
     * @brief Implements the inherited method. getKey.
     *
     * @return The unique SPOT command key that uniquely identifies the SPOT command/reply
     *         associated with this message.
     *
     * @see IMessage::getKey()
     */
     virtual KeyType getKey (void) const;

    /**
     * @brief Always returns the header size, including the ackCode if this is a reply message.
     * @see IMessage::size()
     */
     virtual SizeType size (void) const;

   /**
    * @brief Serializes the header, including the ackCode if this is a reply message.
    * @see IMessage::serialize()
    */
    virtual BufferType serialize (void) const;

   /**
    * @brief Parses  the header, including the ackCode if this is a reply message.
    * @see IMessage::parse()
    */
    virtual bool parse (BufferType const &);


  protected:

    /**
     * @brief Sets the acknowledge response code. The use is typically just before a serialize() call.
     * @note This function is protected because calculating of the total message length is a bit rather complex
     *       operation that involves knowledge of the internal data of the message. Thus, in this way,
     *       total length - reported by size() - is not modifiable by external users, but rather by derived message
     *       classes from this abstract one.
     *
     * @param uTotalSize. The size of a new request command to be sent to SPOT via serialize().
     * @exception Throws no exception.
     *
     * @see AbsSpotMessageWithAppCatCmdID::serialize()
     * @since 0.0.0
     */
    void setSerializedSize(SizeType uTotalSize);

    /**
     * @brief Sets the application ID member variable ( m_bAppId ). This allow to reuse or customize the derived class for
     * cases of high likelihood between messages only varying in application ID and in very few functionality.
     *
     * @note This function is protected because it is only allowed to certain derived class messages.
     *       classes from this abstract one.
     *
     * @param bAppId. The new application ID to reuse the derived class if necessary.
     * @exception Throws no exception.
     *
     * @see AbsSpotMessageWithAppCatCmdID::m_bAppId
     * @since 0.0.0
     */
    void setAppId(unsigned char bAppId);

     /**
      * @brief Sets the sub-command code member variable ( m_bCmdId ). This allow to reuse the derived class for
      * cases of high likelihood between messages only varying in sub-command code and in very few functionality.
      *
      * @note This function is protected because it is only allowed to certain derived class messages.
      *       classes from this abstract one.
      *
      * @param bSubCommand. The new sub-command code to reuse the derived class if necessary.
      * @exception Throws no exception.
      *
      * @see AbsSpotMessageWithAppCatCmdID::m_bCmdId
      * @since 0.0.0
      */
    void setSubCommand(unsigned char bSubCommand);


  private:

    bool           m_blReply;  /// @brief @b boolean , message mode: @b true is Reply, @b false is Request.
    unsigned char  m_bAppId;   /// @brief @b byte , SPOT Application ID to talk with.
    unsigned char  m_bCatId;   /// @brief @b byte , command Category ID, or "command code".
    unsigned char  m_bCmdId;   /// @brief @b byte , Command ID, or "sub-command code".

    /**
     * @brief @b boolean , the total length of this message.
     * @note  It is set (adjusted) by the parse() method every time this is called.
     */
    ISpotMessage::SizeType  m_uTotalLength;
    unsigned char           m_bAckCode; /// @brief @b byte , the acknowledge response code.
};


} // namespace spot

} // namespace communication

} // namespace gvr

#endif // _ABSSPOTMESSAGEWITHAPPCATCMDID_H_

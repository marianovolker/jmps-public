/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotClassifyMessageHeader.h
 * @author Guillermo Paris
 * @date Thursday, February 19, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef ABSSPOTCLASSIFYMESSAGEHEADER_H_
#define ABSSPOTCLASSIFYMESSAGEHEADER_H_


#include <gvr/communication/spot/ISpotClassifyMessage.h>
#include <gvr/communication/spot/SpotMessageSpec.h>

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
 * @brief This class is the base abstract class with basic common functionality for all the spot messages
 *        classifier classes, having six bytes header:  sizeH, sizeL, appID, ssk, catID, cmdID.
 * @note catID is commonly called "command ID". Similarly, cmdID is commonly called "sub-command ID".
 *
 * @see gvr.communication.spot.ISpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-19
 * @version 0.0.0, 2015-02-19
 * @author Guillermo Paris
 */
class AbsSpotClassifyMessageHeader : public ISpotClassifyMessage
{
  public:

   /**
    * @brief default constructor.
    */
    AbsSpotClassifyMessageHeader (void);

   /**
    * @brief default destructor.
    */
    ~AbsSpotClassifyMessageHeader (void);


   /**
    * @brief Set the necessary parameters for header classification.
    *
    * @param bAppID @b byte , SPOT Application ID to talk with.
    * @param bSsk   @b byte , SSK field for compressed messages.
    * @param bCatID @b byte , command Category ID, or "command code".
    * @param bCmdID @b byte , Command ID, or "sub-command code".
    *
    * @since 0.0.0
    */
    void  SetHeaderParameters (unsigned char bAppID, unsigned char bSsk,
                               unsigned char bCatID, unsigned char bCmdID, TypeId classID);

   /**
    * @brief Implements a helper to be used by all the inherited classes in method classify().
    * @see ISpotClassifyMessage::classify()
    */
    ISpotClassifyMessage::IntegrityType classifyHeader (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &);


  private:

    unsigned char  m_bAppId;   /// @brief @b byte , SPOT Application ID to talk with.
    unsigned char  m_bSsk;     /// @brief @b byte , SSK field for compressed messages.
    unsigned char  m_bCatId;   /// @brief @b byte , command Category ID, or "command code".
    unsigned char  m_bCmdId;   /// @brief @b byte , Command ID, or "sub-command code".
    TypeId         m_classID;  /// @brief @b TypeId , class ID of the message handler subclass.
};

} // namespace spot

} // namespace communication

} // namespace gvr



#endif /* ABSSPOTCLASSIFYMESSAGEHEADER_H_ */

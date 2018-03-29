/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSystemStatus.h
 * @author Mariano Volker
 * @date Thursday, March 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGESYSTEMSTATUS_H_
#define _SPOTMESSAGESYSTEMSTATUS_H_

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
       * @brief This class is the implementation of a handler of spot hardware info request messages.
       *
       * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
       * @since 0.0.0, 2015-02-10
       * @version 0.0.0, 2015-02-10
       * @author Mariano Volker
       */
      class SpotMessageSystemStatus : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          // Status info
          // Status info is provided in response to the related pinpad application command (0x10, 0x01) or
          // unsolicited in case of status change. A sequence of four bytes is returned. The first byte identifies
          // the specific PCI PTS subsystem module to which the status info is referred. The possible
          // subsystem codes are listed in the following table:
          enum  ESubsystemTypes
          {
            eSPOT_SYS_MAIN             = 0x01, // The whole system
            eSPOT_SYS_SECURITYMODULE   = 0x02, // Security module subsystem
            eSPOT_SYS_DISPLAY          = 0x04, // Display subsystem
            eSPOT_SYS_KEYBOARD         = 0x08, // Keyboard subsystem
            eSPOT_SYS_CARDREADER       = 0x10, // Card reader subsystem
            eSPOT_SYS_GEMCORE          = 0x20, // SAM interface-module subsystem
            eSPOT_SYS_ALL              = 0xFF, // Request all subsystem informations
            eOPT_PRINTER               = 0x01, // Printer
            eOPT_IO_MODULE             = 0x02, // I/O module
            eOPT_IO_NOTBATT_INPUT1     = 0x04, // Not batterized input # 1
            eOPT_CONTACTLESS           = 0x08, // GCM (OTI contactless reader)
            eOPT_IO_NOTBATT_INPUT2     = 0x10, // Not batterized input # 2
            eOPT_IO_BATT_INPUT4        = 0x20  // Batterized input # 4
          };

          /**@brief default message size*/
          static const int CMinMessageSize = 7; // minimum message size corresponding to only one item.

          /**@brief Spot Message Identification, command category.*/
          static const unsigned char CSystemStatusCommandId = 0x10;

          /**@brief Spot Message Identification, command code.*/
          static const unsigned char CSystemStatusSubCommandId = 0x01;

          /** @brief Item Data. Offset inside message. */
          static const ISpotMessage::SizeType CMessageItemDataOffset = 0x06;
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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageSystemStatus>(); }
          virtual TypeId getTypeId (void) const { return GetTypeId(); }


          /**
           * @brief default Constructor
           */
          SpotMessageSystemStatus(unsigned char bAppId = CMessageResponsePinpadAppId);

         /**
          * @brief Destructor
          * @since 0.0.0
          */
          virtual ~SpotMessageSystemStatus (void);

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
           * @brief Set SubSystem Id to be queried for status.
           * @param subsystem status id.
           *
           * @return none.
           */
           inline void setSubSystemStatusType(ESubsystemTypes subSystemStatus = eSPOT_SYS_ALL) { m_subSystemStatus=subSystemStatus;}


        private:

           /**@brief sub system type to be queried .*/
           unsigned char m_subSystemStatus;
      };
    }
  }
}


#endif

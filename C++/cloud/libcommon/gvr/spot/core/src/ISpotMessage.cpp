/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISpotMessage.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/spot/ISpotMessage.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      const unsigned char ISpotMessage::CMessageRequestSpotAppId            = 0x00;
      const unsigned char ISpotMessage::CMessageRequestPinpadAppId          = 0x01;
      const unsigned char ISpotMessage::CMessageRequestSetupAppId           = 0x03;
      const unsigned char ISpotMessage::CMessageRequestOPTAppId             = 0x04;
      const unsigned char ISpotMessage::CMessageResponseSpotAppId           = 0x80;
      const unsigned char ISpotMessage::CMessageResponsePinpadAppId         = 0x81;
      const unsigned char ISpotMessage::CMessageResponseSetupAppId          = 0x83;
      const unsigned char ISpotMessage::CMessageResponseOPTAppId            = 0x84;
      const unsigned char ISpotMessage::CPinpadExtendedMessageCmdId         = 0x80;
      const unsigned char ISpotMessage::CEphemeralKeySubCmdId               = 0x04;
      const unsigned char ISpotMessage::CNewEphemeralKeySubSubCmdId         = 0x00;
      const unsigned char ISpotMessage::CEphemeralEncryptSubCmdId           = 0x01;
      const unsigned char ISpotMessage::CEphemeralDecryptSubCmdId           = 0x02;
      const unsigned char ISpotMessage::CDefaulNoEncryptionValue            = 0x00;
      const unsigned char ISpotMessage::CErrorCodeNoError                   = 0x00;
      const unsigned char ISpotMessage::CErrorCodeDownloadComplete          = 0x02;
      const unsigned char ISpotMessage::CErrorCodeDebianIntermediateBlock   = 0x03;
      const unsigned char ISpotMessage::CErrorCodeUserAlreadyLogged         = 0xA1;
      const unsigned char ISpotMessage::CErrorCodeDebianActivation          = 0x30;


      ISpotMessage::~ISpotMessage (void)
      {
      }
    }
  }
}

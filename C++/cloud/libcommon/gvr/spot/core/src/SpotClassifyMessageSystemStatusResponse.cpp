/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageSystemStatusResponse.cpp
 * @author mgrieco
 * @date Friday, January 14, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageSystemStatusResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageSystemStatusResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                              ISpotMessage::CDefaulNoEncryptionValue,
                              SpotMessageSystemStatusResponse::CSystemStatusResponseCommandId,
                              SpotMessageSystemStatusResponse::CSystemStatusResponseSubCommandId,
                              SpotMessageSystemStatusResponse::GetTypeId() );

        ISpotClassifyMessage::IntegrityType integrity = classifyHeader(buff, offset, size, typeId, key);

        if( integrity == ISpotClassifyMessage::Integrity::CUnknown ) // header not recognized
        {
            SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponseOPTAppId,
                                  ISpotMessage::CDefaulNoEncryptionValue,
                                  SpotMessageSystemStatusResponse::CSystemStatusResponseCommandId,
                                  SpotMessageSystemStatusResponse::CSystemStatusResponseSubCommandId,
                                  SpotMessageSystemStatusResponse::GetTypeId() );

            return classifyHeader(buff, offset, size, typeId, key); // try with OPT App ID
        }
        else // message already recognized as "complete" or "incomplete"
        {
          return integrity;
        }
      }

      SpotClassifyMessageSystemStatusResponse::~SpotClassifyMessageSystemStatusResponse (void)
      {
      }
    }
  }
}

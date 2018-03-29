/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageSendToCloudResponse.cpp
 * @author mgrieco
 * @date Thursday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageSendToCloudResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageSendToCloudResponse::CMessageOperation = "sendToCloud";

      char const * UccMessageSendToCloudResponse::CSuccessName = "success";
      char const * UccMessageSendToCloudResponse::CCodeName = "code";
      char const * UccMessageSendToCloudResponse::CMessageName = "message";
      
      UccMessageSendToCloudResponse::~UccMessageSendToCloudResponse (void)
      {
      }

      UccMessageSendToCloudResponse::UccMessageSendToCloudResponse(void): AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageSendToCloudResponse::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            result = result && m_json.get(CPayloadName).get(CSuccessName).isBoolean();
            if (result)
            {
              if(!m_json.get(CPayloadName).get(CSuccessName).asBoolean())
              {
                result = result && m_json.get(CPayloadName).get(CCodeName).isNumber();
                result = result && m_json.get(CPayloadName).get(CMessageName).isString();
              }
            }
          }
        }
        return result;
      }

      UccMessageSendToCloudResponse::TypeId UccMessageSendToCloudResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageSendToCloudResponse>();
      };

      UccMessageSendToCloudResponse::TypeId UccMessageSendToCloudResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageConnectResponse.cpp
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageConnectResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageConnectResponse::CMessageOperation = "connect";

      char const * UccMessageConnectResponse::CPayloadName = "payload";
      char const * UccMessageConnectResponse::CSuccessName = "success";
      char const * UccMessageConnectResponse::CCodeName = "code";
      char const * UccMessageConnectResponse::CMessageName = "message";

      UccMessageConnectResponse::~UccMessageConnectResponse (void)
      {
      }

      UccMessageConnectResponse::UccMessageConnectResponse(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageConnectResponse::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          if (result && (!m_json.get(CPayloadName).isNull()))
          {
            result = result && m_json.get(CPayloadName).isObject();
            if (result)
            {
              result = result && m_json.get(CPayloadName).get(CSuccessName).isBoolean();
              if (result)
              {
                if (!m_json.get(CPayloadName).get(CSuccessName).asBoolean())
                {
                  result = result && m_json.get(CPayloadName).get(CCodeName).isNumber();
                  result = result && m_json.get(CPayloadName).get(CMessageName).isString();
                }
              }
            }
          }
        }
        return result;
      }

      UccMessageConnectResponse::TypeId UccMessageConnectResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageConnectResponse>();
      };

      UccMessageConnectResponse::TypeId UccMessageConnectResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

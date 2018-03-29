/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageLoginResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageLoginResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageLoginResponse::CMessageOperation = "login-response";

      char const * UccMessageLoginResponse::CSuccessName = "success";

      UccMessageLoginResponse::~UccMessageLoginResponse (void)
      {
      }

      UccMessageLoginResponse::UccMessageLoginResponse(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageLoginResponse::validate (void)
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
            }
          }
        }
        return result;
      }

      UccMessageLoginResponse::TypeId UccMessageLoginResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageLoginResponse>();
      };

      UccMessageLoginResponse::TypeId UccMessageLoginResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageServerUpdateResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageServerUpdateResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageServerUpdateResponse::CMessageOperation = "server update";

      char const * UccMessageServerUpdateResponse::CSuccessName = "success";
      char const * UccMessageServerUpdateResponse::CCodeName = "code";
      char const * UccMessageServerUpdateResponse::CMessageName = "message";

      UccMessageServerUpdateResponse::~UccMessageServerUpdateResponse (void)
      {
      }

      UccMessageServerUpdateResponse::UccMessageServerUpdateResponse(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageServerUpdateResponse::validate (void)
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

      UccMessageServerUpdateResponse::TypeId UccMessageServerUpdateResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageServerUpdateResponse>();
      };

      UccMessageServerUpdateResponse::TypeId UccMessageServerUpdateResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

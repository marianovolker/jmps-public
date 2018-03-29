/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageFileDownloadResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageFileDownloadResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageFileDownloadResponse::CMessageOperation = "receiveFromCloud";

      char const * UccMessageFileDownloadResponse::CSuccessName = "success";
      char const * UccMessageFileDownloadResponse::CCodeName = "code";
      char const * UccMessageFileDownloadResponse::CMessageName = "message";

      UccMessageFileDownloadResponse::~UccMessageFileDownloadResponse (void)
      {
      }

      UccMessageFileDownloadResponse::UccMessageFileDownloadResponse(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageFileDownloadResponse::validate (void)
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

      UccMessageFileDownloadResponse::TypeId UccMessageFileDownloadResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageFileDownloadResponse>();
      };

      UccMessageFileDownloadResponse::TypeId UccMessageFileDownloadResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

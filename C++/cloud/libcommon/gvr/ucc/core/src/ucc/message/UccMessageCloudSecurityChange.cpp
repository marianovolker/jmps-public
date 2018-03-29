/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudSecurityChange.cpp
 * @author mgrieco
 * @date Wednesday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudSecurityChange.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudSecurityChange::CMessageOperation = "security-change-cmd";

      char const * UccMessageCloudSecurityChange::CUriName = "uri";
      char const * UccMessageCloudSecurityChange::CCertFormatName = "certFormat";
      char const * UccMessageCloudSecurityChange::CCertName = "cert";
      char const * UccMessageCloudSecurityChange::CPwdName = "pwd";

      char const * UccMessageCloudSecurityChange::CCertFormatP12 = "P12";


      UccMessageCloudSecurityChange::~UccMessageCloudSecurityChange (void)
      {
      }

      UccMessageCloudSecurityChange::UccMessageCloudSecurityChange(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudSecurityChange::validate (void)
      {
        bool result = AbsUccMessageCloudCommandRequest::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            result = result && (m_json.get(CPayloadName).get(CUriName).isNull() || m_json.get(CPayloadName).get(CUriName).isString());

            result = result && m_json.get(CPayloadName).get(CCertFormatName).isString();

            result = result && m_json.get(CPayloadName).get(CCertName).isString();
            result = result && (!m_json.get(CPayloadName).get(CCertName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CPwdName).isString();
            result = result && (!m_json.get(CPayloadName).get(CPwdName).asString().empty());
          }
        }
        return result;
      }

      UccMessageCloudSecurityChange::TypeId UccMessageCloudSecurityChange::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudSecurityChange>();
      };

      UccMessageCloudSecurityChange::TypeId UccMessageCloudSecurityChange::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

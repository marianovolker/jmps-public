/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageConnect.cpp
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageConnect.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageConnect::CMessageOperation = "connect";

      char const * UccMessageConnect::CUriName = "uri";
      char const * UccMessageConnect::CNameServer1Name = "nameserver1";
      char const * UccMessageConnect::CNameServer2Name = "nameserver2";
      char const * UccMessageConnect::CCertFormatName = "certFormat";
      char const * UccMessageConnect::CCertTypeName = "cert";
      char const * UccMessageConnect::CUseridName = "userid";
      char const * UccMessageConnect::CPwdName = "pwd";
      char const * UccMessageConnect::CConnectionDelayName = "connectionDelay";

      UccMessageConnect::~UccMessageConnect (void)
      {
      }

      UccMessageConnect::UccMessageConnect(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageConnect::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();

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
            result = result && m_json.get(CPayloadName).get(CUseridName).isString();
            result = result && (!m_json.get(CPayloadName).get(CUseridName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CPwdName).isString();
            result = result && (!m_json.get(CPayloadName).get(CPwdName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CUriName).isString();
            result = result && (!m_json.get(CPayloadName).get(CUriName).asString().empty());

            if (result && (!m_json.get(CPayloadName).get(CNameServer1Name).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CNameServer1Name).isString();
            }
            if (result && (!m_json.get(CPayloadName).get(CNameServer2Name).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CNameServer2Name).isString();
            }

            result = result && m_json.get(CPayloadName).get(CCertFormatName).isString();
            result = result && (!m_json.get(CPayloadName).get(CCertFormatName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CCertTypeName).isString();
            result = result && (!m_json.get(CPayloadName).get(CCertTypeName).asString().empty());

            if (result && (!m_json.get(CPayloadName).get(CConnectionDelayName).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CConnectionDelayName).isNumber();
            }
          }
        }
        return result;
      }

      UccMessageConnect::TypeId UccMessageConnect::GetTypeId (void)
      {
        return _getTypeId<UccMessageConnect>();
      };

      UccMessageConnect::TypeId UccMessageConnect::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

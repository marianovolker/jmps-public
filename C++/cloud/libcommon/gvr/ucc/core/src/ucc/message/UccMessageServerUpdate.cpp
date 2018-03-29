/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageServerUpdate.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageServerUpdate.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageServerUpdate::CMessageOperation = "server update";

      char const * UccMessageServerUpdate::CUrlName = "url";
      char const * UccMessageServerUpdate::CDnsServer1Name = "nameserver1";
      char const * UccMessageServerUpdate::CDnsServer2Name = "nameserver2";
      char const * UccMessageServerUpdate::CCertName = "cert";
      char const * UccMessageServerUpdate::CCertFormatName = "certFormat";
      char const * UccMessageServerUpdate::CCertPwdName = "certPwd";
      char const * UccMessageServerUpdate::CUserName = "user";
      char const * UccMessageServerUpdate::CPwdName = "Pwd";

      UccMessageServerUpdate::~UccMessageServerUpdate (void)
      {
      }

      UccMessageServerUpdate::UccMessageServerUpdate(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageServerUpdate::validate (void)
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
              result = result && m_json.get(CPayloadName).get(CUrlName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CUrlName).asString().empty());

              if (result && (!m_json.get(CPayloadName).get(CDnsServer1Name).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CDnsServer1Name).isString();
              }
              if (result && (!m_json.get(CPayloadName).get(CDnsServer2Name).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CDnsServer2Name).isString();
              }

              result = result && m_json.get(CPayloadName).get(CCertName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CCertName).asString().empty());

              result = result && m_json.get(CPayloadName).get(CCertFormatName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CCertFormatName).asString().empty());

              result = result && m_json.get(CPayloadName).get(CCertPwdName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CCertPwdName).asString().empty());

              result = result && m_json.get(CPayloadName).get(CUserName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CUserName).asString().empty());

              result = result && m_json.get(CPayloadName).get(CPwdName).isString();
              // TODO: Check with Business Analyst!!!
              // result = result && (!m_json.get(CPayloadName).get(CPwdName).asString().empty());
            }
          }
        }
        return result;
      }

      UccMessageServerUpdate::TypeId UccMessageServerUpdate::GetTypeId (void)
      {
        return _getTypeId<UccMessageServerUpdate>();
      };

      UccMessageServerUpdate::TypeId UccMessageServerUpdate::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

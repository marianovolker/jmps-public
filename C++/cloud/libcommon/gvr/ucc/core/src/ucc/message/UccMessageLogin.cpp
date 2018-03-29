/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageLogin.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageLogin.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageLogin::CMessageOperation = "login";

      UccMessageLogin::~UccMessageLogin (void)
      {
      }

      UccMessageLogin::UccMessageLogin(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageLogin::validate (void)
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
            }
          }
        }
        return result;
      }

      UccMessageLogin::TypeId UccMessageLogin::GetTypeId (void)
      {
        return _getTypeId<UccMessageLogin>();
      };

      UccMessageLogin::TypeId UccMessageLogin::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

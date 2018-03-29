/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudReset.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudReset.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudReset::CMessageOperation = "reset-cmd";

      UccMessageCloudReset::~UccMessageCloudReset (void)
      {
      }

      UccMessageCloudReset::UccMessageCloudReset(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudReset::validate (void)
      {
        bool result = AbsUccMessageCloudCommandRequest::validate();
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

      UccMessageCloudReset::TypeId UccMessageCloudReset::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudReset>();
      };

      UccMessageCloudReset::TypeId UccMessageCloudReset::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

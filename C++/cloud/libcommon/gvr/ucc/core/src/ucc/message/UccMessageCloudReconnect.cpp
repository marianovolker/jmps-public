/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudReconnect.cpp
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudReconnect.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudReconnect::CMessageOperation = "reconnect-cmd";

      UccMessageCloudReconnect::~UccMessageCloudReconnect (void)
      {
      }

      UccMessageCloudReconnect::UccMessageCloudReconnect(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudReconnect::validate (void)
      {
        bool result = AbsUccMessageCloudCommandRequest::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());
        }
        return result;
      }

      UccMessageCloudReconnect::TypeId UccMessageCloudReconnect::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudReconnect>();
      };

      UccMessageCloudReconnect::TypeId UccMessageCloudReconnect::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

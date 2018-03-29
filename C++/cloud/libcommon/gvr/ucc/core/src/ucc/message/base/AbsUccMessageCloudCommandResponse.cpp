/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudCommandResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * AbsUccMessageCloudCommandResponse::CSuccessName = "success";

      AbsUccMessageCloudCommandResponse::~AbsUccMessageCloudCommandResponse (void)
      {
      }

      bool AbsUccMessageCloudCommandResponse::validate (void)
      {
        bool result = AbsUccMessageCloud::validate();
        if (result)
        {
          result = result && (m_json.get(CSuccessName).isBoolean());
        }
        return result;
      }

      void AbsUccMessageCloudCommandResponse::resetHeader (bool force = false)
      {
        AbsUccMessageCloud::resetHeader(force);
        if (!m_json.get(CSuccessName).isBoolean() || force)
        {
          m_json.set(CSuccessName) = true;
        }
      }

      bool AbsUccMessageCloudCommandResponse::getSuccess (void) const
      {
        return m_json.get(CSuccessName).asBoolean();
      }

      void AbsUccMessageCloudCommandResponse::setSuccess (bool success)
      {
        m_json.set(CSuccessName) = success;
      }

      AbsUccMessageCloudCommandResponse::TypeId AbsUccMessageCloudCommandResponse::GetTypeId (void)
      {
        return _getTypeId<AbsUccMessageCloud>();
      };

      AbsUccMessageCloudCommandResponse::TypeId AbsUccMessageCloudCommandResponse::getTypeId (void) const
      {
        return GetTypeId();
      }

      AbsUccMessageCloudCommandResponse::AbsUccMessageCloudCommandResponse (void)
       : AbsUccMessageCloud()
      {
        // Ensure this field exists in the message, if not it will be created.
        m_json.set(CSuccessName) = true;
      }
    }
  }
}

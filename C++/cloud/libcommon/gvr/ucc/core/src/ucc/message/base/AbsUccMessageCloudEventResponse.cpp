/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudEventResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * AbsUccMessageCloudEventResponse::CSuccessName = "success";

      AbsUccMessageCloudEventResponse::~AbsUccMessageCloudEventResponse (void)
      {
      }

      bool AbsUccMessageCloudEventResponse::validate (void)
      {
        bool result = AbsUccMessageCloud::validate();
        if (result)
        {
          result = result && (m_json.get(CSuccessName).isBoolean());
        }
        return result;
      }

      void AbsUccMessageCloudEventResponse::resetHeader (bool force = false)
      {
        AbsUccMessageCloud::resetHeader(force);
        if (!m_json.get(CSuccessName).isBoolean() || force)
        {
          m_json.set(CSuccessName) = true;
        }
      }

      bool AbsUccMessageCloudEventResponse::getSuccess (void) const
      {
        return m_json.get(CSuccessName).asBoolean();
      }

      void AbsUccMessageCloudEventResponse::setSuccess (bool success)
      {
        m_json.set(CSuccessName) = success;
      }

      AbsUccMessageCloudEventResponse::TypeId AbsUccMessageCloudEventResponse::GetTypeId (void)
      {
        return _getTypeId<AbsUccMessageCloud>();
      };

      AbsUccMessageCloudEventResponse::TypeId AbsUccMessageCloudEventResponse::getTypeId (void) const
      {
        return GetTypeId();
      }

      AbsUccMessageCloudEventResponse::AbsUccMessageCloudEventResponse (void)
       : AbsUccMessageCloud()
      {
        // Ensure this field exists in the message, if not it will be created.
        m_json.set(CSuccessName) = true;
      }
    }
  }
}

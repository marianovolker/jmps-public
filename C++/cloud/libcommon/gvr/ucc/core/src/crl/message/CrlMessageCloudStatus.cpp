/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageCloudStatus.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageCloudStatus.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageCloudStatus::CMessageOperation = "cloud-status";

      CrlMessageCloudStatus::~CrlMessageCloudStatus (void)
      {
      }

      CrlMessageCloudStatus::CrlMessageCloudStatus(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageCloudStatus::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
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

      CrlMessageCloudStatus::TypeId CrlMessageCloudStatus::GetTypeId (void)
      {
        return _getTypeId<CrlMessageCloudStatus>();
      };

      CrlMessageCloudStatus::TypeId CrlMessageCloudStatus::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudCommandRequest.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandRequest.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * AbsUccMessageCloudCommandRequest::COperationName = "operation";
      char const * AbsUccMessageCloudCommandRequest::CDeviceIdName = "deviceId";
      char const * AbsUccMessageCloudCommandRequest::CPayloadName = "payload";

      AbsUccMessageCloudCommandRequest::~AbsUccMessageCloudCommandRequest (void)
      {
      }

      bool AbsUccMessageCloudCommandRequest::validate (void)
      {
        bool result = AbsUccMessageCloud::validate();
        if (result)
        {
          result = result && (m_json.get(COperationName).isString());
          result = result && (m_kOperation.compare(m_json.get(COperationName).asCString()) == 0);
          result = result && (m_json.get(CDeviceIdName).isString());
        }
        return result;
      }

      std::string AbsUccMessageCloudCommandRequest::getOperationExpected (void) const
      {
        return m_kOperation;
      }

      std::string AbsUccMessageCloudCommandRequest::getOperation (void) const
      {
        return m_json.get(COperationName).asCString();
      }

      std::string AbsUccMessageCloudCommandRequest::getDeviceId (void) const
      {
        return m_json.get(CDeviceIdName).asCString();
      }

      void AbsUccMessageCloudCommandRequest::resetHeader (bool force = false)
      {
        AbsUccMessageCloud::resetHeader(force);
        m_json.set(COperationName) = m_kOperation;
        if (!m_json.get(CDeviceIdName).isString() || force)
        {
          m_json.set(CDeviceIdName) = CEmptyString;
        }
      }

      void AbsUccMessageCloudCommandRequest::setDeviceId (char const * deviceId)
      {
        m_json.set(CDeviceIdName) = deviceId;
      }

      void AbsUccMessageCloudCommandRequest::setDeviceId (std::string const & deviceId)
      {
        setDeviceId(deviceId.c_str());
      }

      AbsUccMessageCloudCommandRequest::JsonType const AbsUccMessageCloudCommandRequest::getPayload (void) const
      {
        return m_json.get(CPayloadName);
      }

      void AbsUccMessageCloudCommandRequest::setPayload (AbsJsonType const & json)
      {
        m_json.set(CPayloadName) = json;
      }

      void AbsUccMessageCloudCommandRequest::removePayload (void)
      {
        m_json.remove(CPayloadName);
      }

      AbsUccMessageCloudCommandRequest::AbsUccMessageCloudCommandRequest (char const * operation)
       : AbsUccMessageCloud(), m_kOperation(operation)
      {
        // Ensure these fields exists in the message, if not they will be created.
        m_json.set(COperationName) = m_kOperation;
        m_json.set(CDeviceIdName) = CEmptyString;
      }
    }
  }
}

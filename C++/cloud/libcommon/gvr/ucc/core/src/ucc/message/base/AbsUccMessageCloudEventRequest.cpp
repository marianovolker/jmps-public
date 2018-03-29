/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudEventRequest.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventRequest.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * AbsUccMessageCloudEventRequest::COperationName = "operation";
      char const * AbsUccMessageCloudEventRequest::CDeviceIdName = "deviceId";
      char const * AbsUccMessageCloudEventRequest::CPayloadName = "payload";

      AbsUccMessageCloudEventRequest::~AbsUccMessageCloudEventRequest (void)
      {
      }

      bool AbsUccMessageCloudEventRequest::validate (void)
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

      std::string AbsUccMessageCloudEventRequest::getOperationExpected (void) const
      {
        return m_kOperation;
      }

      std::string AbsUccMessageCloudEventRequest::getOperation (void) const
      {
        return m_json.get(COperationName).asCString();
      }

      std::string AbsUccMessageCloudEventRequest::getDeviceId (void) const
      {
        return m_json.get(CDeviceIdName).asCString();
      }

      void AbsUccMessageCloudEventRequest::resetHeader (bool force = false)
      {
        AbsUccMessageCloud::resetHeader(force);
        m_json.set(COperationName) = m_kOperation;
        if (!m_json.get(CDeviceIdName).isString() || force)
        {
          m_json.set(CDeviceIdName) = CEmptyString;
        }
      }

      void AbsUccMessageCloudEventRequest::setDeviceId (char const * deviceId)
      {
        m_json.set(CDeviceIdName) = deviceId;
      }

      void AbsUccMessageCloudEventRequest::setDeviceId (std::string const & deviceId)
      {
        setDeviceId(deviceId.c_str());
      }

      AbsUccMessageCloudEventRequest::JsonType const AbsUccMessageCloudEventRequest::getPayload (void) const
      {
        return m_json.get(CPayloadName);
      }

      void AbsUccMessageCloudEventRequest::setPayload (AbsJsonType const & json)
      {
        m_json.set(CPayloadName) = json;
      }

      void AbsUccMessageCloudEventRequest::removePayload (void)
      {
        m_json.remove(CPayloadName);
      }

      AbsUccMessageCloudEventRequest::AbsUccMessageCloudEventRequest (char const * operation)
       : AbsUccMessageCloud(), m_kOperation(operation)
      {
        // Ensure these fields exists in the message, if not they will be created.
        m_json.set(COperationName) = m_kOperation;
        m_json.set(CDeviceIdName) = CEmptyString;
      }
    }
  }
}

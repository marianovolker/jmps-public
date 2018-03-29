/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageInternal.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageInternal.h>

#include <Poco/UUIDGenerator.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * AbsUccMessageInternal::COperationName = "operation";
      char const * AbsUccMessageInternal::CMessageIdName = "messageId";
      char const * AbsUccMessageInternal::CDeviceIdName = "deviceId";
      char const * AbsUccMessageInternal::CPayloadName = "payload";

      AbsUccMessageInternal::~AbsUccMessageInternal (void)
      {
      }

      AbsUccMessageInternal::KeyType AbsUccMessageInternal::getKey (void) const
      {
        return getMessageId();
      }


      bool AbsUccMessageInternal::validate (void)
      {
        bool result = AbsUccMessage::validate();
        if (result)
        {
          result = result && (m_flag == CInteralFlagInternalMessage);
          result = result && (m_json.get(COperationName).isString());
          result = result && (m_kOperation.compare(m_json.get(COperationName).asCString()) == 0);
          result = result && (m_json.get(CMessageIdName).isString());
          result = result && (m_json.get(CDeviceIdName).isString());
        }
        return result;
      }

      std::string AbsUccMessageInternal::getOperationExpected (void) const
      {
        return m_kOperation;
      }

      std::string AbsUccMessageInternal::getMessageId (void) const
      {
        return m_json.get(CMessageIdName).asCString();
      }

      std::string AbsUccMessageInternal::getOperation (void) const
      {
        return m_json.get(COperationName).asCString();
      }

      std::string AbsUccMessageInternal::getDeviceId (void) const
      {
        return m_json.get(CDeviceIdName).asCString();
      }

      void AbsUccMessageInternal::resetHeader (bool force = false)
      {
        m_json.set(COperationName) = m_kOperation;
        if (!m_json.get(CMessageIdName).isString() || force)
        {
          m_json.set(CMessageIdName) = CEmptyString;
        }
        if (!m_json.get(CDeviceIdName).isString() || force)
        {
          m_json.set(CDeviceIdName) = CEmptyString;
        }
      }

      void AbsUccMessageInternal::setMessageId (char const * messageId)
      {
        m_json.set(CMessageIdName) = messageId;
      }

      void AbsUccMessageInternal::setMessageId (std::string const & messageId)
      {
        setMessageId(messageId.c_str());
      }

      void AbsUccMessageInternal::generateMessageId (void)
      {
        setMessageId(Poco::UUIDGenerator::defaultGenerator().createOne().toString());
      }

      void AbsUccMessageInternal::setDeviceId (char const * deviceId)
      {
        m_json.set(CDeviceIdName) = deviceId;
      }

      void AbsUccMessageInternal::setDeviceId (std::string const & deviceId)
      {
        setDeviceId(deviceId.c_str());
      }

      AbsUccMessageInternal::JsonType const AbsUccMessageInternal::getPayload (void) const
      {
        return m_json.get(CPayloadName);
      }

      void AbsUccMessageInternal::setPayload (AbsJsonType const & json)
      {
        m_json.set(CPayloadName) = json;
      }

      void AbsUccMessageInternal::removePayload (void)
      {
        m_json.remove(CPayloadName);
      }

      AbsUccMessageInternal::AbsUccMessageInternal (char const * operation)
       : AbsUccMessage(CInteralFlagInternalMessage), m_kOperation(operation)
      {
        // Ensure these fields exists in the message, if not they will be created.
        m_json.set(COperationName) = m_kOperation;
        m_json.set(CMessageIdName) = CEmptyString;
        m_json.set(CDeviceIdName) = CEmptyString;
      }
    }
  }
}

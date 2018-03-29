/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloud.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageCloud.h>

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
      unsigned char const AbsUccMessageCloud::CInteralFlagCloudMessage = 0x00;

      char const * AbsUccMessageCloud::CMessageIdName = "messageId";

      AbsUccMessageCloud::~AbsUccMessageCloud (void)
      {
      }

      AbsUccMessageCloud::KeyType AbsUccMessageCloud::getKey (void) const
      {
        return getMessageId();
      }

      bool AbsUccMessageCloud::validate (void)
      {
        bool result = AbsUccMessage::validate();
        if (result)
        {
          result = result && (m_flag == CInteralFlagCloudMessage);
          result = result && (m_json.get(CMessageIdName).isString());
        }
        return result;
      }

      std::string AbsUccMessageCloud::getMessageId (void) const
      {
        return m_json.get(CMessageIdName).asCString();
      }

      void AbsUccMessageCloud::resetHeader (bool force = false)
      {
        if (!m_json.get(CMessageIdName).isString() || force)
        {
          m_json.set(CMessageIdName) = CEmptyString;
        }
      }

      void AbsUccMessageCloud::setMessageId (char const * messageId)
      {
        m_json.set(CMessageIdName) = messageId;
      }

      void AbsUccMessageCloud::setMessageId (std::string const & messageId)
      {
        setMessageId(messageId.c_str());
      }

      void AbsUccMessageCloud::generateMessageId (void)
      {
        setMessageId(Poco::UUIDGenerator::defaultGenerator().createOne().toString());
      }

      AbsUccMessageCloud::TypeId AbsUccMessageCloud::GetTypeId (void)
      {
        return _getTypeId<AbsUccMessageCloud>();
      };

      AbsUccMessageCloud::AbsUccMessageCloud (void)
       : AbsUccMessage(CInteralFlagCloudMessage)
      {
        // Ensure this field exists in the message, if not it will be created.
        m_json.set(CMessageIdName) = CEmptyString;
      }
    }
  }
}

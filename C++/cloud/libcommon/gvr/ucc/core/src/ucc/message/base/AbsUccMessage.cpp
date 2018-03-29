/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessage.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessage.h>
#include <gvr/communication/ucc/classifier/base/IUccClassifyMessage.h>
#include <arpa/inet.h>

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
      unsigned int const AbsUccMessage::CSizeFiledSize = IUccClassifyMessage::CSizeFiledSize;
      unsigned int const AbsUccMessage::CFlagFiledSize = IUccClassifyMessage::CFlagFiledSize;
      unsigned int const AbsUccMessage::CBinaryFiledsSize = IUccClassifyMessage::CBinaryFiledsSize;

      char const * AbsUccMessage::CEmptyString = "";

      AbsUccMessage::KeyType const AbsUccMessage::CDefaultKey = AbsUccMessage::CEmptyString;

      AbsUccMessage::~AbsUccMessage (void)
      {
      }

      AbsUccMessage::KeyType AbsUccMessage::getKey (void) const
      {
        return CEmptyString;
      }

      AbsUccMessage::SizeType AbsUccMessage::size (void) const
      {
        std::string jsonMsg;
        m_json.serialize(jsonMsg);
        return (CBinaryFiledsSize + jsonMsg.size());
      }

      AbsUccMessage::BufferType AbsUccMessage::serialize (void) const
      {
        BufferType buffer;
        std::string jsonMsg;
        m_json.serialize(jsonMsg);
        if (jsonMsg.empty() == false)
        {
          std::size_t length = CBinaryFiledsSize + jsonMsg.size();
          std::size_t lengthNetwork = htonl(length);
          unsigned char * lengthNetworkPtrBegin = reinterpret_cast<unsigned char *>(&lengthNetwork);
          unsigned char * lengthNetworkPtrEnd   = lengthNetworkPtrBegin + CSizeFiledSize;
          buffer.reserve(length);
          buffer.assign(lengthNetworkPtrBegin,lengthNetworkPtrEnd);
          buffer.push_back(m_flag);
          buffer.insert(buffer.end(),jsonMsg.begin(),jsonMsg.end());
        }
        return buffer;
      }

      bool AbsUccMessage::parse (BufferType const & buffer)
      {
        SizeType parsedSize = 0;
        SizeType structureSize = 0;
        return (IUccClassifyMessage::parseToStructure(buffer,parsedSize,structureSize,m_flag,m_json) == IUccClassifyMessage::Integrity::CComplete) && (validate());
      }

      bool AbsUccMessage::validate (void)
      {
        return true;
      }

      unsigned char AbsUccMessage::getInternalFlag (void) const
      {
        return m_flag;
      }

      bool AbsUccMessage::isInternalMessage (void) const
      {
        return (getInternalFlag() == CInteralFlagInternalMessage);
      }

      AbsUccMessage::AbsJsonType const & AbsUccMessage::getJson (void) const
      {
        return m_json;
      }

      AbsUccMessage::AbsJsonType & AbsUccMessage::getJson (void)
      {
        return m_json;
      }

      void AbsUccMessage::setJson (AbsJsonType const & json)
      {
        m_json = json;
      }

      AbsUccMessage::AbsUccMessage(unsigned char flag)
       : m_flag(flag), m_json()
      {
      }

      void AbsUccMessage::private_setInternalFlag (unsigned char flg)
      {
        m_flag = flg;
      }
    }
  }
}

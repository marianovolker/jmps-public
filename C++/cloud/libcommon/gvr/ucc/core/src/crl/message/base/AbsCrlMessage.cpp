/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsCrlMessage.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/base/AbsCrlMessage.h>
#include <gvr/communication/crl/classifier/base/ICrlClassifyMessage.h>
#include <arpa/inet.h>

#include <Poco/UUIDGenerator.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      unsigned int const AbsCrlMessage::CSizeFiledSize = ICrlClassifyMessage::CSizeFiledSize;
      unsigned int const AbsCrlMessage::CFlagFiledSize = ICrlClassifyMessage::CFlagFiledSize;
      unsigned int const AbsCrlMessage::CBinaryFiledsSize = ICrlClassifyMessage::CBinaryFiledsSize;

      char const * AbsCrlMessage::CEmptyString = "";

      AbsCrlMessage::KeyType const AbsCrlMessage::CDefaultKey = AbsCrlMessage::CEmptyString;

      AbsCrlMessage::~AbsCrlMessage (void)
      {
      }

      AbsCrlMessage::KeyType AbsCrlMessage::getKey (void) const
      {
        return CEmptyString;
      }

      AbsCrlMessage::SizeType AbsCrlMessage::size (void) const
      {
        std::string jsonMsg;
        m_json.serialize(jsonMsg);
        return (CBinaryFiledsSize + jsonMsg.size());
      }

      AbsCrlMessage::BufferType AbsCrlMessage::serialize (void) const
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

      bool AbsCrlMessage::parse (BufferType const & buffer)
      {
        SizeType parsedSize = 0;
        SizeType structureSize = 0;
        return (ICrlClassifyMessage::parseToStructure(buffer,parsedSize,structureSize,m_flag,m_json) == ICrlClassifyMessage::Integrity::CComplete) && (validate());
      }

      bool AbsCrlMessage::validate (void)
      {
        return true;
      }

      unsigned char AbsCrlMessage::getInternalFlag (void) const
      {
        return m_flag;
      }

      bool AbsCrlMessage::isInternalMessage (void) const
      {
        return (getInternalFlag() == CInteralFlagInternalMessage);
      }

      AbsCrlMessage::AbsJsonType const & AbsCrlMessage::getJson (void) const
      {
        return m_json;
      }

      AbsCrlMessage::AbsJsonType & AbsCrlMessage::getJson (void)
      {
        return m_json;
      }

      void AbsCrlMessage::setJson (AbsJsonType const & json)
      {
        m_json = json;
      }

      AbsCrlMessage::AbsCrlMessage(unsigned char flag)
       : m_flag(flag), m_json()
      {
      }

      void AbsCrlMessage::private_setInternalFlag (unsigned char flg)
      {
        m_flag = flg;
      }
    }
  }
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageNewEphemeralKey.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageNewEphemeralKey.h>

#include <iostream>
#include <gvr/communication/spot/Utils.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageNewEphemeralKey::SpotMessageNewEphemeralKey(void)
      {
      }

      unsigned char SpotMessageNewEphemeralKey::getSpotAppId (void) const
      {
        return CMessageRequestPinpadAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageNewEphemeralKey::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessageNewEphemeralKey::getCommand(void) const
      {
        return this->CPinpadExtendedMessageCmdId;
      }

      unsigned char SpotMessageNewEphemeralKey::getSubCommandOne() const
      {
        return CEphemeralKeySubCmdId;
      }

      unsigned char SpotMessageNewEphemeralKey::getSubCommandTwo() const
      {
        return CNewEphemeralKeySubSubCmdId;
      }

      SpotMessageNewEphemeralKey::KeyType SpotMessageNewEphemeralKey::getKey (void) const
      {
        return (getSpotAppId() << 24) + (getCommand() << 16) + (getSubCommandOne() << 8) + (getSubCommandTwo());
      }

      SpotMessageNewEphemeralKey::SizeType SpotMessageNewEphemeralKey::size (void) const
      {
        return SpotMessageNewEphemeralKey::CMessageSize;
      }

      SpotMessageNewEphemeralKey::BufferType SpotMessageNewEphemeralKey::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommandOne());
        buff.push_back(this->getSubCommandTwo());

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageNewEphemeralKey Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }

      bool SpotMessageNewEphemeralKey::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < SpotMessageNewEphemeralKey::CMessageSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(this->size() != datasize)
        {
          return false;
        }
        if(buff[position++] != this->getSpotAppId())
        {
          return false;
        }
        if(buff[position++] != this->getSsk())
        {
          return false;
        }
        if(buff[position++] != this->getCommand())
        {
          return false;
        }
        if(buff[position++] != getSubCommandOne() || buff[position++] != getSubCommandTwo())
        {
          return false;
        }
        return true;
      }

      SpotMessageNewEphemeralKey::~SpotMessageNewEphemeralKey()
      {
      }
    }
  }
}

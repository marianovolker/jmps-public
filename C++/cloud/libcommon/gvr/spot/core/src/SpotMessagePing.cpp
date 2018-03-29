/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePing.cpp
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessagePing.h>

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
      SpotMessagePing::SpotMessagePing(void)
      {
      }

      unsigned char SpotMessagePing::getSpotAppId (void) const
      {
        return CMessageRequestSpotAppId; //SPOT Applications Id
      }

      unsigned char SpotMessagePing::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessagePing::getCommand(void) const
      {
        return CPingMessageId;
      }

      SpotMessagePing::KeyType SpotMessagePing::getKey (void) const
      {
        return this->getCommand();
      }

      SpotMessagePing::SizeType SpotMessagePing::size (void) const
      {
        return SpotMessagePing::CMessageSize;
      }

      SpotMessagePing::BufferType SpotMessagePing::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessagePing Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }

      bool SpotMessagePing::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < SpotMessagePing::CMessageSize )
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

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessagePing Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return true;
      }

      SpotMessagePing::~SpotMessagePing()
      {
      }

    }
  }
}

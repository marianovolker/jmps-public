/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageReset.cpp
 * @author mvolker
 * @date Friday, Feb 20, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageReset.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageReset::SpotMessageReset(void)
      {
      }

      unsigned char SpotMessageReset::getSpotAppId (void) const
      {
        return CMessageRequestSpotAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageReset::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessageReset::getCommand(void) const
      {
        return CResetMessageId;
      }

      SpotMessageReset::KeyType SpotMessageReset::getKey (void) const
      {
        return this->getCommand();
      }

      SpotMessageReset::SizeType SpotMessageReset::size (void) const
      {
        return SpotMessageReset::CMessageSize;
      }

      SpotMessageReset::BufferType SpotMessageReset::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());

        return buff;
      }


      bool SpotMessageReset::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < SpotMessageReset::CMessageSize )
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
        return true;
      }

      SpotMessageReset::~SpotMessageReset()
      {
      }

    }
  }
}

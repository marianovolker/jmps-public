/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOptSwInfo.cpp
 * @author mgrieco
 * @date Friday, December 22, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageOptSwInfo.h>

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
      SpotMessageOptSwInfo::SpotMessageOptSwInfo(void)
      {
      }

      unsigned char SpotMessageOptSwInfo::getSpotAppId (void) const
      {
        return CMessageRequestOPTAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageOptSwInfo::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessageOptSwInfo::getCommand(void) const
      {
        return COptSwInfoMessageId;
      }

      unsigned char SpotMessageOptSwInfo::getSubCommand (void) const
      {
        return COptSwInfoMessageSubCmd;
      }

      SpotMessageOptSwInfo::KeyType SpotMessageOptSwInfo::getKey (void) const
      {
        return (this->getCommand() << 8) + (this->getSubCommand());
      }

      SpotMessageOptSwInfo::SizeType SpotMessageOptSwInfo::size (void) const
      {
        return SpotMessageOptSwInfo::CMessageSize;
      }

      SpotMessageOptSwInfo::BufferType SpotMessageOptSwInfo::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(0); //default request value

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageOptSwInfo Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }


      bool SpotMessageOptSwInfo::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < this->CMessageSize )
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
        if(buff[position++] != this->getSubCommand())
        {
          return false;
        }
        if(buff[position++] != 0)//default request value
        {
          return false;
        }

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageOptSwInfo Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return true;
      }

      SpotMessageOptSwInfo::~SpotMessageOptSwInfo()
      {
      }

    }
  }
}

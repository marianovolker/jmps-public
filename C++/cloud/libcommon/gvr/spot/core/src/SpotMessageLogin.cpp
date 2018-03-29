/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageLogin.cpp
 * @author mgrieco
 * @date Friday, December 22, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageLogin.h>

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
      SpotMessageLogin::SpotMessageLogin(void)
      {
        this->clientId = 0;
        this->moduloLen = 0;
        this->pingTimeout = 0;
      }

      SpotMessageLogin::SpotMessageLogin(unsigned char clientId, int len, unsigned char pingTimeout)
      {
        this->clientId = clientId;
        this->moduloLen = len;
        this->pingTimeout = pingTimeout;
      }
      unsigned char SpotMessageLogin::getSpotAppId (void) const
      {
        return CMessageRequestSpotAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageLogin::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessageLogin::getCommand(void) const
      {
        return CLoginMessageId;
      }

      void SpotMessageLogin::setClientId( unsigned char id )
      {
        this->clientId = id;
      }

      void SpotMessageLogin::setModuloLen( int moduloLen )
      {
        this->moduloLen = moduloLen;
      }

      void SpotMessageLogin::setPingTimeout( unsigned char timeout )
      {
        this->pingTimeout = timeout;
      }

      unsigned char SpotMessageLogin::getClientId( void ) const
      {
        return this->clientId;
      }

      int SpotMessageLogin::getModuloLen( void ) const
      {
        return moduloLen;
      }

      unsigned char SpotMessageLogin::getPingTimeout( void ) const
      {
        return pingTimeout;
      }

      SpotMessageLogin::KeyType SpotMessageLogin::getKey (void) const
      {
        return this->getCommand();
      }

      SpotMessageLogin::SizeType SpotMessageLogin::size (void) const
      {
        return SpotMessageLogin::CMessageSize;
      }

      SpotMessageLogin::BufferType SpotMessageLogin::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->clientId);
        buff.push_back((this->moduloLen & 0xFF00) >> 8);
        buff.push_back((this->moduloLen & 0xFF));
        buff.push_back(this->pingTimeout);

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageLogin Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }


      bool SpotMessageLogin::parse(BufferType const & buff)
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
        this->clientId = buff[position++];
        this->moduloLen = (((std::size_t)buff[position++])<<8) | buff[position++];
        this->pingTimeout = buff[position++];

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageLogin Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return true;
      }

      SpotMessageLogin::~SpotMessageLogin()
      {
      }

    }
  }
}

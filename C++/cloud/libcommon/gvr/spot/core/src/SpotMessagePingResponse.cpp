/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePingResponse.cpp
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessagePingResponse.h>

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
      SpotMessagePingResponse::SpotMessagePingResponse(void)
      {
        this->ackCode = CErrorCodeNoError;
      }

      SpotMessagePingResponse::SpotMessagePingResponse(unsigned char const ackcode)
      {
        this->ackCode = ackcode;
      }

      unsigned char SpotMessagePingResponse::getSpotAppId (void) const
      {
        return this->CMessageResponseSpotAppId; //SPOT Applications Id
      }

      unsigned char SpotMessagePingResponse::getSsk (void) const
      {
        return CDefaulNoEncryptionValue;
      }

      unsigned char SpotMessagePingResponse::getCommand(void) const
      {
        return CPingMessageId;
      }

      unsigned char SpotMessagePingResponse::getAckCode( void ) const
      {
        return this->ackCode;
      }

      void SpotMessagePingResponse::setAckCode( unsigned char const ackcode )
      {
        this->ackCode = ackcode;
      }


      SpotMessagePingResponse::KeyType SpotMessagePingResponse::getKey (void) const
      {
        return this->getCommand();
      }

      SpotMessagePingResponse::SizeType SpotMessagePingResponse::size (void) const
      {
        return SpotMessagePingResponse::CMessageSize;
      }

      SpotMessagePingResponse::BufferType SpotMessagePingResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getAckCode());

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessagePingResponse Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }


      bool SpotMessagePingResponse::parse(BufferType const & buff)
      {
        if(buff.size() < SpotMessagePingResponse::CMessageSize)
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(buff.size() < datasize)
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
        this->ackCode = buff[position++];

        /*char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessagePingResponse Parsed Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;
        */
        return true;
      }

      SpotMessagePingResponse::~SpotMessagePingResponse()
      {
      }
    }
  }
}

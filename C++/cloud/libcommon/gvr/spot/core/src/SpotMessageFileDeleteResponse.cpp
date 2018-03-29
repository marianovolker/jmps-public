/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDeleteResponse.cpp
 * @author mgrieco
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileDeleteResponse.h"
#include <gvr/communication/spot/Conversion.h>
#include <iostream>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageFileDeleteResponse::SpotMessageFileDeleteResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileDeleteCommandId, CFileDeleteSubCommandId)
      {
        this->setAckCode( CErrorCodeNoError );
      }

      SpotMessageFileDeleteResponse::SpotMessageFileDeleteResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileDeleteCommandId, CFileDeleteSubCommandId)
      {
        this->setAckCode( ackCode );
      }

      SpotMessageFileDeleteResponse::SizeType SpotMessageFileDeleteResponse::size (void) const
      {
        return CFileDeleteResponseSize;
      }

      SpotMessageFileDeleteResponse::BufferType SpotMessageFileDeleteResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());
       return buff;
      }

      bool SpotMessageFileDeleteResponse::parse(BufferType const & buff)
      {
        std::cout << "SpotMessageFileDeleteResponse::parse" << std::endl;
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CFileDeleteResponseSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(datasize != this->size())
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

        this->setAckCode( buff[position++]);
        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileDeleteResponse::~SpotMessageFileDeleteResponse()
      {
      }
    }
  }
}

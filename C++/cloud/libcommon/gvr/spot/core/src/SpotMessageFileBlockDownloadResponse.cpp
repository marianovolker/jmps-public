/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockDownloadResponse.cpp
 * @author mgrieco
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileBlockDownloadResponse.h"
#include <gvr/communication/spot/Conversion.h>


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageFileBlockDownloadResponse::SpotMessageFileBlockDownloadResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileBlockDownloadCommandId, CFileBlockDownloadSubCommandId)
      {
        this->setAckCode( CErrorCodeNoError );
        startingOffset = 0;
      }

      SpotMessageFileBlockDownloadResponse::SpotMessageFileBlockDownloadResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileBlockDownloadCommandId, CFileBlockDownloadSubCommandId)
      {
        this->setAckCode( ackCode );
        startingOffset = 0;
      }

      const SpotMessageFileBlockDownloadResponse::SizeType SpotMessageFileBlockDownloadResponse::getStartingOffset() const
      {
        return startingOffset;
      }

      void SpotMessageFileBlockDownloadResponse::setStartingOffset(const SizeType offset)
      {
        startingOffset = offset;
      }

      SpotMessageFileBlockDownloadResponse::SizeType SpotMessageFileBlockDownloadResponse::size (void) const
      {
        return CFileBlockDownloadResponseSize;
      }

      SpotMessageFileBlockDownloadResponse::BufferType SpotMessageFileBlockDownloadResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());
        unsigned char offsetArray[4];
        Conversion::uint32ToBigEndian(startingOffset, offsetArray);
        buff.push_back( offsetArray[0] );
        buff.push_back( offsetArray[1] );
        buff.push_back( offsetArray[2] );
        buff.push_back( offsetArray[3] );
       return buff;
      }

      bool SpotMessageFileBlockDownloadResponse::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CFileBlockDownloadResponseSize )
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
        startingOffset =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
        startingOffset += ( buff[position++] << 8 ) +  buff[position++];

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileBlockDownloadResponse::~SpotMessageFileBlockDownloadResponse()
      {
      }
    }
  }
}

/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDownloadResponse.cpp
 * @author mgrieco
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileDownloadResponse.h"
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
      SpotMessageFileDownloadResponse::SpotMessageFileDownloadResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileDownloadCommandId, CFileDownloadSubCommandId)
      {
        this->setAckCode( CErrorCodeNoError );
        startingOffset = 0;
      }

      SpotMessageFileDownloadResponse::SpotMessageFileDownloadResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileDownloadCommandId, CFileDownloadSubCommandId)
      {
        this->setAckCode( ackCode );
        startingOffset = 0;
      }

      const SpotMessageFileDownloadResponse::SizeType SpotMessageFileDownloadResponse::getStartingOffset() const
      {
        return startingOffset;
      }

      void SpotMessageFileDownloadResponse::setStartingOffset(const SizeType offset)
      {
        startingOffset = offset;
      }

      SpotMessageFileDownloadResponse::SizeType SpotMessageFileDownloadResponse::size (void) const
      {
        if(this->getAckCode()== CErrorCodeNoError)
        {
          return CFileDownloadResponseSize;
        }
        else
        {
          return CFileDownloadCommonResponseSize;
        }
      }

      SpotMessageFileDownloadResponse::BufferType SpotMessageFileDownloadResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());
        if(getAckCode() == CErrorCodeNoError)
        {
          unsigned char offsetArray[4];
          Conversion::uint32ToBigEndian(startingOffset, offsetArray);
          buff.push_back( offsetArray[0] );
          buff.push_back( offsetArray[1] );
          buff.push_back( offsetArray[2] );
          buff.push_back( offsetArray[3] );
        }
       return buff;
      }

      bool SpotMessageFileDownloadResponse::parse(BufferType const & buff)
      {
        std::cout << "SpotMessageFileDownloadResponse::parse" << std::endl;
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CFileDownloadCommonResponseSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
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
        if(this->getAckCode()== CErrorCodeNoError)
        {
          startingOffset =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
          startingOffset += ( buff[position++] << 8 ) +  buff[position++];
        }

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileDownloadResponse::~SpotMessageFileDownloadResponse()
      {
      }
    }
  }
}

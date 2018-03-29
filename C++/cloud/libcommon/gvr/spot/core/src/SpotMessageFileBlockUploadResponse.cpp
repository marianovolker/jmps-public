/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockUploadResponse.cpp
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/Utils.h>
#include "../include/gvr/communication/spot/SpotMessageFileBlockUploadResponse.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageFileBlockUploadResponse::SpotMessageFileBlockUploadResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileBlockUploadCommandId, CFileBlockUploadSubCommandId)
      {
        this->setAckCode( CErrorCodeNoError );
        nextBlockOffset = 0;
        this->blockData.clear();
      }

      SpotMessageFileBlockUploadResponse::SpotMessageFileBlockUploadResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileBlockUploadCommandId, CFileBlockUploadSubCommandId)
      {
        this->setAckCode( ackCode );
        nextBlockOffset = 0;
        this->blockData.clear();
      }

      bool SpotMessageFileBlockUploadResponse::wasBlockReceivedOK()
      {
        return ( getAckCode() == CBlockUploadedOK || getAckCode() == CLastBlock );
      }

      bool SpotMessageFileBlockUploadResponse::isLastBlock()
      {
        return ( getAckCode() == CLastBlock );
      }

      const SpotMessageFileBlockUploadResponse::SizeType SpotMessageFileBlockUploadResponse::getStartingOffset() const
      {
        return nextBlockOffset;
      }

      void SpotMessageFileBlockUploadResponse::setStartingOffset(const SizeType offset)
      {
        nextBlockOffset = offset;
      }

      const ISpotMessage::BufferType & SpotMessageFileBlockUploadResponse::getBlockData( void ) const
      {
        return this->blockData;
      }

      void SpotMessageFileBlockUploadResponse::setBlockData( BufferType const data)
      {
        blockData.clear();
        for(int position =0; position<data.size(); )
        {
          this->blockData.push_back(data[position++]);
        }
      }


      SpotMessageFileBlockUploadResponse::SizeType SpotMessageFileBlockUploadResponse::size (void) const
      {

        if( getAckCode() == CErrorCodeNoError || getAckCode() == CLastBlock )
        {
          return CFileBlockUploadCommonOkResponseSize + blockData.size();
        }

        return CFileBlockUploadCommonResponseSize;
      }

      SpotMessageFileBlockUploadResponse::BufferType SpotMessageFileBlockUploadResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());

        if(this->getAckCode() == CErrorCodeNoError)
        {
          unsigned char offsetArray[4];
          Conversion::uint32ToBigEndian(nextBlockOffset, offsetArray);
          buff.push_back( offsetArray[0] );
          buff.push_back( offsetArray[1] );
          buff.push_back( offsetArray[2] );
          buff.push_back( offsetArray[3] );

          unsigned char sizeArray[4];
          Conversion::uint32ToBigEndian(blockData.size(), sizeArray);
          buff.push_back( sizeArray[0] );
          buff.push_back( sizeArray[1] );
          buff.push_back( sizeArray[2] );
          buff.push_back( sizeArray[3] );

          for(int position =0; position<blockData.size(); )
          {
            buff.push_back(blockData[position++]);
          }
        }
        return buff;
      }

      bool SpotMessageFileBlockUploadResponse::parse(BufferType const & buff)
      {
        std::cout << "Parsing SpotMessageFileBlockUploadResponse ...\n\n";

        int iSize = CFileBlockUploadCommonOkResponseSize;
        if( buff.size() < CFileBlockUploadCommonOkResponseSize ) iSize = buff.size();
        char acBuffer[3 * iSize + 2];
        std::cout << "\nBlock Header: " << Utils::dumpBuffer(acBuffer, &buff.front(), iSize)
                  << std::endl << std::endl;

        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CFileBlockUploadCommonResponseSize )
        {
          return false;
        }

        int position(0);
        int msgSize = (((std::size_t)buff[position++])<<8) | buff[position++];
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

        nextBlockOffset = 0;
        blockData.clear();
        this->setAckCode( buff[position++] );

        if( getAckCode() == CErrorCodeNoError || ( getAckCode() == CLastBlock && msgSize > CFileBlockUploadCommonOkResponseSize) )
        {
          nextBlockOffset =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
          nextBlockOffset += ( buff[position++] << 8 ) +  buff[position++];

          SizeType blockSize =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
          blockSize += ( buff[position++] << 8 ) +  buff[position++];

          for(int ind = 0; (ind < blockSize) && (position < buff.size()); ind++)
          {
            blockData.push_back(buff[position++]);
          }
        }
        else
        {
          return true; // valid error reply or final null block.
        }

        if(this->size() != msgSize)
        {
          std::cout << "BAD block !! - size: " << this->size() << ", msg size: " << msgSize << std::endl;
          return false;
        }
        return true;
      }

      SpotMessageFileBlockUploadResponse::~SpotMessageFileBlockUploadResponse()
      {
      }
    }
  }
}

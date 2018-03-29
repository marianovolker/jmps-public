/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileUploadResponse.cpp
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/Utils.h>
#include "../include/gvr/communication/spot/SpotMessageFileUploadResponse.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {


      SpotMessageFileUploadResponse::SpotMessageFileUploadResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileUploadCommandId, CFileUploadSubCommandId)
      {
        this->setAckCode( CErrorCodeNoError );
        fileSize = 0;
      }

      SpotMessageFileUploadResponse::SpotMessageFileUploadResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CFileUploadCommandId, CFileUploadSubCommandId)
      {
        this->setAckCode( ackCode );
        fileSize = 0;
      }

      bool SpotMessageFileUploadResponse::canUploadBegin()
      {
        return  ( getAckCode() == CPackageUploadAccepted );
      }

      const SpotMessageFileUploadResponse::SizeType SpotMessageFileUploadResponse::getFileSize() const
      {
        return fileSize;
      }

      void SpotMessageFileUploadResponse::setFileSize(const SizeType offset)
      {
        fileSize = offset;
      }

      SpotMessageFileUploadResponse::SizeType SpotMessageFileUploadResponse::size (void) const
      {
        if(this->getAckCode()== CErrorCodeNoError)
        {
          return CFileUploadResponseSize;
        }
        else
        {
          return CFileUploadCommonResponseSize;
        }
      }

      SpotMessageFileUploadResponse::BufferType SpotMessageFileUploadResponse::serialize (void) const
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
          Conversion::uint32ToBigEndian(this->fileSize, offsetArray);
          buff.push_back( offsetArray[0] );
          buff.push_back( offsetArray[1] );
          buff.push_back( offsetArray[2] );
          buff.push_back( offsetArray[3] );
        }
       return buff;
      }

      bool SpotMessageFileUploadResponse::parse(BufferType const & buff)
      {
        std::cout << "SpotMessageFileUploadResponse::parse\n\n";

        int iSize = CFileUploadResponseSize;
        if( buff.size() < CFileUploadResponseSize ) iSize = buff.size();
        char acBuffer[3 * iSize + 2];
        std::cout << "\nUpload answer: " << Utils::dumpBuffer(acBuffer, &buff.front(), iSize)
                  << std::endl << std::endl;

        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CFileUploadCommonResponseSize )
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
        if(this->getAckCode() == CErrorCodeNoError)
        {
          fileSize =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
          fileSize += ( buff[position++] << 8 ) +  buff[position++];
        }
        else
        {
          return true; // valid error reply.
        }
        return true;
      }

      SpotMessageFileUploadResponse::~SpotMessageFileUploadResponse()
      {
      }


    }
  }
}

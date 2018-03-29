/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileUpload.cpp
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/Conversion.h>
#include "../include/gvr/communication/spot/SpotMessageFileUpload.h"

using namespace std;

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageFileUpload::SpotMessageFileUpload(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileUploadCommandId, CFileUploadSubCommandId)
      {
        fileId = 0x00;
        fileType = 0x00;
      }

      SpotMessageFileUpload::SpotMessageFileUpload(unsigned char const type, unsigned char const id): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileUploadCommandId, CFileUploadSubCommandId)
      {
        fileId = id;
        fileType = type;
      }

      unsigned char SpotMessageFileUpload::getFileType( void ) const
      {
        return fileType;
      }

      void SpotMessageFileUpload::setFileType( unsigned char const type)
      {
        fileType = type;
      }

      unsigned char SpotMessageFileUpload::getFileId( void ) const
      {
        return fileId;
      }

      void SpotMessageFileUpload::setFileId( unsigned char const id)
      {
        fileId = id;
      }

      SpotMessageFileUpload::SizeType SpotMessageFileUpload::size (void) const
      {
        return SpotMessageFileUpload::CMessageCommonSize;
      }

      SpotMessageFileUpload::BufferType SpotMessageFileUpload::serialize (void) const
      {
        std::cout << "SpotMessageFileUpload::serialize" << std::endl;
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());

        buff.push_back(this->getFileType());
        buff.push_back(this->getFileId());

        char* pcBuffer = new char[3 * size() + 2];
        std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size())
                  << std::endl << std::endl;
        delete[] pcBuffer;

        return buff;
      }


      bool SpotMessageFileUpload::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CMessageCommonSize )
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
        setFileType(buff[position++]);
        setFileId(buff[position++]);

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileUpload::~SpotMessageFileUpload()
      {
      }
    }
  }
}

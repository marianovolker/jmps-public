/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDownload.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileDownload.h"
#include <iostream>
#include <iostream>
#include <gvr/communication/spot/Conversion.h>

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
      SpotMessageFileDownload::SpotMessageFileDownload(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileDownloadCommandId, CFileDownloadSubCommandId)
      {
        fileId = 0x00;
        fileType = 0x00;
        fileSize = 0;
        authenType = 0x00;
      }

      SpotMessageFileDownload::SpotMessageFileDownload(unsigned char const type, unsigned char const id): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileDownloadCommandId, CFileDownloadSubCommandId)
      {
        fileId = id;
        fileType = type;
        fileSize = 0;
        authenType = 0x00;
      }

      unsigned char SpotMessageFileDownload::getFileType( void ) const
      {
        return fileType;
      }

      void SpotMessageFileDownload::setFileType( unsigned char const type)
      {
        fileType = type;
      }

      unsigned char SpotMessageFileDownload::getFileId( void ) const
      {
        return fileId;
      }

      void SpotMessageFileDownload::setFileId( unsigned char const id)
      {
        fileId = id;
      }

      SpotMessageFileDownload::SizeType SpotMessageFileDownload::getFileSize( void ) const
      {
        return fileSize;
      }

      void SpotMessageFileDownload::setFileSize( SizeType const size)
      {
        this->fileSize = size;
      }

      SpotMessageFileDownload::BufferType SpotMessageFileDownload::getCrcData( void ) const
      {
        return crcData;
      }

      void SpotMessageFileDownload::setCrcData( SpotMessageFileDownload::BufferType const crc)
      {
        crcData.clear();
        for(int position =0;position<crc.size();)
        {
          crcData.push_back(crc[position++]);
        }
      }

      unsigned char SpotMessageFileDownload::getAuthenType( void ) const
      {
        return authenType;
      }

      void SpotMessageFileDownload::setAuthenType( unsigned char const type)
      {
        this->authenType = type;
      }

      SpotMessageFileDownload::BufferType SpotMessageFileDownload::getSecretAuthenticator( void ) const
      {
        return secretAuthenticator;
      }

      void SpotMessageFileDownload::setSecretAuthenticator( SpotMessageFileDownload::BufferType const authenticator)
      {
        secretAuthenticator.clear();
        for(int position =0;position<authenticator.size();)
        {
          secretAuthenticator.push_back(authenticator[position++]);
        }
      }

      std::string SpotMessageFileDownload::getDescription( void ) const
      {
        return fileDescription;
      }

      void SpotMessageFileDownload::setDescription( std::string const description)
      {
        this->fileDescription = description;
      }

      SpotMessageFileDownload::SizeType SpotMessageFileDownload::size (void) const
      {
        return SpotMessageFileDownload::CMessageCommonSize + this->secretAuthenticator.size();
      }

      SpotMessageFileDownload::BufferType SpotMessageFileDownload::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());

        buff.push_back(this->getFileType());
        buff.push_back(this->getFileId());

        unsigned char offsetArray[4];
        Conversion::uint32ToBigEndian(fileSize, offsetArray);
        buff.push_back( offsetArray[0] );
        buff.push_back( offsetArray[1] );
        buff.push_back( offsetArray[2] );
        buff.push_back( offsetArray[3] );

        for(int position =0; position<crcData.size() && position<4; )
        {
          buff.push_back(crcData[position++]);
        }

        buff.push_back( authenType );

        for(int position =0; position<secretAuthenticator.size();)
        {
          buff.push_back(secretAuthenticator[position++]);
        }
        int position =0;
        for(; position<fileDescription.size() && position < 16;)
        {
          buff.push_back(fileDescription[position++]);
        }
        for(; position<16; position++)
        {
          buff.push_back(0x00);
        }

        return buff;
      }


      bool SpotMessageFileDownload::parse(BufferType const & buff)
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

        SizeType size =  ( buff[position++] << 24 ) + (buff[position++] << 16);
        size += ( buff[position++] << 8 ) +  buff[position++];
        setFileSize(size);

        crcData.clear();
        for(int ind =0; ind<4; ind++)
        {
          crcData.push_back(buff[position++]);
        }
        setAuthenType(buff[position++]);

        secretAuthenticator.clear();
        SizeType authSize = (authenType == 0x00) ? 0 : ((authenType == 0x00 || authenType == 0x01 || authenType == 0x02) ? 4 : 248);
        if(buff.size() < CMessageCommonSize+authSize )
        {
          return false;
        }

        for(int ind =0; ind<authSize; ind++)
        {
          this->secretAuthenticator.push_back(buff[position++]);
        }

        fileDescription.clear();
        for(int ind =0; ind < 16; ind++)
        {
          fileDescription.push_back(buff[position++]);
        }

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileDownload::~SpotMessageFileDownload()
      {
      }
    }
  }
}

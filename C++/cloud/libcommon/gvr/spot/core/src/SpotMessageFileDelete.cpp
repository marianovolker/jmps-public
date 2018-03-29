/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDelete.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileDelete.h"
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
      SpotMessageFileDelete::SpotMessageFileDelete(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileDeleteCommandId, CFileDeleteSubCommandId)
      {
        fileId = 0x00;
        fileType = 0x00;
      }

      SpotMessageFileDelete::SpotMessageFileDelete(unsigned char const  type, unsigned char const id): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileDeleteCommandId, CFileDeleteSubCommandId)
      {
        fileId = type;
        fileType = id;
      }

      unsigned char SpotMessageFileDelete::getFileType( void ) const
      {
        return fileType;
      }

      void SpotMessageFileDelete::setFileType( unsigned char const type)
      {
        fileType = type;
      }

      unsigned char SpotMessageFileDelete::getFileId( void ) const
      {
        return fileId;
      }

      void SpotMessageFileDelete::setFileId( unsigned char const id)
      {
        fileId = id;
      }

      SpotMessageFileDelete::SizeType SpotMessageFileDelete::size (void) const
      {
        return SpotMessageFileDelete::CMessageSize;
      }

      SpotMessageFileDelete::BufferType SpotMessageFileDelete::serialize (void) const
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
        return buff;
      }


      bool SpotMessageFileDelete::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CMessageSize )
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

        return true;
      }

      SpotMessageFileDelete::~SpotMessageFileDelete()
      {
      }
    }
  }
}

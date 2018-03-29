/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockUpload.cpp
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/Utils.h>
#include "../include/gvr/communication/spot/SpotMessageFileBlockUpload.h"

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
      SpotMessageFileBlockUpload::SpotMessageFileBlockUpload(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileBlockUploadCommandId, CFileBlockUploadSubCommandId)
      {
        blockOffset = 0;
      }

      SpotMessageFileBlockUpload::SizeType SpotMessageFileBlockUpload::getBlockOffset( void ) const
      {
        return blockOffset;
      }

      void SpotMessageFileBlockUpload::setBlockOffset( SizeType const offset)
      {
        this->blockOffset = offset;
      }

      SpotMessageFileBlockUpload::SizeType SpotMessageFileBlockUpload::size (void) const
      {
        return SpotMessageFileBlockUpload::CMessageCommonSize;
      }

      SpotMessageFileBlockUpload::BufferType SpotMessageFileBlockUpload::serialize (void) const
      {
        std::cout << "SpotMessageFileBlockUpload::serialize" << std::endl;
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));

        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());

        unsigned char offsetArray[4];
        Conversion::uint32ToBigEndian(blockOffset, offsetArray);
        buff.push_back( offsetArray[0] );
        buff.push_back( offsetArray[1] );
        buff.push_back( offsetArray[2] );
        buff.push_back( offsetArray[3] );
        std::cout << "SpotMessageFileBlockUpload::blockOffset " << std::hex << static_cast<int>(blockOffset) << std::dec << std::endl;

        return buff;
      }


      bool SpotMessageFileBlockUpload::parse(BufferType const & buff)
      {
        std::cout << "SpotMessageFileBlockUpload::parse" << std::endl;

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

        blockOffset =  ( buff[position++]  << 24 ) + (buff[position++] << 16);
        blockOffset += ( buff[position++] << 8 ) +  buff[position++];

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileBlockUpload::~SpotMessageFileBlockUpload()
      {
      }
    }
  }
}

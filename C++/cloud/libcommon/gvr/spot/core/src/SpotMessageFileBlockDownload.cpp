/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockDownload.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageFileBlockDownload.h"
#include <gvr/communication/spot/Conversion.h>
#include <iostream>

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
      SpotMessageFileBlockDownload::SpotMessageFileBlockDownload(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CFileBlockDownloadCommandId, CFileBlockDownloadSubCommandId)
      {
        blockOffset = 0;
      }

      SpotMessageFileBlockDownload::SizeType SpotMessageFileBlockDownload::getBlockOffset( void ) const
      {
        return blockOffset;
      }

      void SpotMessageFileBlockDownload::setBlockOffset( SizeType const offset)
      {
        this->blockOffset = offset;
      }

      SpotMessageFileBlockDownload::BufferType SpotMessageFileBlockDownload::getBlockData( void ) const
      {
        return blockData;
      }

      void SpotMessageFileBlockDownload::setBlockData( SpotMessageFileBlockDownload::BufferType const data)
      {
        blockData.clear();
        for(int position =0;position<data.size();)
        {
          blockData.push_back(data[position++]);
        }
      }

      SpotMessageFileBlockDownload::SizeType SpotMessageFileBlockDownload::size (void) const
      {
        return SpotMessageFileBlockDownload::CMessageCommonSize + blockData.size();
      }

      SpotMessageFileBlockDownload::BufferType SpotMessageFileBlockDownload::serialize (void) const
      {
        //std::cout << "SpotMessageFileBlockDownload::serialize" << std::endl;
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
        std::cout << "SpotMessageFileBlockDownload::blockOffset " << std::dec << static_cast<int>(blockOffset) << std::endl;

        buff.push_back((blockData.size() & 0xFF00) >> 8);
        buff.push_back((blockData.size() & 0xFF));

        for(int position =0; position<blockData.size(); )
        {
          buff.push_back(blockData[position++]);
        }

        return buff;
      }


      bool SpotMessageFileBlockDownload::parse(BufferType const & buff)
      {
        std::cout << "SpotMessageFileBlockDownload::parse" << std::endl;

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

        int blockdatasize = (((std::size_t)buff[position++])<<8) | buff[position++];

        blockData.clear();
        for(int ind =0; ind<blockdatasize; ind++)
        {
          blockData.push_back(buff[position++]);
        }

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessageFileBlockDownload::~SpotMessageFileBlockDownload()
      {
      }
    }
  }
}

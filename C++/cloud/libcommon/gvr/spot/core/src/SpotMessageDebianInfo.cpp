/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageDebianInfo.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessageDebianInfo.h"
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
      SpotMessageDebianInfo::SpotMessageDebianInfo(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CDebianInfoCommandId, CDebianInfoSubCommandId)
      {
      }

      SpotMessageDebianInfo::SpotMessageDebianInfo(std::string package): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CDebianInfoCommandId, CDebianInfoSubCommandId)
      {
        this->packageList = package;
      }

      void SpotMessageDebianInfo::setPackageListData( std::string list )
      {
        this->packageList = list;
      }

      std::string SpotMessageDebianInfo::getPackageListData( void ) const
      {
        return this->packageList;
      }

      SpotMessageDebianInfo::SizeType SpotMessageDebianInfo::size (void) const
      {
        return SpotMessageDebianInfo::CMessageSize;
      }

      SpotMessageDebianInfo::BufferType SpotMessageDebianInfo::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        int position=0;
        for(; position<CDebiansInfoSize && position< packageList.size();)
        {
          buff.push_back(packageList[position++]);
        }
        for(; position<CDebiansInfoSize; position++)
        {
          buff.push_back(0x00);
        }
        return buff;
      }


      bool SpotMessageDebianInfo::parse(BufferType const & buff)
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

        this->packageList.clear();
        for (; position < buff.size();)
        {
          packageList.push_back(buff[position++]);
        }
        return true;
      }

      SpotMessageDebianInfo::~SpotMessageDebianInfo()
      {
      }
    }
  }
}

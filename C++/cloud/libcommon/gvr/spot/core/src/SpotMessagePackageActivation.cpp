/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageActivation.cpp
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotMessagePackageActivation.h"
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
      SpotMessagePackageActivation::SpotMessagePackageActivation(void): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CPackageActivationCommandId, CPackageActivationSubCommandId)
      {
        this->vgdPlatform = false;
        this->activationTime = "0000.00.00-00:00";
      }

      SpotMessagePackageActivation::SpotMessagePackageActivation(std::string const package): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CPackageActivationCommandId, CPackageActivationCommandId)
      {
        this->vgdPlatform = false;
        this->packageName = package;
        this->activationTime = "0000.00.00-00:00";
      }

      SpotMessagePackageActivation::SpotMessagePackageActivation(bool const vgd, std::string const package): AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CPackageActivationCommandId, CPackageActivationCommandId)
      {
        this->packageName = package;
        this->vgdPlatform = vgd;
        this->activationTime = "0000.00.00-00:00";
      }

      void SpotMessagePackageActivation::setPackageActivationName( std::string const name )
      {
        this->packageName = name;
      }

      std::string SpotMessagePackageActivation::getPackageActivationName( void ) const
      {
        return this->packageName;
      }

      void SpotMessagePackageActivation::setPackageActivationTime( std::string const time )
      {
        this->activationTime = time;
      }

      std::string SpotMessagePackageActivation::getPackageActivationTime( void ) const
      {
        return this->activationTime;
      }

      void SpotMessagePackageActivation::setVgdPlatform(bool const vgd)
      {
        this->vgdPlatform = vgd;
      }

      bool SpotMessagePackageActivation::getVgdPlatform( void ) const
      {
        return this->vgdPlatform;
      }

      SpotMessagePackageActivation::SizeType SpotMessagePackageActivation::size (void) const
      {
        return packageName.size()== 0 ? SpotMessagePackageActivation::CMessageCommonSize : SpotMessagePackageActivation::CMessageCommonSize + SpotMessagePackageActivation::CPackageActivationNameSize;
      }

      unsigned char SpotMessagePackageActivation::getSpotAppId( void ) const
      {
        if(this->vgdPlatform)
        {
          return this->CMessageRequestPinpadAppId;
        }
        else
        {
          return this->CMessageRequestSetupAppId;
        }
      }

      unsigned char SpotMessagePackageActivation::getCommand( void ) const
      {
        if(this->vgdPlatform)
        {
          return this->CVgdPackageActivationCommandId;
        }
        else
        {
          return this->CPackageActivationCommandId;
        }
      }

      SpotMessagePackageActivation::BufferType SpotMessagePackageActivation::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());

        int position=0;
        for(; position< this->CPackageActivationTimeSize && position< activationTime.size();)
        {
          buff.push_back(activationTime[position++]);
        }
        for(; position<CPackageActivationTimeSize; position++)
        {
          buff.push_back(0x00);
        }
        if(packageName.size()>0)
        {
          position=0;
          for(; position< this->CPackageActivationNameSize && position< packageName.size();)
          {
            buff.push_back(packageName[position++]);
          }
          for(; position<CPackageActivationNameSize; position++)
          {
            buff.push_back(0x00);
          }
        }
        return buff;
      }


      bool SpotMessagePackageActivation::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CMessageCommonSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];

        if(buff.size() < CMessageCommonSize)
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

        this->activationTime.clear();
        for (int i =0; i < this->CPackageActivationTimeSize; i++)
        {
          activationTime.push_back(buff[position++]);
        }

        this->packageName.clear();
        for (int i =0; position < buff.size() && i < this->CPackageActivationNameSize; i++)
        {
          packageName.push_back(buff[position++]);
        }

        if(this->size() != datasize)
        {
          return false;
        }

        return true;
      }

      SpotMessagePackageActivation::~SpotMessagePackageActivation()
      {
      }
    }
  }
}

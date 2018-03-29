/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageActivationResponse.cpp
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessagePackageActivationResponse.h>
#include <iostream>
#include <iomanip>

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
      SpotMessagePackageActivationResponse::SpotMessagePackageActivationResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CPackageActivationCommandId, CPackageActivationSubCommandId)
      {
        this->setAckCode(CErrorCodeNoError);
        this->vgdPlatform = false;
      }

      SpotMessagePackageActivationResponse::SpotMessagePackageActivationResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CPackageActivationCommandId, CPackageActivationSubCommandId)
      {
        this->setAckCode(ackCode);
        this->vgdPlatform = false;
      }

      void SpotMessagePackageActivationResponse::setVgdPlatform(bool vgd)
      {
        this->vgdPlatform = vgd;
      }

      bool SpotMessagePackageActivationResponse::getVgdPlatform( void ) const
      {
        return this->vgdPlatform;
      }

      bool SpotMessagePackageActivationResponse::hasError( void ) const
      {
        return ( getAckCode() != CErrorCodeNoError && getAckCode() != CErrorCodeDebianActivation );
      }

      const std::string SpotMessagePackageActivationResponse::getInfoResponseData() const
      {
        return infoResponseData;
      }

      void SpotMessagePackageActivationResponse::setInfoResponseData(const std::string& data)
      {
        infoResponseData.clear();
        for(int position =0; position< data.size();position++)
        {
          infoResponseData.push_back(data[position]);
        }
      }

      SpotMessagePackageActivationResponse::SizeType SpotMessagePackageActivationResponse::size (void) const
      {
        return (getAckCode() != CErrorCodeNoError && getAckCode() != CErrorCodeDebianActivation) ? CMessageResponseCommonSize : (CMessageResponseCommonSize + infoResponseData.size());
      }

      unsigned char SpotMessagePackageActivationResponse::getSpotAppId( void ) const
      {
        if(this->vgdPlatform)
        {
          return this->CMessageResponsePinpadAppId;
        }
        else
        {
          return this->CMessageResponseSetupAppId;
        }
      }

      unsigned char SpotMessagePackageActivationResponse::getCommand( void ) const
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


      SpotMessagePackageActivationResponse::BufferType SpotMessagePackageActivationResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());
        if(getAckCode() == CErrorCodeNoError || getAckCode() == CErrorCodeDebianActivation)
        {
          for(int position =0; position<infoResponseData.size();)
          {
            buff.push_back(infoResponseData[position++]);
          }
       }
       return buff;
      }

      bool SpotMessagePackageActivationResponse::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < CMessageResponseCommonSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(this->size() < CMessageResponseCommonSize || buff.size() < CMessageResponseCommonSize)
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

        this->setAckCode(buff[position++]);
        if(getAckCode() == CErrorCodeNoError || getAckCode() == CErrorCodeDebianActivation)
        {
          infoResponseData.clear();
          for(;position < buff.size();)
          {
            infoResponseData.push_back(buff[position++]);
          }
        }

        if(this->size() != datasize)
        {
          return false;
        }
        return true;
      }

      SpotMessagePackageActivationResponse::~SpotMessagePackageActivationResponse()
      {
      }
    }
  }
}

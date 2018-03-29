/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageDebianInfoResponse.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


# include <gvr/communication/spot/SpotMessageDebianInfoResponse.h>


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {

      SpotMessageDebianInfoResponse::SpotMessageDebianInfoResponse(void): AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CDebianInfoCommandId, CDebianInfoSubCommandId)
      {
        setAckCode( CErrorCodeNoError );
      }

      SpotMessageDebianInfoResponse::SpotMessageDebianInfoResponse(unsigned char ackCode): AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CDebianInfoCommandId, CDebianInfoSubCommandId)
      {
        setAckCode( ackCode );
      }

      const std::string SpotMessageDebianInfoResponse::getInfoResponseData() const
      {
        return infoResponseData;
      }

      void SpotMessageDebianInfoResponse::setInfoResponseData(const std::string& data)
      {
        infoResponseData.clear();
        infoResponseData.reserve(data.size());
        std::copy( data.begin(), data.end(), std::back_inserter(infoResponseData) );
      }

      SpotMessageDebianInfoResponse::SizeType SpotMessageDebianInfoResponse::size (void) const
      {
        return ((getAckCode() != CErrorCodeNoError && getAckCode() != CErrorCodeDownloadComplete && getAckCode() != CErrorCodeDebianIntermediateBlock)) ? CMessageResponseCommonSize : (CMessageResponseCommonSize + 2 + infoResponseData.size());
      }

      SpotMessageDebianInfoResponse::BufferType SpotMessageDebianInfoResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());
        if(getAckCode() == CErrorCodeNoError || getAckCode() == CErrorCodeDownloadComplete || getAckCode() == CErrorCodeDebianIntermediateBlock)
        {
          buff.push_back((infoResponseData.size() & 0xFF00) >> 8);
          buff.push_back((infoResponseData.size() & 0xFF));

          std::copy( infoResponseData.begin(), infoResponseData.end(), std::back_inserter(buff) );
       }
       return buff;
      }

      bool SpotMessageDebianInfoResponse::parse(BufferType const & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if( buff.size() < CMessageResponseCommonSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];
        if(this->size() < CMessageResponseCommonSize || buff.size() < CMessageResponseCommonSize)
        {
          return false;
        }
        if(buff[position+2] != this->getSpotAppId())
        {
          return false;
        }
        if(buff[position+3] != this->getSsk())
        {
          return false;
        }
        if(buff[position+4] != this->getCommand())
        {
          return false;
        }
        if(buff[position+5] != this->getSubCommand())
        {
          return false;
        }

        setAckCode( buff[position+6] );

        if( (getAckCode() == CErrorCodeNoError) || (getAckCode() == CErrorCodeDownloadComplete) || (getAckCode() == CErrorCodeDebianIntermediateBlock) )
        {
          int datasize = (((std::size_t)buff[position+7])<<8) | buff[position+8];
          infoResponseData.clear();
          infoResponseData.reserve(datasize);
          position = 9;
          std::copy( (buff.begin()+position), buff.end(), std::back_inserter(infoResponseData) );
        }

        if( size() != datasize )
        {
          return false;
        }

        return true;
      }

      SpotMessageDebianInfoResponse::~SpotMessageDebianInfoResponse()
      {
      }
    }
  }
}

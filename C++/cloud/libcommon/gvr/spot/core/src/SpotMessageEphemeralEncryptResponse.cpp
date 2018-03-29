/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageEphemeralEncryptResponse.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageEphemeralEncryptResponse.h>
#include <gvr/communication/spot/SpotMessageEphemeralEncrypt.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageEphemeralEncryptResponse::SpotMessageEphemeralEncryptResponse(bool decrypt):
        m_ackCode(CErrorCodeNoError),
        m_data(),
        m_decrypt(decrypt),
        m_specificError(0x00)
      {
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getSpotAppId (void) const
      {
        return CMessageResponsePinpadAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getSsk (void) const
      {
        return CDefaulNoEncryptionValue;
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getCommand(void) const
      {
        return CPinpadExtendedMessageCmdId;
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getSubCommandOne() const
      {
        return CEphemeralKeySubCmdId;
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getSubCommandTwo() const
      {
        if(m_decrypt)
        {
          return CEphemeralDecryptSubCmdId;
        }
        else
        {
          return CEphemeralEncryptSubCmdId;
        }
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getAckCode( void ) const
      {
        return m_ackCode;
      }

      const SpotMessageEphemeralEncryptResponse::BufferType& SpotMessageEphemeralEncryptResponse::getData( void ) const
      {
        return m_data;
      }

      void SpotMessageEphemeralEncryptResponse::setData( const BufferType& data )
      {
        m_data.clear();
        m_data.reserve(data.size());
        std::copy( data.begin(), data.end(), std::back_inserter(m_data) );
      }

      unsigned char SpotMessageEphemeralEncryptResponse::getSpecificError( void ) const
      {
        return m_specificError;
      }

      SpotMessageEphemeralEncryptResponse::KeyType SpotMessageEphemeralEncryptResponse::getKey (void) const
      {
        return  (SpotMessageEphemeralEncryptResponse::CMessageResponsePinpadAppId << 24)
            + (SpotMessageEphemeralEncryptResponse::CPinpadExtendedMessageCmdId << 16)
            + (SpotMessageEphemeralEncryptResponse::CEphemeralKeySubCmdId << 8)
            + SpotMessageEphemeralEncryptResponse::CEphemeralEncryptSubCmdId;
      }

      SpotMessageEphemeralEncryptResponse::SizeType SpotMessageEphemeralEncryptResponse::size (void) const
      {
        SizeType dataSize = ((m_ackCode == CErrorCodeNoError) ? m_data.size() + 2 : 1);
        return SpotMessageEphemeralEncryptResponse::CMinimumMessageSize + dataSize;
      }

      SpotMessageEphemeralEncryptResponse::BufferType SpotMessageEphemeralEncryptResponse::serialize (void) const
      {
        BufferType buff;
        buff.push_back((size() & 0xFF00) >> 8);
        buff.push_back((size() & 0xFF));
        buff.push_back(getSpotAppId());
        buff.push_back(getSsk());
        buff.push_back(getCommand());
        buff.push_back(getSubCommandOne());
        buff.push_back(getSubCommandTwo());
        buff.push_back(getAckCode());
        if(m_ackCode == CErrorCodeNoError)
        {
          buff.push_back((m_data.size()& 0xFF00) >> 8);
          buff.push_back((m_data.size()& 0xFF));

          std::copy( m_data.begin(), m_data.end(), std::back_inserter(buff) );
        }
        else
        {
          buff.push_back(m_specificError);
        }

        return buff;
      }

      bool SpotMessageEphemeralEncryptResponse::parse(const BufferType& buff)
      {
        if(buff.size() < SpotMessageEphemeralEncryptResponse::CMinimumMessageSize)
        {
          return false;
        }

        int position(0);
        int messageSize = (((std::size_t)buff[position+0])<<8) | buff[position+1];

        if(buff.size() < messageSize)
        {
          return false;
        }
        if(buff[position+2] != getSpotAppId())
        {
          return false;
        }
        if(buff[position+3] != getSsk())
        {
          return false;
        }
        if(buff[position+4] != getCommand())
        {
          return false;
        }
        if(buff[position+5] != getSubCommandOne())
        {
          return false;
        }
        if(buff[position+6] != getSubCommandTwo())
        {
          return false;
        }

        m_ackCode = buff[position+7];

        if(m_ackCode == CErrorCodeNoError)
        {
          int datasize = (((std::size_t)buff[position+8])<<8) | buff[position+9];
          m_data.clear();
          m_data.reserve(datasize);
          position = 10;
          std::copy( (buff.begin()+position), buff.end(), std::back_inserter(m_data) );
        }
        else
        {
          m_specificError = buff[position+8];
        }

        if(size() != messageSize)
        {
          return false;
        }

        return true;
      }

      SpotMessageEphemeralEncryptResponse::~SpotMessageEphemeralEncryptResponse()
      {
      }
    }
  }
}

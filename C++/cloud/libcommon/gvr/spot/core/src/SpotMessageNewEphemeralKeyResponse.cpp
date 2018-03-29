/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageNewEphemeralKeyResponse.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageNewEphemeralKeyResponse.h>
#include <gvr/communication/spot/SpotMessageNewEphemeralKey.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageNewEphemeralKeyResponse::SpotMessageNewEphemeralKeyResponse(void):
        m_ackCode(CErrorCodeNoError),
        m_specificError(0x00)
      {
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getSpotAppId (void) const
      {
        return CMessageResponsePinpadAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getSsk (void) const
      {
        return CDefaulNoEncryptionValue;
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getCommand(void) const
      {
        return CPinpadExtendedMessageCmdId;
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getSubCommandOne() const
      {
        return CEphemeralKeySubCmdId;
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getSubCommandTwo() const
      {
        return CNewEphemeralKeySubSubCmdId;
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getAckCode( void ) const
      {
        return m_ackCode;
      }

      unsigned char SpotMessageNewEphemeralKeyResponse::getSpecificError( void ) const
      {
        return m_specificError;
      }

      SpotMessageNewEphemeralKeyResponse::KeyType SpotMessageNewEphemeralKeyResponse::getKey (void) const
      {
        return (getSpotAppId() << 24) + (getCommand() << 16) + (getSubCommandOne() << 8) + (getSubCommandTwo());
      }

      SpotMessageNewEphemeralKeyResponse::SizeType SpotMessageNewEphemeralKeyResponse::size (void) const
      {
        return SpotMessageNewEphemeralKeyResponse::CMessageSize;
      }

      SpotMessageNewEphemeralKeyResponse::BufferType SpotMessageNewEphemeralKeyResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((size() & 0xFF00) >> 8);
        buff.push_back((size() & 0xFF));
        buff.push_back(getSpotAppId());
        buff.push_back(getSsk());
        buff.push_back(getCommand());
        buff.push_back(getSubCommandOne());
        buff.push_back(getSubCommandTwo());
        buff.push_back(getAckCode());

        return buff;
      }

      bool SpotMessageNewEphemeralKeyResponse::parse(BufferType const & buff)
      {
        if(buff.size() < SpotMessageNewEphemeralKeyResponse::CMessageSize)
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];

        if(buff.size() < datasize)
        {
          return false;
        }

        if( buff[position+2] != getSpotAppId() )
        {
          return false;
        }

        if( buff[position+3] != getSsk() )
        {
          return false;
        }

        if( buff[position+4] != getCommand() )
        {
          return false;
        }

        if( buff[position+5] != getSubCommandOne() )
        {
          return false;
        }

        if( buff[position+6] != getSubCommandTwo())
        {
          return false;
        }

        m_ackCode = buff[position+7];

        if( m_ackCode != 0x00 )
        {
          m_specificError = buff[position+8];
        }

        return true;
      }

      SpotMessageNewEphemeralKeyResponse::~SpotMessageNewEphemeralKeyResponse()
      {
      }
    }
  }
}

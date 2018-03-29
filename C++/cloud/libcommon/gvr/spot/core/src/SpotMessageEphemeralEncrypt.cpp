/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageEphemeralEncrypt.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

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
      SpotMessageEphemeralEncrypt::SpotMessageEphemeralEncrypt(bool decrypt):
        m_decrypt(decrypt),
        m_data()
      {
      }

      unsigned char SpotMessageEphemeralEncrypt::getSpotAppId (void) const
      {
        return CMessageRequestPinpadAppId; //SPOT Applications Id
      }

      unsigned char SpotMessageEphemeralEncrypt::getSsk (void) const
      {
        return CDefaulNoEncryptionValue; //No encryption value
      }

      unsigned char SpotMessageEphemeralEncrypt::getCommand(void) const
      {
        return CPinpadExtendedMessageCmdId;
      }

      unsigned char SpotMessageEphemeralEncrypt::getSubCommandOne() const
      {
        return CEphemeralKeySubCmdId;
      }

      unsigned char SpotMessageEphemeralEncrypt::getSubCommandTwo() const
      {
        return  m_decrypt ? CEphemeralDecryptSubCmdId : CEphemeralEncryptSubCmdId;
      }

      SpotMessageEphemeralEncrypt::KeyType SpotMessageEphemeralEncrypt::getKey (void) const
      {
        return (SpotMessageEphemeralEncrypt::CMessageRequestPinpadAppId << 24)
          +  (SpotMessageEphemeralEncrypt::CPinpadExtendedMessageCmdId << 16)
          + (SpotMessageEphemeralEncrypt::CEphemeralKeySubCmdId << 8)
          + SpotMessageEphemeralEncrypt::CEphemeralEncryptSubCmdId;

      }

      const SpotMessageEphemeralEncrypt::BufferType& SpotMessageEphemeralEncrypt::getData( void ) const
      {
        return m_data;
      }

      void SpotMessageEphemeralEncrypt::setData( const BufferType& data )
      {
        m_data.clear();
        m_data.reserve(data.size());
        std::copy( data.begin(), data.end(), std::back_inserter(m_data) );
      }

      SpotMessageEphemeralEncrypt::SizeType SpotMessageEphemeralEncrypt::size (void) const
      {
        return SpotMessageEphemeralEncrypt::CMinimumMessageSize + m_data.size() + 2;
      }

      SpotMessageEphemeralEncrypt::BufferType SpotMessageEphemeralEncrypt::serialize (void) const
      {
        BufferType buff;
        buff.push_back((size() & 0xFF00) >> 8);
        buff.push_back((size() & 0xFF));
        buff.push_back(getSpotAppId());
        buff.push_back(getSsk());
        buff.push_back(getCommand());
        buff.push_back(getSubCommandOne());
        buff.push_back(getSubCommandTwo());
        buff.push_back((m_data.size()& 0xFF00) >> 8);
        buff.push_back((m_data.size()& 0xFF));
        std::copy( m_data.begin(), m_data.end(), std::back_inserter(buff) );

        return buff;
      }

      bool SpotMessageEphemeralEncrypt::parse(const BufferType & buff)
      {
        // If the size of message buffer received is less than the minimum SpotMessage size we cannot identify the message well.
        if(buff.size() < SpotMessageEphemeralEncrypt::CMinimumMessageSize )
        {
          return false;
        }

        int position(0);
        int messageSize = (((std::size_t)buff[position+0])<<8) | buff[position+1];
        int buffSize = buff.size();

        if(buffSize < messageSize)
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

        position = 9;
        m_data.clear();
        std::copy( (buff.begin()+position), buff.end(), std::back_inserter(m_data) );

        if(size() != messageSize)
        {
          return false;
        }

        return true;
      }

      SpotMessageEphemeralEncrypt::~SpotMessageEphemeralEncrypt()
      {
      }
    }
  }
}

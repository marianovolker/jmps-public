/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageLoginResponse.cpp
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageLoginResponse.h>

#include <gvr/communication/spot/Utils.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      SpotMessageLoginResponse::SpotMessageLoginResponse(void)
      {
        this->ackCode = CErrorCodeNoError;
      }

      SpotMessageLoginResponse::SpotMessageLoginResponse(const unsigned char ackcode)
      {
        this->ackCode = ackcode;
      }

      unsigned char SpotMessageLoginResponse::getSpotAppId (void) const
      {
        return CMessageResponseSpotAppId;
      }

      unsigned char SpotMessageLoginResponse::getSsk (void) const
      {
        return CDefaulNoEncryptionValue;
      }

      unsigned char SpotMessageLoginResponse::getCommand (void) const
      {
        return CSpotMessageId; //login message id
      }

      unsigned char SpotMessageLoginResponse::getAckCode( void ) const
      {
        return this->ackCode;
      }

      void SpotMessageLoginResponse::setAckCode( const unsigned char ackcode )
      {
        this->ackCode = ackcode;
      }

      SpotMessageLoginResponse::BufferType SpotMessageLoginResponse::getDateTime() const
      {
        return this->dateTime;
      }

      void SpotMessageLoginResponse::setDateTime(const BufferType& datetime)
      {
        this->dateTime = datetime;
      }

      std::vector<SpotMessageLoginResponse::AppEntry> SpotMessageLoginResponse::getAppEntries() const
      {
        return this->entries;
      }

      void SpotMessageLoginResponse::setAppEntries(const std::vector<AppEntry> entries)
      {
        this->entries = entries;
      }

      SpotMessageLoginResponse::BufferType SpotMessageLoginResponse::getRSAKey(void) const
      {
        return this->rsaKey;
      }

      void SpotMessageLoginResponse::setRSAKey(const BufferType& key)
      {
        this->rsaKey = key;
      }

      SpotMessageLoginResponse::KeyType SpotMessageLoginResponse::getKey (void) const
      {
        return (getSpotAppId() << 16) + (getSsk() << 8) + getCommand();
      }

      SpotMessageLoginResponse::SizeType SpotMessageLoginResponse::size (void) const
      {
        return entries.size()== 0 ? CMessageCommonSize : CMessageOkCommonSize + (AppEntry::CEntrySize * entries.size()) +1 + rsaKey.size();
      }

      SpotMessageLoginResponse::BufferType SpotMessageLoginResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);//size
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getAckCode());

        if( this->getAckCode() == CErrorCodeNoError )
        {
          for(int i=0; i< CDefaultDateTimeSize; ++i)
          {
            buff.push_back(this->dateTime[i]);
          }
          buff.push_back(entries.size());

          for(int i=0; i< entries.size(); ++i)
          {
            buff.push_back(this->entries[i].getAppId());

            for (int j=0; j < this->entries[i].getSign().size() && (j < CDefaultAppSignSize); ++j)
            {
              buff.push_back(this->entries[i].getSign()[j]);
            }

            int j=0;
            for (; j < this->entries[i].getAppAscii().size() && (j < CDefaultAppDescriptionSize); ++j)
            {
              buff.push_back(this->entries[i].getAppAscii()[j]);
            }
            for (; j < CDefaultAppDescriptionSize; ++j)
            {
              buff.push_back(' ');
            }
          }

          for(int i=0; i< this->rsaKey.size(); ++i)
          {
            buff.push_back(this->rsaKey[i]);
          }
        }

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageLoginResponse Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }


      bool SpotMessageLoginResponse::parse(BufferType const & buff)
      {
        if( buff.size() < this->CMessageCommonSize )
        {
          return false;
        }

        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(buff.size() < datasize)
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

        this->ackCode = buff[position++];

        if(ackCode == CErrorCodeNoError)
        {
          this->dateTime = BufferType(0);
          for(int i=0; i< CDefaultDateTimeSize; ++i)
          {
            this->dateTime.push_back(buff[position++]);
          }

          int numApps = buff[position++];
          for(int i=0; i< numApps; ++i)
          {
            AppEntry entry = AppEntry();
            entry.setAppId(buff[position++]);

            BufferType signBuff(0);
            signBuff.push_back(buff[position++]);
            signBuff.push_back(buff[position++]);
            signBuff.push_back(buff[position++]);
            signBuff.push_back(buff[position++]);
            entry.setSign(signBuff);

            std::string description;
            description.resize(CDefaultAppDescriptionSize);
            for (int j = 0; j < CDefaultAppDescriptionSize; ++j)
            {
              description[j] = buff[position++];
            }
            entry.setAppAscii(description);
            entries.push_back(entry);
          }

          rsaKey = BufferType(0);
          for(; position< buff.size();)
          {
            this->rsaKey.push_back(buff[position++]);
          }
        }
        else
        {
          entries.clear();
        }

        if(this->size() > buff.size() ) //calculates parsed size.
        {
          return false;
        }

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageLoginResponse Parsed Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return true;
      }

      SpotMessageLoginResponse::~SpotMessageLoginResponse()
      {
      }

    }
  }
}

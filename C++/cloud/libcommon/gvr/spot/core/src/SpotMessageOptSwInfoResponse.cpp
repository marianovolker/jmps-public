/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOptSwInfoResponse.cpp
 * @author mgrieco
 * @date Monday, May 11, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotMessageOptSwInfoResponse.h>

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
      const char * SpotMessageOptSwInfoResponse::CpcOptSwInfo = "OPT software info";

      const SpotMessageOptSwInfoResponse::ItemIDLabel  SpotMessageOptSwInfoResponse::aCItemLabels[SpotMessageOptSwInfoResponse::CMaxItems] =
      {
        { ePR_FWR_VERSION,      "Printer"},
        { eIO_FWR_VERSION,      "IO module"},
        { eCL_FWR_VERSION,      "Contactless"},
        { eCL_PROTOCOL_VERSION, "Contactless protocol"},
        { eTRIND_FWR_VERSION,   "Trind module firmware version"}
      };



      SpotMessageOptSwInfoResponse::SpotMessageOptSwInfoResponse(void)
      {
        this->ackCode = CErrorCodeNoError;
      }

      SpotMessageOptSwInfoResponse::SpotMessageOptSwInfoResponse(const unsigned char ackcode)
      {
        this->ackCode = ackcode;
      }

      unsigned char SpotMessageOptSwInfoResponse::getSpotAppId (void) const
      {
        return CMessageResponseOPTAppId;
      }

      unsigned char SpotMessageOptSwInfoResponse::getSsk (void) const
      {
        return CDefaulNoEncryptionValue;
      }

      unsigned char SpotMessageOptSwInfoResponse::getCommand (void) const
      {
        return COptSwInfoMessageResponseId;
      }

      unsigned char SpotMessageOptSwInfoResponse::getSubCommand (void) const
      {
        return COptSwInfoMessageResponseSubCmd;
      }

      unsigned char SpotMessageOptSwInfoResponse::getAckCode( void ) const
      {
        return this->ackCode;
      }

      void SpotMessageOptSwInfoResponse::setAckCode( const unsigned char ackcode )
      {
        this->ackCode = ackcode;
      }

      std::vector<SpotMessageOptSwInfoResponse::AppEntry> SpotMessageOptSwInfoResponse::getAppEntries() const
      {
        return this->entries;
      }

      void SpotMessageOptSwInfoResponse::setAppEntries(const std::vector<AppEntry> entries)
      {
        this->entries = entries;
      }

      SpotMessageOptSwInfoResponse::KeyType SpotMessageOptSwInfoResponse::getKey (void) const
      {
        return (this->getCommand() << 8) + (this->getSubCommand());
      }


      void SpotMessageOptSwInfoResponse::getAllOptSoftwareDescription( std::string & sLabel, OptSoftwareInfoMap & map ) const
      {
        bool blret = true;

        std::string sItemLabel, sField;

        sLabel = CpcOptSwInfo;
        map.clear();

        for( int i=0; i< entries.size(); i++ )
        {
          bool blresult = true;

          blresult &= getSubItemLabel(entries[i].getTagId(), sItemLabel);
          blresult &= getSubItemDescription(entries[i].getTagId(), sField);

          if( blresult )
          {
            map.insert(make_pair(sItemLabel, sField));
          }

          blret &= blresult;
        }
      }

      SpotMessageOptSwInfoResponse::SizeType SpotMessageOptSwInfoResponse::size (void) const
      {
        int entriesSize = 0;

        for(int i=0; i< entries.size(); i++)
        {
          entriesSize += (2 + this->entries[i].getTagData().size());
        }
        return getAckCode() != 0x00 ? CMessageCommonSize : CMessageCommonSize + 1 + entriesSize;
      }

      SpotMessageOptSwInfoResponse::BufferType SpotMessageOptSwInfoResponse::serialize (void) const
      {
        BufferType buff = BufferType(0);
        buff.push_back((this->size() & 0xFF00) >> 8);//size
        buff.push_back((this->size() & 0xFF));
        buff.push_back(this->getSpotAppId());
        buff.push_back(this->getSsk());
        buff.push_back(this->getCommand());
        buff.push_back(this->getSubCommand());
        buff.push_back(this->getAckCode());

        if( this->getAckCode() == CErrorCodeNoError )
        {
          buff.push_back(entries.size());

          for(int i=0; i< entries.size(); i++)
          {
            buff.push_back(this->entries[i].getTagId());
            buff.push_back(this->entries[i].getTagLen());

            for (int j=0; j < this->entries[i].getTagData().size(); ++j)
            {
              buff.push_back(this->entries[i].getTagData()[j]);
            }
          }
        }

        char pcBuffer[(buff.size()*3)+2];
        std::cout << "SpotMessageOptSwInfoResponse Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;

        return buff;
      }


      bool SpotMessageOptSwInfoResponse::parse(BufferType const & buff)
      {
        if( buff.size() < CMessageCommonSize )
        {
          return false;
        }

        {
          char pcBuffer[(buff.size()*3)+2];
          std::cout << "SpotMessageOptSwInfoResponse Parsed Message Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), buff.size()) << std::endl;
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

        if(buff[position++] != this->getSubCommand())
        {
          return false;
        }

        this->ackCode = buff[position++];

        if(ackCode == CErrorCodeNoError)
        {
          int numApps = buff[position++];
          for(int i=0; i<numApps; i++)
          {
            AppEntry entry = AppEntry();
            entry.setTagId(buff[position++]);
            entry.setTagLen(buff[position++]);

            BufferType data(entry.getTagLen());
            for (int j = 0; j < entry.getTagLen(); j++)
            {
              data[j] = buff[position++];
            }
            entry.setTagData(data);

            entries.push_back(entry);
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

        return true;
      }

      SpotMessageOptSwInfoResponse::~SpotMessageOptSwInfoResponse()
      {
      }

      bool SpotMessageOptSwInfoResponse::getSubItemDescription(int iTagID, std::string & sOut) const
      {
        for( int i=0; i< entries.size(); i++ )
        {
          if(iTagID == entries[i].getTagId())
          {
            std::copy(entries[i].getTagData().begin(), entries[i].getTagData().end(), back_inserter(sOut));
            return true;
          }
        }

        sOut.clear();
        return false;
      }

      bool SpotMessageOptSwInfoResponse::getSubItemLabel(int iTagID, std::string & sOut) const
      {
        for(int i = 0; i < CMaxItems; i++)
        {
          if( iTagID == aCItemLabels[i].itemID )
          {
            sOut = aCItemLabels[i].itemLabel;
            return true;
          }
        }

        sOut.clear();
        return false;
      }

    }
  }
}

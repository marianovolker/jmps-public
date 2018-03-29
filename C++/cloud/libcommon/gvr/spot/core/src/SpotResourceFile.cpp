/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotResourceFile.cpp
 * @author mgrieco
 * @date Tuesday, March 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotResourceFile.h>
#include <gvr/communication/spot/Conversion.h>
#include <Poco/Path.h>

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

      const std::string SpotResourceFile::CResMarker = "/resMark";
      const std::string SpotResourceFile::CResourceNameIdIdentifier = "Id";

      SpotResourceFile::SpotResourceFile (const std::string & sFilename, const std::size_t uInitialOffset /* = 0 */) : AbsSpotPackage(sFilename, uInitialOffset)
      {
        resType = 0x00;
        resId = 0x00;
        authType = 0x00;

        setResFileId(sFilename);
        setResAuthenticationType(sFilename);
        setDescription(sFilename);
      }

      SpotResourceFile::~SpotResourceFile (void)
      {
      }

      std::size_t  SpotResourceFile::setCertificateSize (void)
      {
        return certificateSize;
      }

      std::size_t  SpotResourceFile::setSignatureSize (void)
      {
        return 0;
      }

      unsigned char SpotResourceFile::getAuthType() const
      {
        return authType;
      }

      unsigned char SpotResourceFile::getResId() const
      {
        return resId;
      }

      unsigned char SpotResourceFile::getResType() const
      {
        return resType;
      }

      void SpotResourceFile::setResFileId(std::string const & sFilename)
      {
        int i0 = sFilename.find(CResourceNameIdIdentifier);// search for "id"
        if(i0 < 0)
        {
          i0 =0;
          std::cout <<  "setFileId - Invalid type id - " + sFilename << std::endl;
        }

        int start = i0 + CResourceNameIdPosition;
        int end = start + CResourceNameIdLen;

        try
        {
          unsigned long int id = strtoul( sFilename.substr( start, end ).c_str(), 0, 10 );
          this->resId = id;
        }
        catch(...)
        {
          std::cout << "BAD resource id on filename." << std::endl;
        }
      }

      void SpotResourceFile::setResAuthenticationType(std::string const & sFilePath)
      {
        int i0 = 0, i1 = 0; // some indexes
        std::string sAuthenType;
        std::string folderSeparator = std::string(1, Poco::Path::separator());
        std::string sFilename = sFilePath.substr(sFilePath.find_last_of(folderSeparator)+1);

        // search for '_'
        i0 = sFilename.find('_');
        if(i0 <0)
        {
          this->authType = CAuthenticationNone;
          return;
        }

        // search for '_' again
        i1 = sFilename.find('_', 1 + i0);
        if(i1 < 0)
        {
          this->authType = CAuthenticationNone;
          return;
        }

        try
        {
          unsigned long int type = strtoul(sFilename.substr( i0 + 1, i1-1 ).c_str() , 0, 10);
          this->authType = type;
        }
        catch(...)
        {
          if (this->platformType == SpotSession::eM3 || this->platformType == SpotSession::eM5)
          {
            std::cout << "BAD authentication type." <<  sFilename.substr( i0 + 1, i1-1 ) <<std::endl;
          }
        }
      }

      ISpotMessage::BufferType SpotResourceFile::getCrc() const
      {
        return this->crcData;
      }

      ISpotMessage::BufferType SpotResourceFile::getDescription() const
      {
        return this->description;
      }

      ISpotMessage::BufferType SpotResourceFile::getMacData() const
      {
        if(this->authType == this->CAuthenticationNone)
        {
          ISpotMessage::BufferType nonedata;
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          return nonedata;
        }
        return macData;
      }

      ISpotMessage::BufferType SpotResourceFile::getRsaData() const
      {
        if(this->authType == this->CAuthenticationNone)
        {
          ISpotMessage::BufferType nonedata;
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          nonedata.push_back(0x00);
          return nonedata;
        }
        return rsaData;
      }


      void SpotResourceFile::setDescription(std::string const & filename)
      {
        description.clear();

        std::string folderSeparator = std::string(1, Poco::Path::separator());
        int ind = filename.find_last_of(folderSeparator)+1;

        for (int i = 0; i < this->CResourceDescriptionMaxLen && ind+i < filename.size(); i++)
        {
          this->description.push_back(filename.substr(ind+i)[0]);
        }

        for (int i = 0; i < CResourceDescriptionMaxLen - description.size() ; i++)
        {
          this->description.push_back(0x00);
        }
      }

      void SpotResourceFile::setPlatformMode(const SpotSession::EDevicePlatform platform)
      {
        this->platformType = platform;

        if (this->platformType == SpotSession::eM3)
        {
          this->certificateSize = this->CM3CertificateSize;
        }
        else if(this->platformType == SpotSession::eM7)
        {
          this->certificateSize = this->CM7CertificateSize;
        }
        else
        {
          this->certificateSize = this->CM5CertificateSize;//default m5
        }
      }

      void SpotResourceFile::initializeResource( )
      {
        if(hasErrors())
        {
          return;
        }

        bool present = true;
        ISpotMessage::BufferType certifData;
        certifData.insert(certifData.end(), this->getCertificateData(), this->getCertificateData() + this->getCertificateSize());

        for (int i = 0; i < CResMarker.size(); i++)
        {
          if( certifData.size() <= CResMarker.size() || CResMarker[i] != certifData[i])
          {
            std::cout << "Missing resmarker on resource " << std::endl;
            m_blError = true;
            return ;
          }
        }

        for (int i = 0; i < this->CResourceCrcLen && CResourceCrcCertifPosition+i < certifData.size(); i++)
        {
          this->crcData.push_back(certifData[this->CResourceCrcCertifPosition+i]);
        }

        if(authType!=SpotResourceFile::CAuthenticationNone && this->platformType == SpotSession::eM3)
        {
          macData.clear();
          for (int i = 0; i < this->CMacLenStandardSize && CResourceMacCertifPosition+i < certifData.size(); i++)
          {
            this->macData.push_back(certifData[this->CResourceMacCertifPosition+i]);
          }
        }
        else if(authType!=SpotResourceFile::CAuthenticationNone &&  this->platformType == SpotSession::eM5)
        {
          rsaData.clear();
          for (int i = 0; i < this->CCertStandardSize && CResourceRsaCertifPosition+i < certifData.size(); i++)
          {
            this->rsaData.push_back(certifData[this->CResourceRsaCertifPosition+i]);
          }
        }
        else
        {
          authType = SpotResourceFile::CAuthenticationNone;
          macData.clear();
          for (int i = 0; i < this->CMacLenStandardSize; i++)
          {
            this->macData.push_back(0x00);
          }
          rsaData.clear();
          for (int i = 0; i < this->CCertStandardSize; i++)
          {
            this->rsaData.push_back(0x00);
          }
        }
      }
    }
  }
}

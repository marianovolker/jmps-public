/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotResourceFile.h
 * @author mgrieco
 * @date Tuesday, March 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef SPOTRESOURCEFILE_H_
#define SPOTRESOURCEFILE_H_

#include <gvr/communication/spot/ISpotMessage.h>
#include <gvr/communication/spot/AbsSpotPackage.h>
#include <gvr/communication/spot/SpotSession.h>
#include <iostream>
#include <stdlib.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      /**
       * @class SpotResourceFile
       * @brief This class implements the functionality for resources files that can be downloaded to any SPOT device.
       */
      class SpotResourceFile : public AbsSpotPackage
      {
        public:

        static const std::size_t CCommonCertificateSize = 30;
        static const std::size_t CMacLenStandardSize = 4;
        static const std::size_t CCertStandardSize = 248;

        static const std::size_t  CM3CertificateSize = CCommonCertificateSize + CMacLenStandardSize;///< Constant certificate length for M3 certificates.
        static const std::size_t  CM5CertificateSize = CCommonCertificateSize + CCertStandardSize;///< Constant certificate length for M5 certificates.
        static const std::size_t  CM7CertificateSize = CCommonCertificateSize + CCertStandardSize;///< Constant certificate length for M7 certificates.

        /**Authentication types*/
        static const unsigned char CAuthenticationNone  = 0x00;
        static const std::string CResMarker;

        /** Default value for resource name id position after the id identifier */
        static const int CResourceNameIdPosition       = 2;
        /** Default value for resource name id length */
        static const int CResourceNameIdLen             = 3;
        /** Default value for resource name possition on local file name */
        static const int CResourceNamePosition          = 6;
        /** Default value for resource name id identifier */
        static const std::string CResourceNameIdIdentifier;

        /** Default value for resource name max len */
        static const int CResourceCrcLen            = 4;

        /** Default value for resource name possition on local file name */
        static const int CResourceIdCertifPosition          = 8;
        static const int CResourceTypeCertifPosition          = 9;
        static const int CResourceCrcCertifPosition          = 10;
        static const int CResourceMacCertifPosition          = 14;
        static const int CResourceRsaCertifPosition          = 14;
        /** Default value for resource name max len */
        static const int CResourceDescriptionMaxLen            = 16;

        /**
        * @brief Constructor
        * @param @b sFilename is the package filename.
        * @param @b uInitialOffset is the initial file offset only used in resume downloads.
        */
        SpotResourceFile (const std::string & sFilename, const std::size_t uInitialOffset = 0);

         /**
         * @brief Destructor
         */
        virtual ~SpotResourceFile (void);

        /** @brief authType getter*/
        unsigned char getAuthType() const;

        /** @brief resId getter*/
        unsigned char getResId() const;

        /** @brief resType getter*/
        unsigned char getResType() const;

        /** @brief crc getter*/
        ISpotMessage::BufferType getCrc() const;

        /** @brief description getter*/
        ISpotMessage::BufferType getDescription() const;

        /** @brief platform mode setter
         * @param the platform*/
        void setPlatformMode(const SpotSession::EDevicePlatform platform);

        /** @brief load the SpotResourcesFile field from the resource file*/
        void initializeResource();

        /** @brief resType macData*/
        ISpotMessage::BufferType getMacData() const;
        /** @brief resType rsaData*/
        ISpotMessage::BufferType getRsaData() const;
        /** @brief certifType getter*/
        unsigned char getCertifType() const;

        private:
        /**
        * @brief  Called during the initialization, to set the certificate field length.
        * @note   Derived classes MUST supply this information overriding this method.
        * @return @b The certificate field length, set by this class.
        * @see AbsSpotPackage::initialize()
        */
        virtual std::size_t  setCertificateSize (void);

        /**
        * @brief  Called during the initialization, to set the signature field length.
        * @note   Derived classes MUST supply this information overriding this method.
        * @return @b The signature field length, set by this class.
        * @see AbsSpotPackage::initialize()
        */
        virtual std::size_t  setSignatureSize   (void);

        /**
         * Sets the current file id from a file name
         * @param sFilename, the filename to get the file id.
         * The file name should have the substring IDXXX where XXX is the file id.
         * @throws BadResourceFilenameException
         **/
        void setResFileId(std::string const & sFilename);

        /** Sets the current file authentication type from a given full file name
         * @param sFilename, the file full name
         */
        void setResAuthenticationType(std::string const & sFilename);

        /** Sets the current file description from a given file name
         * @param sFilename, the file name
         */
        void setDescription(std::string const & desc);

        std::size_t certificateSize;
        SpotSession::EDevicePlatform platformType;

        unsigned char resType;
        unsigned char resId;
        unsigned char authType;

        ISpotMessage::BufferType crcData;
        ISpotMessage::BufferType macData;
        ISpotMessage::BufferType rsaData;
        ISpotMessage::BufferType description;

      };
    }
  }
}
#endif

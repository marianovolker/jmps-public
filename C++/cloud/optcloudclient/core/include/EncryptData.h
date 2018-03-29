/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EncryptData.h
 * @author mvolker
 * @date Tuesday, May 12, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ENCRYPTDATA_H_
# define _ENCRYPTDATA_H_

# include <string>
# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>

# include <gvr/communication/spot/SpotSession.h>

using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      /**
       * @brief This class is for encrypt data using Spot Ephemeral keys.
       * @since 0.0.0, 2015-05-12
       * @version 0.0.0, 2015-05-12
       * @author mvolker
       */
      class EncryptData
      {
        public:

          /**
           * @brief default destructor.
           */
          virtual ~EncryptData (void);

          /**
           * @brief Constructor.
           */
          EncryptData( SpotSession & );

          /**
           * @brief Generate Spot ephemeral key.
           *
           * @return true if ok.
           */
          bool generateKey( );

          /**
           * @brief Encrypt data using Spot ephemeral keys.
           * @param string is the input of this method, the data in clear.
           * @param buffer is where the encrypted data will be put.

           * @return true if ok.
           */
          bool encrypt( const std::string&, std::vector<unsigned char>& );

          /**
           * @brief Decrypt data using Spot ephemeral keys.
           * @param buffer is where the encrypted data will be passed to the method.
           * @param string is the output of this method, the decrypted data.
           *
           * @return true if ok.
           */
           bool decrypt( const std::vector<unsigned char>&, std::string& );

           /**
            * @brief Decrypts the given connectuib data.
            * @param pathCloudServerDataFile, the file where the encrypted file is
            * @param strDecryptedData, the result data
            */
           bool decryptConnectionData(const std::string& pathCloudServerDataFile, std::string& strDecryptedData);

        protected:

           SpotSession & m_spotSession;

           /**
            * @brief Send a message with a buffer to decrypt/encrypt to SPOT.
            * @param buffer with encrypted/decrypted data, that is the input of this method.
            * @param buffer with encrypted/decrypted data, that is the output of this method.
            * @param decrypt flag is to indicate what type of message is need to be used.
            *
            * @return true if ok.
            */
           bool send( const SpotMessageEphemeralEncryptResponse::BufferType&,
                      SpotMessageEphemeralEncryptResponse::BufferType&, bool decrypt = false );

           static const int CMaxSizeOfPacketToSendToSPOT;

           typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
           Poco::FastMutex m_mtx; /** @brief Current instance synchronization mutex. */
      };
    }
  }

#endif

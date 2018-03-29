/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EncryptData.cpp
 * @author mvolker
 * @date Tuesday, May 12, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <EncryptData.h>

#include <gvr/communication/spot/SpotMessageNewEphemeralKey.h>
#include <gvr/communication/spot/SpotMessageNewEphemeralKeyResponse.h>
#include <gvr/communication/spot/SpotMessageEphemeralEncrypt.h>
#include <gvr/communication/spot/SpotMessageEphemeralEncryptResponse.h>

# include <gvrlog/log.h>
#include <iostream>
#include <fstream>

using namespace gvr::communication::spot;
using namespace gvr::cloud;

const int EncryptData::CMaxSizeOfPacketToSendToSPOT = 1024;

DECLARE_LOG_MODULE(core);

/**
 * @brief Is needed to padding the original string according to the encryption algorithm.
 */
#define paddTo16(str) { while( (str.size()%16)!=0 ) str.push_back(0x00); }



/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Cloud Domain
  namespace cloud
  {
      /* Public Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      EncryptData::EncryptData(SpotSession & spotSession):
          m_spotSession(spotSession),
          m_mtx()
      {
      }

      EncryptData::~EncryptData (void)
      {
      }

      bool EncryptData::generateKey( )
      {
        ScopedLock lck(m_mtx);
        SpotMessageNewEphemeralKey msgSpotReq;
        SpotMessageNewEphemeralKeyResponse msgSpotRes;
        m_spotSession.sendSync(msgSpotReq, msgSpotRes);
        if( m_spotSession.getError() != SpotSession::SpotSessionError::CNoError )
        {
          LOG_VCritical(core, "EncryptDataSetNewKey SpoSession Error: [%s], sending a message.", m_spotSession.getErrorDescription().c_str());
          return false;
        }

        if( !msgSpotRes.valid() )
        {
          LOG_VCritical(core, "EncryptDataSetNewKey message response has an error: [0x%x].", msgSpotRes.getSpecificError());
          return false;
        }

        return true;
      }

      bool EncryptData::encrypt( const std::string& dataInClear, std::vector<unsigned char>& encryptedData )
      {
        ScopedLock lck(m_mtx);
        SpotMessageEphemeralEncryptResponse::SizeType offset = 0;
        SpotMessageEphemeralEncryptResponse::BufferType data;

        std::string dataToEncrypt(dataInClear);

        paddTo16(dataToEncrypt);

        for( ; (offset < dataToEncrypt.size()); offset += CMaxSizeOfPacketToSendToSPOT )
        {
          std::string strChunk( dataToEncrypt.substr(offset, CMaxSizeOfPacketToSendToSPOT) );

          SpotMessageEphemeralEncryptResponse::BufferType chunkToEncrypt(strChunk.begin(), strChunk.end());

          if( send(chunkToEncrypt, data) )
          {
            SpotMessageEphemeralEncryptResponse::SizeType newSize = encryptedData.size() + data.size();
            encryptedData.reserve(newSize);
            encryptedData.insert(encryptedData.end(), data.begin(), data.end());
          }
          else
          {
            return false;
          }
        }

        return true;
      }

      bool EncryptData::decrypt( const std::vector<unsigned char>& encryptedData, std::string& dataInClear )
      {
        ScopedLock lck(m_mtx);
        SpotMessageEphemeralEncryptResponse::SizeType offset = 0;
        SpotMessageEphemeralEncryptResponse::BufferType data;

        for( ; (offset < encryptedData.size()); offset += CMaxSizeOfPacketToSendToSPOT )
        {
          SpotMessageEphemeralEncryptResponse::BufferType chunkToDecrypt(CMaxSizeOfPacketToSendToSPOT);
          std::copy ( (encryptedData.begin()+offset), (encryptedData.begin()+offset+CMaxSizeOfPacketToSendToSPOT), chunkToDecrypt.begin() );

          if( send(chunkToDecrypt, data, true) )
          {
            std::string strChunkOfDecryptedData(data.begin(), data.end());
            dataInClear.append(strChunkOfDecryptedData.c_str());
          }
          else
          {
            return false;
          }
        }

        return true;
      }

      bool EncryptData::send( const SpotMessageEphemeralEncryptResponse::BufferType& dataIn, SpotMessageEphemeralEncryptResponse::BufferType& dataOut, bool decrypt )
      {
        SpotMessageEphemeralEncrypt msgSpotReq(decrypt);

        SpotMessageEphemeralEncryptResponse::BufferType dataToSend(dataIn.begin(), dataIn.end());
        msgSpotReq.setData(dataToSend);

        SpotMessageEphemeralEncryptResponse msgSpotRes(decrypt);
        m_spotSession.sendSync(msgSpotReq, msgSpotRes);
        if( m_spotSession.getError() != SpotSession::SpotSessionError::CNoError )
        {
          LOG_VCritical(core, "EncryptData SpoSession Error: [%s], sending a message.", m_spotSession.getErrorDescription().c_str());
          return false;
        }

        if( !msgSpotRes.valid() )
        {
          LOG_VCritical(core, "EncryptData message response has an error: [0x%x].", msgSpotRes.getSpecificError());
          return false;
        }

        dataOut.clear();
        dataOut = msgSpotRes.getData();

        return true;
      }

      bool EncryptData::decryptConnectionData(const std::string& pathCloudServerDataFile, std::string& strDecryptedData)
      {
        std::ifstream binaryFile(pathCloudServerDataFile.c_str(), std::ifstream::binary);
        if (binaryFile.is_open())
        {
          std::vector<unsigned char> encryptedBuffer((std::istreambuf_iterator<char>(binaryFile)), (std::istreambuf_iterator<char>()));

          if (m_spotSession.getDevicePlatform() != gvr::communication::spot::SpotSession::eM3)
          {
            if (!decrypt(encryptedBuffer, strDecryptedData))
            {
              LOG_Error(core, "Fail to decrypt data.");
              strDecryptedData.clear();
            }
          }
          else
          {
            std::copy(encryptedBuffer.begin(), encryptedBuffer.end(), back_inserter(strDecryptedData));
          }
        }
        else
        {
          LOG_VError(core, "Could not open binary file [%s]\n", pathCloudServerDataFile.c_str());
        }
        return strDecryptedData.empty() == false;
      }

      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Public Interface ********/

  }
}

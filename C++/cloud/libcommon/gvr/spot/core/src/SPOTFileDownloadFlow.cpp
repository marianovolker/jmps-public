/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SPOTFileDownloadFlow.cpp
 * @author Guillermo Paris
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/UnsignedBinaryFile.h>
#include <gvr/communication/spot/M3M5SpotPackage.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransfer.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransferResponse.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotMessagePackageDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackageResponse.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownload.h>
#include <gvr/communication/spot/SpotMessagePackageDownload.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackage.h>
#include <gvr/communication/spot/SpotResourceFile.h>
#include <gvr/communication/spot/SpotMessageFileBlockDownload.h>
#include <gvr/communication/spot/SpotMessageFileDownload.h>
#include <gvr/communication/spot/SPOTFileDownloadFlow.h>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/SpotMessageFileDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageFileDownload.h>
#include <gvr/communication/spot/SpotMessageFileBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageFileBlockDownload.h>
#include <gvr/communication/spot/SpotMessageFileDeleteResponse.h>
#include <gvr/communication/spot/SpotMessageFileDelete.h>
#include <gvr/communication/spot/Utils.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Path.h>
#include <Poco/DirectoryIterator.h>
#include <fstream>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const std::string SPOTFileDownload::CResourcesFoldername = "ResourcesToUpload";
const std::string SPOTFileDownload::CVersionsFoldername = "Versions";
const std::string SPOTFileDownload::CDebiansFoldername = "VersionsDebian";


SPOTFileDownload::SPOTFileDownload (SpotSession & session) : AbsSpotTransferFlow(session)
{
}

SPOTFileDownload::~SPOTFileDownload (void)
{
}


bool SPOTFileDownload::downloadResourceFile(const std::string & sPathToFile, const unsigned char type)
{
  SpotSession::EDevicePlatform  eDevPlatform = m_Session.getDevicePlatform();

  if( eDevPlatform != SpotSession::eM3 && eDevPlatform !=  SpotSession::eM5 && eDevPlatform != SpotSession::eM7)
  {
        m_iLastErrorCode = eError0x100;
        m_blError = true;
        return false; // unknown device
  }

  ISpotMessage::SizeType uBlockLength = SpotMessagePackageBlockDownload::CMaxBlockSize; // 2 Kb block

  SpotResourceFile resource(sPathToFile);
  resource.setPlatformMode(m_Session.getDevicePlatform());
  resource.initialize( uBlockLength );
  resource.initializeResource(  );//load the certificate
  if( resource.hasErrors() )
  {
    m_iLastErrorCode = eError0x200 + resource.getLastOperationError();
    m_blError = true;
    return false; // some kind of error during package object initialization
  }

  //DELETE the resource
  std::cout << "sending a file delete.." << std::endl;
  SpotMessageFileDelete msgDeleteRequest;
  SpotMessageFileDeleteResponse msgDeleteResponse;
  msgDeleteRequest.setFileType(type);
  msgDeleteRequest.setFileId(resource.getResId());
  ErrorType error = m_Session.sendSync(msgDeleteRequest, msgDeleteResponse, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError )
  {
    std::cout << "SPOTFileDownload - Error sending a file delete.." << std::endl;
    m_iLastErrorCode = ( msgDeleteResponse.getAckCode() == 0x00 ? eError0x300 + error : eError0x400  + msgDeleteResponse.getAckCode() );
    m_blError = true;
    return false;
  }

  //DOWNLOAD the resource
  SpotMessageFileDownload msgFileRequest;
  SpotMessageFileDownloadResponse msgFileResponse;
  msgFileRequest.setFileId(resource.getResId());
  msgFileRequest.setFileType(type);
  int size = resource.fileLength() - resource.getCertificateSize();
  msgFileRequest.setFileSize(size);
  msgFileRequest.setCrcData(resource.getCrc());
  msgFileRequest.setAuthenType(resource.getAuthType());

  if(m_Session.getDevicePlatform() ==  SpotSession::eM3)
  {
    msgFileRequest.setSecretAuthenticator(resource.getMacData());
  }
  else
  {
    msgFileRequest.setSecretAuthenticator(resource.getRsaData());
  }
  msgFileRequest.setDescription(sPathToFile.substr(sPathToFile.find_last_of("/")+1));

  std::cout << "sending a file download.." << std::endl;
  error = m_Session.sendSync(msgFileRequest, msgFileResponse, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError || msgFileResponse.getAckCode() != 0x00 )
  {
    std::cout << "SPOTFileDownload - Error sending a file download.." <<  std::endl;
    SpotMessageFileDownload::BufferType buffer = msgFileRequest.serialize();
    m_iLastErrorCode = ( msgFileResponse.getAckCode() == 0x00 ? eError0x300 + error : eError0x400  + msgFileResponse.getAckCode() );
    m_blError = true;
    return false;
  }

  resource.setInitialOffset( msgFileResponse.getStartingOffset() );

  ISpotMessage::SizeType uReadingSize = 0; // should be = uBlockLength except for the last block
  const unsigned char* pBlockData = resource.getNextBlock(uReadingSize);
  if( pBlockData == NULL )
  {
    std::cout << "SPOTFileDownload - Error getting the next block.." << std::endl;
    m_iLastErrorCode = eError0x500 + resource.getLastOperationError();
    m_blError = true;
    return false; // first block must have data !!
  }

  size_t newOffset = msgFileResponse.getStartingOffset();
  ISpotMessage::BufferType blockData;
  while( pBlockData != NULL )
  {
    blockData.clear();
    blockData.insert(blockData.end(), pBlockData, pBlockData + uReadingSize);
    if( ! downloadResourceBlock(newOffset, blockData ))
    {
      std::cout << "SPOTFileDownload - Error downloading a resource block.." << std::endl;
      m_blError = true;
      return false;
    }
    pBlockData = resource.getNextBlock(uReadingSize);
  }
  blockData.clear();
  return true;
}

bool SPOTFileDownload::downloadResourceBlock(ISpotMessage::SizeType & offset, const ISpotMessage::BufferType blockData )
{
  SpotMessageFileBlockDownload          msgBlockRequest;
  SpotMessageFileBlockDownloadResponse  msgBlockResponse;

  msgBlockRequest.setBlockData(blockData);
  msgBlockRequest.setBlockOffset(offset);
  ErrorType  error = m_Session.sendSync(msgBlockRequest, msgBlockResponse, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError || (msgBlockResponse.getAckCode() != 0x00 && msgBlockResponse.getAckCode() != 0x02))
  {
    m_iLastErrorCode = ( eError0x700 ) + error;
    m_blError = true;
    return false;
  }

  offset = msgBlockResponse.getStartingOffset();
  return true;
}

bool SPOTFileDownload::decompressRelease(const std::string & pathToFile, const std::string & pathToDestination)
{
  try
  {
    std::cout << "SPOTFileDownload - uncompressing release.." << std::endl;
    std::ifstream inp(pathToFile.data(), std::ios::binary);
    Poco::Path pathZipfile(pathToFile);

    std::string destFolderName = pathZipfile.getFileName();
    std::string::size_type i = destFolderName.find(CZipExtensionName);
    if (i != std::string::npos)
    {
      destFolderName.erase(i, CZipExtensionName.length());
    }

    if (pathToDestination.empty())
    {
      releaseFolderPath = Poco::Path::current() + destFolderName;
    }
    else
    {
      Poco::Path path(pathToDestination);
      releaseFolderPath = path.append(destFolderName).toString();
    }
    releaseName = destFolderName;

    std::cout << "releaseFolderPath:" + releaseFolderPath << std::endl;
    std::cout << "releaseName:" + releaseName << std::endl;
    std::cout << "pathToFile:" + pathToFile << std::endl;

    Poco::Zip::Decompress dec(inp, releaseFolderPath);
    dec.decompressAllFiles();

    std::cout << "SPOTFileDownload - release uncompressed" << std::endl;
    std::cout << "uncompressed files:" + releaseFolderPath << std::endl;
  }
  catch(...)
  {
    std::cout << "SPOTFileDownload - Invalid release zip file" << std::endl;
    return false;
  }
  return true;
}

bool SPOTFileDownload::checkAndGetResourceFolder( std::string & sFolder )
{
  std::cout << "SPOTFileDownload - looking for resources.." << std::endl;
  sFolder.clear();
  std::string resFolder = releaseFolderPath + folderSeparator() + CResourcesFoldername + folderSeparator() + releaseName;
  try
  {
    Poco::File foldr(resFolder);
    if(!foldr.exists() || !foldr.isDirectory())
    {
      std::cout << "SPOTFileDownload - not a valid "<< CResourcesFoldername << " folder. " << resFolder << std::endl;
      return false;
    }
  }
  catch(...)
  {
    std::cout << "SPOTFileDownload - not a valid "<< CResourcesFoldername << " folder" << std::endl;
    return false;
  }

  sFolder = resFolder;
  return true;
}

bool SPOTFileDownload::downloadResources(const std::string & resFolder)
{
  std::cout << "\nDownloading resources to SPOT." << resFolder << std::endl<< std::endl;

  Poco::File foldr(resFolder);
  if(!foldr.exists() || !foldr.isDirectory())
  {
    std::cout << "SPOTFileDownload - not a valid "<< CResourcesFoldername << " folder. "<< resFolder << std::endl;
    return true;
  }


  Poco::DirectoryIterator end;
  for (Poco::DirectoryIterator it(resFolder); it != end; ++it)
  {
    if(it->isDirectory())
    {
      if(!downloadResources(it->path()))
      {
        return false;
      }
    }
    else
    {
      unsigned char resType;
      if(getResTypeId(resFolder, resType) )
      {
        std::cout << "SPOTFileDownload - ready to install " << it->path() << std::endl << std::endl;
        if(!this->downloadResourceFile(it->path(),resType))
        {
          std::cout << "resource download fail" << std::endl<< std::endl;
          return false;
        }
        else
        {
          std::cout << "resource download Ok" << std::endl<< std::endl;
        }
      }
    }
  }
  return true;
}

bool SPOTFileDownload::downloadDebians()
{
  std::cout << "SPOTFileDownload - looking for debians.." << std::endl;
  std::string debFolder = releaseFolderPath +  folderSeparator() + CDebiansFoldername +  folderSeparator() + releaseName;
  try
  {
    Poco::File foldr(debFolder);
    if(!foldr.exists() || !foldr.isDirectory())
    {
      std::cout << "SPOTFileDownload - not a valid "<< CDebiansFoldername << " folder. "<< debFolder << std::endl;
      return true;
    }
  }
  catch(...)
  {
    std::cout << "SPOTFileDownload - not a valid "<< CDebiansFoldername << " folder" << std::endl;
    return true;
  }

  Poco::DirectoryIterator end;
  for (Poco::DirectoryIterator it(debFolder); it != end; ++it)
  {
    if(!it->isDirectory())
    {
      std::cout << "SPOTFileDownload - ready to install " << it->path() << std::endl << std::endl;
      if( !this->downloadDebianPackage(it->path()) )
      {
        return false;
      }
      else
      {
        std::cout << "debian download Ok" << std::endl<< std::endl;
      }
    }
  }
  return true;
}

bool SPOTFileDownload::downloadSpotPackages()
{
  std::cout << "SPOTFileDownload - looking for packages versions.." << std::endl;
  std::string versFolder = releaseFolderPath + folderSeparator() + CVersionsFoldername +  folderSeparator() + releaseName;
  try
  {
    Poco::File foldr(versFolder);
    if(!foldr.exists() || !foldr.isDirectory())
    {
      std::cout << "SPOTFileDownload - not a valid " << CVersionsFoldername << " folder. "<<  versFolder << std::endl;
      return true;
    }
  }
  catch(...)
  {
    std::cout << "SPOTFileDownload - not a valid " << CVersionsFoldername << " folder" << std::endl;
    return true;
  }

  Poco::DirectoryIterator end;
  for (Poco::DirectoryIterator it(versFolder); it != end; ++it)
  {
    if(!it->isDirectory())
    {
      std::cout << "SPOTFileDownload - ready to install " << it->path() << std::endl << std::endl;
      if( !this->downloadSPOTPackage(it->path()) )
      {
        return false;
      }
      else
      {
        std::cout << "package download Ok" << std::endl<< std::endl;
      }
    }
    else
    {
      std::cout << "No Version folder" << std::endl<< std::endl;
    }
  }
  return true;
}

void SPOTFileDownload::removeTemporaryFolder()
{
  if(!releaseFolderPath.empty() )
  {
    Poco::File tempFolder(releaseFolderPath);
    if(tempFolder.exists())
    {
      tempFolder.remove(true);
    }
  }
}

bool SPOTFileDownload::downloadBundle(const std::string & pathToFile, const std::string & pathTmp)
{
  if(!decompressRelease(pathToFile,pathTmp))
  {
    return false;
  }

  std::string resFolder;

  if(!checkAndGetResourceFolder(resFolder))
  {
    return false;
  }

  if(!downloadDebians())
  {
    return false;
  }

  if(!downloadSpotPackages())
  {
    return false;
  }

  removeTemporaryFolder();

  return true;
}

bool SPOTFileDownload::downloadDebianPackage(const std::string & sPathToFile)
{
  ISpotMessage::SizeType              uBlockLength = SpotMessagePackageBlockDownload::CMaxBlockSizeDebian;
  SpotMessagePackageDownload          msgPkgRequest;
  SpotMessagePackageDownloadResponse  msgPkgReply;
  M3M5SpotPackage                     package(sPathToFile);

  package.initialize( uBlockLength );
  if( package.hasErrors() )
  {
    m_iLastErrorCode = eError0x200 + package.getLastOperationError();
    m_blError = true;
    return false; // some kind of error during package object initialization
  }

  msgPkgRequest.setFileLength( package.packageLength() );
  msgPkgRequest.setDownloadOperation();
  msgPkgRequest.setResumeMode();
  msgPkgRequest.setCertificateData( package.getCertificateData() );

  ErrorType  error = m_Session.sendSync(msgPkgRequest, msgPkgReply, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError || ! msgPkgReply.canDownloadBegin() )
  {
    m_iLastErrorCode = ( msgPkgReply.canDownloadBegin() ? eError0x300 + error : eError0x400  + msgPkgReply.getAckCode() );
    m_blError = true;
    return false;
  }

  package.setInitialOffset( msgPkgReply.getInitialOffset() );

  ISpotMessage::SizeType uReadingSize = 0; // should be = uBlockLength except for the last block
  const unsigned char* pBlockData = package.getNextBlock(uReadingSize);
  if( pBlockData == NULL )
  {
    m_iLastErrorCode = eError0x500 + package.getLastOperationError();
    m_blError = true;
    return false; // first block must have data !!
  }

  while( pBlockData != NULL )
  {
    if( ! downloadDebianBlock(uReadingSize, pBlockData) )
    {
      m_blError = true;
      return false;
    }

    pBlockData = package.getNextBlock(uReadingSize);
  }
  return downloadDebianBlock( 0, NULL); // send last NULL block, signaling end of transfer.
}

bool SPOTFileDownload::downloadSPOTPackage(const std::string & sPathToFile)
{
  SpotSession::EDevicePlatform  eDevPlatform = m_Session.getDevicePlatform();

  switch( eDevPlatform )
  {
    case SpotSession::eM3:
      return downloadClassicSPOTPackage(sPathToFile);

    case SpotSession::eM5:
      return downloadClassicSPOTPackage(sPathToFile);

    case SpotSession::eM7:
      return downloadVanguardSPOTPackage(sPathToFile);

    default:
      m_iLastErrorCode = eError0x100;
      m_blError = true;
      return false; // unknown device
  }
}


//======================== PRIVATE MEMBERS ==========================//

bool SPOTFileDownload::downloadClassicSPOTPackage(const std::string & sPathToFile)
{
    ISpotMessage::SizeType              uBlockLength = SpotMessagePackageBlockDownload::CMaxBlockSize; // 2 Kb block
    SpotMessagePackageDownload          msgPkgRequest;
    SpotMessagePackageDownloadResponse  msgPkgReply;
    M3M5SpotPackage                     package(sPathToFile);

    package.initialize( uBlockLength );
    if( package.hasErrors() )
    {
      m_iLastErrorCode = eError0x200 + package.getLastOperationError();
      m_blError = true;
      return false; // some kind of error during package object initialization
    }

    msgPkgRequest.setFileLength( package.fileLength() );
    msgPkgRequest.setCertificateData( package.getCertificateData() );

    ErrorType  error = m_Session.sendSync(msgPkgRequest, msgPkgReply, CPackageDownloadTimeout);
    if( error != SpotSession::ISessionError::CNoError || ! msgPkgReply.canDownloadBegin() )
    {
      m_iLastErrorCode = ( msgPkgReply.canDownloadBegin() ? eError0x300 + error : eError0x400 + msgPkgReply.getAckCode() );
      m_blError = true;
      return false;
    }

    package.setInitialOffset( msgPkgReply.getInitialOffset() );

    ISpotMessage::SizeType uReadingSize = 0; // should be = uBlockLength except for the last block
    const unsigned char* pBlockData = package.getNextBlock(uReadingSize);
    if( pBlockData == NULL )
    {
      m_iLastErrorCode = eError0x500 + package.getLastOperationError();
      m_blError = true;
      return false; // first block must have data !!
    }

    while( pBlockData != NULL )
    {
      if( ! downloadPackageBlock(uReadingSize, pBlockData) )
      {
        m_blError = true;
        return false;
      }

      pBlockData = package.getNextBlock(uReadingSize);
    }

    return downloadPackageBlock( 0, NULL); // send last NULL block, signaling end of transfer.
}

bool SPOTFileDownload::downloadVanguardSPOTPackage(const std::string & sPathToFile)
{
	//TODO: To be implemented.

    ISpotMessage::SizeType              uBlockLength = SpotMessageVGDFileTransfer::CMaxBlockSize; // 64 Kb block
    ISpotMessage::SizeType              uBytesSent = 0;
    SpotMessageVGDFileTransfer          msgPkgRequest;
    SpotMessageVGDFileTransferResponse  msgPkgReply;
    UnsignedBinaryFile                  package(sPathToFile);

    package.initialize( uBlockLength );
    if( package.hasErrors() )
    {
      m_iLastErrorCode = eError0x200 + package.getLastOperationError();
      m_blError = true;
      return false; // some kind of error during package object initialization
    }

    msgPkgRequest.setOperationCommand(SpotMessageVGDFileTransfer::CVGDFileDownloadTransferCommandId);
    msgPkgRequest.setFileLength( package.fileLength() );

    ErrorType  error = m_Session.sendSync(msgPkgRequest, msgPkgReply, 5000);
    if( error != SpotSession::ISessionError::CNoError || ! msgPkgReply.canDownloadBegin() )
    {
      m_iLastErrorCode = ( msgPkgReply.canDownloadBegin() ? eError0x300 + error : eError0x400 + msgPkgReply.getAckCode() );
      m_blError = true;
      std::cout << std::endl << "Session Error: [" << m_Session.getErrorDescription() << "]\n";
      return false;
    }

    ISpotMessage::SizeType uReadingSize = 0; // should be = uBlockLength except for the last block
    const unsigned char* pBlockData = package.getNextBlock(uReadingSize);
    if( pBlockData == NULL )
    {
      m_iLastErrorCode = eError0x500 + package.getLastOperationError();
      m_blError = true;
      return false; // first block must have data !!
    }

    std::cout << "First block size is " << uReadingSize << ", MAX is " << uBlockLength << std::endl;
    while( pBlockData != NULL )
    {
      if( ! downloadVGDBlock(package.fileLength() - uBytesSent <= uBlockLength, uReadingSize, pBlockData) )
      {
        m_blError = true;
        return false;
      }

      uBytesSent += uReadingSize;
      pBlockData = package.getNextBlock(uReadingSize);
    }

    std::cout << "\nFile transfer successfully performed.\n";

    SpotMessageVgdProcessPackage         msgProcessPkg;
    SpotMessageVgdProcessPackageResponse msgProcessPkgReply;

    error = m_Session.sendSync(msgProcessPkg, msgProcessPkgReply, CPackageDownloadTimeout);
    if( error != SpotSession::ISessionError::CNoError || ! msgProcessPkgReply.hasBeenProcessedOK() )
    {
      m_blError = true;

      if( msgProcessPkgReply.hasBeenProcessedOK() )
      {
        m_iLastErrorCode = eError0x800 + error;
        std::cout << "\nSession Error: [" << m_Session.getErrorDescription() << "]\n";
      }
      else // session OK, SPOT notifies error.
      {
        m_iLastErrorCode = eError0x900 + msgProcessPkgReply.getAckCode();
        std::cout << "\nPackage Process Error:  Ack [" << std::hex << msgProcessPkgReply.getAckCode() << "]\n";
      }

      return false;
    }

    std::cout << "\nPackage Process successfully performed!\n";
    return true;
}

bool SPOTFileDownload::downloadPackageBlock(ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData)
{
  SpotMessagePackageBlockDownload          msgBlockRequest;
  SpotMessagePackageBlockDownloadResponse  msgBlockReply;

  msgBlockRequest.setBlock(uBlockLength, pBlockData);

  ErrorType  error = m_Session.sendSync(msgBlockRequest, msgBlockReply, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError  || ! msgBlockReply.wasBlockReceivedOK() )
  {
    m_iLastErrorCode = ( msgBlockReply.wasBlockReceivedOK() ? eError0x600 + error : eError0x700 + msgBlockReply.getAckCode() );
    m_blError = true;
    return false;
  }

  return true;
}

bool SPOTFileDownload::downloadDebianBlock(ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData)
{
  SpotMessagePackageBlockDownload          msgBlockRequest;
  SpotMessagePackageBlockDownloadResponse  msgBlockReply;

  msgBlockRequest.setBlock(uBlockLength, pBlockData);

  ErrorType  error = m_Session.sendSync(msgBlockRequest, msgBlockReply, CPackageDownloadTimeout);
  if( error != SpotSession::ISessionError::CNoError  || (!msgBlockReply.wasBlockReceivedOK() && msgBlockReply.getAckCode() != ISpotMessage::CErrorCodeDebianActivation))
  {
    m_iLastErrorCode = ( msgBlockReply.wasBlockReceivedOK() ? eError0x600 + error : eError0x700 + msgBlockReply.getAckCode() );
    m_blError = true;
    return false;
  }

  return true;
}


bool SPOTFileDownload::downloadVGDBlock(bool blLastBlock, ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData)
{
  ErrorType  bError = 0;
  SpotMessageVGDFileTransfer          msgBlockRequest;
  SpotMessageVGDFileTransferResponse  msgBlockReply;

  msgBlockRequest.setOperationCommand(SpotMessageVGDFileTransfer::CVGDBlockDownloadTransferCommandId);
  msgBlockRequest.setBlock(uBlockLength, pBlockData);

  std::cout << "Sending " << ( blLastBlock ? "LAST " : " ") << "block, size " << uBlockLength << std::endl;
  if( blLastBlock )
  {
    bError = m_Session.sendSync(msgBlockRequest, msgBlockReply, CPackageDownloadTimeout);
  }
  else
  {
    bError = m_Session.send(msgBlockRequest); // only for the last block, a response is waited for.
    if( bError != SpotSession::ISessionError::CNoError )
    {
      std::cout << "Error after send: " << int(bError) << std::endl;
      m_iLastErrorCode = eError0xA00 + bError;
      return false;
    }
    return true;
  }

  if( bError != SpotSession::ISessionError::CNoError  || ! msgBlockReply.wasBlockTransferOK() )
  {
    m_iLastErrorCode = ( msgBlockReply.wasBlockTransferOK() ? eError0x600 + bError : eError0x700 + msgBlockReply.getAckCode() );
    m_blError = true;
    return false;
  }

  return true;
}

} // namespace spot

} // namespace communication

} // namespace gvr



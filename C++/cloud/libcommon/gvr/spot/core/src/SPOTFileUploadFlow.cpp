/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SPOTFileUploadFlow.cpp
 * @author Guillermo Paris
 * @date Tuesday, June 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <sys/stat.h>

#include <fstream>

#include <Poco/Zip/Compress.h>
#include <Poco/Path.h>
#include <Poco/File.h>

#include <gvr/communication/spot/UnsignedBinaryFile.h>
#include <gvr/communication/spot/SpotResourceFile.h>
#include <gvr/communication/spot/SpotMessageFileUpload.h>
#include <gvr/communication/spot/SpotMessageFileUploadResponse.h>
#include <gvr/communication/spot/SpotMessageFileBlockUpload.h>
#include <gvr/communication/spot/SpotMessageFileBlockUploadResponse.h>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransfer.h>
#include <gvr/communication/spot/Utils.h>

#include <gvr/communication/spot/SPOTFileUploadFlow.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const char * SPOTFileUpload::CResourcesFoldername = "resources";
const char * SPOTFileUpload::CLogsFoldername = "logs";
const char * SPOTFileUpload::CConfigurationFoldername = "configuration";

SPOTFileUpload::SPOTFileUpload (SpotSession & session) : AbsSpotTransferFlow(session)
{
}

SPOTFileUpload::~SPOTFileUpload (void)
{
}


int SPOTFileUpload::uploadLogs(const std::vector<UploadableFileList::FileNameAndAttrib> &  vLogFiles)
{
  m_blError = false; // clear any previous error condition
  std::cout << "SPOTFileUpload - looking for log files..\n\n";

  std::string sLogFolder = m_sArchiveFolderPath +  folderSeparator() + CLogsFoldername +  folderSeparator();
  try
  {
    Poco::File foldr(sLogFolder);
    if(!foldr.exists() || !foldr.isDirectory())
    {
      std::cout << "SPOTFileUpload - not a valid "<< CLogsFoldername << " folder. "<< sLogFolder << std::endl;
      return -1;
    }
  }
  catch(...)
  {
    std::cout << "SPOTFileUpload - not a valid "<< CLogsFoldername << " folder" << std::endl;
    return -2;
  }

  bool blOk = true;
  std::string sFullPathToFile;
  int idx = 0;

  while( blOk && idx < vLogFiles.size() )
  {
    sFullPathToFile = sLogFolder + vLogFiles[idx].m_sFilename;
    std::cout << "Uploading File: " << vLogFiles[idx].m_sFilename << std::endl;
    blOk = uploadLogFile(sFullPathToFile, vLogFiles[idx++].m_attrib);
  }

  return blOk ? idx : idx - 1;
}

bool SPOTFileUpload::uploadConfigurationFiles()
{
  // TODO: Implement here the configuration files retrieving. Feel free to change the method
  //       prototype to support some kind of file list like SPOTFileUpload::uploadLogs().

  return true;
}

bool SPOTFileUpload::uploadResources(const std::string & resFolder)
{
  m_blError = false; // clear any previous error condition
  // TODO: Implement here the resource files retrieving. Feel free to change the method
  //       prototype to support some kind of file list like SPOTFileUpload::uploadLogs().

/*  Code copied from Download below: see what is good to reuse here
  Poco::DirectoryIterator end;
  for (Poco::DirectoryIterator it(resFolder); it != end; ++it)
  {
    if(it->isDirectory())
    {
      if(!uploadResources(it->path()))
      {
        return false;
      }
    }
    else
    {
      unsigned char resType;
      if( getResTypeId(resFolder, resType) )
      {
        std::cout << "SPOTFileUpload - ready to install " << it->path() << std::endl << std::endl;
        if(!this->uploadResourceFile(it->path(),resType))
        {
          std::cout << "resource upload fail" << std::endl<< std::endl;
          return false;
        }
        else
        {
          std::cout << "resource upload Ok" << std::endl<< std::endl;
        }
      }
    }
  }
*/

  return true;
}


bool SPOTFileUpload::uploadResourceFile(const std::string & sPathToFile, const unsigned char type)
{
  m_blError = false; // clear any previous error condition
  /*  Code copied from Download below: see what is good to reuse here
  SpotSession::EDevicePlatform  eDevPlatform = m_Session.getDevicePlatform();

  if( eDevPlatform != SpotSession::eM3 && eDevPlatform !=  SpotSession::eM5 && eDevPlatform != SpotSession::eM7)
  {
    m_iLastErrorCode = eError0x100;
    m_blError = true;
    return false; // unknown device
  }

  std::cout << "uploadResourceFile.." << std::endl;

  ISpotMessage::SizeType uBlockLength = SpotMessageFileBlockUpload::CMaxBlockSize; // 2 Kb block
  SpotMessageFileUpload msgFileRequest;
  SpotMessageFileUploadResponse msgFileResponse;

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

  //UPLOAD the resource
  msgFileRequest.setFileId(resource.getResId());
  msgFileRequest.setFileType(type);
  int size = resource.fileLength() - resource.getCertificateSize();
*/
  //TODO: implement resource upload here

  return true;
}

bool SPOTFileUpload::uploadLogFile( const std::string & sFullPathToFile,
                                    const UploadableFileList::FileAttrib fileAtt )
{
  ISpotMessage::SizeType         uBlockLength = SpotMessageFileBlockUpload::CMaxBlockSize;
  SpotMessageFileUpload          msgFileRequest(fileAtt.b_type, fileAtt.b_fileId);
  SpotMessageFileUploadResponse  msgFileReply;

  m_blError = false; // clear any previous error condition
  m_iLastErrorCode = 0;

  ErrorType  error = m_Session.sendSync(msgFileRequest, msgFileReply, CFileUploadTimeout);
  if( error == SpotSession::ISessionError::CNoError && msgFileReply.getAckCode() == CInexistentFile)
  {
    return true; // inexistent log file in SPOT
  }
  else if( error != SpotSession::ISessionError::CNoError || ! msgFileReply.canUploadBegin() )
  {
    m_iLastErrorCode = ( msgFileReply.canUploadBegin() ? eError0x100 + error : eError0x200 + msgFileReply.getAckCode() );
    m_blError = true;
    return false;
  }

  SpotSession::SizeType uByteOffset = 0;
  SpotSession::SizeType uBytesRead = 0;
  SpotSession::SizeType uWrittingSize = SpotMessageFileBlockUpload::CMaxBlockSize;
  SpotSession::SizeType uFileSize = msgFileReply.getFileSize();
  std::string           sFullPathName(sFullPathToFile + ".log");

  if( fileAtt.b_age != UploadableFileList::eCurrent )
  {
    sFullPathName += ".tar.gz";
  }

  FILE* pFile = fopen( sFullPathName.c_str() , "wb" );
  if( pFile == NULL )
  {
    m_blError = true;
    m_iLastErrorCode = eError0x300;
    return false;
  }

  const unsigned char* pBlockData = NULL;

  if( uFileSize == 0 ) // zero length SPOT issue
  {
    uFileSize = CFileSizeBigEnough;
  }

  std::cout << "Uploading blocks ...\n";

  while( uBytesRead < uFileSize && uWrittingSize == SpotMessageFileBlockUpload::CMaxBlockSize )
  {
    pBlockData = uploadLogBlock( uWrittingSize, uByteOffset );

    if( uWrittingSize == 0 && uByteOffset == 0 )
    {
      m_blError = false;
      break; // last null block received OK.
    }

    if( pBlockData == NULL ) // genuine error condition
    {
      m_blError = true;
      break;
    }

    uBytesRead += uWrittingSize;
    SpotSession::SizeType nWritten = fwrite(pBlockData, 1, uWrittingSize, pFile);

    if( nWritten != uWrittingSize )
    {
      m_blError = true;
      m_iLastErrorCode = eError0x400; // file writing error.
      break;
    }

    if( uByteOffset == 0 )
    {
      m_blError = false;
      break; // last not null block received OK.
    }
  }

  fclose(pFile);
  return ! m_blError;
}


void SPOTFileUpload::setArchiveName(const std::string & sArchname)
{
  m_sArchiveName = sArchname;
}

bool SPOTFileUpload::checkAndSetFolder( const std::string & sFolder )
{
  try
  {
    Poco::File foldr(sFolder);
    if(!foldr.exists() || !foldr.isDirectory())
    {
      std::cout << "SPOTFileUpload - not a valid root collector folder: " << sFolder << std::endl;
      return false;
    }

    std::string sLogFolder = sFolder;
    sLogFolder += Poco::Path::separator();
    sLogFolder += CLogsFoldername;
    Poco::File logFoldr( sLogFolder );
    if( !logFoldr.exists() || !logFoldr.isDirectory() )
    {
      if( mkdir( sLogFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 )
      {
        std::cout << "SPOTFileUpload - could not create log folder: " << sFolder << std::endl;
        return false;
      }
      else
      {
        std::cout << "Log directory successfully created: " << sLogFolder << std::endl;
      }
    }
  }
  catch(...)
  {
    std::cout << "SPOTFileUpload - not a valid collector folder: " << sFolder << std::endl;
    return false;
  }

  m_sArchiveFolderPath = sFolder; // remember this valid folder for further operations
  m_blError = false; // clear any previous error condition
  return true;
}

int SPOTFileUpload::compressFiles()
{
  int iArchiveSize = -1;
  std::string sPathToArchive( m_sArchiveFolderPath + folderSeparator() );
  std::string sLogFolder( sPathToArchive );

  sPathToArchive += m_sArchiveName;
  sLogFolder += CLogsFoldername;

  try
  {
    std::cout << "SPOTFileUpload - compressing archive " << sPathToArchive << " ...\n" << std::endl;

    std::ofstream  ofs(sPathToArchive.c_str(), std::ios::binary);
    Poco::Path     pathLogFolder(sLogFolder);

    std::cout << "log folder path: " + pathLogFolder.toString() << std::endl;
    std::cout << "path to archive: " + sPathToArchive << std::endl;

    Poco::Zip::Compress compress(ofs, true);
    compress.addRecursive( pathLogFolder, Poco::Zip::ZipCommon::CL_MAXIMUM, true);
    compress.close();
    Poco::File archive(sPathToArchive);
    iArchiveSize = archive.getSize();

    std::cout << "SPOTFileUpload - archive successfully compressed for file tree in " + m_sArchiveFolderPath
              << " , size " << iArchiveSize << " bytes.\n";
  }
  catch(...)
  {
    std::cout << "SPOTFileUpload - Invalid folder for file tree : " + m_sArchiveFolderPath << std::endl;
    return -1;
  }

  return iArchiveSize;
}


//======================== PRIVATE MEMBERS ==========================//

bool SPOTFileUpload::uploadResourceBlock( unsigned char* pBlockData, ISpotMessage::SizeType & uBlockLength )
{
  SpotMessageFileBlockUpload          msgBlockRequest;
  SpotMessageFileBlockUploadResponse  msgBlockResponse;

  // TODO: Implement logic to upload a resource file block here.
/*
  msgBlockRequest.setBlockOffset(offset);
  ErrorType  error = m_Session.sendSync(msgBlockRequest, msgBlockResponse, CPackageUploadTimeout);
  if( error != SpotSession::ISessionError::CNoError || (msgBlockResponse.getAckCode() != 0x00 && msgBlockResponse.getAckCode() != 0x02))
  {
    m_iLastErrorCode = 0x700  + error;
    m_blError = true;
    return false;
  }

  offset = msgBlockResponse.getStartingOffset();
  msgBlockRequest.getBlockData(blockData);
*/
  return true;
}

const unsigned char*  SPOTFileUpload::uploadLogBlock(  ISpotMessage::SizeType & uBlockLength,
                                                       ISpotMessage::SizeType & uOffset  )
{
  SpotMessageFileBlockUpload          msgBlockRequest;
  SpotMessageFileBlockUploadResponse  msgBlockReply;

  msgBlockRequest.setBlockOffset( uOffset );
  ErrorType  error = m_Session.sendSync(msgBlockRequest, msgBlockReply, CFileUploadTimeout);
  if( error != SpotSession::ISessionError::CNoError || !msgBlockReply.wasBlockReceivedOK() )
  {
    m_iLastErrorCode = ( msgBlockReply.wasBlockReceivedOK() ? eError0x500 + error : eError0x600 + msgBlockReply.getAckCode() );
    m_blError = true;
    return NULL;
  }

  if( msgBlockReply.isLastBlock() )
  {
    uOffset = 0; // last block retrieved is signaled 0x02 by SPOT (instead of 0x00), but is not really an error.
  }

  uOffset = msgBlockReply.getStartingOffset();
  const ISpotMessage::BufferType & rVecBytes = msgBlockReply.getBlockData();
  uBlockLength = rVecBytes.size();
  if( uBlockLength > 0 )
  return ( uBlockLength > 0 ? & rVecBytes.front() : NULL );
}


} // namespace spot

} // namespace communication

} // namespace gvr



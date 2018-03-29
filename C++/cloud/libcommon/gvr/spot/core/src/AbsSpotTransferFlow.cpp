/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsTransferFlowFlow.cpp
 * @author Guillermo Paris
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <Poco/Path.h>
#include <Poco/DirectoryIterator.h>
#include <gvr/communication/spot/AbsSpotTransferFlow.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const std::string AbsSpotTransferFlow::CZipExtensionName = ".zip";


AbsSpotTransferFlow::AbsSpotTransferFlow (SpotSession & spotSession)
  : m_blError(false), m_iLastErrorCode(CNoError), m_Session(spotSession)
{
}

AbsSpotTransferFlow::~AbsSpotTransferFlow (void)
{  // It just assigns physical address to this virtual destructor method,
}  // avoiding this way potential future link errors.

int AbsSpotTransferFlow::getLastOperationError (void) const
{
  return m_iLastErrorCode;
}


std::string AbsSpotTransferFlow::folderSeparator()
{
  return std::string(1, Poco::Path::separator());
}

bool AbsSpotTransferFlow::getResTypeId(const std::string resFolder, unsigned char & resType)
{
  if (resFolder.find("pinpad" + folderSeparator() + "message") != std::string::npos)
  {
    resType = 0x10;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "image")!= std::string::npos)
  {
    resType =  0x11;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "font")!= std::string::npos)
  {
    resType = 0x12;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "keymaps")!= std::string::npos)
  {
    resType = 0x14;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "displayscreen")!= std::string::npos)
  {
    resType = 0x15;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "display_template")!= std::string::npos)
  {
    resType = 0x15;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "log")!= std::string::npos)
  {
    resType = 0x16;
    return true;
  }

  if(resFolder.find("pinpad" + folderSeparator() + "filter")!= std::string::npos)
  {
    resType = 0x1E;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "table")!= std::string::npos)
  {
    resType = 0x20;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "messages")!= std::string::npos)
  {
    resType = 0x21;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "script")!= std::string::npos)
  {
    resType = 0x22;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "log")!= std::string::npos)
  {
    resType = 0x23;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "ui_step_list")!= std::string::npos)
  {
    resType = 0x2F;
    return true;
  }

  if(resFolder.find("emv" + folderSeparator() + "clesstable")!= std::string::npos)
  {
    resType = 0x45;
    return true;
  }

  if(resFolder.find("opt" + folderSeparator() + "bitmap")!= std::string::npos)
  {
    resType = 0x42;
    return true;
  }

  if(resFolder.find("barcode" + folderSeparator() + "config_resources")!= std::string::npos)
  {
    resType = 0x48;
    return true;
  }

  if(resFolder.find("as2805")!= std::string::npos)
  {
    resType = 0x5F;
    return true;
  }

  if(resFolder.find("application" + folderSeparator() + "rescrind")!= std::string::npos)
  {
    resType = 0x7A;
    return true;
  }

  return false;
}

void AbsSpotTransferFlow::removeTemporaryFolder(const std::string & sFolderPath)
{
  if( ! sFolderPath.empty() )
  {
    Poco::File tempFolder(sFolderPath);
    if(tempFolder.exists())
    {
      tempFolder.remove(true);
    }
  }
}

void AbsSpotTransferFlow::removeTemporaryFiles(const std::string & sFolderPath)
{
  Poco::DirectoryIterator it(sFolderPath);
  Poco::DirectoryIterator itend;

  while(it != itend)
  {
    it->remove(true); // Recursive removal of everything inside the given folder path.
    ++it;
  }
}



} // namespace gvr

} // namespace communication

} // namespace spot



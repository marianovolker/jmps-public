/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SPOTFileDownloadFlow.h
 * @author Guillermo Paris
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTFILEDOWNLOADFLOW_H_
#define SPOTFILEDOWNLOADFLOW_H_

#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/ISession.h>
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


/**
 * @class AbsDownloadFlow
 * @headerfile <gvr/communication/spot/AbsDownloadFlow.h>
 * @brief This class implements the basic common functionality for all the flow or "workers"
 *        that download "something" to SPOT.
 * @see gvr.communication.spot.IDownloadFlow
 * @since 0.0.0, 2015-03-06
 * @version 0.0.0, 2015-03-06
 * @author Guillermo Paris
 */
class SPOTFileDownload : public AbsSpotTransferFlow
{
  public:

    static const ISession<SpotMessageSpec>::TimeMillisecondsType CPackageDownloadTimeout = 15000; // 15 sec

   /**
    * @brief Spurious errors that might arise running this code
    */
    enum ELocalErrors
    {
      eError0x100 = 0x100, ///< 0x100 = "Invalid platform."
      eError0x200 = 0x200, ///< 0x200 = "Package initialization error."
      eError0x300 = 0x300, ///< 0x300 = "Session error sending/receiving message."
      eError0x400 = 0x400, ///< 0x400 = "Download can not begin."
      eError0x500 = 0x500, ///< 0x500 = "Null data for initial block."
      eError0x600 = 0x600, ///< 0x600 = "Session Error sending block message."
      eError0x700 = 0x700, ///< 0x700 = "Block was not received OK."
      eError0x800 = 0x800, ///< 0x800 = "Session Error sending/receiving process package command."
      eError0x900 = 0x900, ///< 0x900 = "Process Package command denied. See SPOT Ack code."
      eError0xA00 = 0xA00  ///< 0xA00 = "Block transfer error in VGD file transfer."
    };


    /**
    * @brief Constructor.
    *
    * @since 0.0.0
    */
    SPOTFileDownload (SpotSession &);

    /**
    * @brief Uncompress a Release Bundle.
    * @param pathToFile the release file path.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool decompressRelease(const std::string & pathToFile, const std::string & pathToDestination = "");

    /**
    * @brief Download the release bundle Spot Packages
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool downloadSpotPackages();

    /**
    * @brief Download the release bundle Spot Packages
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool downloadDebians();

    /**
    * @brief Check and get the resource folder belonging to a decompressed Release Bundle.
    * @param [Out] @b sFolder is the directory where resources can be taken for download to SPOT,
    *                         if return value is true.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool checkAndGetResourceFolder( std::string & sFolder);

    /**
    * @brief Download the resources of an specific folder.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool downloadResources(const std::string & resFolder);

   /**
    * @brief Remove the temporary stuff used by decompressed target files and download process.
    */
    void removeTemporaryFolder();

    /**
    * @brief Downloads a debian package to SPOT.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadDebianPackage(const std::string & sPathToFile);

    /**
    * @brief Downloads a SPOT resource (screen, message, font, image, etc) to SPOT.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @param typeId is the type id for the resource to download
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadResourceFile(const std::string & sPathToFile, const unsigned char typeId);

    /**
    * @brief Downloads a SPOT package (ROMfs, firmware, app package, etc) to SPOT.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadSPOTPackage(const std::string & sPathToFile);

    /**
    * @brief Decompress and downloads a complete installation release bundle to SPOT.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadBundle(const std::string & sPathToFile, const std::string & pathTmp = "");

    /**
    * @brief Destructor.
    *
    * @since 0.0.0
    */
    virtual ~SPOTFileDownload (void);


  private:

    /**
    * @brief Downloads a SPOT package (ROMfs, firmware, app package, etc) to an M3/M5 device.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadClassicSPOTPackage(const std::string & sPathToFile);

    /**
    * @brief Downloads a Vanguard SPOT package (ROMfs, firmware, app package, etc) to an M7 device.
    * @param @b sPathToFile is the string containing the full pathname of the file to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadVanguardSPOTPackage(const std::string & sPathToFile);

    /**
    * @brief Downloads a SPOT package block.
    *
    * @param @b uBlockLength is the size of the block to download.
    * @param @b pBlockData is the block binary data to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadPackageBlock(ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData);
    /**
    * @brief Downloads an SPOT debian package block.
    *
    * @param @b uBlockLength is the size of the block to download.
    * @param @b pBlockData is the block binary data to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadDebianBlock(ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData);

    /**
    * @brief Downloads a block for vanguard M7 SPOTs.
    *
    * @param @b blLastBlock is only @b true if it is the last block, @b false otherwise.
    * @param @b uBlockLength is the size of the block to download.
    * @param @b pBlockData is the block binary data to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadVGDBlock(bool blLastBlock, ISpotMessage::SizeType uBlockLength, const unsigned char* pBlockData);

    /**
    * @brief Downloads a SPOT resource block.
    *
    * @param @b offset is the offset file to download.
    * @param @b pBlockData is the block binary data to download.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool downloadResourceBlock(ISpotMessage::SizeType & offset, const ISpotMessage::BufferType pBlockData );


  private:

    std::string releaseFolderPath;/**@brief Release folder path for a release download flow*/
    std::string releaseName;/**@brief Release name for a release download flow*/

    static const std::string CResourcesFoldername ; /**@brief Default folder name for Resources in a release bundle*/
    static const std::string CVersionsFoldername; /**@brief Default folder name for Versions in a release bundle*/
    static const std::string CDebiansFoldername; /**@brief Default folder name for Deians in a release bundle*/
};


} // namespace spot

} // namespace communication

} // namespace gvr



#endif /* SPOTFILEDOWNLOADFLOW_H_ */


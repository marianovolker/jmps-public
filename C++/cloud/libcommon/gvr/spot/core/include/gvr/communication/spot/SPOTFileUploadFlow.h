/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SPOTFileUploadFlow.h
 * @author Guillermo Paris
 * @date Tuesday, June 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTFILEUPLOADFLOW_H_
#define SPOTFILEUPLOADFLOW_H_

#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/ISession.h>
#include <gvr/communication/spot/UploadableFileList.h>
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
 * @class AbsUploadFlow
 * @headerfile <gvr/communication/spot/AbsDownloadFlow.h>
 * @brief This class implements the basic common functionality for all the flow or "workers"
 *        that upload/upload "something" from/to SPOT.
 * @see gvr.communication.spot.IUploadFlow
 * @since 0.0.0, 2015-06-09
 * @version 0.0.0, 2015-06-09
 * @author Guillermo Paris
 */
class SPOTFileUpload : public AbsSpotTransferFlow
{
  public:

    static const ISession<SpotMessageSpec>::TimeMillisecondsType CFileUploadTimeout = 15000;

   /**
    * @brief Spurious errors that might arise running this code
    */
    enum ELocalErrors
    {
      eError0x100 = 0x100, ///< 0x100 = "Session error sending/receiving file message."
      eError0x200 = 0x200, ///< 0x200 = "Upload can not be started by SPOT."
      eError0x300 = 0x300, ///< 0x300 = "Can't open file for writing."
      eError0x400 = 0x400, ///< 0x400 = "Some error writing block data into log file."
      eError0x500 = 0x500, ///< 0x500 = "Session error sending/receiving block message."
      eError0x600 = 0x600, ///< 0x600 = "Block received with errors."
    };

    static const char * CResourcesFoldername;    ///< @brief Default folder name for the collected Resources.
    static const char * CLogsFoldername;         ///< @brief Default folder name for the collected logs.
    static const char * CConfigurationFoldername;///< @brief Default folder name for the collected configuration files.

    /**
    * @brief Constructor.
    *
    * @since 0.0.0
    */
    SPOTFileUpload (SpotSession &);

    /**
    * @brief Destructor.
    *
    * @since 0.0.0
    */
    ~SPOTFileUpload (void);

    /**
    * @brief Upload the selected logs from SPOT to "logs" directory.
    *        Return the number of files successfully uploaded.
    * @return @b index of the file having SPOT upload trouble, or vector length if whole process went OK.
    * @since 0.0.0
    */
    int uploadLogs(const std::vector<UploadableFileList::FileNameAndAttrib> &  vLogFiles);


    /**
    * @brief Upload the requested configuration files.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool uploadConfigurationFiles();


    /**
    * @brief Upload the resources of an specific folder.
    * @param @b resFolder is the top resource directory to scan into.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool uploadResources(const std::string & resFolder);

    /**
    * @brief Uploads a log file from SPOT.
    * @param @b sFullPathToFile is the string containing the full pathname of the file to upload from SPOT.
    * @param @b fileAtt is the structure containing the filename and SPOT attributes of the file to upload.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool uploadLogFile( const std::string & sFullPathToFile,
                        const UploadableFileList::FileAttrib fileAtt );


    /**
    * @brief Uploads a SPOT resource (screen, message, font, image, etc) to SPOT.
    * @param @b sPathToFile is the string containing the full pathname of the file to upload.
    * @param typeId is the type id for the resource to upload
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool uploadResourceFile(const std::string & sPathToFile, const unsigned char typeId);

    /**
    * @brief Set the archive filename.
    * @param @b sArchname is the archive name to be constructed from collected files,
    *                         if return value is true.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    void setArchiveName(const std::string & sArchname);

    /**
    * @brief Check and set the root folder for uploading files.
    *        In this directory the archive will be generated.
    * @param @b sFolder is the directory where archive is built from collected file tree,
    *                         if return value is true.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    bool checkAndSetFolder( const std::string & sFolder );


    /**
    * @brief Compress a file collection tree into an archive.
    * @return The @b size of the compressed archive, or -1 if errors happened.
    * @since 0.0.0
    */
    int compressFiles();


  private:

    /**
    * @brief Uploads a data block of a log file.
    *
    * @param [Out] @b uBlockLength is the size of the block recently uploaded.
    * @param [In|Out] @b uOffset is the offset inside file being uploaded.
    * @return @b pointer to the block data uploaded, if operation was performed successfully.
    *         @b NULL if any error stopped the operation.
    *
    * @since 0.0.0
    */
    const unsigned char*  uploadLogBlock( ISpotMessage::SizeType & uBlockLength,
                                          ISpotMessage::SizeType & uOffset );

    /**
    * @brief Uploads a SPOT resource data block.
    *
    * @param @b uOffset is the offset file to upload.
    * @param @b pBlockData is the block binary data to upload.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    *
    * @since 0.0.0
    */
    bool uploadResourceBlock(  unsigned char* pBlockData, ISpotMessage::SizeType & uBlockLength);

    static const unsigned int CInexistentFile    = 0x19;
    static const unsigned int CFileSizeBigEnough = 1000000;

    std::string m_sArchiveFolderPath;///< @brief Archive folder path for a file upload flow.
    std::string m_sArchiveName;      ///< @brief Archive filename for a file upload flow.
};


} // namespace spot

} // namespace communication

} // namespace gvr



#endif /* SPOTFILEUPLOADFLOW_H_ */


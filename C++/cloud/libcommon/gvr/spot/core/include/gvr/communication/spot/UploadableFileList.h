/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UploadableFileList.h
 * @author Guillermo Paris
 * @date Thursday, Jun 25, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef UPLOADABLEFILELIST_H_
#define UPLOADABLEFILELIST_H_

#include <map>
#include <vector>
#include <string>


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
  * @class UploadableFileList
  * @headerfile <gvr/communication/spot/UploadableFileList.h>
  * @brief This class handles the total list of files that can be retrieved from SPOT protocol/file system,
  *        and thus can be uploaded to the cloud.
  *
  * @since 0.0.0, 2015-06-25
  * @version 0.0.0, 2015-06-25
  * @author Guillermo Paris
  */
  class UploadableFileList
  {
  public:

    enum EAge
    {
      eNoSpecified, ///< Indicates no restriction over age.
      eCurrent,     ///< Indicates a requirement over current (today) log files ONLY.
      eHistoric,    ///< Indicates a requirement over historic (past days) log files ONLY.
      eBoth,        ///< Indicates a requirement ONLY over logs that don't differentiate age.
    };

    struct FileAttrib
    {
      FileAttrib() : b_age(eNoSpecified), b_type(0), b_fileId(0) {}

      FileAttrib(unsigned char age, unsigned char typ, unsigned char fId, const char * path = NULL)
        : b_age(age), b_type(typ), b_fileId(fId), s_FilePath( path == NULL ? "" : path ) {}

      unsigned char b_age;      ///< @brief Age of the log file.
      unsigned char b_type;     ///< @brief File type when retrieving from SPOT.
      unsigned char b_fileId;   ///< @brief File Id when retrieving from SPOT.
      std::string   s_FilePath; ///< @brief Full path to the file when retrieving from [virtual] filesystem.
    };

    struct FileNameAndAttrib
    {
      FileNameAndAttrib(const std::string & sFName, const FileAttrib & attr)
        : m_sFilename(sFName), m_attrib( attr ) { }

      std::string m_sFilename;
      FileAttrib  m_attrib;
    };

    typedef std::map<std::string, FileAttrib>  SupportedFilesMap;
    typedef std::vector<FileNameAndAttrib>     RequestedFilesVector;

    /**
     * @brief The constructor taking 2 parameters.
     *
     * @param @b sConfFilename is the full pathname to the configuration file having the list of all
     *                         the file names supported.
     * @param @b pcPlatform is the 2 char C string indicating the platform ("m5" is the default if it is NULL).
     */
    UploadableFileList(std::string sConfFilename, const char* pcPlatform = NULL);

    /**
     * @brief Indicates if the supplied file type name is supported by this device for uploading to cloud.
     *
     * @param sFileType is the text file type name to find out.
     *
     * @return @b true If this file type name is supported and thus can be upload to cloud.
     *         @b false If it is not supported.
     */
    bool isSupported(std::string sFileType);

    /**
     * @brief Return if this object is in error or not.
     */
    bool hasErrors() { return m_blErrors; }

    /**
     * @brief Return the total number of records loaded from configuration file.
     */
    std::size_t getTotalOfRecords() { return m_nRecords; }

    /**
     * @brief Retrieves the list of all the cloud uploadable files.
     *
     * @param [Out] @b vout The output filename vector.
     *
     * @return @b true  If there was no errors retrieving the list from file system.
     *         @b false Otherwise.
     */
    bool getSupportedFilenameList( std::vector<std::string> & vout );

    /**
     * @brief
     *
     * @param @b sFname is the required file type name or a wildcard ("ALL", "ALL_CURR", etc)
     * @param @b eAge as in enumeration EAge. eNoSpecified means no restriction (filter) about age.
     * @param @b iFid is the required file ID. A negative value indicates no restriction about file ID.
     * @param @b iTyp is the required file type code. A negative value indicates no restriction about file type.
     * @param [Out] @b vout Is the supplied vector (passed as a reference) filled with the files that meet
     *                      the search criteria. A vector size of zero means no files meeting criteria or a
     *                      previous error reading the configuration file.
     *
     * @return The same vector reference passed as last parameter.
     */
    RequestedFilesVector &  getRequeriedFileList( const std::string & sFname,
                                                  EAge eAge, int iFid, int iTyp,
                                                  RequestedFilesVector & vout );


  private:

    bool       loadList( const std::string & sFilename );
    bool       parseLine( const std::string & sLine );
    bool       validateFirstField( const std::string & sFirstField,
                                   unsigned int & uAge, std::string & sKey);
    static int getFieldValue(std::string s);
    static int getDigitValue(bool blHex, char c);

    static const char *       CpcKeyNameSelectAll;
    static const char *       CpcKeyNameSelectAllCurrent;
    static const char *       CpcAgeCurrent;
    static const char *       CpcAgeHistoric;
    static const char *       CpcAgeBoth;
    static const char *       CpcConfigurationFile;
    static const unsigned int CMinCharsInLine   = 12;    // Minimum line length.
    static const unsigned int CMinFileTypeNameLength = 3;// 3 or more chars define file type name.
    static const unsigned int CMaxPlatformChars  = 2;    // Only 1 or 2 chars define platform ("m3" , "m5", "m7")

    bool                      m_blErrors;
    unsigned int              m_nRecords;
    const char *              m_pcPlatform;
    SupportedFilesMap         m_mapSupportedFiles;
  };



} // namespace spot

} // namespace communication

} // namespace gvr



#endif /* UPLOADABLEFILELIST_H_ */

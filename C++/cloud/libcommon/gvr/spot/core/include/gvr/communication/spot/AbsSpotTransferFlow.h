/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsTransferFlow.h
 * @author Guillermo Paris
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef ABSSPOTTRANSFERFLOW_H_
#define ABSSPOTTRANSFERFLOW_H_

#include <gvr/communication/spot/SpotSession.h>


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
 * @class AbsSpotTransferFlow
 * @headerfile <gvr/communication/spot/AbsDownloadFlow.h>
 * @brief This class implements the basic common functionality for all the flow or "workers"
 *        that download "something" to SPOT.
 * @see gvr.communication.spot.IDownloadFlow
 * @since 0.0.0, 2015-03-06
 * @version 0.0.0, 2015-03-06
 * @author Guillermo Paris
 */
class AbsSpotTransferFlow
{
  public:

    static const int CNoError = 0; ///< @brief Last operation yielded no error (successfully completed).

   /**
    * @brief Gets the error code yielded by the last operation error condition.
    *
    * @return The error code yielded by the last operation, or @b CNoError if the last operation ended successfully.
    */
    int getLastOperationError (void) const;

    /**
    * @brief Constructor.
    *
    * @since 0.0.0
    */
    AbsSpotTransferFlow (SpotSession &);

    /**
    * @brief Destructor.
    *
    * @since 0.0.0
    */
    virtual  ~AbsSpotTransferFlow (void) = 0;

    /**
     * @brief Remove the temporary folder and ALL its stuff used by the upload process in that folder.
     */
     static void removeTemporaryFolder(const std::string & sFolderPath);


     /**
      * @brief Remove the temporary files in the folder,  used by the upload process.
      */
     static void removeTemporaryFiles(const std::string & sFolderPath);


  protected:

    /**
    * @brief Gets the resources type id.
    * @param resfolder the resources folder.
    * @param restype the resulting resource type.
    * @return @b true if operation was performed successfully, @b false if any error stopped the operation.
    * @since 0.0.0
    */
    static bool getResTypeId(const std::string resFolder, unsigned char & resType);

    /**
    * @brief Returns the current platform folder separator.
    * @return the current platform folder separator.
    * @since 0.0.0
    */
    static std::string folderSeparator();


    static const std::string CZipExtensionName; ///< @brief Default extensions for zip files.


    bool          m_blError;        ///< @brief @b true if Flow ended in error.
    int           m_iLastErrorCode; ///< @brief The error code of the last operation rising an error.
    SpotSession&  m_Session;        ///< @brief The spot session to talk with.
};


} // namespace gvr

} // namespace communication

} // namespace spot


#endif /* ABSSPOTTRANSFERFLOW_H_ */


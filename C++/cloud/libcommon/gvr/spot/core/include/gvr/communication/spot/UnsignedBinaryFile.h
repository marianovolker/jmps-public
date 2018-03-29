/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UnsignedBinaryFile.h
 * @author Guillermo Paris
 * @date Wednesday, March 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef UNSIGNEDBINARYFILE_H_
#define UNSIGNEDBINARYFILE_H_


#include <gvr/communication/spot/AbsSpotPackage.h>

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
 * @class UnsignedBinaryFile
 * @headerfile <gvr/communication/spot/UnsignedBinaryFile.h>
 * @brief This class implements the functionality for SPOT packages/resources/etc. having
 *        no signature or certificate field. This means a simple binary file.
 *
 * @since 0.0.0, 2015-03-18
 * @version 0.0.0, 2015-03-18
 * @author Guillermo Paris
 */
class UnsignedBinaryFile : public AbsSpotPackage
{
  public:

   /**
    * @brief Constructor
    *
    * @param @b sFilename is the package filename.
    * @param @b uInitialOffset is the initial file offset only used in resume downloads.
    *
    */
    UnsignedBinaryFile (const std::string & sFilename, std::size_t uInitialOffset = 0);

   /**
	* @brief Destructor
	*/
    virtual ~UnsignedBinaryFile (void);


  private:

   /**
    * @brief  Called during the initialization, to set the certificate field length.
    *
    * @return @b The certificate field length, set by this class.
    *
    * @see AbsSpotPackage::initialize()
    */
    virtual std::size_t  setCertificateSize (void);

    /**
    * @brief  Called during the initialization, to set the signature field length.
    *
    * @return @b The signature field length, set by this class.
    *
    * @see AbsSpotPackage::initialize()
    */
    virtual std::size_t  setSignatureSize   (void);
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* UNSIGNEDBINARYFILE_H_ */

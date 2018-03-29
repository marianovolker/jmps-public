/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file M3M5SpotPackage.h
 * @author Guillermo Paris
 * @date Tuesday, March 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef M3M5SPOTPACKAGE_H_
#define M3M5SPOTPACKAGE_H_

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
 * @class M3M5SpotPackage
 * @headerfile <gvr/communication/spot/M3M5SpotPackage.h>
 * @brief This class implements the functionality for M3 and M5 SPOT packages
 *        that can be downloaded to any SPOT device.
 * @note  This implementation relies on certificate checking to SPOT device. This means that
 *        CERTIFICATE STRUCTURE IS NEITHER CHECKED NOR VALIDATED in any way.
 * @since 0.0.0, 2015-03-09
 * @version 0.0.0, 2015-03-09
 * @author Guillermo Paris
 */
class M3M5SpotPackage : public AbsSpotPackage
{
  public:

    static const std::size_t  CCertificateSize = 248;///< Constant certificate length for M3 and M5 certificates.

   /**
    * @brief Constructor
    *
    * @param @b sFilename is the package filename.
    * @param @b uInitialOffset is the initial file offset only used in resume downloads.
    *
    */
	M3M5SpotPackage (const std::string & sFilename, std::size_t uInitialOffset = 0);

   /**
	* @brief Destructor
	*/
    virtual ~M3M5SpotPackage (void);


  private:

    /**
    * @brief  Called during the initialization, to set the certificate field length.
    *
    * @note   Derived classes MUST supply this information overriding this method.
    *
    * @return @b The certificate field length, set by this class.
    *
    * @see AbsSpotPackage::initialize()
    */
    virtual std::size_t  setCertificateSize (void);

    /**
    * @brief  Called during the initialization, to set the signature field length.
    *
    * @note   Derived classes MUST supply this information overriding this method.
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


#endif /* M3M5SPOTPACKAGE_H_ */

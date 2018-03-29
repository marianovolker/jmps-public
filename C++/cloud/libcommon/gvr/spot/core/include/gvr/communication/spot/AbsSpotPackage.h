/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotPackage.h
 * @author Guillermo Paris
 * @date Monday, March 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef ABSSPOTPACKAGE_H_
#define ABSSPOTPACKAGE_H_

#include <cstdio>
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
 * @class AbsSpotPackage
 * @headerfile <gvr/communication/spot/AbsSpotPackage.h>
 * @brief This class implements the basic common functionality for all the SPOT packages
 *        that can be downloaded to any SPOT device.
 *
 * @since 0.0.0, 2015-03-09
 * @version 0.0.0, 2015-03-09
 * @author Guillermo Paris
 */
class AbsSpotPackage
{
  public:

   /**
    * @brief Spurious errors that might arise running this code.
    */
    enum LocalErrors
    {
      eError0x01 = 0x01, ///< 0x01 = "Already initialized."
      eError0x02 = 0x02, ///< 0x02 = "Empty filename."
      eError0x03 = 0x03, ///< 0x03 = "Error opening package file."
      eError0x04 = 0x04, ///< 0x04 = "Error moving to EOF."
      eError0x05 = 0x05, ///< 0x05 = "Empty package file."
      eError0x06 = 0x06, ///< 0x06 = "Can't find package data."
      eError0x07 = 0x07, ///< 0x07 = "Can't allocate block buffer."
      eError0x08 = 0x08, ///< 0x08 = "Can't allocate signature buffer"
      eError0x09 = 0x09, ///< 0x09 = "Error moving to signature."
      eError0x0A = 0x0A, ///< 0x0A = "Error reading signature."
      eError0x0B = 0x0B, ///< 0x0B = "Can't allocate certificate buffer"
      eError0x0C = 0x0C, ///< 0x0C = "Error moving to certificate."
      eError0x0D = 0x0D, ///< 0x0D = "Error reading certificate."
      eError0x0E = 0x0E, ///< 0x0E = "Error moving to initial offset."
      eError0x0F = 0x0F, ///< 0x0F = "Error positioning over initial offset."
      eError0x10 = 0x10  ///< 0x10 = "Error reading block of package."
    };
    /**
     * @brief The only constructor possible.
     * @note This constructor just clean and initializes member variables, does not allocate memory.
     *
     * @param @b sFilename is the SPOT Package filename.
     * @param @b uInitialOffset is the initial offset for resume download operations.
     */
    AbsSpotPackage (const std::string & sFilename, std::size_t uInitialOffset = 0);

    /**
     * @brief Destructor
     * @note MUST BE VIRTUAL because this releases all the byte buffers and closes the file.
     */
    virtual ~AbsSpotPackage (void);

    /**
     * @brief Initializes this object and make it "usable" if no errors arise.
     * @note  This is a real "constructor" opening the file and allocating the buffers. This method also
     * call the two virtual methods setCertificateSize() and setSignatureSize() where derived class can
     * force these sizes according to their real needs.
     *
     * @param The block size to be kept constant during all the package download flow.
     *
     * @return @b false If an error of any kind arises and blocks the complete initialization.
     *         @b true  If everything goes OK and the initialization ends completely.
     */
    bool                   initialize (std::size_t uBlockSize);

   /**
    * @brief Positions into the file according to this supplied offset.
    * @note  This affects the starting point of successive readings done by getNextBlock().
    *
    * @param @b uInitialOffset is the new file position for further readings. It is zero for
    *           the most common restart operations, and has a positive value for resume ones.
    *
    * @return @b true  if the file position was successfully changed.
    *         @b false if any error has arisen moving the file pointer.
    */
    bool                   setInitialOffset(std::size_t uInitialOffset);

   /**
    * @brief Gets the certificate size. Consistent value is only returned after the initialization phase,
    *        when the derived classes have already set this value.
    *
    * @return @b The certificate size.
    */
    std::size_t            getCertificateSize (void) const;

   /**
    * @brief Gets the signature size. Consistent value is only returned after the initialization phase,
    *        when the derived classes have already set this value.
    *
    * @return @b The signature size.
    */
    std::size_t            getSignatureSize (void) const;

    /**
    * @brief Gets the certificate binary data. Consistent value is only returned after the initialization phase,
    *        when the derived classes have already the size of this data.
    *
    * @return @b The certificate size.
    */
    const unsigned char *  getCertificateData (void) const;

    /**
    * @brief Gets the signature binary data. Consistent value is only returned after the initialization phase,
    *        when the derived classes have already the size of this data.
    *
    * @return @b The certificate size.
    */
    const unsigned char *  getSignatureData (void) const;

    /**
    * @brief Gets the total length of the package file. Consistent value is only returned after the
    *        initialization phase, when the file is open and the real length is obtained
    *        from the file system.
    *
    * @return @b The total length of the package file.
    */
    std::size_t            fileLength (void) const; // whole file length

    /**
    * @brief Gets the length of the useful package data, excluding signature and certificate.
    *        Consistent value is only returned after the initialization phase,
    *        when the file is open and the real length is obtained from the file system.
    *
    * @return @b The length of the useful package data.
    */
    std::size_t            packageLength (void) const; // package data length

   /**
    * @brief Gets the data of a new block reading of the file. The reading is adjacent and upwards
    *        (regarding the last position of the file pointer) compared to the former one.
    *
    * @param [Out] @b uReadingSize Returns the quantity of bytes read. Should be set equal to the
    *                 value of the member variable m_uBlockSize (that is constant after construction),
    *                 unless the last block is being read or an error condition occurs.
    *
    * @return A reference to the private constant block buffer.
    *         This represent the data (bytes) of the last block reading.
    *
    * @see AbsSpotPackage::m_uBlockSize
    */
    const unsigned char *  getNextBlock (std::size_t & uReadingSize);

    /**
    * @brief Tells if this object is not usable because a past critical error.
    *        Some file operation errors are unrecoverable and this object is marked in error is those cases.
    *
    * @return @b true if this object is in error, @false if everything is OK.
    *
    * @see AbsSpotPackage::m_blError
    */
    bool                   hasErrors (void) const;


    /**
    * @brief Gets the internal error code related with the last operation producing an error.
    *        This is an internal code only used for debugging. This is used to avoid logging too much,
    *        and in multiple sections of this code. This number reflects an error exit situation along the code.
    *
    * @return @b true if this object is in error, @false if everything is OK.
    *
    * @see AbsSpotPackage::m_iLastErrorCode
    */
    int                    getLastOperationError (void) const;


  protected:

    /**
     * @brief Read and arbitrary block of file data, the block can be as big as the entire package file.
     *        This read does not disturb the sequence performed by getNextBlock() due this method restore
     *        the file pointer to the value it had got before this call.
     *
     * @note this method is just a service to the derived class. this is not used in the implementation of
     * this class.
     *
     * @param [Out] @b pOutputBuffer is the supplied output buffer to hold the reading. Should be at least
     *                 ulength bytes long.
     * @param [In]  @b uBeginning is the starting position of the file to begin reading.
     * @param [In]  @b ulength is the proposed quantity of bytes to read.
     *
     * @return The number of bytes read. This values could be less than ulength parameter if the
     *         reading finds an EOF or error condition.
     *
     * @see AbsSpotPackage::getNextBlock()
     */
    int                    peekBytes(unsigned char* pOutputBuffer,
                                     std::size_t uBeginning, std::size_t ulength);

    bool                   m_blError; ///< @brief The error flag that marks the object unusable.

    int                    m_iLastErrorCode; ///< @brief The error exit code of the last wrong operation.


  private:


    /**
    * @brief  Called during the initialization, derived and specialized classes have the chance to set
    *         the certificate field length.
    * @note   Derived classes MUST supply this information overriding this method.
    *
    * @return @b The certificate field length as better understood by a derived class.
    *
    * @see AbsSpotPackage::initialize()
    */
    virtual std::size_t    setCertificateSize (void) = 0;

    /**
    * @brief  Called during the initialization, derived and specialized classes have the chance to set
    *         the signature field length.
    * @note   Derived classes MUST supply this information overriding this method.
    *
    * @return @b The signature field length as better understood by a derived class.
    *
    * @see AbsSpotPackage::initialize()
    */
    virtual std::size_t    setSignatureSize   (void) = 0;

    bool                   m_blInitialized;     //< Flag to indicate this object is already initialized.
    FILE*                  m_pFile;             //< File stream pointer
    std::size_t            m_uInitialOffset;    //< Initial file offset for resume download operations.
    std::size_t            m_uBlockSize;        //< Block Size defined at construction.
    std::size_t            m_uCertificateSize;  //< Certificate size defined in derived class.
    std::size_t            m_uSignatureSize;    //< Signature size defined in derived class.
    std::size_t            m_uFileLength;       //< Package file length.
    const std::string      m_strFilename;       //< Package filename.
    unsigned char*         m_pBlockBuffer;      //< Buffer to hold the last block reading.
    unsigned char*         m_pCertificateData;  //< Certificate binary data.
    unsigned char*         m_pSignatureData;    //< Signature binary data.
};


} // namespace spot

} // namespace communication

} // namespace gvr



#endif /* ABSSPOTPACKAGE_H_ */

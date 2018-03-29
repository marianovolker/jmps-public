/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file TagLengthValueVector.h
 * @author Guillermo Paris
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef TAGLENGTHVALUEVECTOR_H_
#define TAGLENGTHVALUEVECTOR_H_

#include <vector>

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
 * @brief This class deals with basic TLV common functionality. That is a structure having this secuence:
 *        TagId (one byte data type), data length (1, 2 or 4 bytes) and binary data.
 * @note  TLV acronym stands for Tag Length Value
 * @see gvr.communication.spot.ISpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-18
 * @version 0.0.0, 2015-02-18
 * @author Guillermo Paris
 */
class TagLengthValueVector
{
  public:

    static const std::size_t CMaxVectorLength = 128; ///< @brief  aprox. 16Kb maximum total size
    static const std::size_t CMaxTLVLength = 1024;    ///< @brief  1024 bytes maximum per TLV data size

    class TagLengthValue
    {
      public:

       /**
        * @brief TLV default Constructor
        *
        * @since 0.0.0
        */
        TagLengthValue();

       /**
        * @brief TLV Constructor
        * @param tagID @b int .It is the data type or Tag ID.
        * @param length @b unsigned int .It is the data length.
        * @param pRawTLV @b byte* .It is the TLV's binary data.
        *
        * @since 0.0.0
        */
        TagLengthValue(int tagID, std::size_t length, const unsigned char* pRawTLV);

       /**
        * @brief TLV Copy Constructor
        *
        * @since 0.0.0
        */
        TagLengthValue(const TagLengthValue&);

       /**
        * @brief TLV Destructor
        *
        * @since 0.0.0
        */
        ~TagLengthValue();

       /**
        * @brief Returns if this object is in error due to a wrong past operation.
        *
        * @return @b true is this TLV is operable, @b false if this TLV is invalid due a wrong past operation.
        * @since 0.0.0
        */
        bool                  hasError(void)  const ;

       /**
        * @brief Returns the handled data type or Tag ID.
        *
        * @return TLV's TagId integer.
        * @since 0.0.0
        */
        int                   getTagID(void)  const ;

        /**
         * @brief Returns the handled data length.
         *
         * @return TLV's data length.
         * @since 0.0.0
         */
        std::size_t           getLength(void) const ;

        /**
         * @brief Returns the handled binary data.
         *
         * @return TLV's binary data as a constant byte buffer.
         * @since 0.0.0
         */
        const unsigned char*  getBytes(void)  const ;


      private:

       /**
        * @brief Forbidden assignment operator. This operation is hidden and made null.
        */
        TagLengthValue& operator=(TagLengthValue&);

       /**
        * @brief Forbidden assignment operator. This operation is hidden and made null.
        */
        TagLengthValue& operator=(const TagLengthValue&);

        bool            m_blError; ///<  @brief Error flag to invalidate further operations.
        int             m_iTagID;  ///<  @brief Data type ID.
        std::size_t     m_uLength; ///<  @brief Data Length.
        unsigned char*  m_pBytes;  ///<  @brief Binary Data.
    };


   /**
    * @brief Default constructor.
    *
    * @note Set the length size field to only one length byte, no items, clean vector.
    */
    TagLengthValueVector();

   /**
    * @brief vector Constructor
    * @param bLenSize @b byte .It is the length in bytes of all of the TLV's length fields.
    *                          In other words this says how long is the length of every tlv.length field.
    *
    * @since 0.0.0
    */
    TagLengthValueVector(unsigned char bLenSize);

   /**
    * @brief vector Constructor
    *
    * @param length @b unsigned int It is the number of elements present in the binary stream pRawTLVvector.
    *                               This parameter MUST BE the EXACT NUMBER of the TLV elements present in the
    *                               binary stream, or the parse method called by this constructor WILL FAIL.
    * @param bLenSize @b byte .It is the length in bytes of all of the TLV's length fields.
    *                          In other words this says how long is the length of every tlv.length field.
    * @param pRawTLVvector     The serialized TLV,TLV,TLV.... as present in some SPOT responses.
    *                          Parsing this byte stream the vector is constructed.
    * @since 0.0.0
    */
    TagLengthValueVector(std::size_t length, unsigned char lenSize, const unsigned char* pRawTLVvector);

   /**
    * @brief vector Destructor
    *
    * @since 0.0.0
    */
    ~TagLengthValueVector();

   /**
    * @brief Returns if this object is in error due to a wrong past operation.
    *
    * @return @b true is this vector is operable, @b false if this TLV is invalid due a wrong past operation.
    * @since 0.0.0
    */
    bool                  hasError(void)   const ;

   /**
    * @brief Gets the total number of items present in the vector.
    *
    * @return The number of the items present in the vector, that is the vector length.
    *
    */
    std::size_t           getLength(void)  const ;

    /**
     * @brief Gets a read only pointer to a TLV item.
     *
     * @param @b UINT uIndex  : The linear position of the item into the TLV array.
     *
     * @return The selected item as a read only TLV* , or NULL if the index is out of range.
     *
     * @see TagLengthValue
     */
    const TagLengthValue * getElement(std::size_t uIndex) const ;

    /**
     * @brief Gets a read only pointer to the first TLV item having a given ID.
     *
     * @param @b int id  : The given ID to search into the vector.
     *
     * @return The selected item having the requested ID as a read only TLV* , or NULL if the ID was not found.
     *
     * @see TagLengthValue
     */
    const TagLengthValue * getElementByID(int id) const ;

   /**
    * @brief Gets a read only pointer to the selected item binary data, as a byte array.
    *        Additionally it returns the data length and data type (tag ID) into [Out] parameters.
    *
    * @param @b [In]  UINT uIndex  : The linear position of the item into the TLV array.
    * @param @b [Out] UINT uTagID  : The tag ID (type of data) of the selected item.
    * @param @b [Out] UINT uLength : The binary data length of the selected item.
    *
    * @return The pointer to the selected data (byte* ), or NULL if there is not item data or the
    *         index is out of range.
    *
    * @see TagLengthValue
    */
    const unsigned char *  getElementData(std::size_t    uIndex,
                                          unsigned int & uTagID, std::size_t & uLength) const ;

   /**
    * @brief Gets a read only pointer to the selected item binary data, as a byte array.
    *        The returned element data correspond to the first element found having the requested TagID.
    *        Additionally it returns the index into the array and the data length. and data type (tag ID) into [Out] parameters.
    *
    * @param @b [In]  int  iTagID  : The tag ID (type of data) to search for.
    * @param @b [Out] int  index   : The linear position of the first found item into the TLV array, or -1 if not found.
    * @param @b [Out] UINT uLength : The binary data length of the selected item, or 0 if not found.
    *
    * @return The pointer to the selected data (byte* ), or NULL if the given ID was not found in the vector.
    *         index is out of range.
    *
    * @see TagLengthValue
    */
    const unsigned char *  getElementDataByID(int iTagID, std::size_t & index, std::size_t & uLength) const ;

   /**
    * @brief Clears completely this vector to a "brand new" empty one..
    *
    * @note All the contained items are deleted previously to clear the vector.
    *
    * @see destroyElements()
    */
    void clear();

   /**
    * @brief Adds a new fresh TLV element (item) to the vector.
    * @note  The TLV user element is copied (cloned), so the vector is always the owner of the data.
    *
    * @param @b tlv   : The reference to the item to be cloned and added into the TLV array.
    *
    * @return @b bool : @b true if the item was successfully copied and appended to the vector.
    *                   @b false Otherwise.
    *
    * @see TagLengthValue
    */
    bool addElement(const TagLengthValue& tlv);

   /**
    * @brief Adds a new fresh TLV element (item) to the vector.
    * @note  The element data is copied to the new added element inside the vector,
    *        so this vector element (TLV) is always the owner of its data.
    *
    * @param @b uTagID is the new element (TLV) tag ID
    * @param @b uItemLength is the length of the new element binary data.
    * @param @b pbData : The binary data for the item to be added into the TLV array.
    *
    * @return @b bool : @b true if the item was successfully copied and appended to the vector.
    *                   @b false Otherwise.
    *
    * @see TagLengthValue
    */
    bool addElement(unsigned int uTagID, std::size_t uItemLength, const unsigned char* pbData);

   /**
    * @brief Gets the size in bytes of an hypothetical buffer capable of holding the serialization
    *        of all the TLV items inside the vector.
    *
    * @param @b UINT uIndex  : The linear position of the item into the TLV array.
    *
    * @return The buffer size to hold the complete serialization of the vector.
    *
    * @see serialize()
    */
    std::size_t getSerializationSize(void);

    /**
     * @brief Serializes all the vector items to a binary buffer in the form TLV,TLV,TLV ....
     *
     * @param pBuffer @b byte is the buffer to hold the serialized TLV,TLV,TLV.... as present in some SPOT responses.
     *                    This buffer MUST BE LARGE ENOUGH to hold all the serialized items.
     * @return The total number of the bytes written to the serialization buffer. Must be equal to the number
     *         previously returned by getSerializationSize().
     *
     * @see getSerializationSize()
     *
     * @since 0.0.0
     */
    std::size_t serialize(unsigned char* pBuffer);

    /**
     * @brief Serializes all the vector items to a byte output vector, supplied by reference, in the form TLV,TLV,TLV ....
     *
     * @param outVector is the byte vector to hold the serialized TLV,TLV,TLV.... as present in some SPOT responses.
     *                    This vector is cleaned previously to copy all the serialized items.
     * @return The total number of the bytes written to the serialization vector. Must be equal to the number
     *         previously returned by getSerializationSize().
     *
     * @see getSerializationSize()
     *
     * @since 0.0.0
     */
    std::size_t serialize( std::vector<unsigned char> & outVector );

   /**
    * @brief TLV constructor helper method.
    * @param nItems @b int .It is the number of items to be added to the vector.
    * @param pRawTLVvector  It is the serialized TLV,TLV,TLV.... as present in some SPOT responses.
    *                       Parsing this byte stream the vector is constructed.
    * @since 0.0.0
    */
    bool parseSourceData(unsigned int nItems, const unsigned char* pRawTLVvector);


  private:

    /**
     * @brief Forbidden copy constructor. This operation is hidden and made null.
     */
    TagLengthValueVector(TagLengthValueVector&);

    /**
     * @brief Forbidden copy constructor. This operation is hidden and made null.
     */
    TagLengthValueVector(const TagLengthValueVector&);

   /**
    * @brief Forbidden assignment operator. This operation is hidden and made null.
    */
    TagLengthValueVector& operator=(TagLengthValueVector&);

    /**
     * @brief Forbidden assignment operator. This operation is hidden and made null.
     */
     TagLengthValueVector& operator=(const TagLengthValueVector&);

   /**
    * @brief Destroys all the TLV elements of the vector, freeing its memory.
    *
    * @see TagLengthValueVector::~TagLengthValueVector()
    */
    void destroyElements(void);


    bool                          m_blError;      ///< @brief Error flag to invalidate further operations.
    unsigned char                 m_bLengthSize;  ///< @brief Byte length of each TLV's length field.
    std::size_t                   m_uLength;      ///< @brief Number of elements of this vector.
    std::vector<TagLengthValue*>  m_TLVvector;    ///< @brief Underlying std::vector to collect the TLVs
};


} // namespace spot

} // namespace communication

} // namespace gvr

#endif // TAGLENGTHVALUEVECTOR


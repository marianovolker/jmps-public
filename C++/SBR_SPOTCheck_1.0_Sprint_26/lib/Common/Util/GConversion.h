// GConversion.h

#ifndef GConversion_h
#define GConversion_h 1

#include <GTypes.h>


//	The purpose of this class is to reduce duplication of code and provide a
//	common point for everyone to access data conversion methods.

class GConversion
{
  public:

      //    Name: UIToA
      //
      //	Purpose: Convert an *UNSIGNED* 4 byte integer value to an ASCII base 10 
      //    string (NULL terminated) representation. The ASCII representation is left
      //    justified in the specified buffer.
      //
      //	Arguments:
      //
      //	u32Val:  the integer value to be converted to ASCII.
      //
      //	pcBuffer:  character buffer that will be filled with the NULL terminated
      //	ASCII decimal representation of u32Val.  The size of this buffer **MUST BE
      //	>= THE NUMBER OF DIGITS IN u32Val + 1** (+1 for the NULL terminator).  The
      //	buffer size needed to store the maximum UINT32 value is 11, including the
      //	NULL terminator.
      static void UIToA (UINT32 u32Val, char* pcBuffer);

      //	=============================================================================
      //
      //	 Name:	ASCIIHexDigitToDecimal
      //
      //	 Purpose:	Convert an ASCII hex digit to its decimal value.
      //
      //	 Returns:	The decimal value corresponding to the ASCII Hex digit passed in.
      //			If an invalid character (not corresponding to a hex digit) is passed in,
      //			the value 0 is returned.
      //
      //	 Inputs:	An ASCII hex digit {byASCIIDigit : '0'-'9', 'A'-'F', 'a'-'f'}.
      //
      //	=============================================================================
      static UINT8 ASCIIHexDigitToDecimal (UINT8 byASCIIDigit);

      //	=============================================================================
      //
      //	 Name:	ASCIIHexByteToDecimal
      //
      //	 Purpose:	Convert an ASCII hex byte to its decimal value.
      //
      //	 Returns:	The decimal value corresponding to the ASCII Hex byte passed in.
      //			If an invalid character (not corresponding to a hex digit) is passed in
      //			as part one of the hex digits in the input, the value 0 will be used for
      //			that digit.
      //
      //	 Inputs:	A pointer to two ASCII hex digits.  The hex digits must come from
      //			the set {'0'-'9', 'A'-'F', 'a'-'f'}.  The digits in argument
      //			pbyASCIIHexByte are are interpreted as follows:
      //
      //			pbyASCIIHexByte[0] = MSD
      //			pbyASCIIHexByte[1] = LSD
      //
      //	 Notes:	This routine ASSUMES THAT ARGUMENT pbyASCIIHexByte POINTS TO TWO
      //			HEX DIGITS.  If argument pbyASCIIHexByte does not point to at least
      //			two ASCII hex digits A CORE DUMP (OR SEG FAULT) IS SURE TO FOLLOW!!!
      //
      //	=============================================================================
      static UINT8 ASCIIHexByteToDecimal (UINT8* pbyASCIIHexByte);

      /**
       * Convert string in Ascii-Hex (base 16) to byte[ ]. <p>
       * Each group of two consecutives hex digits can be followed by a separator character,
       * to make the string more readable. Moreover, a separator can be placed between hex
       * nibbles (very weird but allowed) because any non hex digit character is simply discarded.
       *
       * Example:
       *    Input:  "01 23 45 67 89 AB CD EF"
       *    Output: {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}
       *
       * @param byte* : out_pu8Buffer A pointer to a supplied buffer with enough length
       *                              to hold the conversion.
       * @param const char* : sHexData The string to convert.
       * @return The quantity of bytes converted.
       */
       static UINT32 ASCIIHexStringToBytes(UINT8* out_pu8Buffer, const char* szHexData);

       //  ===========================================================================
       //
       //    Name:  BytesToASCIIHexString
       //
       //    Purpose:  Takes bytes in pairs from pu8Buffer and converts them into
       //              the byte each pair represents, leaving those bytes in
       //              pcbuffer.
       //
       //    Inputs:   pu8Buffer   			Input bytes.
       //
       //              u8BufferLen  		The length of the pu8Buffer.
       //
       //              pcbuffer 			The output buffer for the conversion. Note
       //                                  	this buffer must be previously allocated
       //                                  	and its size must be at least of u8BufferLen * 3.
       //
       //  ===========================================================================
       static char* BytesToASCIIHexString(char* pcbuffer, const UINT8* pu8Buffer, const UINT16 u8BufferLen);

       //  ===========================================================================
       //
       //    Name:  PrintBytesAsASCIIHexString
       //
       //    Purpose:  Prints the buffer as hex string.
       //
       //    Inputs:   pu8Buffer   			Input bytes.
       //
       //              u8BufferLen  		The length of the pu8Buffer.
       //
       //
       //  ===========================================================================
       static void LogBytesAsASCIIHexString(const UINT8* pu8Buffer, const UINT16 u8BufferLen);

      /**
       * Convert the hex nibble inside the input character to a single output byte.
       * If the character is an invalid hex digit (not in the range 0..9, A..F or a..f)
       * then an invalid 0xFF result is returned.
       */
       static UINT8 HexNibbleToByte(char cDigit);

      /**
       * Returns true if the input character is a valid hex digit (in the range 0..9, A..F or a..f)
       * Otherwise a false value is returned.
       */
       static bool isHexDigit(char cDigit);

      //	=============================================================================
      //
      //	 Name:	ConvertToASCII
      //
      //	 Purpose: The purpose of this method is to convert a  nibble from the input
      //	          byte  to an  ASCII character. An input flag is used to dictate if
      //              the upper or lower nibble of the input byte is converted.
      //
      //	 INPUT: INT8  cByteToConvert - This is the byte of data that will be converted
      //	                               to two ASCII characters. The most significant
      //	                               (upper) nibble will be converted	first, followed
      //	                               by the least significant nibble on the next call
      //	                               to this routine (i.e. this routine is called
      //	                               twice to convert one byte to two ascii chars).
      //
      //	      	bool bUpperNibble - is true if the most significant nibble is to be
      //	               	            converted to an ascii character and false if the
      //	                            least significant nibble is to be converted to an
      //	                            ascii character.
      //
      //	OUTPUT: INT8 - The portion (upper or lower nibble of byte) of the byte that is
      //	               converted to ASCII.
      //
      //	EXAMPLE If in put where:
      //
      //			cByteToConvert = 0x45
      //			bUpperNibble   = true
      //
      //	        Output (returned value) = '4'
      //
      //	=============================================================================
      static INT8 ConvertToASCII (INT8 cByteToConvert, bool bUpperNibble);

      //    Name: ConvertToDecimalASCII
      //
      //	It converts an UINT32 number into its printable ASCII representation in
      //	decimal (or base 10).
      //
      //	Arguments:
      //	   u32Number - The number to be converted
      //	   pu8ASCIIBuffer - The output buffer to be filled with ASCII codes
      //	   u8ASCIIBufferLength - the length of pu8ASCIIBuffer
      //
      //	Returns:
      //	    true if valid ASCII code is copied into the buffer.
      static bool ConvertToDecimalASCII (UINT32 u32Number, UINT8* pu8ASCIIBuffer, UINT8 u8ASCIIBufferLength);

      //	===========================================================================
      //
      //	  Name:  BuildUINT16
      //
      //	  Purpose:  Build an UINT16 using the next 2 bytes at the given address.
      //
      //	  Inputs:   pbyData   A pointer to the address to fetch the bytes from. The
      //	                      bytes are expected to come in LSB-MSB order.
      //
      //	  Returns:  The UINT16.
      //
      //	===========================================================================
      static UINT16 BuildUINT16 (UINT8* pbyData);

      //	===========================================================================
      //
      //	  Name:  BuildUINT32
      //
      //	  Purpose:  Build an UINT32 using the next 4 bytes at the given address.
      //
      //	  Inputs:   pbyData   A pointer to the address to fetch the bytes from. The
      //	                      bytes are expected to come in LSB-MSB order.
      //
      //	  Returns:  The UINT32.
      //
      //	===========================================================================
      static UINT32 BuildUINT32 (UINT8* pbyData);


      // Endian conversion routines.
      //
      // These routines convert multibyte value data between host byte order and
      // desired endian order.
      static UINT16 HostToLittleEndian(UINT16 Data);
      static UINT16 LittleEndianToHost(UINT16 Data);

      static UINT32 HostToLittleEndian(UINT32 Data);
      static UINT32 LittleEndianToHost(UINT32 Data);

      static UINT16 HostToBigEndian(UINT16 Data);
      static UINT16 BigEndianToHost(UINT16 Data);

      static UINT32 HostToBigEndian(UINT32 Data);
      static UINT32 BigEndianToHost(UINT32 Data);


      //  ===========================================================================
      //
      //    Name:  ConvertHexBufferToDecimalBuffer
      //
      //    Purpose:  Takes bytes in pairs from pu8HexBuffer and converts them into
      //              the byte each pair represents, leaving those bytes in
      //              pu8OutDecimalBuffer.
      //
      //    Inputs:   pu8HexBuffer   A buffer of bytes ranging '0'-'9', 'A'-'F' or
      //                             'a'-'f'. Each pair is the hex representation of
      //                             a byte, from 0 to 255.
      //
      //              u32HexBufferLength  The length of the pu8HexBuffer.
      //
      //              pu8OutDecimalBuffer The output buffer for the conversion. Note
      //                                  this buffer must be previously allocated
      //                                  and its size must be at least the half of
      //                                  u32HexBufferLength.
      //
      //  ===========================================================================
      static void ConvertHexBufferToDecimalBuffer(const UINT8* pu8HexBuffer, UINT32 u32HexBufferLength, UINT8* pu8OutDecimalBuffer);


      //  ===========================================================================
      //
      //    Name:  ConvertDecimalBufferToHexBuffer
      //
      //    Purpose:  Takes each byte from pu8DecimalBuffer and converts it into its
      //              hex representation with two bytes, each of them.ranging
      //              '0'-'9', 'A'-'F' or 'a'-'f'.
      //
      //    Inputs:   pu8DecimalBuffer        A buffer of bytes between 0 and 255.
      //
      //              u32DecimalBufferLength  The length of the pu8DecimalBuffer.
      //
      //              pu8OutHexBuffer     The output buffer for the conversion. Note
      //                                  this buffer must be previously allocated
      //                                  and its size must be at least twice
      //                                  u32DecimalBufferLength.
      //
      //  ===========================================================================
      static void ConvertDecimalBufferToHexBuffer(const UINT8* pu8DecimalBuffer, UINT32 u32DecimalBufferLength, UINT8* pu8OutHexBuffer);

      static void xtoa (unsigned long val, char *buf, unsigned radix, int is_neg);


  private:
      GConversion(const GConversion &right);
      GConversion & operator=(const GConversion &right);

};

#endif


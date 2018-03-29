/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file Conversion.cpp
 * @author Guillermo Paris
 * @date Monday, March 03, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/Conversion.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const uint32_t Conversion::CTestEndianness = 0x11223344;

bool Conversion::isBigEndian()
{
	return (reinterpret_cast<const unsigned char*>(& CTestEndianness))[0] == 0x11 ? true : false;
}

bool Conversion::isLittleEndian()
{
	return (reinterpret_cast<const unsigned char*>(& CTestEndianness))[0] == 0x44 ? true : false;
}

bool Conversion::isMiddleEndian()
{
	return !isLittleEndian() && !isBigEndian();
}

uint32_t Conversion::littleToBigEndian(uint32_t u32InputNumber,
                                       uint8_t* pbOutputByteRepresentation /*= NULL */)
{
  return changeEndianness(u32InputNumber, pbOutputByteRepresentation);
}

uint32_t Conversion::bigToLittleEndian(uint32_t u32InputNumber,
                                       uint8_t* pbOutputByteRepresentation /*= NULL */)
{
  return changeEndianness(u32InputNumber, pbOutputByteRepresentation);
}

uint32_t Conversion::changeEndianness(uint32_t u32InputNumber,
                                      uint8_t* pbOutputByteRepresentation /*= NULL */)
{
  uint8_t abRetNum[4]; // byte array with the result number in opposite Endian Format
  if( pbOutputByteRepresentation == NULL )
  {
    abRetNum[0] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[3];
    abRetNum[1] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[2];
    abRetNum[2] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[1];
    abRetNum[3] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[0];
  }
  else
  {
    abRetNum[0] = pbOutputByteRepresentation[0] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[3];
    abRetNum[1] = pbOutputByteRepresentation[1] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[2];
    abRetNum[2] = pbOutputByteRepresentation[2] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[1];
    abRetNum[3] = pbOutputByteRepresentation[3] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[0];
  }

  return * reinterpret_cast<uint32_t*>(abRetNum);
}

uint32_t Conversion::uint32ToBigEndian(uint32_t u32InputNumber,
                                       uint8_t* pbOutputByteRepresentation /*= NULL */)
{
  if( (reinterpret_cast<const unsigned char*>(& CTestEndianness))[0] == 0x11 ) // Is big endian machine ?
  {
    if( pbOutputByteRepresentation != NULL )
    {
      pbOutputByteRepresentation[0] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[0];
      pbOutputByteRepresentation[1] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[1];
      pbOutputByteRepresentation[2] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[2];
      pbOutputByteRepresentation[3] = (reinterpret_cast<uint8_t*>(& u32InputNumber))[3];
    }
    return u32InputNumber;
  }
  else if( (reinterpret_cast<const unsigned char*>(& CTestEndianness))[0] == 0x44 ) // Is little endian machine ?
  {
    return changeEndianness( u32InputNumber, pbOutputByteRepresentation );
  }
  else // middle endian or undefined, mathematics must be used in this case.
  {
    uint32_t u32LenH = u32InputNumber / (256 * 256); // upper 16 bits of the item length.
    uint32_t u32LenL = u32InputNumber % (256 * 256); // lower 16 bits of the item length.
    uint8_t  abBENum[4]; // byte array with the result number in Big Endian Format

    if( pbOutputByteRepresentation == NULL )
    {
      abBENum[0] = static_cast<uint8_t> (u32LenH / 256); // upper 8 bits of the item length.
      abBENum[1] = static_cast<uint8_t> (u32LenH % 256);
      abBENum[2] = static_cast<uint8_t> (u32LenL / 256);
      abBENum[3] = static_cast<uint8_t> (u32LenL % 256); // lower 8 bits of the item length.
    }
    else
    {
      pbOutputByteRepresentation[0] = abBENum[0] = static_cast<uint8_t> (u32LenH / 256); // upper 8 bits of the item length.
      pbOutputByteRepresentation[1] = abBENum[1] = static_cast<uint8_t> (u32LenH % 256);
      pbOutputByteRepresentation[2] = abBENum[2] = static_cast<uint8_t> (u32LenL / 256);
      pbOutputByteRepresentation[3] = abBENum[3] = static_cast<uint8_t> (u32LenL % 256); // lower 8 bits of the item length.
    }
    return * reinterpret_cast<uint32_t*>(abBENum);
  }
}


} // namespace spot

} // namespace communication

} // namespace gvr


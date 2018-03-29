/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file Conversion.h
 * @author Guillermo Paris
 * @date Monday, March 03, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef CONVERSION_H_
#define CONVERSION_H_

#include <stdint.h>
#include <stddef.h>

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
 * @brief This class is a STATIC one that encompasses all the miscellaneous functions about conversions,
 *        in spot module. The aim of this class is stand alone function grouping.
 *
 * @note  Everyone is invited to populate miscellaneous conversion functions here, provided that these
 *        are used inside the spot module.
 *
 * @since 0.0.0, 2015-03-03
 * @version 0.0.0, 2015-03-03
 * @author Guillermo Paris
 */
class Conversion
{
  public:

   /**
    * @brief Tells if the machine running this code is Big Endian.
    *
    * @return @b true if this machine is Big Endian, @b false otherwise.
    */
    static bool isBigEndian();

   /**
    * @brief Tells if the machine running this code is Little Endian.
    *
    * @return @b true if this machine is Little Endian, @b false otherwise.
    */
    static bool isLittleEndian();

   /**
    * @brief Tells if the machine running this code is neither Big Endian nor Little Endian.
    *
    * @return @b false if this machine is Big Endian or Little Endian, @b true otherwise.
    */
    static bool isMiddleEndian();

   /**
    * @brief Converts a 32 bits integer to an equivalent arithmetic value number but represented in
    *        memory as a Big Endian byte sequence.
    * @note  If the machine where this code runs is already big endian, then the returned number
    *        is identical to the supplied one ( u32InputNumber ).
    *
    * @param [In]  @b u32InputNumber is the number to convert.
    * @param [Out] @b pbOutputCharRepresentation is an optional conversion output to byte array.
    *
    * @return An equivalent arithmetic value number but with Big Endian layout in memory.
    *         If the machine where this code runs is already big endian, then the returned number
    *         is identical to the supplied one ( u32InputNumber ).
    */
    static uint32_t uint32ToBigEndian(uint32_t u32InputNumber, uint8_t* pbOutputCharRepresentation = NULL);

   /**
    * @brief Converts an assumed little endian 32 bits integer to an equivalent arithmetic value number but
    *        represented in memory as a Big Endian byte sequence.
    *
    * @param [In]  @b u32InputNumber is the assumed little endian number to convert.
    * @param [Out] @b pbOutputCharRepresentation is an optional conversion output to byte array.
    *
    * @return An equivalent arithmetic value number but with Big Endian layout in memory.
    *         The CONVERSION IS ALWAYS PERFORMED since the input is little endian and the output is
    *         big endian number. So input and output are different except for particular symmetric values.
    */
    static uint32_t littleToBigEndian(uint32_t u32InputNumber, uint8_t* pbOutputCharRepresentation = NULL);

   /**
    * @brief Converts an assumed big endian 32 bits integer to an equivalent arithmetic value number but
    *        represented in memory as a Little Endian byte sequence.
    *
    * @param [In]  @b u32InputNumber is the assumed big endian number to convert.
    * @param [Out] @b pbOutputCharRepresentation is an optional conversion output to byte array.
    *
    * @return An equivalent arithmetic value number but with Little Endian layout in memory.
    *         The CONVERSION IS ALWAYS PERFORMED since the input is big endian and the output is
    *         little endian number. So input and output are different except for particular symmetric values.
    */
    static uint32_t bigToLittleEndian(uint32_t u32InputNumber, uint8_t* pbOutputCharRepresentation = NULL);


  private:

   /**
    * @brief Converts a 32 bits integer to an equivalent arithmetic value number but
    *        represented in memory as the Opposite Endianness byte sequence.
    *
    * @param [In]  @b u32InputNumber is the number to convert.
    * @param [Out] @b pbOutputCharRepresentation is an optional conversion output to byte array.
    *
    * @return An equivalent arithmetic value number but with the Opposit Endianness layout in memory.
    *         The CONVERSION IS ALWAYS PERFORMED since the input and output has opposite endianness.
    *         So input and output are different except for particular symmetric values.
    */
    static uint32_t changeEndianness(uint32_t u32InputNumber, uint8_t* pbOutputByteRepresentation = NULL);


   /**
    * @brief A constant non symmetric value to be used as "test instrument" to determine
    *        the endianess of the machine running this code.
    *
    * @note The value 0x11223344 is deliberately assigned to this test integer.
    *
    */
    static const uint32_t  CTestEndianness; // = 0x11223344;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* CONVERSION_H_ */


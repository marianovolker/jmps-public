/*
 * ©2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * GConversionUT.h
 * Created on: Feb 01 2013
 *     Author: gparis
 */

#include "gtest/gtest.h"
#include "Common/Util/GConversion.h"


TEST(GConversion, HexDigitCharacter)
{
	EXPECT_TRUE(GConversion::isHexDigit('0'));
	EXPECT_TRUE(GConversion::isHexDigit('5'));
	EXPECT_TRUE(GConversion::isHexDigit('9'));
	EXPECT_TRUE(GConversion::isHexDigit('A'));
	EXPECT_TRUE(GConversion::isHexDigit('C'));
	EXPECT_TRUE(GConversion::isHexDigit('F'));
	EXPECT_TRUE(GConversion::isHexDigit('a'));
	EXPECT_TRUE(GConversion::isHexDigit('d'));
	EXPECT_TRUE(GConversion::isHexDigit('f'));
	EXPECT_FALSE(GConversion::isHexDigit(' '));
	EXPECT_FALSE(GConversion::isHexDigit('+'));
	EXPECT_FALSE(GConversion::isHexDigit('-'));
	EXPECT_FALSE(GConversion::isHexDigit('*'));
	EXPECT_FALSE(GConversion::isHexDigit('/'));
	EXPECT_FALSE(GConversion::isHexDigit('G'));
	EXPECT_FALSE(GConversion::isHexDigit('H'));
	EXPECT_FALSE(GConversion::isHexDigit('g'));
	EXPECT_FALSE(GConversion::isHexDigit('h'));
}

TEST(GConversion, NibbleToByte)
{
	EXPECT_EQ(0, GConversion::HexNibbleToByte ('0'));
	EXPECT_EQ(1, GConversion::HexNibbleToByte ('1'));
	EXPECT_EQ(2, GConversion::HexNibbleToByte ('2'));
	EXPECT_EQ(3, GConversion::HexNibbleToByte ('3'));
	EXPECT_EQ(4, GConversion::HexNibbleToByte ('4'));
	EXPECT_EQ(5, GConversion::HexNibbleToByte ('5'));
	EXPECT_EQ(6, GConversion::HexNibbleToByte ('6'));
	EXPECT_EQ(7, GConversion::HexNibbleToByte ('7'));
	EXPECT_EQ(8, GConversion::HexNibbleToByte ('8'));
	EXPECT_EQ(9, GConversion::HexNibbleToByte ('9'));
	EXPECT_EQ(10, GConversion::HexNibbleToByte ('A'));
	EXPECT_EQ(11, GConversion::HexNibbleToByte ('B'));
	EXPECT_EQ(12, GConversion::HexNibbleToByte ('C'));
	EXPECT_EQ(13, GConversion::HexNibbleToByte ('D'));
	EXPECT_EQ(14, GConversion::HexNibbleToByte ('E'));
	EXPECT_EQ(15, GConversion::HexNibbleToByte ('F'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte (' '));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('i'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('y'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('L'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('Z'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('+'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('-'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('*'));
	EXPECT_EQ(255, GConversion::HexNibbleToByte ('/'));
	EXPECT_EQ(10, GConversion::HexNibbleToByte ('a'));
	EXPECT_EQ(11, GConversion::HexNibbleToByte ('b'));
	EXPECT_EQ(12, GConversion::HexNibbleToByte ('c'));
	EXPECT_EQ(13, GConversion::HexNibbleToByte ('d'));
	EXPECT_EQ(14, GConversion::HexNibbleToByte ('e'));
	EXPECT_EQ(15, GConversion::HexNibbleToByte ('f'));
}

TEST(GConversion, ASCIIHexStringToBytes)
{
	UINT8  aryu8Bytes[20];
	memset(aryu8Bytes, 0, 20);
	GConversion::ASCIIHexStringToBytes(aryu8Bytes,
	                                   "01 23 4 5|6 7 + 8,9  A...B  C----DEFaxxx0b1c2d3e4f5 ab cd ef");

	EXPECT_EQ(0x01, aryu8Bytes[0]);
	EXPECT_EQ(0x23, aryu8Bytes[1]);
	EXPECT_EQ(0x45, aryu8Bytes[2]);
	EXPECT_EQ(0x67, aryu8Bytes[3]);
	EXPECT_EQ(0x89, aryu8Bytes[4]);
	EXPECT_EQ(0xAB, aryu8Bytes[5]);
	EXPECT_EQ(0xCD, aryu8Bytes[6]);
	EXPECT_EQ(0xEF, aryu8Bytes[7]);
	EXPECT_EQ(0xA0, aryu8Bytes[8]);
	EXPECT_EQ(0xB1, aryu8Bytes[9]);
	EXPECT_EQ(0xC2, aryu8Bytes[10]);
	EXPECT_EQ(0xD3, aryu8Bytes[11]);
	EXPECT_EQ(0xE4, aryu8Bytes[12]);
	EXPECT_EQ(0xF5, aryu8Bytes[13]);
	EXPECT_EQ(0xAB, aryu8Bytes[14]);
	EXPECT_EQ(0xCD, aryu8Bytes[15]);
	EXPECT_EQ(0xEF, aryu8Bytes[16]);
}



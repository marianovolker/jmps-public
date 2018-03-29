/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file utils_unittest.cpp
 * @author Guillermo Paris
 * @date Monday, February 23, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gtest/gtest.h>
#include "../include/gvrspot_unittest/utils_unittest.h"
#include <algorithm>
#include <iostream>

using namespace gvr::communication::spot;

TEST(GUtilsTestUT, TestTagLengthValueObject)
{
  // Check default construction
  TagLengthValueVector::TagLengthValue tlv0;
  EXPECT_FALSE( tlv0.hasError() );
  EXPECT_EQ( tlv0.getTagID(), 0 );
  EXPECT_EQ( tlv0.getLength(), 0 );
  EXPECT_EQ( tlv0.getBytes(), (unsigned char*) 0 );

  // Check full construction
  const unsigned char abTestData[5] = { 0x10, 0x11, 0x12, 0x13, 0x14 };
  TagLengthValueVector::TagLengthValue tlv(0xF1, 5, abTestData);
  EXPECT_EQ( tlv.getTagID(), 0xF1 );
  EXPECT_EQ( tlv.getLength(), 5 );
  EXPECT_TRUE( std::equal( abTestData, abTestData + 5, tlv.getBytes() ) );

  // Check construction limits
  TagLengthValueVector::TagLengthValue tlvHuge(0xFF, 1025, abTestData);
  EXPECT_TRUE( tlvHuge.hasError() ); // wrongly constructed, too big :)
  EXPECT_EQ( tlv.getTagID(), 0xF1 );
  EXPECT_EQ( tlv.getLength(), 5 );
  EXPECT_TRUE( std::equal( abTestData, abTestData + 5, tlv.getBytes() ) );

  // Check copy construction
  TagLengthValueVector::TagLengthValue tlv1(tlv);
  EXPECT_EQ( tlv.getTagID(), 0xF1 );
  EXPECT_EQ( tlv.getLength(), 5 );
  EXPECT_TRUE( std::equal( abTestData, abTestData + 5, tlv.getBytes() ) );
}

TEST(GUtilsTestUT, TestTagLengthValueVector)
{
  // Check default construction
  TagLengthValueVector tlvVector0;
  EXPECT_FALSE( tlvVector0.hasError() );
  EXPECT_EQ( tlvVector0.getLength(), 0 );

  // Check bad parameter construction
  TagLengthValueVector tlvBadVector(3);
  EXPECT_TRUE( tlvBadVector.hasError() );
  EXPECT_EQ( tlvBadVector.getLength(), 0 );

  const unsigned char abTestData[22] =
  {
    0xF1, 0x05, 0x10, 0x11, 0x12, 0x13, 0x14,
    0xF2, 0x04, 0x20, 0x21, 0x22, 0x23,
    0xF3, 0x03, 0x30, 0x31, 0x32,
    0xF4, 0x02, 0x40, 0x41
  };

  // Check construction limits
  TagLengthValueVector tlvHugeVector(130, 2, abTestData);
  EXPECT_TRUE( tlvHugeVector.hasError() ); // wrongly constructed, too big :)
  EXPECT_EQ( tlvHugeVector.getLength(), 0 );

  // Check full construction
  TagLengthValueVector tlvVector(4, 1, abTestData);
  EXPECT_FALSE( tlvVector.hasError() );
  EXPECT_EQ( tlvVector.getLength(), 4 );

  // check first element
  const char abTest1st[5] = { 0x10, 0x11, 0x12, 0x13, 0x14 };
  unsigned int uTagID1  = 0;
  std::size_t  uLength1 = 0;
  const unsigned char* pData1 = tlvVector.getElementData(0, uTagID1, uLength1);
  EXPECT_EQ( uTagID1, 0xF1);
  EXPECT_EQ( uLength1,   5);
  EXPECT_TRUE( std::equal( abTest1st, abTest1st + 5, pData1 ) );

  // check second element
  const char abTest2nd[4] = { 0x20, 0x21, 0x22, 0x23 };
  unsigned int uTagID2  = 0;
  std::size_t  uLength2 = 0;
  const unsigned char* pData2 = tlvVector.getElementData(1, uTagID2, uLength2);
  EXPECT_EQ( uTagID2, 0xF2);
  EXPECT_EQ( uLength2,   4);
  EXPECT_TRUE( std::equal( abTest2nd, abTest2nd + 4, pData2 ) );

  // check third element
  const TagLengthValueVector::TagLengthValue*  pTLV3 = tlvVector.getElement(2);
  EXPECT_FALSE( pTLV3->hasError() );
  EXPECT_EQ( pTLV3->getTagID(), 0xF3);
  EXPECT_EQ( pTLV3->getLength(), 3);
  const unsigned char* pData3 = pTLV3->getBytes();
  EXPECT_EQ( pData3[0], 0x30);
  EXPECT_EQ( pData3[1], 0x31);
  EXPECT_EQ( pData3[2], 0x32);

  // check fourth element
  const TagLengthValueVector::TagLengthValue*  pTLV4 = tlvVector.getElement(3);
  EXPECT_FALSE( pTLV4->hasError() );
  EXPECT_EQ( pTLV4->getTagID(), 0xF4);
  EXPECT_EQ( pTLV4->getLength(), 2);
  const unsigned char* pData4 = pTLV4->getBytes();
  EXPECT_EQ( pData4[0], 0x40);
  EXPECT_EQ( pData4[1], 0x41);

  // check all the bytes accumulated so far
  unsigned char abTestDataBis[44]; // enough room for next test and the following
  EXPECT_EQ( tlvVector.getSerializationSize(), 22);
  EXPECT_EQ( tlvVector.serialize(abTestDataBis), 22);
  EXPECT_TRUE( std::equal( abTestData, abTestData + 22, abTestDataBis ) );

  // add four more elements, and duplicate the serialized size
  tlvVector.addElement(uTagID1, uLength1, pData1);
  tlvVector.addElement(uTagID2, uLength2, pData2);
  tlvVector.addElement(*pTLV3);
  tlvVector.addElement(*pTLV4);

  // check the new vector length: 8
  EXPECT_EQ( tlvVector.getLength(), 8);

  // check the new serialized size and bytes
  EXPECT_EQ( tlvVector.getSerializationSize(), 44);
  EXPECT_EQ( tlvVector.serialize(abTestDataBis), 44);
  EXPECT_TRUE( std::equal( abTestData, abTestData + 22, abTestDataBis ) );      // first half
  EXPECT_TRUE( std::equal( abTestData, abTestData + 22, abTestDataBis + 22) );  // second "mirrored" half
}


// uploadable-logs.cfg table

// UploadableFileList unit tests

static const std::size_t uM3TotalRecords = 33;
static const std::size_t uM3TotalCurrent = 17;
static const std::size_t uM5TotalRecords = 43;
static const std::size_t uM5TotalCurrent = 23;
static const std::size_t uM7TotalRecords = 48;
static const std::size_t uM7TotalCurrent = 22;

static int find( const std::vector<std::string> & v, const std::string s)
{
  for( int i = 0; i < v.size(); i++ )
  {
    if( v[i] == s ) return i;
  }
  return -1;
}

static bool verifyAllCurrent( UploadableFileList::RequestedFilesVector v )
{
  if( v.size() == 0) return false;

  for( int i = 0; i < v.size(); i++ )
  {
    if( v[i].m_attrib.b_age != UploadableFileList::eCurrent ) return false;
  }

  return true;
}

TEST(GUtilsTestUT, TestUploadableFileListM3)
{
  // Check DEFAULT construction for M5
  std::vector<std::string>                 vKeys;
  UploadableFileList::RequestedFilesVector vAll, vAllCurrent, vCrind;
  UploadableFileList uloadableLogList("upload_conf/uploadable-logs.cfg", "m3");

  EXPECT_FALSE( uloadableLogList.hasErrors() );
  EXPECT_EQ( uloadableLogList.getTotalOfRecords(), uM3TotalRecords );

  EXPECT_FALSE( uloadableLogList.isSupported("TWOWIRE_CURR") ); // is an M7 log
  EXPECT_TRUE(  uloadableLogList.isSupported("SYSMANSERV_CURR") );

  EXPECT_TRUE( uloadableLogList.getSupportedFilenameList(vKeys));
  EXPECT_EQ( vKeys.size() , uM3TotalRecords);
  EXPECT_FALSE( find(vKeys, "DIAGCOUNTERS_BOTH") >= 0 ); // is an M7 log
  EXPECT_TRUE(  find(vKeys, "ABI_HIST") >= 0 );

  uloadableLogList.getRequeriedFileList( "CRINDBIOS_CURR",
                                         UploadableFileList::eNoSpecified, -1, -1, vCrind );
  EXPECT_EQ( vCrind.size(), 1 );
  EXPECT_TRUE( vCrind[0].m_sFilename == "CRINDBIOS_CURR" );
  EXPECT_TRUE( vCrind[0].m_attrib.s_FilePath.empty() ); // not appliable, is a SPOT protocol log
  EXPECT_TRUE( vCrind[0].m_attrib.b_age == UploadableFileList::eCurrent );
  EXPECT_TRUE( vCrind[0].m_attrib.b_type == 0x71 );
  EXPECT_TRUE( vCrind[0].m_attrib.b_fileId == 0x00 );

  uloadableLogList.getRequeriedFileList( "ALL",
                                         UploadableFileList::eNoSpecified, -1, -1, vAll );
  EXPECT_EQ( vAll.size(), uM3TotalRecords );

  uloadableLogList.getRequeriedFileList( "ALL_CURRENT",
                                         UploadableFileList::eNoSpecified, -1, -1, vAllCurrent );
  EXPECT_EQ( vAllCurrent.size(), uM3TotalCurrent );
  EXPECT_TRUE( verifyAllCurrent(vAllCurrent) );
}

TEST(GUtilsTestUT, TestUploadableFileListM5)
{
  // Check DEFAULT construction for M5
  std::vector<std::string>                 vKeys;
  UploadableFileList::RequestedFilesVector vAll, vAllCurrent, vCrind, vPinpad;
  UploadableFileList uloadableLogList("upload_conf/uploadable-logs.cfg");

  EXPECT_FALSE( uloadableLogList.hasErrors() );
  EXPECT_EQ( uloadableLogList.getTotalOfRecords(), uM5TotalRecords );

  EXPECT_FALSE( uloadableLogList.isSupported("TWOWIRE_CURR") ); // is an M7 log
  EXPECT_TRUE(  uloadableLogList.isSupported("CRINDBIOSVERSIONS_CURR") );

  EXPECT_TRUE( uloadableLogList.getSupportedFilenameList(vKeys));
  EXPECT_EQ( vKeys.size() , uM5TotalRecords);
  EXPECT_FALSE( find(vKeys, "DIAGCOUNTERS_BOTH") >= 0 ); // is an M7 log
  EXPECT_TRUE(  find(vKeys, "DISPLAYMGR_BOTH") >= 0 );

  uloadableLogList.getRequeriedFileList( "CRINDBIOSVERSIONS_CURR",
                                         UploadableFileList::eNoSpecified, -1, -1, vCrind );
  EXPECT_EQ( vCrind.size(), 1 );
  EXPECT_TRUE( vCrind[0].m_sFilename == "CRINDBIOSVERSIONS_CURR" );
  EXPECT_TRUE( vCrind[0].m_attrib.b_age == UploadableFileList::eCurrent );
  EXPECT_TRUE( vCrind[0].m_attrib.b_type == 0x00 );   // not appliable, is a file system log
  EXPECT_TRUE( vCrind[0].m_attrib.b_fileId == 0x00 ); // not appliable, is a file system log
  EXPECT_TRUE( vCrind[0].m_attrib.s_FilePath == "/var/opt/crindbios/log/versions.log" );


  uloadableLogList.getRequeriedFileList( "PINPAD_CURR",
                                         UploadableFileList::eNoSpecified, -1, -1, vPinpad );
  EXPECT_EQ( vPinpad.size(), 1 );
  EXPECT_TRUE( vPinpad[0].m_sFilename == "PINPAD_CURR" );
  EXPECT_TRUE( vPinpad[0].m_attrib.s_FilePath.empty() ); // not appliable, is a SPOT protocol log
  EXPECT_TRUE( vPinpad[0].m_attrib.b_age == UploadableFileList::eCurrent );
  EXPECT_TRUE( vPinpad[0].m_attrib.b_type == 0x13 );
  EXPECT_TRUE( vPinpad[0].m_attrib.b_fileId == 0x00 );

  uloadableLogList.getRequeriedFileList( "ALL",
                                         UploadableFileList::eNoSpecified, -1, -1, vAll );
  EXPECT_EQ( vAll.size(), uM5TotalRecords );

  uloadableLogList.getRequeriedFileList( "ALL_CURRENT",
                                         UploadableFileList::eNoSpecified, -1, -1, vAllCurrent );
  EXPECT_EQ( vAllCurrent.size(), uM5TotalCurrent );
  EXPECT_TRUE( verifyAllCurrent(vAllCurrent) );
}

TEST(GUtilsTestUT, TestUploadableFileListM7)
{
  // Check construction for M7
  std::vector<std::string>                 vKeys;
  UploadableFileList::RequestedFilesVector vAll, vAllCurrent, vPinpad;
  UploadableFileList uloadableLogList("upload_conf/uploadable-logs.cfg", "m7");

  EXPECT_FALSE( uloadableLogList.hasErrors() );
  EXPECT_EQ( uloadableLogList.getTotalOfRecords(), uM7TotalRecords );

  EXPECT_FALSE( uloadableLogList.isSupported("FILESYNC_CURR") ); // is an M3 log
  EXPECT_TRUE(  uloadableLogList.isSupported("TWOWIRE_CURR") );

  EXPECT_TRUE( uloadableLogList.getSupportedFilenameList(vKeys));
  EXPECT_EQ( vKeys.size() , uM7TotalRecords);
  EXPECT_FALSE( find(vKeys, "FILESYNC_CURR") >= 0 ); // is an M5 log
  EXPECT_TRUE(  find(vKeys, "TWOWIRE_CURR") >= 0 );

  uloadableLogList.getRequeriedFileList( "PINPAD_HIST",
                                         UploadableFileList::eNoSpecified, -1, -1, vPinpad );
  EXPECT_EQ( vPinpad.size(), 1 );
  EXPECT_TRUE( vPinpad[0].m_sFilename == "PINPAD_HIST" );
  EXPECT_TRUE( vPinpad[0].m_attrib.s_FilePath.empty() ); // not appliable, is a SPOT protocol log
  EXPECT_TRUE( vPinpad[0].m_attrib.b_age == UploadableFileList::eHistoric );
  EXPECT_TRUE( vPinpad[0].m_attrib.b_type == 0x13 );
  EXPECT_TRUE( vPinpad[0].m_attrib.b_fileId == 0x80 );

  uloadableLogList.getRequeriedFileList( "ALL",
                                         UploadableFileList::eNoSpecified, -1, -1, vAll );
  EXPECT_EQ( vAll.size(), uM7TotalRecords );

  uloadableLogList.getRequeriedFileList( "ALL_CURRENT",
                                         UploadableFileList::eNoSpecified, -1, -1, vAllCurrent );
  EXPECT_EQ( vAllCurrent.size(), uM7TotalCurrent );
  EXPECT_TRUE( verifyAllCurrent(vAllCurrent) );
}

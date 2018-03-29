/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @brief This file contains all the unit test that check the message structure for different types
 *        of messages, without mocking any other part of the system (session, etc)
 *
 * @file MessageStructure_unittest.cpp
 * @author Guillermo Paris
 * @date Wednesday, March 04, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <gvr/communication/spot/Utils.h>
#include "../include/gvrspot_unittest/MessageStructure_unittest.h"


/**
 * @brief  Message Structure Unit Tests.
 *         An exhaustive check for the correct structure of all the SPOT Message classes used in this project.
 * @since 1.0.0
 */

using namespace std;
using namespace gvr::communication::spot;


/**
* @brief Performs an SpotMessageLogin request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageLoginRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageLoginRequest)
{
  bool test(true);

  unsigned char clientId = 86;
  int moduloLen = 20;
  unsigned char pingTimeout = 100;
  unsigned char messageSize = 9;

  SpotMessageLogin message = SpotMessageLogin(clientId, moduloLen, pingTimeout);
  test = message.getSpotAppId() == 0x00;
  test &= message.getSsk() == 0x00;
  test &= message.getClientId() == clientId;
  test &= message.getModuloLen() == moduloLen;
  test &= message.getPingTimeout() == pingTimeout;
  test &= message.getCommand() == 0x01; //SPOT Message Command ID
  test &= message.size() == messageSize;
  SpotMessageLogin::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

 SpotMessageLogin parsedMessage = SpotMessageLogin();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageLogin::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;

  EXPECT_EQ(test,true);
}

/**
* @brief Performs an SpotMessageLogin response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageLoginRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageLoginResponse)
{

  bool test(true);
  int ackCode = 0x00;

  SpotMessageLoginResponse message = SpotMessageLoginResponse(ackCode);
  std::string strDatetime = std::string("14000005012014");
  SpotMessageLoginResponse::BufferType datetime(strDatetime.begin(), strDatetime.end());
  message.setDateTime(datetime);

  std::vector<SpotMessageLoginResponse::AppEntry> apps(0);
  SpotMessageLoginResponse::AppEntry app;
  app.setAppId(10);
  std::string appName("Application 0");
  app.setAppAscii( appName );
  SpotMessageLoginResponse::BufferType sign(0);
  sign.push_back(1);
  sign.push_back(2);
  sign.push_back(3);
  sign.push_back(4);
  app.setSign( sign );
  apps.push_back(app);
  message.setAppEntries(apps);

  SpotMessageLoginResponse::BufferType key(0);
  key.push_back(0);
  key.push_back(0);
  key.push_back(0);
  key.push_back(0);
  message.setRSAKey(key);

  test &= message.getSpotAppId() == 0x80;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x01; //SPOT Message Command ID
  test &= message.getAckCode() == ackCode;

  SpotMessageLoginResponse::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageLoginResponse parsedMessage = SpotMessageLoginResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "Error parsing serialized message." << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageLoginResponse::BufferType parsedBuff = parsedMessage.serialize();
  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  test &= (parsedMessage.size() == message.size()) && (serializedBuff.size() == parsedBuff.size());
  test &= std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin());

  std::cout << std::endl << std::endl;
  EXPECT_EQ(test,true);
}

/**
* @brief Performs an SpotMessageDebianInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageDebianInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageDebianInfoRequest)
{
  bool test(true);

  unsigned char messageSize = 254;

  SpotMessageDebianInfo message = SpotMessageDebianInfo();
  test = message.getSpotAppId() == 0x03;
  test &= message.getSsk() == 0x00;
  test &= message.getPackageListData().empty();
  test &= message.getCommand() == 0x10; //SPOT Message Command ID
  test &= message.getSubCommand() == 0x05; //SPOT Message SubCommand ID
  test &= message.size() == messageSize;
  SpotMessageDebianInfo::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageDebianInfo parsedMessage = SpotMessageDebianInfo();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageDebianInfo::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  EXPECT_EQ(test,true);
}


/**
* @brief Performs an SpotMessageDebianInfoResponse request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageDebianInfoResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageDebianInfoResponse)
{
  bool test(true);


  SpotMessageDebianInfoResponse message = SpotMessageDebianInfoResponse();
  std::string infoData("Prueba de Info de Debians");
  unsigned char messageSize = 7+2+infoData.size();//commondata+size+data
  message.setInfoResponseData(infoData);
  message.setAckCode(0x00);

  test = message.getSpotAppId() == 0x83;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x10; //SPOT Message Command ID
  test &= message.getSubCommand() == 0x05; //SPOT Message SubCommand ID
  test &= message.size() == messageSize;
  SpotMessageDebianInfoResponse::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageDebianInfoResponse parsedMessage = SpotMessageDebianInfoResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageDebianInfoResponse::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  EXPECT_EQ(test,true);
}

/**
* @brief Performs an SpotMessagePackageActivation request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageActivationRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageActivationRequest)
{
  bool test(true);

  unsigned char messageSize = 102;

  SpotMessagePackageActivation message = SpotMessagePackageActivation();
  message.setPackageActivationName("package to activate");
  message.setPackageActivationTime("0000.00.00-00:00");
  test = message.getSpotAppId() == 0x03;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x10; //SPOT Message Command ID
  test &= message.getSubCommand() == 0x04; //SPOT Message SubCommand ID
  test &= message.size() == messageSize;
  SpotMessagePackageActivation::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessagePackageActivation parsedMessage = SpotMessagePackageActivation();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessagePackageActivation::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  EXPECT_EQ(test,true);
}


/**
* @brief Performs an SpotMessagePackageActivationResponse request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageActivationResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageActivationResponse)
{
  bool test(true);

  SpotMessagePackageActivationResponse message = SpotMessagePackageActivationResponse();
  std::string infoData("Prueba de package activation");
  unsigned char messageSize = 7+infoData.size();//commondata+data
  message.setInfoResponseData(infoData);
  message.setAckCode(0x00);

  test = message.getSpotAppId() == 0x83;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x10; //SPOT Message Command ID
  test &= message.getSubCommand() == 0x04; //SPOT Message SubCommand ID
  test &= message.size() == messageSize;
  SpotMessagePackageActivationResponse::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessagePackageActivationResponse parsedMessage = SpotMessagePackageActivationResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessagePackageActivationResponse::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  EXPECT_EQ(test,true);
}
/*
* @brief Performs an SpotMessagePing request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePingRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePingRequest)
{
  bool test(true);

  unsigned char messageSize = 5;

  SpotMessagePing message = SpotMessagePing();
  test &= message.getSpotAppId() == 0x00;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x03; //SPOT Message Command ID
  test &= message.size() == messageSize;

  SpotMessagePing::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  SpotMessagePing parsedMessage = SpotMessagePing();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessagePing::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  EXPECT_EQ(test,true);
}

/*
* @brief Performs an SpotMessageReset request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageResetRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageResetRequest)
{
  bool test(true);

  unsigned char messageSize = 5;

  SpotMessageReset message = SpotMessageReset();
  test &= message.getSpotAppId() == 0x00;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x06; //SPOT Message Command ID
  test &= message.size() == messageSize;

  SpotMessageReset::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  SpotMessageReset parsedMessage = SpotMessageReset();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageReset::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  EXPECT_EQ(test,true);
}

/**
* @brief Performs an SpotMessagePing response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePingResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePingResponse)
{
  bool test(true);

  int ackCode = 0x02;
  unsigned char messageSize = 6;

  std::cout << std::endl;

  SpotMessagePingResponse message = SpotMessagePingResponse(ackCode);
  test = message.getSpotAppId() == 0x80;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x03; //SPOT Message Command ID
  test &= message.getAckCode() == ackCode;
  test &= message.size() == messageSize;

  SpotMessagePingResponse::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();

  SpotMessagePingResponse parsedMessage = SpotMessagePingResponse();
  if(!parsedMessage.parse(serializedBuff))
  {

   std::cout << "getCommand:" << parsedMessage.getCommand() << std::endl;
   std::cout << "serializedBuff[4]:" << serializedBuff[4] << std::endl;

   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }
  SpotMessagePingResponse::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
     std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
     std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
  EXPECT_EQ(test,true);
}

/*
* @brief Performs an SpotMessageSystemStatusNotification request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSystemStatusNotification The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageSystemStatusNotification)
{
  unsigned char bExpectedMessageSize = 7;

  SpotMessageSystemStatus message;
  EXPECT_EQ(message.getSpotAppId()  , 0x01);
  EXPECT_EQ(message.getSsk()        , 0x00);
  EXPECT_EQ(message.getCommand()    , 0x10);  // System Information command category.
  EXPECT_EQ(message.getSubCommand() , 0x01);  // SPOT System Status Notification.

  message.setSubSystemStatusType(SpotMessageSystemStatus::eSPOT_SYS_MAIN);

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  SpotMessageSystemStatus::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageSystemStatus parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageSystemStatus::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[(parsedBuff.size()*3)+2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/*
* @brief Performs an OPT SpotMessageSystemStatusNotification request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSystemStatusNotification The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOPTSystemStatusNotification)
{
  unsigned char bExpectedMessageSize = 7;

  SpotMessageSystemStatus message(SpotMessageSystemStatus::CMessageRequestOPTAppId);
  EXPECT_EQ(message.getSpotAppId()  , 0x04);
  EXPECT_EQ(message.getSsk()        , 0x00);
  EXPECT_EQ(message.getCommand()    , 0x10);  // System Information command category.
  EXPECT_EQ(message.getSubCommand() , 0x01);  // SPOT System Status Notification.

  message.setSubSystemStatusType(SpotMessageSystemStatus::eOPT_PRINTER);

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  SpotMessageSystemStatus::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageSystemStatus parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageSystemStatus::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[(parsedBuff.size()*3)+2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageSystemStatusNotification response test, applied to a pinpad response.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSystemStatusNotificationResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageSystemStatusNotificationResponse)
{
  const unsigned char bExpectedMessageSize = 12;

  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0C, 0x81, 0x00, 0x10, 0x01, 0x00, 0x01, 0x01, 0x04, 0x02, 0x00
  };

  SpotMessageSystemStatusResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageSystemStatusResponse  message;
  EXPECT_EQ(message.getSpotAppId()  , 0x81);
  EXPECT_EQ(message.getSsk()        , 0x00);
  EXPECT_EQ(message.getCommand()    , 0x10); // System Information command category.
  EXPECT_EQ(message.getSubCommand() , 0x01); // SPOT System Status Notification.
  EXPECT_EQ(message.getAckCode()    , 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << message.getCommand() << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  SpotMessageSystemStatusResponse::ItemType item;

  item = message.getItemData(0);
  EXPECT_EQ(item.deviceCode , 0x01);
  EXPECT_EQ(item.status     , 0x04);
  EXPECT_EQ(item.statusExt  , 0x02);
  EXPECT_EQ(item.errorCode  , 0x00);

  SpotMessageSystemStatusResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageSystemStatusNotification response test, applied to a pinpad response.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSystemStatusNotificationResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageSystemStatusNotificationResponse_6Devices)
{
  const unsigned char bExpectedMessageSize = 32;

  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x20, 0x81, 0x00, 0x10, 0x01, 0x00, 0x06, 0x01, 0x04, 0x00, 0x00, 0x02, 0x00, 0x80, 0x00,
     0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x10, 0x01, 0x80, 0x00, 0x20, 0x01, 0x00, 0x00
  };

  SpotMessageSystemStatusResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageSystemStatusResponse  message;
  EXPECT_EQ(message.getSpotAppId()  , 0x81);
  EXPECT_EQ(message.getSsk()        , 0x00);
  EXPECT_EQ(message.getCommand()    , 0x10); // System Information command category.
  EXPECT_EQ(message.getSubCommand() , 0x01); // SPOT System Status Notification.
  EXPECT_EQ(message.getAckCode()    , 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << message.getCommand() << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 6);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  SpotMessageSystemStatusResponse::ItemType item;

  item = message.getItemData(0);
  EXPECT_EQ(item.deviceCode , 0x01);
  EXPECT_EQ(item.status     , 0x04);
  EXPECT_EQ(item.statusExt  , 0x00);
  EXPECT_EQ(item.errorCode  , 0x00);

  item = message.getItemData(1);
  EXPECT_EQ(item.deviceCode , 0x02);
  EXPECT_EQ(item.status     , 0x00);
  EXPECT_EQ(item.statusExt  , 0x80);
  EXPECT_EQ(item.errorCode  , 0x00);

  item = message.getItemData(3);
  EXPECT_EQ(item.deviceCode , 0x08);
  EXPECT_EQ(item.status     , 0x00);
  EXPECT_EQ(item.statusExt  , 0x80);
  EXPECT_EQ(item.errorCode  , 0x00);

  item = message.getItemData(4);
  EXPECT_EQ(item.deviceCode , 0x10);
  EXPECT_EQ(item.status     , 0x01);
  EXPECT_EQ(item.statusExt  , 0x80);
  EXPECT_EQ(item.errorCode  , 0x00);

  int idx = -1; // data index, invalid at initialization

  item = message.getItemById(0x20, idx);
  EXPECT_EQ(idx , 5);
  EXPECT_EQ(item.deviceCode , 0x20);
  EXPECT_EQ(item.status     , 0x01);
  EXPECT_EQ(item.statusExt  , 0x00);
  EXPECT_EQ(item.errorCode  , 0x00);

  item = message.getItemById(0x04, idx);
  EXPECT_EQ(idx , 2);
  EXPECT_EQ(item.deviceCode , 0x04);
  EXPECT_EQ(item.status     , 0x00);
  EXPECT_EQ(item.statusExt  , 0x00);
  EXPECT_EQ(item.errorCode  , 0x00);


  SpotMessageSystemStatusResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}


/**
* @brief Performs an SpotMessageSystemStatusNotification response test, applied to an OPT response.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageOPTSystemStatusNotificationResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOPTSystemStatusNotificationResponse)
{
  const unsigned char bExpectedMessageSize = 12;

  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0C, 0x84, 0x00, 0x10, 0x01, 0x00, 0x01, 0x01, 0x02, 0x01, 0x00
  };

  SpotMessageSystemStatusResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageSystemStatusResponse  message;
  EXPECT_EQ(message.getSpotAppId()  , 0x81); // Default application ID is pinpad by construction
  EXPECT_EQ(message.getSsk()        , 0x00);
  EXPECT_EQ(message.getCommand()    , 0x10); // System Information command category.
  EXPECT_EQ(message.getSubCommand() , 0x01); // SPOT System Status Notification.
  EXPECT_EQ(message.getAckCode()    , 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << message.getCommand() << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  SpotMessageSystemStatusResponse::ItemType item;

  item = message.getItemData(0);
  EXPECT_EQ(item.deviceCode , 0x01);
  EXPECT_EQ(item.status     , 0x02);
  EXPECT_EQ(item.statusExt  , 0x01);
  EXPECT_EQ(item.errorCode  , 0x00);

  SpotMessageSystemStatusResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}


/*
* @brief Performs an SpotMessageHardwareInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageHardwareInfo The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageHardwareInfo)
{
  unsigned char bExpectedMessageSize = 8;

  SpotMessageHardwareInfo message;
  EXPECT_EQ(message.getSpotAppId(), 0x01);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // SPOT Hardware Info command.

  const unsigned char bItem[1] = { 0x11 }; // Hardware Release ID request
  message.setItems(1, bItem);

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  const unsigned char bItems[2] = { 0x01, 0x11 }; // SPOT serial number + Hardware Release ID request
  message.setItems(2, bItems);
  EXPECT_EQ(message.size(), bExpectedMessageSize + 1);

  SpotMessageHardwareInfo::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageHardwareInfo parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageHardwareInfo::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageHardwareInfo response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageHardwareInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageHardwareInfoResponse)
{
  const unsigned char bExpectedMessageSize = 12;
  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0C, 0x81, 0x00, 0x10, 0x02, 0x00, 0x01, 0x11, 0x02, 0x41, 0x33
  };
  const char acExpectedItemData[2] = {0x41, 0x33};

  SpotMessageHardwareInfoResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageHardwareInfoResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x81);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // SPOT Hardware Info command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << message.getCommand() << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  const unsigned char* pItemData = NULL;

  pItemData = message.getItemData(0, uItemID, uItemLength);
  EXPECT_EQ(uItemID, 0x11);
  EXPECT_EQ(uItemLength, 2);
  EXPECT_EQ(pItemData[0], acExpectedItemData[0]);
  EXPECT_EQ(pItemData[1], acExpectedItemData[1]);

  SpotMessageHardwareInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}


/*
* @brief Performs an SpotMessageHardwareInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageHardwareInfo The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOPTHwrInfo)
{
  unsigned char bExpectedMessageSize = 8;

  SpotMessageOPTHwrInfo message;
  EXPECT_EQ(message.getSpotAppId(), 0x04);  // OPT application request ID
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // OPT Hardware Info command.

  const unsigned char bItem[1] = { 0x02 }; // Printer device type request
  message.setItems(1, bItem);

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  const unsigned char abItems[2] = { 0x02, 0x11 }; // Printer + I/O module device type request
  message.setItems(2, abItems);
  EXPECT_EQ(message.size(), bExpectedMessageSize + 1);

  SpotMessageOPTHwrInfo::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageOPTHwrInfo parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageOPTHwrInfo::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageHardwareInfo response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageHardwareInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOPTHwrInfoResponse)
{
  const unsigned char bExpectedMessageSize = 14;
  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0E, 0x84, 0x00, 0x10, 0x02, 0x00, 0x01, 0x03, 0x04, 0x07, 0x05, 0x01, 0x00
  };
  const char acExpectedItemData[12] = "0x07050100"; // expected printer functions item

  SpotMessageOPTHwrInfoResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageOPTHwrInfoResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x84);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // SPOT Hardware Info command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << message.getCommand() << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  const unsigned char* pItemData = NULL;

  pItemData = message.getItemData(0, uItemID, uItemLength);
  EXPECT_EQ(uItemID, SpotMessageOPTHwrInfoResponse::ePRINTER_DEVFUNCTIONS); // 0x03 : printer functions in 4 bytes
  EXPECT_EQ(uItemLength, 4);
  EXPECT_EQ(pItemData[0], 0x07);
  EXPECT_EQ(pItemData[1], 0x05);
  EXPECT_EQ(pItemData[2], 0x01);
  EXPECT_EQ(pItemData[3], 0x00);

  std::string  sDeviceName, sPrinterFunctions;
  SpotMessageOPTHwrInfoResponse::DeviceInfoMap printerInfoMap;

  // Printer info is incomplete, then yields false.
  EXPECT_FALSE( message.getPrinterInfo(sDeviceName, printerInfoMap) );
  sPrinterFunctions = printerInfoMap[ "functions" ]; // get "functions" entry
  int iCompare = sPrinterFunctions.compare( acExpectedItemData ); // == "0x07050100" ???
  EXPECT_EQ( iCompare, 0 );

  SpotMessageHardwareInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}


/*
* @brief Performs an SpotMessageSoftwareInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSoftwareInfo The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageSoftwareInfo)
{
  unsigned char bExpectedMessageSize = 8;

  SpotMessageSoftwareInfo message;
  EXPECT_EQ(message.getSpotAppId(), 0x01);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x03); // SPOT Software Info command.

  const unsigned char bItem[1] = { 0x11 }; // Software Release ID request
  message.setItems(1, bItem);

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  const unsigned char bItems[2] = { 0x01, 0x11 }; // SPOT serial number + Software Release ID request
  message.setItems(2, bItems);
  EXPECT_EQ(message.size(), bExpectedMessageSize + 1);

  SpotMessageSoftwareInfo::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageSoftwareInfo parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageSoftwareInfo::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageSoftwareInfo response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageSoftwareInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageSoftwareInfoResponse)
{
  const unsigned char bExpectedMessageSize = 12;
  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0C, 0x81, 0x00, 0x10, 0x03, 0x00, 0x01, 0x01, 0x02, 0x01, 0x02
  };
  const char acExpectedItemData[2] = {0x01, 0x02};

  SpotMessageSoftwareInfoResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageSoftwareInfoResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x81);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x03); // SPOT Software Info command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
    std::cout << "getCommand: " << static_cast<int>(message.getCommand()) << std::endl;
    std::cout << "parsedBuff[2]: " << static_cast<int>(abTestReplyBytes[2]) << std::endl;
    std::cout << "parsedBuff[4]: " << static_cast<int>(abTestReplyBytes[4]) << std::endl;
    std::cout << "parsedBuff[5]: " << static_cast<int>(abTestReplyBytes[5]) << std::endl;
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  unsigned int uItemID = 0;
  ISpotMessage::SizeType uItemLength = 0;
  const unsigned char* pItemData = NULL;

  pItemData = message.getItemData(0, uItemID, uItemLength);
  EXPECT_EQ(uItemID, 0x01);
  EXPECT_EQ(uItemLength, 2);
  EXPECT_EQ(pItemData[0], acExpectedItemData[0]);
  EXPECT_EQ(pItemData[1], acExpectedItemData[1]);

  SpotMessageSoftwareInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/*
* @brief Performs an SpotMessagePackageInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageInfo The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageInfo)
{
  unsigned char bExpectedMessageSize = SpotMessagePackageInfo::CMessageSize;

  // Request message creation.
  SpotMessagePackageInfo message;
  EXPECT_EQ(message.getSpotAppId(), 0x03);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x03); // SPOT Package Info command.

  // Request data is set.
  message.selectPackageSet(0x01); // Package Set selector byte

  // Final length is checked.
  EXPECT_EQ(message.size(), bExpectedMessageSize);

  // 1) Request message is serialized.
  SpotMessagePackageInfo::BufferType serializedBuff = message.serialize();
  // Length consistency is checked.
  EXPECT_EQ(message.size(), serializedBuff.size());
  // Data consistency is checked.
  EXPECT_EQ(serializedBuff[SpotMessagePackageInfo::CSetSelectionByteOffset], 0x01);

  // 2) The serialized buffer is parsed into a new request parsedMessage.
  SpotMessagePackageInfo parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  // 3) The parsed message is serialized back to a final parsed buffer.
  SpotMessagePackageInfo::BufferType  parsedBuff ( parsedMessage.serialize() );
  // Length consistency is checked.
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());

  // 4) Initial serialized buffer and final one should be identical.
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * SpotMessagePackageInfo::CMessageSize + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessagePackageInfo response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageInfoResponse)
{
  const ISpotMessage::SizeType  uiExpectedMessageSize = 256;
  const unsigned char           abExampleFirstReplyBytes[13] =
  {
     0x01, 0x00, 0x83, 0x00, 0x10, 0x03, 0x00, 0x01, // only one certificate[248] in the reply.
     'G' , 'V' , 'R' , 0x20, 0x20                    // certificate data is only "GVR  "
  };

  const char acExpectedCertData[8] = { 'G' , 'V' , 'R' , 0x20, 0x20, 0x00, 0x00, 0x00 };

  unsigned char abTestReplyBytes[uiExpectedMessageSize];
  memset(abTestReplyBytes, 0x00, uiExpectedMessageSize);
  memcpy(abTestReplyBytes, abExampleFirstReplyBytes, sizeof(abExampleFirstReplyBytes));


  SpotMessagePackageInfoResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + uiExpectedMessageSize);

  std::cout << std::endl;

  SpotMessagePackageInfoResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x83);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // Setup command category.
  EXPECT_EQ(message.getSubCommand(), 0x03); // SPOT Package Info command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
    std::cout << "getCommand: " << static_cast<int>(message.getCommand()) << std::endl;
    std::cout << "parsedBuff[2]: " << static_cast<int>(abTestReplyBytes[2]) << std::endl;
    std::cout << "parsedBuff[4]: " << static_cast<int>(abTestReplyBytes[4]) << std::endl;
    std::cout << "parsedBuff[5]: " << static_cast<int>(abTestReplyBytes[5]) << std::endl;
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), uiExpectedMessageSize);
  EXPECT_EQ(message.getItemQty(), 1);

  const unsigned char* pCertData = message.getItemData(0, 0);
  EXPECT_EQ( memcmp(pCertData, acExpectedCertData, sizeof(acExpectedCertData)), 0);

  SpotMessagePackageInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}


/*
* @brief Performs an SpotMessagePackageDownload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageDownload The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageDownload)
{
  ISpotMessage::SizeType uExpectedMessageSize = SpotMessagePackageDownload::CMessageSize;

  // Test data set
  const unsigned char acRow[16] =
  {
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  };

  unsigned char abTestCertificate[256];
  for(int i = 0; i < 16; i++)
  {
    memcpy(abTestCertificate + 16 * i, acRow, 16);
  }

  // Request message creation.
  SpotMessagePackageDownload message;
  EXPECT_EQ(message.getSpotAppId(), 0x03);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x01); // SPOT Package Download command.

  // Request data is set.
  message.setFileLength(0x12345678);
  message.setCertificateData(abTestCertificate);

  // Final length is checked.
  EXPECT_EQ(message.size(), uExpectedMessageSize);

  // 1) Request message is serialized.
  ISpotMessage::BufferType serializedBuff = message.serialize();
  // Length consistency is checked.
  EXPECT_EQ(message.size(), serializedBuff.size());
  // Data consistency is checked.
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::CFileSizeOffset],   0x12);
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::CFileSizeOffset+1], 0x34);
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::CFileSizeOffset+2], 0x56);
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::CFileSizeOffset+3], 0x78);
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::CCommandOffset],    0x01);
  EXPECT_EQ(serializedBuff[SpotMessagePackageDownload::COptionOffset],     0x00);

  EXPECT_EQ( memcmp( &serializedBuff.front() + SpotMessagePackageDownload::CCertificateOffset,
                     abTestCertificate,
                     SpotMessagePackageDownload::CCertificateSize ), 0 );

  // 2) The serialized buffer is parsed into a new request parsedMessage.
  SpotMessagePackageDownload parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  // 3) The parsed message is serialized back to a final parsed buffer.
  SpotMessagePackageDownload::BufferType  parsedBuff ( parsedMessage.serialize() );
  // Length consistency is checked.
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());

  // 4) Initial serialized buffer and final one should be identical.
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * SpotMessagePackageDownload::CMessageSize + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessagePackageDownload response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageHardwareInfoRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageDownloadResponse)
{
  const unsigned char bExpectedMessageSize = SpotMessagePackageDownloadResponse::CMessageSize;
  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x0B, 0x83, 0x00, 0x10, 0x01, 0x00, 0x12, 0x34, 0x56, 0x78
  };
  ISpotMessage::SizeType uExpectedOffset = 0x12345678;

  SpotMessagePackageDownloadResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessagePackageDownloadResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x83);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x01); // SPOT Hardware Info command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand: " << static_cast<int>(message.getCommand()) << std::endl;
   std::cout << "parsedBuff[2]: " << static_cast<int>(abTestReplyBytes[2]) << std::endl;
   std::cout << "parsedBuff[4]: " << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "parsedBuff[5]: " << static_cast<int>(abTestReplyBytes[5]) << std::endl;
   std::cout << "error parsing !!!" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  ISpotMessage::SizeType uInitialOffset = message.getInitialOffset();
  EXPECT_EQ(uInitialOffset, uExpectedOffset);

  SpotMessageHardwareInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/*
* @brief Performs an SpotMessagePackageBlockDownload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageBlockDownload The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageBlockDownload)
{
  ISpotMessage::SizeType uTestBlockSize = 1024; // Hex: 04 00
  ISpotMessage::SizeType uExpectedMessageSize = uTestBlockSize + SpotMessagePackageBlockDownload::CMinMessageSize;

  // Test data set
  const unsigned char abRow[32] =
  {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
  };

  unsigned char abTestBlock[uTestBlockSize];
  for(int i = 0; i < 32; i++)
  {
    memcpy(abTestBlock + 32 * i, abRow, 32);
  }

  // Request message creation.
  SpotMessagePackageBlockDownload message;
  EXPECT_EQ(message.getSpotAppId(), 0x03);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // SPOT Package Block Download command.

  // Block data is set.
  message.setBlock(uTestBlockSize, abTestBlock);


  // Final length is checked.
  EXPECT_EQ(message.size(), uExpectedMessageSize);

  // 1) Request message is serialized.
  ISpotMessage::BufferType serializedBuff = message.serialize();
  // Length consistency is checked.
  EXPECT_EQ(message.size(), serializedBuff.size());
  // Data consistency is checked.
  EXPECT_EQ(serializedBuff[SpotMessagePackageBlockDownload::CBlockSizeOffset],   0x04);
  EXPECT_EQ(serializedBuff[SpotMessagePackageBlockDownload::CBlockSizeOffset+1], 0x00);

  EXPECT_EQ( memcmp( &serializedBuff.front() + SpotMessagePackageBlockDownload::CBlockDataOffset,
                     abTestBlock,
                     uTestBlockSize ), 0 );

  // 2) The serialized buffer is parsed into a new request parsedMessage.
  SpotMessagePackageBlockDownload parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  // 3) The parsed message is serialized back to a final parsed buffer.
  SpotMessagePackageBlockDownload::BufferType  parsedBuff ( parsedMessage.serialize() );
  // Length consistency is checked.
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());

  // 4) Initial serialized buffer and final one should be identical.
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessagePackageBlockDownloadResponse test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessagePackageBlockDownloadResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagePackageBlockDownloadResponse)
{
  ISpotMessage::SizeType uExpectedDependenciesLength = 0x400; // 1024 bytes
  ISpotMessage::SizeType uDepIndex = SpotMessagePackageBlockDownloadResponse::CMessageDependenciesIndex;
  ISpotMessage::SizeType bExpectedMessageSize = uDepIndex + uExpectedDependenciesLength;

  const unsigned char abTestReplyHeader[SpotMessagePackageBlockDownloadResponse::CMessageDependenciesIndex] =
  {
     0x04, 0x09, 0x83, 0x00, 0x10, 0x02, 0x00, 0x04, 0x00
  };

  // Test data set
  const unsigned char abRow[32] =
  {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '.', '2', '3', 0x20, 0x20
  };

  // Simulated Test message - storage for whole message raw data
  unsigned char abTestReplyBytes[bExpectedMessageSize];

  // Initialize the first part of constant size.
  memcpy(abTestReplyBytes, abTestReplyHeader, uDepIndex);

  // Initialize the second part of variable size (debian dependencies).
  for(int i = 0; i < 32; i++)
  {
    memcpy(abTestReplyBytes + uDepIndex + 32 * i, abRow, 32);
  }

  SpotMessagePackageBlockDownloadResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessagePackageBlockDownloadResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x83);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // System Information command category.
  EXPECT_EQ(message.getSubCommand(), 0x02); // SPOT Package Block Download command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand: " << static_cast<int>(message.getCommand()) << std::endl;
   std::cout << "parsedBuff[2]: " << static_cast<int>(abTestReplyBytes[2]) << std::endl;
   std::cout << "parsedBuff[4]: " << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "parsedBuff[5]: " << static_cast<int>(abTestReplyBytes[5]) << std::endl;
   std::cout << "error parsing !!!" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  ISpotMessage::SizeType  uDebianDependeciesLength = message.getDebianDependenciesTotalLength();
  EXPECT_EQ(uDebianDependeciesLength, 0x400);
  EXPECT_EQ(memcmp(message.getDebianDependencies(),          abRow, 32), 0);
  EXPECT_EQ(memcmp(message.getDebianDependencies() + 32,     abRow, 32), 0);
  EXPECT_EQ(memcmp(message.getDebianDependencies() + 0x3C0,  abRow, 32), 0);
  EXPECT_EQ(memcmp(message.getDebianDependencies() + 0x3E0,  abRow, 32), 0);

  ISpotMessage::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageVGDFileTransfer request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageVGDFileTransfer The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageVGDFileTransfer)
{
  ISpotMessage::SizeType uTestFileSize = 1056; // = 33 * 32 = Hex: 04 20
  ISpotMessage::SizeType uExpectedMinimumMessageSize = SpotMessageVGDFileTransfer::CMinMessageSize;
  ISpotMessage::SizeType uExpectedMessageSize = uExpectedMinimumMessageSize + uTestFileSize;

  // Test data set
  const unsigned char abRow[32] =
  {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
  };

  unsigned char abTestBlock[uTestFileSize];
  for(int i = 0; i < 33; i++)
  {
    memcpy(abTestBlock + 32 * i, abRow, 32);
  }

  // Request message creation.
  SpotMessageVGDFileTransfer message;
  EXPECT_EQ(message.getSpotAppId(), 0x00);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // File Transfer command category.

  EXPECT_EQ(message.getOperationCommand(), 0x03); // By construction the default operation should be "file upload".
  message.setOperationCommand(0x02); // set block download operation
  EXPECT_EQ(message.getOperationCommand(), 0x02);

  // Inital length is checked.
  EXPECT_EQ(message.size(), uExpectedMinimumMessageSize);

  // Block data is set.
  message.setBlock(uTestFileSize, abTestBlock);
  // Final length is checked.
  EXPECT_EQ(message.size(), uExpectedMessageSize);

  // 1) Request message is serialized.
  ISpotMessage::BufferType serializedBuff = message.serialize();
  // Length consistency is checked.
  EXPECT_EQ(message.size(), serializedBuff.size());
  // Data consistency is checked.
  EXPECT_EQ(serializedBuff[SpotMessageVGDFileTransfer::CFileDataOffset],   0x00); // Initial couple of bytes.
  EXPECT_EQ(serializedBuff[SpotMessageVGDFileTransfer::CFileDataOffset+1], 0x01);

  EXPECT_EQ( memcmp( &serializedBuff.front() + SpotMessageVGDFileTransfer::CFileDataOffset,
                     abTestBlock,
                     uTestFileSize ), 0 );

  // 2) The serialized buffer is parsed into a new request parsedMessage.
  SpotMessageVGDFileTransfer parsedMessage;
  parsedMessage.setOperationCommand(0x02);
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing !!!" << std::endl;
    EXPECT_TRUE(false);
  }

  // 3) The parsed message is serialized back to a final parsed buffer.
  SpotMessageVGDFileTransfer::BufferType  parsedBuff ( parsedMessage.serialize() );
  // Length consistency is checked.
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());

  // 4) Initial serialized buffer and final one should be identical.
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageVGDFileTransfer response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageVGDFileTransferResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageVGDFileTransferResponse)
{
  ISpotMessage::SizeType uExpectedFileDataLength = 0x420; // 1056 bytes
  ISpotMessage::SizeType uDataIndex = SpotMessageVGDFileTransferResponse::CFileDataOffset;
  ISpotMessage::SizeType bExpectedMessageSize = uDataIndex + uExpectedFileDataLength;

  const unsigned char abTestReplyHeader[SpotMessageVGDFileTransferResponse::CFileDataOffset] =
  {
     0x04, 0x26, 0x80, 0x00, 0x10, 0x00
  };

  // Test data set
  const unsigned char abRow[32] =
  {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '.', '2', '3', 0x20, 0x20
  };

  // Simulated Test message - storage for whole message raw data
  unsigned char abTestReplyBytes[bExpectedMessageSize];

  // Initialize the first part of constant size.
  memcpy(abTestReplyBytes, abTestReplyHeader, uDataIndex);

  // Initialize the second part of variable size (file data).
  for(int i = 0; i < 33; i++)
  {
    memcpy(abTestReplyBytes + uDataIndex + 32 * i, abRow, 32);
  }

  SpotMessageVGDFileTransferResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageVGDFileTransferResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x80);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x10);    // File Transfer command category.
  EXPECT_EQ(message.getAckCode(), 0x00);

  EXPECT_EQ(message.getOperationCommand(), 0x01); // Default by construction is 0x01 "File Download"
  message.setOperationCommand(0x04); // set block upload operation
  EXPECT_EQ(message.getSubCommand(), 0x04);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand: " << static_cast<int>(message.getCommand()) << std::endl;
   std::cout << "parsedBuff[2]: " << static_cast<int>(abTestReplyBytes[2]) << std::endl;
   std::cout << "parsedBuff[3]: " << static_cast<int>(abTestReplyBytes[3]) << std::endl;
   std::cout << "parsedBuff[4]: " << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "parsedBuff[5]: " << static_cast<int>(abTestReplyBytes[5]) << std::endl;
   std::cout << "error parsing !!!" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  ISpotMessage::SizeType  uFileDataLength = 0;
  const unsigned char* pData = message.getBlock( uFileDataLength );

  EXPECT_EQ(uFileDataLength, uExpectedFileDataLength);
  EXPECT_EQ(memcmp(pData,          abRow, 32), 0); // 1st data "row"
  EXPECT_EQ(memcmp(pData + 32,     abRow, 32), 0); // 2nd data "row"
  EXPECT_EQ(memcmp(pData + 0x3E0,  abRow, 32), 0); // next to last "row"
  EXPECT_EQ(memcmp(pData + 0x400,  abRow, 32), 0); // last data "row"

  ISpotMessage::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageVgdProcessPackage request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageVgdProcessPackage The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageVgdProcessPackage)
{
  unsigned char bExpectedMessageSize = 7;

  SpotMessageVgdProcessPackage message;
  EXPECT_EQ(message.getSpotAppId(), 0x01);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x60);    // Firmware/Software update command category.
  EXPECT_EQ(message.getSubCommand(), 0x05); // Vgd Process Package command.
  EXPECT_EQ(message.size(), bExpectedMessageSize);
  EXPECT_EQ(message.getCommandOperation(), 0x01);

  message.setVerifyOperation();
  EXPECT_EQ(message.getCommandOperation(), 0x00);
  message.setSaveOperation();
  EXPECT_EQ(message.getCommandOperation(), 0x01);

  SpotMessageVgdProcessPackage::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  SpotMessageVgdProcessPackage parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageVgdProcessPackage::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_EQ(parsedMessage.size(), parsedBuff.size());
  EXPECT_EQ(serializedBuff.size(), parsedBuff.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()) );

  char acOutPrtBuffer[3 * parsedBuff.size() + 2];
  std::cout << "\nSerialized buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nParsed buffer data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, parsedBuff)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageVgdProcessPackage response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageVgdProcessPackageResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageVgdProcessPackageResponse)
{
  const unsigned char bExpectedMessageSize = 7;
  const unsigned char abTestReplyBytes[bExpectedMessageSize] =
  {
     0x00, 0x07, 0x81, 0x00, 0x60, 0x05, 0x00
  };

  SpotMessageVgdProcessPackageResponse::BufferType  testReplyBuffer;
  testReplyBuffer.assign(abTestReplyBytes, abTestReplyBytes + bExpectedMessageSize);

  std::cout << std::endl;

  SpotMessageVgdProcessPackageResponse  message;
  EXPECT_EQ(message.getSpotAppId(), 0x81);
  EXPECT_EQ(message.getSsk(), 0x00);
  EXPECT_EQ(message.getCommand(), 0x60);    // Firmware/Software update command category.
  EXPECT_EQ(message.getSubCommand(), 0x05); // Vgd Process Package command.
  EXPECT_EQ(message.getAckCode(), 0x00);

  if(!message.parse(testReplyBuffer))
  {
   std::cout << "getCommand:" << static_cast<int>(message.getCommand()) << std::endl;
   std::cout << "parsedBuff[2]:" << static_cast<int>(abTestReplyBytes[2]) << std::endl;
   std::cout << "parsedBuff[3]:" << static_cast<int>(abTestReplyBytes[3]) << std::endl;
   std::cout << "parsedBuff[4]:" << static_cast<int>(abTestReplyBytes[4]) << std::endl;
   std::cout << "parsedBuff[5]:" << static_cast<int>(abTestReplyBytes[5]) << std::endl;
   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }

  EXPECT_EQ(message.size(), bExpectedMessageSize);

  SpotMessageVgdProcessPackageResponse::BufferType serializedBuff = message.serialize();
  EXPECT_EQ(message.size(), serializedBuff.size());

  EXPECT_EQ(serializedBuff.size(), testReplyBuffer.size());
  EXPECT_TRUE( std::equal(serializedBuff.begin(), serializedBuff.end(), testReplyBuffer.begin()) );

  char acOutPrtBuffer[3 * testReplyBuffer.size() + 2];
  std::cout << "\nSerialized data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, serializedBuff)
            << "\nOriginal data:\n"
            << Utils::dumpBuffer(acOutPrtBuffer, testReplyBuffer)
            << std::endl << std::endl;
}

/**
* @brief Performs an SpotMessageFileDownload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileDownloadRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileDownloadRequest)
{
  bool test(true);
  unsigned char messageSize = 33;
  SpotMessageFileDownload message(5,6);
  message.setFileSize(100);

  SpotMessageFileDownload::BufferType crc;
  crc.push_back(0x01);
  crc.push_back(0x01);
  crc.push_back(0x01);
  crc.push_back(0x01);
  message.setCrcData(crc);

  message.setAuthenType(0x00);
  SpotMessageFileDownload::BufferType aut;
  message.setSecretAuthenticator(aut);
  message.setDescription("Bitmap.jpg");

  test = message.getSpotAppId() == 0x01;
  test &= message.getSsk() == 0x00;
  test &= message.getCommand() == 0x00; //SPOT Message Command ID
  test &= message.getSubCommand() == 0x01; //SPOT Message SubCommand ID
  test &= message.size() == messageSize;

  SpotMessageFileDownload::BufferType serializedBuff = message.serialize();
  test &= message.size() == serializedBuff.size();
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileDownload parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileDownload::BufferType parsedBuff = parsedMessage.serialize();
  test &= (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  EXPECT_EQ(test,true);
}


/**
* @brief Performs an SpotMessageFileDownloadResponse test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileDownloadResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileDownloadResponse)
{
  bool test(true);
  int messageSize = 11;
  SpotMessageFileDownloadResponse message(0x00);
  message.setStartingOffset(100);

  EXPECT_TRUE( message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x01); //SPOT Message SubCommand ID
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageFileDownloadResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileDownloadResponse parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageFileDownloadResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE((parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  EXPECT_TRUE(test);
}


/**
* @brief Performs an SpotMessageFileBlockDownload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileBlockDownloadRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileBlockDownloadRequest)
{
  unsigned char messageSize = 12;
  SpotMessageFileBlockDownload message;
  message.setBlockOffset(1);

  EXPECT_TRUE( message.getSpotAppId() == 0x01);
  EXPECT_TRUE( message.getSsk() == 0x00);
  EXPECT_TRUE( message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE( message.getSubCommand() == 0x02); //SPOT Message SubCommand ID
  EXPECT_TRUE( message.size() == messageSize);

  SpotMessageFileBlockDownload::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE( message.size() == serializedBuff.size());
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileBlockDownload parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileBlockDownload::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE( (parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}


/**
* @brief Performs an SpotMessageFileBlockDownloadResponse test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileBlockDownloadResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileBlockDownloadResponse)
{
  int messageSize = 11;
  SpotMessageFileBlockDownloadResponse message(0x00);
  message.setStartingOffset(100);

  EXPECT_TRUE( message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x02); //SPOT Message SubCommand ID
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageFileBlockDownloadResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileBlockDownloadResponse parsedMessage;
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }

  SpotMessageFileBlockDownloadResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE((parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageFileDelete request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileDeleteRequest The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileDeleteRequest)
{
  unsigned char messageSize = 8;
  SpotMessageFileDelete message = SpotMessageFileDelete();
  message.setFileType(0x11);
  message.setFileId(0x12);
  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x04); //SPOT Message SubCommand ID
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileDelete::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileDelete parsedMessage = SpotMessageFileDelete();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileDelete::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE((parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageFileDeleteResponse test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileDeleteRespose The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileDeleteResponse)
{
  unsigned char messageSize = 7;
  SpotMessageFileDeleteResponse message(0);
  EXPECT_TRUE(message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x04); //SPOT Message SubCommand ID
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileDeleteResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());
  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;
  SpotMessageFileDeleteResponse parsedMessage = SpotMessageFileDeleteResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileDeleteResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE((parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/*
* @brief Performs an SpotMessageNewEphemeralKey request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageNewEphemeralKey The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageNewEphemeralKey)
{
  unsigned char messageSize = 7;

  SpotMessageNewEphemeralKey message = SpotMessageNewEphemeralKey();
  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x80);
  EXPECT_TRUE(message.getSubCommandOne() == 0x04);
  EXPECT_TRUE(message.getSubCommandTwo() == 0x00);
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageNewEphemeralKey::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  SpotMessageNewEphemeralKey parsedMessage = SpotMessageNewEphemeralKey();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageNewEphemeralKey::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size());
  EXPECT_TRUE(serializedBuff.size() == parsedBuff.size());
  EXPECT_TRUE(std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/*
* @brief Performs an SpotMessageEphemeralEncrypt request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageEphemeralEncrypt The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageEphemeralEncrypt)
{
  unsigned char messageSize = 7;
  SpotMessageEphemeralEncrypt message = SpotMessageEphemeralEncrypt();
  std::string strData = std::string("data to encrypt");
  SpotMessageEphemeralEncryptResponse::BufferType data(strData.begin(), strData.end());
  message.setData(data);
  messageSize += data.size() + 2;

  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x80);
  EXPECT_TRUE(message.getSubCommandOne() == 0x04);
  EXPECT_TRUE(message.getSubCommandTwo() == 0x01);
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageEphemeralEncrypt::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  SpotMessageEphemeralEncrypt parsedMessage = SpotMessageEphemeralEncrypt();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageEphemeralEncrypt::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size());
  EXPECT_TRUE(serializedBuff.size() == parsedBuff.size());
  EXPECT_TRUE(std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/**
* @brief Performs an TestSpotMessageEphemeralEncrypt response test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageEphemeralEncryptResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageEphemeralEncryptResponse)
{
  int ackCode = 0x00;

  std::cout << std::endl;

  SpotMessageEphemeralEncryptResponse message = SpotMessageEphemeralEncryptResponse();
  std::string strData = std::string("data to encrypt");
  SpotMessageEphemeralEncryptResponse::BufferType data(strData.begin(), strData.end());
  message.setData(data);

  unsigned char messageSize = 8 + strData.size() + 2;

  EXPECT_TRUE(message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x80);
  EXPECT_TRUE(message.getSubCommandOne() == 0x04);
  EXPECT_TRUE(message.getSubCommandTwo() == 0x01);
  EXPECT_TRUE(message.getAckCode() == ackCode);
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageEphemeralEncryptResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  SpotMessageEphemeralEncryptResponse parsedMessage = SpotMessageEphemeralEncryptResponse();
  if(!parsedMessage.parse(serializedBuff))
  {

   std::cout << "getCommand:" << parsedMessage.getCommand() << std::endl;
   std::cout << "serializedBuff[4]:" << serializedBuff[4] << std::endl;

   std::cout << "error parsing" << std::endl;
   EXPECT_TRUE(false);
  }
  SpotMessageEphemeralEncryptResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE((parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin())));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
     std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
     std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}

/*
* @brief Performs an SpotMessageEphemeralEncrypt decrypt request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageEphemeralDecrypt The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageEphemeralDecrypt)
{
  unsigned char messageSize = 7;
  SpotMessageEphemeralEncrypt message = SpotMessageEphemeralEncrypt(true);
  std::string strData = std::string("data to decrypt");
  SpotMessageEphemeralEncryptResponse::BufferType data(strData.begin(), strData.end());
  message.setData(data);
  messageSize += data.size() + 2;

  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x80);
  EXPECT_TRUE(message.getSubCommandOne() == 0x04);
  EXPECT_TRUE(message.getSubCommandTwo() == 0x02);
  EXPECT_TRUE(message.size() == messageSize);

  SpotMessageEphemeralEncrypt::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  SpotMessageEphemeralEncrypt parsedMessage = SpotMessageEphemeralEncrypt(true);
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageEphemeralEncrypt::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size());
  EXPECT_TRUE(serializedBuff.size() == parsedBuff.size());
  EXPECT_TRUE(std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }
  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageOptSwInfo request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageOptSwInfo The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOptSwInfo)
{
  unsigned char messageSize = 7;

  SpotMessageOptSwInfo message = SpotMessageOptSwInfo();
  EXPECT_TRUE(message.getSpotAppId() == 0x04);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x10); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x03);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageOptSwInfo::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());
  std::cout << "message.size(): " << message.size() <<std::endl;
  std::cout << "serializedBuff.size(): " << serializedBuff.size() <<std::endl;

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageOptSwInfo parsedMessage = SpotMessageOptSwInfo();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageOptSwInfo::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageOptSwInfoResponse request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageOptSwInfoResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageOptSwInfoResponse)
{
  unsigned char messageSize = 8;

  SpotMessageOptSwInfoResponse message = SpotMessageOptSwInfoResponse();
  message.setAckCode(0x00);
  EXPECT_TRUE(message.getSpotAppId() == 0x84);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x10); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x03);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageOptSwInfoResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageOptSwInfoResponse parsedMessage = SpotMessageOptSwInfoResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageOptSwInfoResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}


/**
* @brief Performs an SpotMessageFileUpload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileUpload The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileUpload)
{
  unsigned char messageSize = 8;

  SpotMessageFileUpload message = SpotMessageFileUpload();
  message.setFileId(0x10);
  message.setFileType(0x01);
  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x05);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileUpload::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageFileUpload parsedMessage = SpotMessageFileUpload();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileUpload::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageFileUploadResponse request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileUploadResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileUploadResponse)
{
  unsigned char messageSize = 11;

  SpotMessageFileUploadResponse message = SpotMessageFileUploadResponse();
  message.setAckCode(0x00);
  message.setFileSize(1024);
  EXPECT_TRUE(message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x05);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileUploadResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageFileUploadResponse parsedMessage = SpotMessageFileUploadResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileUploadResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}

/**
* @brief Performs an SpotMessageFileBlockUpload request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileBlockUpload The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileBlockUpload)
{
  unsigned char messageSize = 10;

  SpotMessageFileBlockUpload message = SpotMessageFileBlockUpload();
  message.setBlockOffset(0);
  EXPECT_TRUE(message.getSpotAppId() == 0x01);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x06);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileBlockUpload::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageFileBlockUpload parsedMessage = SpotMessageFileBlockUpload();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileBlockUpload::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}


/**
* @brief Performs an SpotMessageFileBlockUploadResponse request test.
* @param GSpotTestUT The test case name.
* @param TestSpotMessageFileBlockUploadResponse The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessageFileBlockUploadResponse)
{
  unsigned char messageSize = 0x19;

  SpotMessageFileBlockUploadResponse message = SpotMessageFileBlockUploadResponse();
  message.setAckCode(0x00);

  std::string fileUpData = std::string("1234567890");
  SpotMessageFileBlockUploadResponse::BufferType data(fileUpData.begin(), fileUpData.end());
  message.setBlockData(data);
  EXPECT_TRUE(message.getSpotAppId() == 0x81);
  EXPECT_TRUE(message.getSsk() == 0x00);
  EXPECT_TRUE(message.getCommand() == 0x00); //SPOT Message Command ID
  EXPECT_TRUE(message.getSubCommand() == 0x06);
  EXPECT_TRUE(message.size() == messageSize);
  SpotMessageFileBlockUploadResponse::BufferType serializedBuff = message.serialize();
  EXPECT_TRUE(message.size() == serializedBuff.size());

  std::cout << "serializedBuff.data: " <<std::endl;
  for (unsigned i=0; i<serializedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(serializedBuff[i]) << ' ';
  }
  std::cout << std::endl;

  SpotMessageFileBlockUploadResponse parsedMessage = SpotMessageFileBlockUploadResponse();
  if(!parsedMessage.parse(serializedBuff))
  {
    std::cout << "error parsing" << std::endl;
    EXPECT_TRUE(false);
  }
  SpotMessageFileBlockUploadResponse::BufferType parsedBuff = parsedMessage.serialize();
  EXPECT_TRUE(parsedMessage.size() == parsedBuff.size() && serializedBuff.size() == parsedBuff.size() && std::equal(serializedBuff.begin(), serializedBuff.end(), parsedBuff.begin()));


  std::cout << "parsedBuff.data: "<< std::endl;
  for (unsigned i=0; i<parsedBuff.size(); ++i)
  {
      std::cout << setw(2) << setfill('0') << std::hex << static_cast<int>(parsedBuff[i]) << ' ';
  }

  std::cout << std::endl;
}

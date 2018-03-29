/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file log_unittest.cpp
 * @author gparis
 * @date Tuesday, September 30, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>
#include <log_unittest.h>

using namespace gvr::log;

DEFINE_LOG_MODULE(log1);
DEFINE_LOG_MODULE(log2);
DEFINE_LOG_MODULE(log3);
DEFINE_LOG_MODULE(log4);
DEFINE_LOG_MODULE(log5);
DEFINE_LOG_MODULE(log6);
DEFINE_LOG_MODULE(log7);
DEFINE_LOG_MODULE(log8);

GLogTestUT::GLogTestUT()
{
	std::cout << "             GLogTestUT::GLogTestUT()\n";
}

GLogTestUT::~GLogTestUT()
{
	std::cout << "             GLogTestUT::~GLogTestUT()\n";
}

void GLogTestUT::SetUp()
{
	std::cout << "             GLogTestUT::SetUp()\n";
}

void GLogTestUT::TearDown()
{
	std::cout << "             GLogTestUT::TearDown()\n";
}

/**
 * @brief  Log unit test GLogTestUT::TestGilbarcoLog entry point.
 *
 * @since 1.0.0
 */
TEST_F(GLogTestUT, TestGilbarcoLog)
{
  std::cout << "             GLogTestUT_TestGilbarcoLog() begin.\n";

  bool blret = init("configuration/Log.ini", gvr::log::CNORMAL_USE_OF_LOG_LIBRARY);
  EXPECT_TRUE(blret); // Expects log library initialization OK.


  // Priority check

  blret = Poco::Logger::get("log1").fatal(); // Log1 has fatal priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log2").critical(); // Log2 has critical priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log3").error(); // Log3 has error priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log4").warning(); // Log4 has warning priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log5").notice(); // Log5 has notice priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log6").information(); // Log6 has information priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log7").debug(); // Log7 has debug priority ?
  EXPECT_TRUE(blret); // OK.

  blret = Poco::Logger::get("log8").trace(); // Log8 has trace priority ?
  EXPECT_TRUE(blret); // OK.


  // variadic macro and built-in types log test

  try
  {
    LOG_VInfo( log6,
              " Native types logged:\n"
              "             char %c, short %d, ushort %u\n"
              "             int %d, uint %u, long %d, ulong %u\n"
              "             long long %lld, long ulong %llu\n"
              "             double float %1.4f, long double float %1.12Lf\n\n",
              '*', -32768, 32767,
              -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
              -1234567890123456LL, 1234567890123456LL,
               3.141592654, 3.14159265358979323846264338328L );
  }
  catch(Poco::RuntimeException& e)
  {
    std::cerr << "             Runtime exception: " << e.what() << std::endl
              << "             Text: " << e.displayText()  << std::endl << std::endl;
    EXPECT_TRUE(false);
  }
  catch(Poco::Exception& e)
  {
    std::cerr << "             library exception: " << "Class: " << e.className()
              << " | " << e.what() << std::endl
              << "             Text: " << e.displayText()  << std::endl << std::endl;
    EXPECT_TRUE(false);
  }
  catch(std::exception& e)
  {
    std::cerr << "             Generic exception: " << e.what() << std::endl << std::endl;
    EXPECT_TRUE(false);
  }

  cleanUp();
  EXPECT_TRUE(true);
  std::cout << "             GLogTestUT_TestGilbarcoLog() end.\n";
}



/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file spc_log_UT.cpp
 * @author gparis
 * @date Tuesday, October 07, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>
#include "spc_log_UT.h"
#include "../../../src/internals/spc_log/src/GLogger.h"

using namespace gvr::log;

GSPCLogUT::GSPCLogUT()
{
  printf("             Configuring logger...\n");
  m_pLogConfig = new GLoggerConfig("configuration/gvr_spc_log_ut.ini");
  printf("             Configured.\n");
}

GSPCLogUT::~GSPCLogUT()
{
  printf("             Releasing log configurator...\n");
  delete m_pLogConfig;
  printf("             Log configurator successfully released.\n");
}

void GSPCLogUT::SetUp()
{
  printf("             Initializing log configurator...\n");
  if( m_pLogConfig->Initialize() )
  {
	  printf("             Initialized.\n             Creating logger ...\n");
    GLogger::CreateInstance(m_pLogConfig);
    printf("             Logger successfully created.\n");
  }
  else
  {
    delete m_pLogConfig;
    printf("             Fatal error configuring logger. Test aborted!!\n");
    m_pLogConfig = NULL;
  }
}

void GSPCLogUT::TearDown()
{
  printf("             Destroying logger ...\n");
  GLogger::DestroyInstance();
  printf("             Logger successfully destroyed.\n");
}

/**
 * @brief  Unit test over class GLogger.
 *
 * @since 1.0.0
 */
TEST_F(GSPCLogUT, TestGLogger)
{
  bool blret = true;
  int  ntest = 1; // The first test is #1

  ASSERT_FALSE(m_pLogConfig == NULL);

  // Note: general console level never allows console logging.
  // This is due default hard-coded level, or the configuration file for this UT.

  GLogger* pLog = GLogger::GetInstance();
  ASSERT_FALSE(pLog == NULL);
  
  blret = pLog->Log(eCommon, eInfo, "This is an %s test #%d for the interface LOG.\n", "elemental", ntest++);
  EXPECT_FALSE(blret); // eCommon module level doesn't allow file logging.

  m_pLogConfig->SetModuleInfo(eCommon, eTrace);
  blret = pLog->Log(eCommon, eTrace, "This is an %s test #%d for the interface LOG.\n", "elemental", ntest++);
  EXPECT_FALSE(blret); // general file level does not allow file logging.

  m_pLogConfig->SetGeneralLogLevel(eTrace); // The third try will be the successful one! :)
  blret = pLog->Log(eCommon, eTrace, "This is an %s test #%d for the interface LOG.\n", "elemental", ntest++);
  EXPECT_TRUE(blret); // OK. File and module levels are now set in eTrace level.

  blret = pLog->Log(eCommon, eTrace,
                    "Native types logged:\nchar %c, short %d, ushort %u\n"
                    "int %d, uint %u, long %d, ulong %u\n"
                    "long long %lld, long ulong %llu\n"
                    "double float %1.4f, long double float %1.12Lf\n",
                    '*', -32768, 32767,
                    -1234567890, 1234567890, 0xFFFFFFF0, 0xFFFFFFF0,
                    -1234567890123456LL, 1234567890123456LL,
                     3.141592654, 3.14159265358979323846264338328L);

  EXPECT_TRUE(blret);
}


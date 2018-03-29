/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * GGlobalSystemInformationUT.h
 * Created on: Feb 01 2013
 * Author: gparis
 */

#ifndef GGlobalSystemInformationUT_h
#define GGlobalSystemInformationUT_h

#include <gtest/gtest.h>
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

class GTestedSystemInformation : public GGlobalSystemInformation
{
public:
	static GGlobalSystemInformation*   CreateGlobalSystemInformationToTest();
	static void                        DestroyTestedGlobalSystemInformation();
};

class GGlobalSystemInformationUT : public testing::Test
{
public:
	GGlobalSystemInformationUT();
	virtual ~GGlobalSystemInformationUT();

	static void SetUpTestCase();
	static void TearDownTestCase();

	void SetUp();
	void TearDown();

protected:
	GGlobalSystemInformation*   m_SysInfoToTest;
};

#endif /* GGlobalSystemInformationUT_h */

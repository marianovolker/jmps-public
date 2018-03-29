/*
 * GTimerUT.cpp
 *
 *  Created on: 08/07/2011
 *      Author: pcrespo
 */

#include "GTimerUT.h"
#include <GSystem.h>
#include <Common/Util/GSimpleCommand.h>
#include <Common/Util/GTimer.h>
#include <iostream>

#include "GSystem.h"

using namespace std;


GTimerCallback::GTimerCallback()
 : m_bExpired(false),
   m_iCounter(0)
{
}

GTimerCallback::~GTimerCallback()
{
}

void GTimerCallback::Timeout()
{
	m_bExpired = true;
}

bool GTimerCallback::IsTimerExpired()
{
	return m_bExpired;
}

void GTimerCallback::IncrementCounter()
{
	m_Mutex.Lock();
	++m_iCounter;
	m_Mutex.Unlock();
}

int GTimerCallback::GetCounter()
{
	GMutexLock lock(m_Mutex);
	return m_iCounter;
}

GTimerUT::GTimerUT()
{
}

GTimerUT::~GTimerUT()
{
}

TEST_F(GTimerUT, TestStart)
{
	const unsigned int CTimeout = 500;
	GTimerCallback theTimerCallback;
	GSimpleCommand<GTimerCallback> theCommandTimer(&theTimerCallback, &GTimerCallback::Timeout);
	GTimer* pTimer(new GTimer(&theCommandTimer, CTimeout, true, false));

	GSystem::Sleep(1000);
	EXPECT_EQ(theTimerCallback.IsTimerExpired(),false);

	pTimer->Start();

	GSystem::Sleep(1000);
	EXPECT_EQ(theTimerCallback.IsTimerExpired(),true);

	delete pTimer;
	pTimer = NULL;
}

TEST_F(GTimerUT, TestAutoStart)
{
	const unsigned int CTimeout = 500;
	GTimerCallback theTimerCallback;
	GSimpleCommand<GTimerCallback> theCommandTimer(&theTimerCallback, &GTimerCallback::Timeout);
	GTimer* pTimer(new GTimer(&theCommandTimer, CTimeout, true, true));

	GSystem::Sleep(1000);
	EXPECT_EQ(theTimerCallback.IsTimerExpired(),true);

	delete pTimer;
	pTimer = NULL;
}

TEST_F(GTimerUT, TestStop)
{
	const unsigned int CTimeout = 1000;
	GTimerCallback theTimerCallback;
	GSimpleCommand<GTimerCallback> theCommandTimer(&theTimerCallback, &GTimerCallback::Timeout);
	GTimer* pTimer(new GTimer(&theCommandTimer, CTimeout, true, false));

	pTimer->Start();
	GSystem::Sleep(500);
	pTimer->Stop();

	GSystem::Sleep(1000);
	EXPECT_EQ(theTimerCallback.IsTimerExpired(),false);

	delete pTimer;
	pTimer = NULL;
}

TEST_F(GTimerUT, TestContinuous)
{
	const unsigned int CTimeout = 100;
	GTimerCallback theTimerCallback;
	GSimpleCommand<GTimerCallback> theCommandTimer(&theTimerCallback, &GTimerCallback::IncrementCounter);
	GTimer* pTimer(new GTimer(&theCommandTimer, CTimeout, false, false));

	pTimer->Start();

	int PrevCount = 0, CurrCount = 0;
	for (int i=0;i < 5;i++)
	{
		GSystem::Sleep(200);
		CurrCount = theTimerCallback.GetCounter();
		EXPECT_GT(CurrCount,PrevCount);
		PrevCount = CurrCount;
	}

	pTimer->Stop();
	PrevCount = theTimerCallback.GetCounter();
	GSystem::Sleep(200);
	CurrCount = theTimerCallback.GetCounter();
	EXPECT_EQ(CurrCount,PrevCount);

	delete pTimer;
	pTimer = NULL;
}

TEST_F(GTimerUT, TestTimer)
{
	const unsigned int CTimeout = 500;
	GTimerCallback theTimerCallback;
	GSimpleCommand<GTimerCallback> theCommandTimer(&theTimerCallback, &GTimerCallback::Timeout);
	GTimer* pTimer(new GTimer(&theCommandTimer, CTimeout, true, false));

	pTimer->Start();
	GSystem::Sleep(750);
	EXPECT_EQ(theTimerCallback.IsTimerExpired(),true);

	delete pTimer;
	pTimer = NULL;
}

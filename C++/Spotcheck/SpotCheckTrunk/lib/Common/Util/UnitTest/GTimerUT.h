/*
 * GTimerUT.h
 *
 *  Created on: 08/07/2011
 *      Author: pcrespo
 */

#ifndef GTimerUT_h
#define GTimerUT_h

#include <gtest/gtest.h>
#include <GMutex.h>

class GTimerCallback
{
public:
	GTimerCallback();
	virtual ~GTimerCallback();

	void Timeout();

	bool IsTimerExpired();

	void IncrementCounter();

	int GetCounter();

private:
	bool m_bExpired;
	int m_iCounter;
	GMutex m_Mutex;
};

class GTimerUT : public testing::Test
{
public:
	GTimerUT();
	virtual ~GTimerUT();
};

#endif /* GTimerUT_h */

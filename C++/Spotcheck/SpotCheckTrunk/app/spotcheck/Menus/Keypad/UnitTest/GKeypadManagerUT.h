/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeypadManager.h
 * @author Sebastian Lipchak
 * @date Mar 18 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GKeypadManagerUT_h
#define GKeypadManagerUT_h

#include <gtest/gtest.h>

#include "Menus/Keypad/GKeypadManager.h"

class GTestedKeypadManager : public GKeypadManager
{
public:
	GTestedKeypadManager();
	~GTestedKeypadManager();

	inline void  Initialize()                 {return;}
	inline void  Dispose()                    {return;}
	inline bool  DeactivateTest_KeypadAction(){return true;}// To get the approval in
	                                                         // ProposeChangeStateTo()
private:

	inline void  ShowScreenTestKeystrokes()   {return;}//Do nothing, we don't use the Menu
	inline void  ShowScreenStatistics()       {return;}//Do nothing, we don't use the Menu

};



class GKeypadManagerUT : public testing::Test
{
public:
	GKeypadManagerUT();
	virtual ~GKeypadManagerUT();

	void SetUp();
	void TearDown();

	GTestedKeypadManager*   m_pManager;
};

#endif /* GKeypadManagerUT_h */

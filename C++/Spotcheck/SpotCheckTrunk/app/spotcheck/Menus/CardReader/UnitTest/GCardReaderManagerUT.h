/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderManager.h
 * @author Sebastian Lipchak
 * @date Apr 22 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderManagerUT_h
#define GCardReaderManagerUT_h

#include <gtest/gtest.h>

#include "Menus/CardReader/GCardReaderManager.h"

class GTestedCardReaderManager : public GCardReaderManager
{
public:
	GTestedCardReaderManager();
	~GTestedCardReaderManager();

	inline void  Initialize()      { return ; }
	inline void  Dispose()         { return ; }

private:


};



class GCardReaderManagerUT : public testing::Test
{
public:
	GCardReaderManagerUT();
	virtual ~GCardReaderManagerUT();

	void SetUp();
	void TearDown();

	GTestedCardReaderManager*   m_pManager;
};

#endif /* GCardReaderManagerUT_h */

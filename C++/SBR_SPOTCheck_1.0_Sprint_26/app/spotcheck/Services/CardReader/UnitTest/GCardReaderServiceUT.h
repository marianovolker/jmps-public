/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderService.h
 * @author Guillermo Paris
 * @date Feb 18 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderServiceUT_h
#define GCardReaderServiceUT_h

#include <gtest/gtest.h>

class GCardReaderService;

class GCardReaderServiceUT : public testing::Test
{
public:
	GCardReaderServiceUT();
	virtual ~GCardReaderServiceUT();

	void SetUp();
	void TearDown();

	GCardReaderService*		m_pService;
};

#endif /* GCardReaderServiceUT_h */

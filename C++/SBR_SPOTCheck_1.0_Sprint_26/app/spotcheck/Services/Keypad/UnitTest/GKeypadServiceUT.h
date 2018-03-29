/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeypadService.h
 * @author Mariano Volker
 * @date March 19 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GKeypadServiceUT_h
#define GKeypadServiceUT_h

#include <gtest/gtest.h>


class GKeypadService;


class GKeypadServiceUT : public testing::Test
{
public:
	GKeypadServiceUT();
	virtual ~GKeypadServiceUT();

	void SetUp();
	void TearDown();

	GKeypadService*         m_pService;


};

#endif

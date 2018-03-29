/*
 * XmlUT.h
 *
 *  Created on: Jun 22, 2011
 *      Author: pcantarini
 */
#include <gtest/gtest.h>

#ifndef XmlUT_h
#define XmlUT_h

class XmlUT : public testing::Test
{
public:
	XmlUT (void);
	virtual ~XmlUT (void);

	void SetUp (void);
	void TearDown (void);
};

#endif

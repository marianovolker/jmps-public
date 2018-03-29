/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GContactlessReaderService.h
 * @author Mariano Volker
 * @date Jul 8 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GContactlessReaderServiceUT_h
#define GContactlessReaderServiceUT_h

#include <gtest/gtest.h>

#include "Common/Util/DEBUGOUT.h"

class GContactlessReaderService;

class GContactlessReaderServiceUT : public testing::Test
{
public:
	GContactlessReaderServiceUT();
	virtual ~GContactlessReaderServiceUT();

	void SetUp();
	void TearDown();

	void ConvertToDecimal(UINT8* pui8Buffer, UINT16 u16lenclp, std::string& strOut);

	GContactlessReaderService*		m_pService;
};

#endif /* GContactlessReaderServiceUT_h */

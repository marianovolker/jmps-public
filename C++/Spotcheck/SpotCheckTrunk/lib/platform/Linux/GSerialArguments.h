//-----------------------------------------------------------------------
//
//  File: SerialArguments.h
//
//  Copyright 2011 Gilbarco Inc.
//  All rights reserved.
//
//  Description:
//     This file defines SerialComParameters object.
//
//  Warnings, Limitations:
//
//  See Also:
//
//  Revision History:
//
//  Date        Name             Change Description
//  ---------------------------------------------------------------------
//
//  2011-11-11  Andrei K		File creation.
//
//-----------------------------------------------------------------------
//

#ifndef __SerialArguments_h__
#define __SerialArguments_h__

#include "GSerialPortTypes.h"

#include <termios.h>

class GSerialArguments
{
public:
	GSerialArguments(int baudRate, eParityType parity, eStopBits stopBits, eDataBits dataSize);
	virtual ~GSerialArguments() {}

	termios getPortSettings(const termios& referencePortSettings) const;
	bool operator != (const GSerialArguments& other) const;

	static tcflag_t getTermiosBaud(int baudRate);

private:
	static void setParity(eParityType parity, termios& settings);
	static void setStopBits(eStopBits stopBits, termios& settings);
	static void setCharacterSize(eDataBits dataBits, termios& settings);
	static bool isValidBaudRate(int baudRate);

private:
	int 		m_baudRate;
	eParityType 		m_parity;
	eStopBits 	m_stopBits;
	eDataBits 	m_dataBits;
};

#endif /* __SerialArguments_h__ */

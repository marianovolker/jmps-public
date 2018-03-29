//-----------------------------------------------------------------------
//
//  File:  SerialArguments.cpp
//
//  Copyright 2011 Gilbarco Inc.
//  All rights reserved.
//
//  Description:
//     This file contains the implementation for a SerialArguments  class.
//
//  Warnings, Limitations:
//
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


//
//----------------------------
//  Dependencies
//----------------------------
//

#include "GSerialArguments.h"

GSerialArguments::GSerialArguments(int baudRate, eParityType parity, eStopBits stopBits, eDataBits dataSize)
{
	m_baudRate = baudRate;
	m_parity = parity;
	m_dataBits = dataSize;
	m_stopBits = stopBits;
}

termios GSerialArguments::getPortSettings(const termios& referencePortSettings) const
{
	termios outputSettings = referencePortSettings;

	// Ensure no unwanted port config flags are set
	outputSettings.c_cflag = 0;
	outputSettings.c_iflag = 0;
	outputSettings.c_oflag = 0;
	outputSettings.c_lflag = 0;
	
	int baud = getTermiosBaud(m_baudRate);

	outputSettings.c_cflag |= (CREAD | CLOCAL | baud);
	
	setParity(m_parity,  outputSettings);
	setStopBits(m_stopBits, outputSettings);
	setCharacterSize(m_dataBits, outputSettings);

	return outputSettings;
}

bool GSerialArguments::operator != (const GSerialArguments& other) const
{
	if(m_baudRate != other.m_baudRate) return true;
	if(m_parity != other.m_parity) return true;
	if(m_stopBits != other.m_stopBits) return true;
	if(m_dataBits != other.m_dataBits) return true;

	return false;
}

tcflag_t GSerialArguments::getTermiosBaud(int baudRate)
{
	tcflag_t termiosBaud = B0;

	switch (baudRate)
     {
		case 115200:
			termiosBaud = B115200;
			break;
		case 57600:
			termiosBaud = B57600;
			break;
        case 38400:
           termiosBaud = B38400;
           break;
        case 19200:
           termiosBaud  = B19200;
           break;
        case 9600:
           termiosBaud  = B9600;
           break;
        case 4800:
           termiosBaud  = B4800;
           break;
        case 2400:
           termiosBaud  = B2400;
           break;
        case 1800:
           termiosBaud  = B1800;
           break;
        case 1200:
           termiosBaud  = B1200;
           break;

        default:
        	//TODO: LOG throw std::invalid_argument("baudRate");
        	break;
     }

	return termiosBaud;
}

void GSerialArguments::setCharacterSize(eDataBits dataBits, termios& settings)
{
	settings.c_cflag &= ~CSIZE;
	switch (dataBits)
     {
        case eDataBits8:
        default:
        	settings.c_cflag |= CS8;
           break;
        case eDataBits7:
        	settings.c_cflag |= CS7;
           break;
     }
}

void GSerialArguments::setStopBits(eStopBits stopBits, termios& settings)
{
     switch (stopBits)
     {
        case eStopBits1:
        default:
        	settings.c_cflag &= ~CSTOPB;
           break;
        case eStopBits2:
        	settings.c_cflag |= CSTOPB;
           break;
     }  //end of switch stop bits

}

void GSerialArguments::setParity(eParityType parity, termios& settings)
{
	switch (parity)
     {
        case eParityNone:
        default:
        	settings.c_cflag &=  ~PARENB;
        	settings.c_iflag |=  IGNPAR;
           break;
        case eParityOdd:
        	settings.c_cflag |= PARENB;
        	settings.c_cflag |= PARODD;
        	settings.c_iflag |= (INPCK);
           break;
        case eParityEven:
        	settings.c_cflag |= PARENB;
        	settings.c_cflag &= ~PARODD;
        	settings.c_iflag |= (INPCK);
           break;
     }

}

bool GSerialArguments::isValidBaudRate(int baudRate)
{
	switch (baudRate)
     {
		case 115200:
		case 57600:
        case 38400:
        case 19200:
        case 9600:
        case 4800:
        case 2400:
        case 1800:
        case 1200:
        	return true;
           break;
     }

	return false;
}

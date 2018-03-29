/*
 * GSerialPortUT.cpp
 *
 *  Created on: Jun 9, 2011
 *      Author: pcrespo
 *
 * On Windows com0com program must be installed otherwise this class will fail
 * it can be found here http://com0com.sourceforge.net/
 * use default virtual port pair CNCA0 <-> CNCB0
 */

#include <string.h>
#include <string>
#include <iostream>

#include "GSerialPortUT.h"
#include "GSerialPort.h"
#include "GPseudoSerialDevice.h"

using namespace std;

TEST(GSerialPortUT, TestOpenClose)
{
	GPseudoSerialDevice device(9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	GSerialPort serialPort(device.GetClientPortName(),
			9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	EXPECT_FALSE(serialPort.Close());

	EXPECT_TRUE(serialPort.Open());

	EXPECT_TRUE(serialPort.Close());
}

TEST(GSerialPortUT, TestSetTimeouts)
{
	GPseudoSerialDevice device(9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	GSerialPort serialPort(device.GetClientPortName(),
			9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	EXPECT_TRUE(serialPort.SetReadTimeout(1000));

	EXPECT_TRUE(serialPort.SetWriteTimeout(500));

	EXPECT_TRUE(serialPort.Open());
}

TEST(GSerialPortUT, TestReadTimeout)
{
	GPseudoSerialDevice device(9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	GSerialPort serialPort(device.GetClientPortName(),
			9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);


	unsigned char ucBuffer[128];

	memset(ucBuffer, 0, sizeof(ucBuffer));

	EXPECT_TRUE(serialPort.SetReadTimeout(500));

	EXPECT_TRUE(serialPort.Open());

	//timeout
	int iret = serialPort.Read(ucBuffer, sizeof(ucBuffer));
	EXPECT_EQ(iret, 0);
}


TEST(GSerialPortUT, TestFlowControlNone)
{
	GPseudoSerialDevice device(9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	GSerialPort* pClient = new GSerialPort(device.GetClientPortName(),
			9600,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlNone);

	pClient->Open();

	unsigned char ucBuffer[128];

	memset(ucBuffer, 0, sizeof(ucBuffer));
	string strDummy("dummy message\n");
	string strRead;

	for( int i=0; i<10; ++i )
	{
		device.Write(reinterpret_cast<const unsigned char*>(strDummy.c_str()), strDummy.length());

		int iread = pClient->Read(ucBuffer, sizeof(ucBuffer));
		ASSERT_EQ(iread, strDummy.length());

		strRead.assign(reinterpret_cast<char*>(ucBuffer), iread);
		ASSERT_STREQ(strDummy.c_str(), strRead.c_str());
	}

	pClient->Close();

	delete pClient;
}

TEST(GSerialPortUT, TestSoftFlowControl)
{
	GPseudoSerialDevice device(115200,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlSoftRTSCTS);

	GSerialPort* pClient = new GSerialPort(device.GetClientPortName(),
										115200,
										eDataBits8,
										eParityNone,
										eStopBits1,
										eFlowControlSoftRTSCTS);

	pClient->Open();

	unsigned char ucBuffer[128];

	memset(ucBuffer, 0, sizeof(ucBuffer));
	string strDummy("dummy message\n");
	string strRead;

	for( int i=0; i<10; ++i )
	{

		device.Write(reinterpret_cast<const unsigned char*>(strDummy.c_str()), strDummy.length());

		int iread = pClient->Read(ucBuffer, sizeof(ucBuffer));
		ASSERT_EQ(iread, strDummy.length());

		strRead.assign(reinterpret_cast<char*>(ucBuffer), iread);
		ASSERT_STREQ(strDummy.c_str(), strRead.c_str());
	}

	pClient->Close();

	delete pClient;
}

TEST(GSerialPortUT, TestHardFlowControl)
{
	GPseudoSerialDevice device(115200,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlHardware);

	GSerialPort* pClient = new GSerialPort(device.GetClientPortName(),
			115200,
			eDataBits8,
			eParityNone,
			eStopBits1,
			eFlowControlHardware);

	pClient->Open();

	unsigned char ucBuffer[512];

	memset(ucBuffer, 0, sizeof(ucBuffer));

	int iread(0);
	stringstream strout;
	string strRead;

	for( int i=1; i<=10; ++i )
	{
		strout << "dummy message #\n" << i;

		//write on serial 1
		device.Write(reinterpret_cast<const unsigned char*>(strout.str().c_str()), strout.str().length());
		//read from serial 2
		iread = pClient->Read(ucBuffer, sizeof(ucBuffer));
		ASSERT_GT(iread, 0);

		strRead.assign(reinterpret_cast<char*>(ucBuffer), iread);
		//compare string sent and received
		ASSERT_STREQ(strout.str().c_str(), strRead.c_str());

		//write on serial 2
		device.Write(reinterpret_cast<const unsigned char*>(strout.str().c_str()), strout.str().length());
		//read from serial 1
		iread = pClient->Read(ucBuffer, sizeof(ucBuffer));
		ASSERT_GT(iread, 0);

		strRead.assign(reinterpret_cast<char*>(ucBuffer), iread);
		//compare string sent and received
		ASSERT_STREQ(strout.str().c_str(), strRead.c_str());
	}

	pClient->Close();

	delete pClient;
}

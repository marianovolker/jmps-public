/**
* @file GSerialPortUT.h
* @brief unit test for serial port. it validates basic device operations.
* In the following list of test,
* 	- TestFlowControlNone
* 	- TestSoftFlowControl
* 	- TestHardFlowControl
* Write and Read operations are tested with a virtual loopback. the com0com program must be installed otherwise test will fail
* it can be found here http://com0com.sourceforge.net/
* use default virtual port pair CNCA0 <-> CNCB0
*
* @author Pablo Crespo
*
* @date 6/21/2011
*/

#ifndef GSerialPortUT_h
#define GSerialPortUT_h

#include <gtest/gtest.h>

class GSerialPort;

class GSerialPortUT : public testing::Test
{
public:
	GSerialPortUT();
	virtual ~GSerialPortUT();

	void SetUp();

	void TearDown();

protected:
	GSerialPort* m_pSerialPort;

};

#endif /* GSerialPortUT_h */

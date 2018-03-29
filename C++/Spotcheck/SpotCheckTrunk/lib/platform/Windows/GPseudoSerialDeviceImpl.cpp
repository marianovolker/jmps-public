/*
 * GPseudoSerialDeviceImpl.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: khaitasa
 */

#include "GPseudoSerialDeviceImpl.h"

#include <stdexcept>

/*!
* com0com program must be installed otherwise this class will fail
* it can be found here http://com0com.sourceforge.net/
* use default virtual port pair CNCA0 <-> CNCB0
*/
GPseudoSerialDeviceImpl::GPseudoSerialDeviceImpl(unsigned long BaudRate,
	    eDataBits     DataBits,
	    eParityType   ParityType,
	    eStopBits     StopBits,
	    eFlowControl  FlowControl /*= eFlowControlNone*/)
{
	m_Port = new GSerialPort("CNCA0",
			BaudRate,
			DataBits,
			ParityType,
			StopBits,
			FlowControl);

	m_Port->Open();
}

GPseudoSerialDeviceImpl::~GPseudoSerialDeviceImpl()
{
	m_Port->Close();
	delete m_Port;
}

void GPseudoSerialDeviceImpl::Write(const void *buf, size_t count)
{
	size_t writtenBytes = m_Port->Write((unsigned char*)buf, count);
	if(writtenBytes != count) throw std::runtime_error("writtenBytes not equal to expected");
}

bool GPseudoSerialDeviceImpl::TryRead(std::string& buffer, int maxLenToRead)
{
	  unsigned char* message = new unsigned char[maxLenToRead+1];
	  int readBytesCount = m_Port->Read(message, maxLenToRead);

	  bool readSuccess = false;
	  if(readBytesCount>0)
	  {
		  buffer = std::string((char*)message, readBytesCount);
		  readSuccess = true;
	  }

	  delete message;
	  return readSuccess;
}

const char* GPseudoSerialDeviceImpl::GetClientPortName()  const
{
	//Assuming default virtual port pair CNCA0 <-> CNCB0
	static const char* ClinetPortName = "CNCB0";
	return ClinetPortName;
}

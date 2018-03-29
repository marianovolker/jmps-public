/*
 * GPseudoSerialDeviceImpl.cpp
 *
 *  Created on: Apr 27, 2012
 *      Author: khaitasa
 */

#include "GPseudoSerialDeviceImpl.h"

GPseudoSerialDeviceImpl::GPseudoSerialDeviceImpl(unsigned long BaudRate,
	    eDataBits     DataBits,
	    eParityType   ParityType,
	    eStopBits     StopBits,
	    eFlowControl  FlowControl /*= eFlowControlNone*/)
{
	m_PortFd = getpt();
	if(m_PortFd == InvalidPortFd)
	{
		//TODO: LOG??	throw std::runtime_error("Comm::init, getpt failed");
	}

	if(grantpt(m_PortFd) == -1 )
	{
		//TODO: LOG??	throw std::runtime_error("Comm::init, master port failed to grant access");
	}

	if(unlockpt(m_PortFd) == -1 )
	{
		//TODO: LOG??	throw std::runtime_error("Comm::init, master port failed to unlock pt");
	}
}

GPseudoSerialDeviceImpl::~GPseudoSerialDeviceImpl()
{
	if(m_PortFd != InvalidPortFd) close(m_PortFd);
}

void GPseudoSerialDeviceImpl::Write(const void *buf, size_t count)
{
	//Serial data has to be with following end line otherwise data is waiting in the server
	size_t writtenBytes = ::write(m_PortFd, buf, count);
	if(writtenBytes != count)
	{
	  // TODO: throw std::runtime_error("writtenBytes not equal to expected");
	}
}

bool GPseudoSerialDeviceImpl::TryRead(std::string& buffer, int maxLenToRead)
{
	  char* message = new char[maxLenToRead+1];
	  ssize_t readBytesCount = read(m_PortFd, message, maxLenToRead);

	  bool readSuccess = false;
	  if(readBytesCount>0)
	  {
		  buffer = std::string(message, readBytesCount);
		  readSuccess = true;
	  }

	  delete message;
	  return readSuccess;
}

const char* GPseudoSerialDeviceImpl::GetClientPortName()  const
{
	return ptsname(m_PortFd);
}

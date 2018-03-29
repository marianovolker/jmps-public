/*
 * GPseudoSerialDeviceImpl.h
 *
 *  Created on: Apr 27, 2012
 *      Author: khaitasa
 */

#ifndef GPSEUDOSERIALDEVICEIMPL_H_
#define GPSEUDOSERIALDEVICEIMPL_H_

#include <stdlib.h>
#include <string>

#include "GSerialPortTypes.h"
#include "GSerialPort.h"

class GPseudoSerialDeviceImpl
{
public:
	GPseudoSerialDeviceImpl(unsigned long BaudRate,
		    eDataBits     DataBits,
		    eParityType   ParityType,
		    eStopBits     StopBits,
		    eFlowControl  FlowControl = eFlowControlNone);

	virtual ~GPseudoSerialDeviceImpl();

	void Write(const void *buf, size_t count);
	bool TryRead(std::string& buffer, int maxLenToRead);
	const char* GetClientPortName()  const;

private:
  GSerialPort* m_Port ;
};

#endif /* GPSEUDOSERIALDEVICEIMPL_H_ */

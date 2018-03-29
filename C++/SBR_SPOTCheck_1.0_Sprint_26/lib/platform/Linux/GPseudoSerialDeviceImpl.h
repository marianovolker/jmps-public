/*!
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GPseudoSerialDeviceImpl.h
 * @date Apr 27, 2012
 * @author khaitasa
 * @note Modified by Guillermo Paris on Feb 22 2013 for gcc 4.7 builds
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

#ifndef GPSEUDOSERIALDEVICEIMPL_H_
#define GPSEUDOSERIALDEVICEIMPL_H_

#include <GLinux.h>
#ifdef SPOT_M3 // Excluded by header macros, included by brute force :)
#	include "stdlib_lost.h"
#endif
#include <string>

#include "GSerialPortTypes.h"

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
  static const int InvalidPortFd = -1;
  int m_PortFd ;
};

#endif /* GPSEUDOSERIALDEVICEIMPL_H_ */

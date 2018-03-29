// GPseudoSerialDevice.h


#ifndef GPseudoSerialDevice_h
#define GPseudoSerialDevice_h

#include "GPseudoSerialDeviceImpl.h"

class GPseudoSerialDevice : GPseudoSerialDeviceImpl
{
public:
	GPseudoSerialDevice(unsigned long BaudRate,
			    eDataBits     DataBits,
			    eParityType   ParityType,
			    eStopBits     StopBits,
			    eFlowControl  FlowControl = eFlowControlNone)
	   : GPseudoSerialDeviceImpl( BaudRate,
			             DataBits,
			             ParityType,
			             StopBits,
			             FlowControl)
	{
	}

  void Write(const void *buf, size_t count)
  {
	  GPseudoSerialDeviceImpl::Write(buf, count);
  }

  bool TryRead(std::string& buffer, int maxLenToRead)
  {
	  return GPseudoSerialDeviceImpl::TryRead(buffer, maxLenToRead);
  }

  const char* GetClientPortName()  const
  {  
  	return GPseudoSerialDeviceImpl::GetClientPortName();
  }
  
  
private:
	// Prohibit GFile copy.
	GPseudoSerialDevice(const GPseudoSerialDevice &right);
	GPseudoSerialDevice& operator=(const GPseudoSerialDevice &right);
};

#endif //#ifndef GPseudoSerialDevice_h

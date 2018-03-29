// GSerialPortImpl.h
//
// Linux implementation of the GSerialPort class.

#ifndef GSerialPortImpl_h
#define GSerialPortImpl_h

#include <string>

#include "GSerialPortTypes.h"
#include "GMutex.h"
#include "GSerialArguments.h"

// GSerialPortImpl

class GSerialPortImpl
{
protected:
	// Constructor
	GSerialPortImpl(const char*   PortName,
			        unsigned long BaudRate,
			        eDataBits     DataBits,
			        eParityType   ParityType,
			        eStopBits     StopBits,
			        eFlowControl  FlowControl);

	// Destructor
	virtual ~GSerialPortImpl(void);

	bool Open(void);

	bool Close(void);

	int Read(unsigned char *Buf, int BufSize);

	int Write(const unsigned char *Buf, int BufLen);

	bool SetConfiguration(unsigned long BaudRate,
			              eDataBits     DataBits,
			              eParityType   ParityType,
			              eStopBits     StopBits,
			              eFlowControl  FlowControl);

	bool SetModemControlLineStatus(eModemControlLine Line,bool bOn);

	bool GetModemControlLineStatus(eModemControlLine Line,bool& bOn);

	void  FlushInput();

	void  FlushOutput();

	bool SetWriteTimeout(int iMilliseconds);

    bool SetReadTimeout(int iMilliseconds);

private:
	bool SetConfiguration();

    bool WaitForCTS();

protected:
	// Port name.
	std::string   m_PortName;

	// Port settings.
	eFlowControl  m_eFlowControl;

	//it is defined in milliseconds
	int m_iTxTimeout;
	int m_iRxTimeout;

	// device file descriptor
	int m_iFileDescriptor;

	//mutex to synchronize operations over device handler
	GMutex m_Mutex;

	GSerialArguments m_arguments;
	
private:
	// Prohibit copy.
	GSerialPortImpl(const GSerialPortImpl &right);
	GSerialPortImpl& operator=(const GSerialPortImpl &right);
};


#endif

// GSerialPort.h

#ifndef GSerialPort_h
#define GSerialPort_h


#include <GSerialPortTypes.h>
#include <GSerialPortImpl.h>

// GSerialPort

class GSerialPort : private GSerialPortImpl
{
public:
	// Constructor
	GSerialPort(const char*   PortName,
			    unsigned long BaudRate,
			    eDataBits     DataBits,
			    eParityType   ParityType,
			    eStopBits     StopBits,
			    eFlowControl  FlowControl = eFlowControlNone)
	   : GSerialPortImpl(PortName,
			             BaudRate,
			             DataBits,
			             ParityType,
			             StopBits,
			             FlowControl)
	{
	}

	// Destructor
	virtual ~GSerialPort(void)
	{
	}

	bool Open(void)
	{
		return GSerialPortImpl::Open();
	}

	bool Close(void)
	{
		return GSerialPortImpl::Close();
	}

	/**
	 * @param Buf
	 * 	byte reads
	 * @param BufSize
	 * 	Buf size
	 * @return 0 timeout
	 * @return > 0 byte read
	 * @return < 0 errors see eReturnCodes enum
	 */
	int Read(unsigned char *Buf, int BufSize)
	{
		return GSerialPortImpl::Read(Buf, BufSize);
	}

	/**
	 * @param Buf
	 * 	byte to be written to serial port
	 * @param BufSize
	 * 	Buf size
	 * @return 0 timeout
	 * @return > 0 byte written to port
	 * @return < 0 errors see eReturnCodes enum
	 */
	int Write(const unsigned char *Buf, int BufLen)
	{
		return GSerialPortImpl::Write(Buf, BufLen);
	}

	bool SetConfiguration(unsigned long BaudRate,
			              eDataBits     DataBits,
			              eParityType   ParityType,
			              eStopBits     StopBits,
			              eFlowControl  FlowControl)
	{
		return GSerialPortImpl::SetConfiguration(BaudRate,
				                                 DataBits,
				                                 ParityType,
				                                 StopBits,
				                                 FlowControl);
	}

	bool SetModemControlLineStatus(eModemControlLine Line,bool bOn)
	{
		return GSerialPortImpl::SetModemControlLineStatus(Line, bOn);
	}

	bool GetModemControlLineStatus(eModemControlLine Line,bool& bOn)
	{
		return GSerialPortImpl::GetModemControlLineStatus(Line, bOn);
	}

	void  FlushInput()
	{
		return GSerialPortImpl::FlushInput();
	}

	void  FlushOutput()
	{
		return GSerialPortImpl::FlushOutput();
	}

	/**
	 *	Number of milliseconds to wait for a character to be sent.
	 * @param iMilliseconds
	 * 	time in milliseconds
	 * @return
	 * true/false indicate if timeout could be applied
	 */
	bool SetWriteTimeout(int iMilliseconds)
    {
    	return GSerialPortImpl::SetWriteTimeout(iMilliseconds);
    }

	/**
	 *	Number of milliseconds to wait for a character read before timing out.
	 * @param iMilliseconds
	 * 	time in milliseconds
	 * @return
	 *	true/false indicate if timeout could be applied
	 */
    bool SetReadTimeout(int iMilliseconds)
    {
    	return GSerialPortImpl::SetReadTimeout(iMilliseconds);
    }

private:
	// Prohibit GSerialPort copy.
	GSerialPort(const GSerialPort &right);
	GSerialPort& operator=(const GSerialPort &right);
};

#endif

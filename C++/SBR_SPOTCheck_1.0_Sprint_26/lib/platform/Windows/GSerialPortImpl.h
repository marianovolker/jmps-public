// GSerialPortImpl.h
//
// Windows implementation of the GSerialPort class.

#ifndef GSerialPortImpl_h
#define GSerialPortImpl_h

#include <GSerialPortTypes.h>
#include <GMutex.h>
#include <windows.h>
#include <string>

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
    /**
     * @brief 	it must be called only after a communication error.
     * @param	ret - an eReturnCode value in case an error has occurred
     * @return	true - if an error occurred, ret will contain error code
     * @return 	false - could not retrieve error code
     */
    bool GetCommError(eSerialPortReturnCodes& ret);

    bool WaitForCTS();

protected:
	// Port name.
	std::string   m_PortName;

	// Port settings.
	unsigned long m_BaudRate;
	eDataBits     m_eDataBits;
	eParityType   m_eParityType;
	eStopBits     m_eStopBits;
	eFlowControl  m_eFlowControl;

	//it is defined in milliseconds
	int m_iTxTimeout;
	int m_iRxTimeout;

	//device handler
	HANDLE m_hSerial;

	//mutex to synchronize operations over device handler
	GMutex m_Mutex;
private:
	// Prohibit copy.
	GSerialPortImpl(const GSerialPortImpl &right);
	GSerialPortImpl& operator=(const GSerialPortImpl &right);
};


#endif

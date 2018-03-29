// GSerialPortImpl.cpp
//
// Telium implementation of the GSerialPort class.

#include <GSerialPortImpl.h>

using namespace std;

GSerialPortImpl::GSerialPortImpl(const char*   PortName,
		                         unsigned long BaudRate,
		                         eDataBits     DataBits,
		                         eParityType   ParityType,
		                         eStopBits     StopBits,
		                         eFlowControl  FlowControl)
: m_PortName(PortName),
  m_BaudRate(BaudRate),
  m_eDataBits(DataBits),
  m_eParityType(ParityType),
  m_eStopBits(StopBits),
  m_eFlowControl(FlowControl),
  m_iTxTimeout(eDefaultTxTimeout),
  m_iRxTimeout(eDefaultRxTimeout),
  m_hSerial(NULL)
{

}

GSerialPortImpl::~GSerialPortImpl(void)
{
	Close();
}

bool GSerialPortImpl::Open(void)
{
	if( m_hSerial )
		return false;

	//open device
	{
		GMutexLock oMutexLock(m_Mutex);
		string strDeviceName("\\\\.\\");

		//use full windows device name format
		strDeviceName += m_PortName;
		//open port
		m_hSerial = CreateFile(strDeviceName.c_str(),	// device name in form \\.\name
					GENERIC_READ | GENERIC_WRITE,		// access mode
					0, 									// not shared
					NULL,								// handler can not be inherit by child process
					OPEN_EXISTING,						// open device only if it exist
					FILE_ATTRIBUTE_NORMAL,				// file attributes
					NULL);								// template file not needed for opening devices

		// device could not be opened
		if( m_hSerial==INVALID_HANDLE_VALUE )
		{
			m_hSerial = NULL;
			return false;
		}
	}

	//set configuration
	if( !SetConfiguration(m_BaudRate, m_eDataBits, m_eParityType, m_eStopBits, m_eFlowControl) )
	{
		Close();
		return false;
	}

	//set timeouts
	if( !SetWriteTimeout(m_iTxTimeout) || !SetReadTimeout(m_iRxTimeout) )
	{
		Close();
		return false;
	}
	return true;
}

void  GSerialPortImpl::FlushInput()
{
	PurgeComm(m_hSerial, PURGE_RXCLEAR | PURGE_RXABORT);
}

void  GSerialPortImpl::FlushOutput()
{
	PurgeComm(m_hSerial, PURGE_TXCLEAR | PURGE_TXABORT);
}

bool GSerialPortImpl::SetWriteTimeout(int iMilliseconds)
{
	//if the port has not been yet open just override the default timeout
	if( !m_hSerial )
	{
		m_iTxTimeout = iMilliseconds;
		return true;
	}

	//if the port is open so go ahead and set the new timeout
	COMMTIMEOUTS timeouts;
	bool bRet(false);

	memset(static_cast<void*>(&timeouts), 0, sizeof(timeouts));

	GMutexLock oMutexLock(m_Mutex);
	if( GetCommTimeouts(m_hSerial, &timeouts) )
	{
		//minimum time to transmit msg
		timeouts.WriteTotalTimeoutConstant = iMilliseconds;
		//time allowed per tx byte
		timeouts.WriteTotalTimeoutMultiplier = 10;
		if( SetCommTimeouts(m_hSerial, &timeouts) )
		{
			m_iTxTimeout = iMilliseconds;
			bRet = true;
		}
	}
	return bRet;
}

bool GSerialPortImpl::SetReadTimeout(int iMilliseconds)
{
	//if the port has not been yet open just override the default timeout
	if( !m_hSerial )
	{
		m_iRxTimeout = iMilliseconds;
		return true;
	}

	//if the port is open so go ahead and set the new timeout
	COMMTIMEOUTS timeouts;
	bool bRet(false);

	memset(static_cast<void*>(&timeouts), 0, sizeof(timeouts));

	GMutexLock oMutexLock(m_Mutex);
	if( GetCommTimeouts(m_hSerial, &timeouts) )
	{
		//max time between rx bytes in msg
		timeouts.ReadIntervalTimeout = 0;
		//time allowed per rx byte
		timeouts.ReadTotalTimeoutMultiplier = 0;
		//minimum time allowed for rx msg
		timeouts.ReadTotalTimeoutConstant = iMilliseconds;
		if( SetCommTimeouts(m_hSerial, &timeouts) )
		{
			m_iRxTimeout = iMilliseconds;
			bRet = true;
		}
	}
	return bRet;
}

bool GSerialPortImpl::Close(void)
{
	if( !m_hSerial )
		return false;

	bool bRet(false);

	GMutexLock oMutexLock(m_Mutex);
	if( CloseHandle(m_hSerial) )
	{
		m_hSerial = NULL;
		bRet = true;
	}

	return bRet;
}

bool GSerialPortImpl::GetCommError(eSerialPortReturnCodes& ret)
{
	DWORD dwError(0);

	if( ClearCommError(m_hSerial, &dwError, NULL) && (dwError != 0) )
	{
		if( dwError & CE_BREAK )
		{
			ret = eSerialPortBreak;
		}
		else if( dwError & CE_FRAME )
		{
			ret = eSerialPortFramingError;
		}
		else if( dwError & CE_OVERRUN )
		{
			ret = eSerialPortOverRunError;
		}
		else if( dwError & CE_RXOVER )
		{
			ret = eSerialPortOverRunError;
		}
		else if( dwError & CE_RXPARITY )
		{
			ret = eSerialPortParityError;
		}
		return true;
	}
	return false;
}

bool GSerialPortImpl::WaitForCTS()
{
	int iTxTimeout(m_iTxTimeout);
	bool bStatus(false);

	do
	{
		// Sleep 10 ms.
		Sleep(10);
		// Get CTS status.
		if( !GetModemControlLineStatus(eCTS, bStatus) )
		{
			// Attempt to clear RTS. We might expect this to fail since the call above failed.
			SetModemControlLineStatus(eRTS, FALSE);
			return false;
		}
		iTxTimeout-=10;
	} while((bStatus != true) && (iTxTimeout > 0));

	return bStatus;
}


int GSerialPortImpl::Read(unsigned char *Buf, int BufSize)
{
	DWORD dwBytesRead(0);

	if( !m_hSerial )
		return eSerialPortError;

	if( !ReadFile(m_hSerial, static_cast<void*>(Buf), BufSize, &dwBytesRead, NULL) )
	{
		eSerialPortReturnCodes eRet(eSerialPortError);
		GetCommError(eRet);
		return eRet;
	}

	//timeout
	if( !dwBytesRead )
		return eSerialPortTimeout;

	return static_cast<int>(dwBytesRead);
}

int GSerialPortImpl::Write(const unsigned char *Buf, int BufLen)
{
	DWORD dwBytesWritten(0);

	if( !m_hSerial )
		return eSerialPortError;

	// Check for soft RTSCTS flow control handling.
	if (m_eFlowControl == eFlowControlSoftRTSCTS)
	{
        // Set RTS.
		if( !SetModemControlLineStatus(eRTS, true) )
			return eSerialPortError;

		// Wait for CTS.
		if( !WaitForCTS() )
		{
			//Timeout waiting for CTS
			// Clear RTS.
			SetModemControlLineStatus(eRTS, false);
			return eSerialPortTimeout;
		}
	}

	if( !WriteFile(m_hSerial, static_cast<const void*>(Buf), BufLen, &dwBytesWritten, NULL) )
	{
		eSerialPortReturnCodes eRet(eSerialPortError);
		GetCommError(eRet);
		return eRet;
	}

	// Check for soft RTSCTS flow control handling.
	if (m_eFlowControl == eFlowControlSoftRTSCTS)
	{
        // Clear RTS.
		SetModemControlLineStatus(eRTS, false);
	}

	//timeout
	if( !dwBytesWritten )
		return eSerialPortTimeout;

	return static_cast<int>(dwBytesWritten);
}

bool GSerialPortImpl::SetConfiguration(unsigned long BaudRate, eDataBits DataBits, eParityType ParityType, eStopBits StopBits,eFlowControl FlowControl)
{
	DCB dcbSerial;
	COMMTIMEOUTS timeouts;

	if( !m_hSerial )
		return false;

	memset(static_cast<void*>(&dcbSerial), 0, sizeof(dcbSerial));
	memset(static_cast<void*>(&timeouts), 0, sizeof(timeouts));

	// *** begin DCB configuration *** //
	dcbSerial.DCBlength = sizeof(dcbSerial);

	// get current configuration to use as base
	{
		GMutexLock oMutexLock(m_Mutex);
		if( !GetCommState(m_hSerial, &dcbSerial) )
			return false;
	}

	dcbSerial.fBinary = TRUE;
	dcbSerial.BaudRate = static_cast<DWORD>(BaudRate);
	dcbSerial.ByteSize = static_cast<BYTE>(DataBits);
	dcbSerial.StopBits = ( StopBits == eStopBits1 ? ONESTOPBIT : TWOSTOPBITS );

	//define parity to be used in winbase form
	BYTE u8Parity(NOPARITY);
	if(ParityType == eParityEven )
	{
		u8Parity = EVENPARITY;
	}
	else if ( ParityType == eParityOdd )
	{
		u8Parity = ODDPARITY;
	}
	dcbSerial.Parity = u8Parity;

	//set params related to flow control
	if( FlowControl == eFlowControlHardware )
	{
		//Hardware flow control RTS/CTS
		dcbSerial.fOutxCtsFlow = TRUE;
		dcbSerial.fOutxDsrFlow = FALSE;
		dcbSerial.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcbSerial.fDtrControl = DTR_CONTROL_ENABLE;
	}
	else if( FlowControl == eFlowControlSoftRTSCTS )
	{
		dcbSerial.fOutxCtsFlow = TRUE;
		dcbSerial.fOutxDsrFlow = FALSE;
		dcbSerial.fRtsControl = RTS_CONTROL_ENABLE;
		dcbSerial.fDtrControl = DTR_CONTROL_ENABLE;
	}
	else
	{
		dcbSerial.fOutxCtsFlow = FALSE;
		dcbSerial.fOutxDsrFlow = FALSE;
		dcbSerial.fRtsControl = RTS_CONTROL_DISABLE;
		dcbSerial.fDtrControl = DTR_CONTROL_DISABLE;
	}
	dcbSerial.fDsrSensitivity = FALSE;
	dcbSerial.fOutX = FALSE;
	dcbSerial.fInX = FALSE;
	// *** end DCB configuration *** //

	//set new port configuration
	{
		GMutexLock oMutexLock(m_Mutex);
		if( !SetCommState(m_hSerial, &dcbSerial) )
			return false;
	}

	// Save current settings.
	m_BaudRate     = BaudRate;
	m_eDataBits    = DataBits;
	m_eParityType  = ParityType;
	m_eStopBits    = StopBits;
	m_eFlowControl = FlowControl;

	return true;
}

bool GSerialPortImpl::SetModemControlLineStatus(eModemControlLine Line,bool bStatus)
{
	int  iCmd(0);
	bool bRet(false);

	if( !m_hSerial )
		return false;

	// Process specified line status.
	switch (Line)
	{
		case eDTR:
			// Set or clear line based on set status.
			iCmd = (bStatus)? SETDTR : CLRDTR;
			bRet = (EscapeCommFunction(m_hSerial, iCmd))? true : false;
			break;

		case eRTS:
			// Set or clear line based on set status.
			iCmd = (bStatus)? SETRTS : CLRRTS;
			bRet = (EscapeCommFunction(m_hSerial, iCmd))? true : false;
			break;

		case eDSR:
		case eCTS:
		case eDCD:
		case eRI:
			// Can't set status of these lines.
			bRet = false;
			break;

		default:
			// Unknown line specified.
			bRet = false;
			break;
	}

	return bRet;
}

bool GSerialPortImpl::GetModemControlLineStatus(eModemControlLine Line,bool& bStatus)
{
	unsigned int uiFlag(0);


	if ( !m_hSerial )
		return false;

	// Process specified line status.
	switch (Line)
	{
		case eDSR:
			// Get current status of DSR line.
			uiFlag = MS_DSR_ON;
			break;

		case eCTS:
			// Get current status of CTS line.
			uiFlag = MS_CTS_ON;
			break;

		case eDCD:
			// Get current status of DCD line.
			uiFlag = MS_RLSD_ON;
			break;

		case eRI:
			// Get current status of RI line.
			uiFlag = MS_RING_ON;
			break;

		case eDTR:
		case eRTS:
			// Can't get status of these lines.
		default:
			// Unknown line specified.
			return false;
	}

	DWORD dwModemStat(0);
	if( !GetCommModemStatus(m_hSerial, &dwModemStat) )
		return false;

	bStatus = ((uiFlag & dwModemStat) != 0 )? true : false;
	return true;
}

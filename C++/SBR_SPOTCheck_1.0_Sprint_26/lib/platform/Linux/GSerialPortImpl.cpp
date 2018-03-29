// GSerialPortImpl.cpp
//
// Linux implementation of the GSerialPort class.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

#include "GSerialPortImpl.h"


GSerialPortImpl::GSerialPortImpl(const char*   PortName,
		                         unsigned long BaudRate,
		                         eDataBits     DataBits,
		                         eParityType   ParityType,
		                         eStopBits     StopBits,
		                         eFlowControl  FlowControl)
: m_PortName(PortName),
  m_arguments(BaudRate, ParityType, StopBits, DataBits),
  m_eFlowControl(FlowControl),
  m_iTxTimeout(eDefaultTxTimeout),
  m_iRxTimeout(eDefaultRxTimeout),
  m_iFileDescriptor(-1)
{

}

GSerialPortImpl::~GSerialPortImpl(void)
{
	Close();
}

bool GSerialPortImpl::Open(void)
{
	if( m_iFileDescriptor != -1 )
		return false;

	//open device
	{
		GMutexLock Lock(m_Mutex);

		m_iFileDescriptor = open(m_PortName.c_str(),O_RDWR);
		if (m_iFileDescriptor < 0)
		{
			return false;
		}
	}

	//set configuration
	if( !SetConfiguration() )
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

bool GSerialPortImpl::Close(void)
{
	if( m_iFileDescriptor < 0 )
		return false;

	GMutexLock Lock(m_Mutex);

	if( close(m_iFileDescriptor) != 0 )
	{
		return false;
	}

	m_iFileDescriptor = -1;

	return true;
}

void  GSerialPortImpl::FlushInput()
{
    tcflush(m_iFileDescriptor, TCIFLUSH);
}

void  GSerialPortImpl::FlushOutput()
{
    tcflush(m_iFileDescriptor, TCOFLUSH);
}

bool GSerialPortImpl::SetWriteTimeout(int iMilliseconds)
{
	m_iTxTimeout = iMilliseconds;
	return true;
}

bool GSerialPortImpl::SetReadTimeout(int iMilliseconds)
{
    struct termios TerminalAttributes;

	if( m_iFileDescriptor < 0 )
	{
		m_iRxTimeout = iMilliseconds;
		return true;
	}

    // Get the terminal attr
    if (tcgetattr(m_iFileDescriptor, &TerminalAttributes) != 0)
    {
        // TODO ERROROUT(m_pbyDeviceName <<" GSerialPort::SetReadTimeout() Failed to set device attributes. errno=" << errno << endl);
        return false;
    }

    // Set the timeout in the termios structure
    TerminalAttributes.c_cc[VMIN]  = 0;
    long time = (iMilliseconds + 99)/100;
    TerminalAttributes.c_cc[VTIME] = time;

    // Set the new attributes on the device - and set it now (TCSANOW)
    if (tcsetattr(m_iFileDescriptor, TCSANOW, &TerminalAttributes) != 0)
    {
        // TODO ERROROUT(m_pbyDeviceName <<" GSerialPort::SetReadTimeout() Failed to set device attributes. errno=" << errno << endl);
        return false;
    }

	m_iRxTimeout = iMilliseconds;

    // Success.
    return true;
}


bool GSerialPortImpl::WaitForCTS()
{
	int iTxTimeout(m_iTxTimeout);
	bool bStatus(false);

	do
	{
		// Sleep 10 ms.
		sleep(10);
		// Get CTS status.
		if( !GetModemControlLineStatus(eCTS, bStatus) )
		{
			// Attempt to clear RTS. We might expect this to fail since the call above failed.
			SetModemControlLineStatus(eRTS, false);
			return false;
		}
		iTxTimeout-=10;
	} while((bStatus != true) && (iTxTimeout > 0));

	return bStatus;
}


int GSerialPortImpl::Read(unsigned char *Buf, int BufSize)
{
	int iBytesRead(0);

	if( m_iFileDescriptor < 0 )
	{
		return eSerialPortError;
	}

	unsigned char u8Byte;
	for (int iIndex = 0; iIndex < BufSize; ++iIndex)
	{
		int iNewBytesRead = read(m_iFileDescriptor, &u8Byte, 1);
		if (iNewBytesRead == 0)
		{
			// Timed out
			break;
		}
		if (iNewBytesRead < 0)
		{
			return eSerialPortError;
		}
		// Handle parity marking
		if (u8Byte == 0xff)
		{
			// Get the next byte to determine what was marked
			read(m_iFileDescriptor, &u8Byte, 1);
			switch (u8Byte)
			{
			case 0xff:
				// Escaped 0xff:  <0xff> <0xff>.  The 0xff will be returned.
				break;
			case 0x00:
				// A parity/framing error or line break has occurred.  Get the
				// next byte to determine which.
				read(m_iFileDescriptor, &u8Byte, 1);
				if (u8Byte == 0x00)
				{
					// A line break has occurred:  <0xff> <0x00> <0x00>
					return eSerialPortBreak;
				}
				else
				{
					// A parity or framing error has occurred:  
					// <0xff> <0x00> <byte as it was received>.  The byte will be 
					// placed in the buffer, but the return status will
					// indicate a parity error
					Buf[iIndex] = u8Byte;
					return eSerialPortParityError;
				}
				break;
			default:
				// This should never occur because after reading a 0xff, the driver
				// should **ALWAYS** supply 0xff or 0x00.
				return eSerialPortError;
			}
		}
		Buf[iIndex] = u8Byte;
		++iBytesRead;
	}
	
	// read timeout
	if( !iBytesRead )
	{
		return eSerialPortTimeout;
	}

	return (iBytesRead);
}


int GSerialPortImpl::Write(const unsigned char *Buf, int BufLen)
{
	int iBytesWritten(0);

	if( m_iFileDescriptor < 0 )
	{
		return eSerialPortError;
	}

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

	iBytesWritten = write(m_iFileDescriptor, static_cast<const void*>(Buf), BufLen);

	// TODO TxTimeout

	// Check for soft RTSCTS flow control handling.
	if (m_eFlowControl == eFlowControlSoftRTSCTS)
	{
        // Clear RTS.
		SetModemControlLineStatus(eRTS, false);
	}

	// write error
	if (iBytesWritten < 0)
	{
		return eSerialPortError;
	}

	// timeout
	if( !iBytesWritten )
		return eSerialPortTimeout;

	return (iBytesWritten);
}

bool GSerialPortImpl::SetConfiguration()
{
    // Device must be open.
    if (m_iFileDescriptor < 0)
    {
        // TODO ERROROUT(m_pbyDeviceName << " GSerialPort::SetConfiguration() Failed, device not open." << endl);
        return false;
    }

    // Get the current settings.
    struct termios TerminalAttributes;
    if (tcgetattr(m_iFileDescriptor, &TerminalAttributes) != 0)
    {
        // TODO ERROROUT(m_pbyDeviceName << " GSerialPort::SetConfiguration() Failed to get device attributes. errno=" << errno << endl);
        return false;
    }
    struct termios NewSettings = m_arguments.getPortSettings(TerminalAttributes);

    switch (m_eFlowControl)
	{
		case eFlowControlNone:
			// Turn off hardware flow control.
			NewSettings.c_cflag &= ~CRTSCTS;
			break;

		case eFlowControlHardware:
			// Turn on hardware flow control.
			NewSettings.c_cflag |= CRTSCTS;
			break;

		case eFlowControlSoftRTSCTS:
			// Turn off hardware flow control.
			NewSettings.c_cflag &= ~CRTSCTS;
			break;

		default:
			// TODO ERROROUT(m_pbyDeviceName << " GSerialPort::SetFlowControl() Invalid flow control type " << (int)FlowControl << endl);
			break;
	}

	// Enable parity marking - the Read method will turn the markings to errors
	NewSettings.c_iflag |= PARMRK;

    // set input mode (non-canonical, no echo,...)
    NewSettings.c_lflag = 0;

    // Apply the new settings.
    if (tcsetattr(m_iFileDescriptor, TCSAFLUSH, &NewSettings) != 0)
    {
       // TODO ERROROUT(m_pbyDeviceName << " GSerialPort::SetConfiguration() Failed to set device attributes. errno=" << errno << endl);
        return false;
    }

    // Success.
    return true;
}

bool GSerialPortImpl::SetConfiguration(unsigned long BaudRate, eDataBits DataBits, eParityType  ParityType, eStopBits StopBits, eFlowControl  FlowControl)
{
	m_arguments = GSerialArguments(BaudRate, ParityType, StopBits, DataBits);
	m_eFlowControl = FlowControl;

	return SetConfiguration();
}


bool GSerialPortImpl::SetModemControlLineStatus(eModemControlLine Line,bool bStatus)
{
	int iCmd(0);
	int iLine(0);

    // Device must be open.
	if( m_iFileDescriptor < 0 )
	{
		return false;
	}

	// Process specified line status.
	switch (Line)
	{
		case eDTR:
			// Set or clear line based on set status.
			iCmd  = (bStatus)? TIOCMBIS : TIOCMBIC;
			iLine = TIOCM_DTR;
			break;

		case eRTS:
			// Set or clear line based on set status.
			iCmd  = (bStatus)? TIOCMBIS : TIOCMBIC;
			iLine = TIOCM_RTS;
			break;

		case eDSR:
		case eCTS:
		case eDCD:
		case eRI:
			// Can't set status of these lines.
			return false;

		default:
			// Unknown line specified.
			return false;
	}

	if (ioctl(m_iFileDescriptor, iCmd, &iLine) != 0)
    {
        // TODO ERROROUT(m_pbyDeviceName << " GSerialPort::SetModemControlLineStatus() Failed to set control line status. errno=" << errno << endl);
        return false;
    }

	return true;
}

bool GSerialPortImpl::GetModemControlLineStatus(eModemControlLine Line,bool& bStatus)
{
	int iLine(0);

    // Device must be open.
	if( m_iFileDescriptor < 0 )
	{
		return false;
	}
	// Process specified line status.
	switch (Line)
	{
		case eDSR:
			// Get current status of DSR line.
			iLine = TIOCM_DTR;
			break;

		case eCTS:
			// Get current status of CTS line.
			iLine = TIOCM_CTS;
			break;

		case eDCD:
			// Get current status of DCD line.
			iLine = TIOCM_CAR;
			break;

		case eRI:
			// Get current status of RI line.
			iLine = TIOCM_RI;
			break;

		case eDTR:
		case eRTS:
			// Can't get status of these lines.
			return false;

		default:
			// Unknown line specified.
			return false;
	}

	int mcr(0);

    // Get current MCR status.
    if (ioctl(m_iFileDescriptor, TIOCMGET, &mcr) != 0)
	{
    	// TODO ERROROUT(m_pbyDeviceName << " GSerialPort::GetModemControlLineStatus() Failed to get MCR status. errno=" << errno << endl);
		return false;
	}

	// Get status of selected control line.
	if (mcr & iLine)
		bStatus = true;
	else
		bStatus = false;

	return true;
}

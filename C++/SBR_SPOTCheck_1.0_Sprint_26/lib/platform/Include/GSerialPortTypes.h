// GSerialPortTypes.h

#ifndef GSerialPortTypes_h
#define GSerialPortTypes_h


typedef enum
{
	eDataBits5 = 5,
	eDataBits6 = 6,
	eDataBits7 = 7,
	eDataBits8 = 8
} eDataBits;

typedef enum
{
	eParityNone,
	eParityEven,
	eParityOdd
} eParityType;

typedef enum
{
	eStopBits1 = 1,
	eStopBits2 = 2
} eStopBits;

typedef enum
{
	eFlowControlNone,
	eFlowControlHardware,
	eFlowControlSoftRTSCTS
} eFlowControl;

typedef enum
{
	eDSR,
	eDTR,
	eRTS,
	eCTS,
	eDCD,
	eRI
} eModemControlLine;

typedef enum
{
	eSerialPortTimeout 			= int(0),
	eSerialPortOverRunError		= int(-1),
	eSerialPortParityError		= int(-2),
	eSerialPortFramingError		= int(-3),
	eSerialPortBreak			= int(-4),
	eSerialPortError			= int(-5)
} eSerialPortReturnCodes;

typedef enum
{
	eDefaultTxTimeout = int(500),
	eDefaultRxTimeout = int(200)
} eDefaultTimeouts;

#endif

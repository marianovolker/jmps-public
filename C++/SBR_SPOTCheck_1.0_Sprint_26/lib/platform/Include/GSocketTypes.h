//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#ifndef GSocketTypes_h
#define GSocketTypes_h

typedef enum
{
	eTCP,
	eUDP
}
eSocketType;

typedef enum
{
	eSocketTimeout	= int(0),
	eSocketClosed	= int(-1),
	eSocketError	= int(-2)
}eSocketErrorCodes;

#endif

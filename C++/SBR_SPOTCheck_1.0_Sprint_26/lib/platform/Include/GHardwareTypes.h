// GHardwareTypes.h

#ifndef GHardwareTypes_h
#define GHardwareTypes_h

namespace GHardwareTypes
{

	typedef enum
	{
		eSideUnknown,
		eSideMaster,
		eSideSlave
	} eSideId;

	typedef enum
	{
		eSwitchStateUnknown,
		eSwitchOpen,
		eSwitchClosed
	} eSwitchState;
};

#endif

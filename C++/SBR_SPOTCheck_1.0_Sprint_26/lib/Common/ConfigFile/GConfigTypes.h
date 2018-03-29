#ifndef GConfigTypes_h
#define GConfigTypes_h

#include "GTypes.h"


namespace GConfigTypes
{
	typedef enum
	{
		eConfigOk = 0,
		eConfigError,
		eConfigCanNotChangeNonDefault,
		eConfigConversionError,
		eConfigItemDoesNotExist,
		eConfigFileWriteError
	} eConfigStatusType;

	typedef enum
	{
		eNoDisplay = 1,
		eSingleLine = 2,
		eMonochrome = 3,
		eColor = 4,
		eSPOTDisplayM3 = 5,
		eSPOTDisplayM5 = 6
	} eCustomerDisplayType;

	typedef enum
	{
		eDefSideATerminalID = 1,
		eDefSideBTerminalID = 2,
		eDefMonoContrast = 35,
		eDefCrindID = 99
	} eDefaultConfigValues;

	// This used to have a SPOT M2 enum but it was removed, since the config files use the hard numeric value
	// reordering the enumeration was dangerous.
	typedef enum
	{
		eCCNX = 1,
		eAIMB_330 = 2,
		eSPOT_M3 = 4,
		eSPOT_M5 = 5
	} eHostPlatform;

	typedef enum
	{
		eLegacyMonochrome = 1,
		eHTMLGenericColor = 2
	} eDisplayMode;

	typedef enum
	{
		ePMP2W,
		ePOS2W,
		eZMODEMPORT,
		eTRINDPORT,
		eSCANNERPORT,
		eUSBLP0,
		eUSBLP1
	} eDeviceAlias;

	typedef enum
	{
		eAdvantage_E300 = 0,
		eE500_E500S = 1,
		eGenericColor = 2,
		eEMV = 3,
		eFlexPay_CRIND_UPT = 4,
	} eCRINDType;

	struct DisplayInfoStruct 
	{
		UINT32 m_u32BitDepth;
		UINT32 m_u32Width;
		UINT32 m_u32Height;
		bool m_bDispTouchEnabled;
	};

};

#endif


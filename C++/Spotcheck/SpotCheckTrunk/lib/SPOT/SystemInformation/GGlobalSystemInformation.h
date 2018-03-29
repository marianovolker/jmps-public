/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GGlobalSystemInformation_h
#define GGlobalSystemInformation_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include <GMutex.h>
#include <GCondition.h>

const int  HWR_ID_LEN              =  2;
const int  DEV_FUNCTIONS_LEN       =  4;
const int  HWINFO_CMD_LEN          =  7;
const int  SWINFO_CMD_LEN          =  7;
const int  OPT_HW_CONFIG_CMD_LEN   =  7;
const int  SERIALNO_LEN            = 48;
const int  OPT_SERIALNO_LEN        = 16;
const int  SWR_VERSION_NO_LEN      = 32;
const int  FWR_VERSION_NO_LEN      = 16;
const int  DESCRIPTION_LEN         = 24;
const int  SWINFO_BASE_ENUM        =  2;
const int  FWINFO_BASE_ENUM        = 11;
const int  OPT_STATUS_INFO_CMD_LEN =  7;
const int  PKG_INFO_LEN            = 32;


/**
 *
 * Hardware Information related structures.
 *
 */

typedef struct _SPOT_hardware_info
{
	char*       m_pcSerialNumber;
	const char* m_pcIPAddress;

} SPOT_HW_INFO;

typedef struct _security_module_info
{
	char*  m_pcSerialNumber;
	UINT8  m_aryu8HwrID[HWR_ID_LEN];

} SM_HW_INFO;

typedef struct _display_info
{
	UINT8  m_u8DevType;
	UINT8  m_aryu8DevFunctions[DEV_FUNCTIONS_LEN];
	char*  m_pcDevModel;

	struct Dev_Hwr_Config
	{
		UINT16  m_u16Xres;
		UINT16  m_u16Yres;
		UINT8   m_u8ColorDepth;

	}      m_DevHwrConfig;

} DY_HW_INFO;

typedef struct _keypad_info
{
	char*  m_pcSerialNumber;
	UINT8  m_aryu8HwrID[HWR_ID_LEN];
	UINT8  m_u8DevType;

} KB_HW_INFO;

typedef struct _card_reader_info
{
	char*  m_pcSerialNumber;
	UINT8  m_aryu8HwrID[HWR_ID_LEN];
	UINT8  m_u8DevType;
	UINT8  m_aryu8DevFunctions[DEV_FUNCTIONS_LEN];
	char*  m_pcDevModel;

} CR_HW_INFO;

typedef struct _printer_info
{
	char*  m_pcSerialNumber;
	UINT8  m_u8DeviceType;
	UINT8  m_aryu8DevFunctions[DEV_FUNCTIONS_LEN];
	char*  m_pcDevModel;

} PR_HW_INFO;

typedef struct _trind_info // Unused for now
{
	char*  m_pcSerialNumber;
	UINT8  m_aryu8HwrID[HWR_ID_LEN];
	UINT8  m_u8DeviceType;
	UINT8  m_aryu8DevFunctions[DEV_FUNCTIONS_LEN];
	char*  m_pcDevModel;

} TR_HW_INFO;


typedef struct _hardware_info
{
	SPOT_HW_INFO   m_SPOTInfo;
	SM_HW_INFO     m_SecurityModuleInfo;
	DY_HW_INFO     m_DisplayInfo;
	KB_HW_INFO     m_KeypadInfo;
	CR_HW_INFO     m_CardReaderInfo;
	TR_HW_INFO     m_TrindInfo;
	PR_HW_INFO     m_PrinterInfo;

} HARDWARE_INFO;

typedef struct _oti_info
{
	UINT8  m_u8Status;
	UINT8  m_u8StatusExt;
	UINT8  m_u8ErrorCode;

} OTI_INFO;

// SPOT model enumeration
enum eSPOTModel
{
	eUnknown, eM3 = 3, eM5 = 5, eVGD=7, eOther
};


/**
 *
 * Software Information related structures.
 *
 */

enum eSwfInfoIndex
{
	eMinSwfIndex = 0,
	eOSVersion = 0,
	eROMFs1Version, // 1
	eROMFs2Version,
	eROMFs3Version,
	eROMFs4Version,
	eROMFs5Version,
	eROMFs6Version,
	eROMFs7Version,
	eROMFs8Version, // 8
	eMaxSwfIndex = eROMFs8Version,
	eSwfInfoLength // 9
};

enum eFwrInfoIndex
{
	eMinFwrIndex = 0,
	eCRFwrVersion = 0,
	eKPFwrVersion,  // 1
	eSecFwrVersion, // 2
	eMaxFwrIndex = eSecFwrVersion, // 2
	eFwrInfoLength, // 3
	eFwrInfoMaxLength = 8, //
	ePkgInfoLength = 12     //  for VGD platform
};

// for VGD platform
typedef struct _vgdPkg_info
{
	char*          m_pkgs;
} VGD_PKG_INFO;

typedef struct _software_info
{
	char*          m_pcSoftwareVersions;
	char*          m_pcFirmwareVersions;
	char*          m_vgdFirmwareVersion;       // for Vanguard platform.
	char           m_vgdCRFwrVersion[FWR_VERSION_NO_LEN * eFwrInfoMaxLength];          // for Vanguard platform.
	char           m_vgdUPMAppVersion[FWR_VERSION_NO_LEN * eFwrInfoMaxLength ];         // for Vanguard platform
	char           m_vgdUPMBuildVersion[FWR_VERSION_NO_LEN * eFwrInfoMaxLength];        // for Vanguard platform
	char*          m_vgdOSVersion;

} SOFTWARE_INFO;


class GGlobalSystemInformation
{
public:
	inline static GGlobalSystemInformation* GetInstance();
	inline HARDWARE_INFO& GetAllHardwareInfo();
	inline SPOT_HW_INFO&  GetSPOTInfo();
	inline SM_HW_INFO&    GetSecModuleInfo();
	inline DY_HW_INFO&    GetDisplayInfo();
	inline KB_HW_INFO&    GetKeypadInfo();
	inline CR_HW_INFO&    GetCardReaderInfo();
	inline TR_HW_INFO&    GetTrindInfo();
	inline PR_HW_INFO&    GetPrinterInfo();
	inline const char*    GetIPAddress();
	inline void           SetIPAddress(const char*);
	inline SOFTWARE_INFO& GetAllSoftwareInfo();
		   const char*    GetSoftwareInfo(int index);
		   const char*    GetPkgInfo(int index);         // for VGD platform
		   const char*    GetFirmwareInfo(int index);
	inline OTI_INFO&      GetOTIInfo();
		   const char*    GetSoftPkgInfo();


	//Asynchronous interface against SPOT
	bool SendHwrInfoCommandToSPOT();
	bool SendSwrInfoCommandToSPOT();
	bool SendOPTHwrConfigCommandToSPOT();
	bool SendOPTSystemInfoCommandToSPOT();
	bool SendKeypadFWRCommandToSPOT();
	bool SendPkgInfoCommandToSPOT();
	bool SendCRFwrCommandToSPOT();
	bool SendUPMAppVerCommandToSPOT();
	bool SendUPMBuildVerCommandToSPOT();

	//Synchronous interface against SPOT calling thread must be the same
	bool SendSyncHwrInfoCommandToSPOT();
	bool SendSyncSwrInfoCommandToSPOT();
	bool SendSyncOPTHwrConfigCommandToSPOT();
	bool SendSyncOPTSystemInfoCommandToSPOT();
	bool SendSyncKeypadFWRCommandToSPOT();
	bool SendSyncPkgInfoCommandToSPOT();

	bool SendSyncUPMAppVerCommandToSPOT();
	bool SendSyncUPMBuildVerCommandToSPOT();
	bool SendSyncCRFwrCommandToSPOT();


	int  ParseHrwInfoReply(GSPOTMessage* pMsg);
	int  ParseHardwareConfigReply(GSPOTMessage* pMsg);
	int  ParseSwfInfoReply(GSPOTMessage* pMsg);
	int	 ParseUPMMessageReply(GSPOTMessage* pMsg);
	int  ParsePkgInfoReply(GSPOTMessage* pMsg);
	int  ParseOPTStatusInfoRequestReply(GSPOTMessage* pMsg);


	static bool isPinpadSysInfoMessageGroupReply(GSPOTMessage* pMsg);
	static bool isHardwareInfoMessageReply(GSPOTMessage* pMsg);
	static bool isSoftwareInfoMessageReply(GSPOTMessage* pMsg);
	static bool isOPTHardwareConfigMessageReply(GSPOTMessage* pMsg);
	static bool isOPTStatusInfoRequestMessageReply(GSPOTMessage* pMsg);
	static bool isUPMMessageReply(GSPOTMessage* pMsg);

	static bool isPkgInfoMessageReply(GSPOTMessage* pMsg);

	inline static const char* GetIPAddressForEmbeddedSPC();

	inline bool isM3Unit();
	inline bool isM5Unit();
	inline bool isVGDUnit();
//	inline bool isEmbeddedSPC();
//	inline bool isSPCEmbeddedIntoM3();

	static void WriteCharField(UINT8* pSrc, char* pcField, int iSrcLen, int iFieldSize);
	static void WriteByteField(UINT8* pSrc, UINT8* pField, int iSrcLen, int iFieldSize);

public:
	static GGlobalSystemInformation* CreateInstance(GSPOTSession* pSession);
	static void DestroyInstance();

private:
	GGlobalSystemInformation(GSPOTSession* pSession);
	~GGlobalSystemInformation();

	void UpdateSPOTSerialNumber(int iItemLen, UINT8* pb);
	int  UpdateSecureModuleInfo(int iItemID, int iItemLen, UINT8* pb);
	int  UpdateDisplayInfo(int iItemID, int iItemLen, UINT8* pb);
	int  UpdateKeypadInfo(int iItemID, int iItemLen, UINT8* pb);
	int  UpdateCardReaderInfo(int iItemID, int iItemLen, UINT8* pb);
	int  UpdatePrinterInfo(int iItemID, int iItemLen, UINT8* pb);
	int  UpdateTrindInfo(int iItemID, int iItemLen, UINT8* pData);
	void UpdateSoftwareInfoItem(int iItemID, int iItemLen, UINT8* pb);
	void UpdateFirmwareInfoItem(int iItemID, int iItemLen, UINT8* pb);
	int  UpdateOPTStatusInfo(int iItemID, int iItemLen, UINT8* pData);

	int ReceiveHrwInfoReply(GSPOTMessage* pMsg);
	int ReceiveHardwareConfigReply(GSPOTMessage* pMsg);
	int ReceiveSwfInfoReply(GSPOTMessage* pMsg);
	int ReceiveOPTStatusInfoRequestReply(GSPOTMessage* pMsg);
	int ReceiveUPMMessageReply(GSPOTMessage* pMsg);
	int ReceivePkgInfoReply(GSPOTMessage* pMsg);
	void ReceivedCommandFromSPOT();
	void ParsePkgInfoFields(UINT8*, int);

	void DestroyInternalReferences();

	static GGlobalSystemInformation* mst_pSoleInstance;
	static GMutex mst_CreationMutex;
	static UINT8  mst_aryu8AllHardwareInfoCommand[HWINFO_CMD_LEN];
	static UINT8  mst_aryu8AllSoftwareInfoCommand[SWINFO_CMD_LEN];
	static UINT8  mst_aryu8PrinterOPTHardwareConfigurationCommand[OPT_HW_CONFIG_CMD_LEN];
	static UINT8  mst_aryu8OTIStatusInfoCommand[OPT_STATUS_INFO_CMD_LEN];

	static UINT8  mst_aryu8EnableClessCommand[8];
	static UINT8  mst_aryu8DisableClessCommand[6];
	static UINT8  mst_aryu8OTIpassthru[8];
	static UINT8  mst_aryu8KeypadFWRCommand[8];
	static UINT8  mst_aryu8PkgInfoCommand[7];
	static UINT8  mst_aryu8UPMAppVerCommand[8];
	static UINT8  mst_aryu8UPMBuildVerCommand[8];
	static UINT8  mst_aryu8CRFwrCommand[8];


	static const char* mst_CpcEmbeddedIPAddress; // = LOOPBACK_SPOT_IP

	GSPOTSession* m_pSession;
	eSPOTModel    m_eSPOTModel;
	HARDWARE_INFO m_HardwareInfo;
	SOFTWARE_INFO m_SoftwareInfo;
	VGD_PKG_INFO  m_VGDPkgInfo;
	OTI_INFO      m_OTI_Info;
	GMutex		  m_Mutex;
	GCondition    m_Condition;
};

inline GGlobalSystemInformation* GGlobalSystemInformation::GetInstance()
{
	return mst_pSoleInstance;
}

inline HARDWARE_INFO& GGlobalSystemInformation::GetAllHardwareInfo()
{
	return m_HardwareInfo;
}

inline SPOT_HW_INFO&  GGlobalSystemInformation::GetSPOTInfo()
{
	return m_HardwareInfo.m_SPOTInfo;
}

inline SM_HW_INFO&    GGlobalSystemInformation::GetSecModuleInfo()
{
	return m_HardwareInfo.m_SecurityModuleInfo;
}

inline DY_HW_INFO&    GGlobalSystemInformation::GetDisplayInfo()
{
	return m_HardwareInfo.m_DisplayInfo;
}

inline KB_HW_INFO&    GGlobalSystemInformation::GetKeypadInfo()
{
	return m_HardwareInfo.m_KeypadInfo;
}

inline CR_HW_INFO&    GGlobalSystemInformation::GetCardReaderInfo()
{
	return m_HardwareInfo.m_CardReaderInfo;
}
inline TR_HW_INFO&    GGlobalSystemInformation::GetTrindInfo()
{
	return m_HardwareInfo.m_TrindInfo;
}

inline PR_HW_INFO&    GGlobalSystemInformation::GetPrinterInfo()
{
	return m_HardwareInfo.m_PrinterInfo;
}

inline const char*    GGlobalSystemInformation::GetIPAddressForEmbeddedSPC()
{
	return mst_CpcEmbeddedIPAddress;
}

inline const char*    GGlobalSystemInformation::GetIPAddress()
{
	return m_HardwareInfo.m_SPOTInfo.m_pcIPAddress;
}

inline void           GGlobalSystemInformation::SetIPAddress(const char* pcIP)
{
	m_HardwareInfo.m_SPOTInfo.m_pcIPAddress = pcIP;

/** This dynamic resolution of embedded/external app would be prefered in next releases.

	if( strncmp(pcIP, mst_CpcEmbeddedIPAddress, 9) == 0 )
		m_blEmbeddedSPC = true;
	else
		m_blEmbeddedSPC = false;
 */
}

inline SOFTWARE_INFO& GGlobalSystemInformation::GetAllSoftwareInfo()
{
	return m_SoftwareInfo;
}

inline OTI_INFO& GGlobalSystemInformation::GetOTIInfo()
{
	return m_OTI_Info;
}

inline bool GGlobalSystemInformation::isM3Unit()
{
	return ( m_eSPOTModel == eM3 );
}

inline bool GGlobalSystemInformation::isM5Unit()
{
	return ( m_eSPOTModel == eM5 );
}

inline bool GGlobalSystemInformation::isVGDUnit()
{
	return ( m_eSPOTModel == eVGD );
}

#endif // GGlobalSystemInformation_h

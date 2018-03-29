/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeyPadService.h
 * @author Mariano volker
 * @date Mar 12 2013
 * @note Jul 18 2013 Changed by Guillermo Paris to support graphical test (data entry)
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GKeypadService_h
#define GKeypadService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/Keypad/GAbsKeyboardManager.h"
#include "Menus/GMenuManager.h"

const int    KB_DATAENTRY_TAG_ID   = 99;
const int    KB_ENABLE_WIN_ID_IDX  = 14;
const int    KB_ENABLE_TAG_ID_IDX  = 15;
const int    KB_ENABLE_AU_ENTER_IDX=  8;
const int    KB_ENABLE_CMD_LEN     = 20;
const int    KB_DISABLE_CMD_LEN    =  6;

class GKeypadManager;

enum eSATestStatus
{
	eSATStatusInitial,
	eSATStatusYes,
	eSATStatusNo,
};


class GKeypadService
{
	public:

		enum eStatus
		{
			eDisabled,
			eEnabled,
			eBusy,
			eKPError
		};

		enum eKeyValue
		{
			eKey00			= 0x00,
			eKey01			= 0x01,
			eKey02			= 0x02,
			eKey03			= 0x03,
			eKey04			= 0x04,
			eKey05			= 0x05,
			eKey06			= 0x06,
			eKey07			= 0x07,
			eKey08			= 0x08,
			eKey09			= 0x09,
			eKeyHelp		= 0x0A,
			eKeyUp			= 0x0B,
			eKeyCancel		= 0x0C,
			eKeyEnter		= 0x0D,
			eKeyClear		= 0x0E,
			eKeyNo			= 0x0F,
			eKeyL1			= 0x10,
			eKeyL2			= 0x11,
			eKeyL3			= 0x12,
			eKeyL4			= 0x13,
			eKeyR1			= 0x14,
			eKeyR2			= 0x15,
			eKeyR3			= 0x16,
			eKeyR4			= 0x17,
			eKeyP1			= 0x18,
			eKeyP2			= 0x19,
			eKeyP3			= 0x1A,
			eKeyP4			= 0x1B,
			eKeyP5			= 0x1C,
			eKeyP6			= 0x1D,
			eKeyP7			= 0x1E,
			eKeyP8			= 0x1F,
			eKeyP9			= 0x20,
			eKeyP10			= 0x21,
			eKeyYes			= 0x22,
			eKeyAL1			= 0x23,			//AL1 -AR4 are for ADA Keypad
			eKeyAL2			= 0x24,
			eKeyAL3			= 0x25,
			eKeyAL4			= 0x26,
			eKeyAR1			= 0x27,
			eKeyAR2			= 0x28,
			eKeyAR3			= 0x29,
			eKeyAR4			= 0x2A,

			eKeyMinValue	= (eKey00),
			eMaxKeys		= (eKeyAR4 + 1)
		};

		inline static GKeypadService* GetService();
		inline static GKeypadService* CreateInstance(GSPOTSession* pSession);
		inline static void DestroyInstance();

		inline static bool isSoftKeyCode(UINT8 u8KeyCode);
		inline static bool isAuxKeyCode(UINT8 u8KeyCode);
		inline static bool isKeypadKeyCode(UINT8 u8KeyCode);
		inline static bool isADASoftKeyCode(UINT8 u8KeyCode);
		static bool        isKeypadReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);
		static const char* GetKeyLabel(UINT8 u8Key);

		inline bool        KeyTranslate(UINT8 u8KeyPressed, UINT8 & u8NormalizeKey);
		eStatus            GetStatus() {return m_eStatus;}
		//eStatus            GetSAResults_1() {return m_eSAResult;}

		eSATestStatus      GetNonSecStatusResults() {return m_NonSecureStatus;}
		eSATestStatus      GetSecStatusResults() {return m_SecureStatus;}
		void               SetNonSecStatusResults(eSATestStatus value);
		void               SetSecStatusResults(eSATestStatus value);

		const char*        GetSAResults();
		void               SetSAResults(eSAResult value);

		void ClearKeysStatistics();
		UINT8 GetKeyStatistics(UINT8 key);
		void NotifySoftKey(UINT8 KeyCode);
		void AttachManager(GAbsKeyboardManager* pMgr);
		void DetachManager();
		bool EnableRequest();
		//bool NonSecureEnableRequest();
		bool EnableRequestForSecAud();
		bool DisableRequest();
		bool ProcessKeypadReply(GSPOTMessage* pMsg);
		int  ParseDiagnosticInfoReply(GSPOTMessage* pMsg);
		bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);


	private:

		GKeypadService(GSPOTSession* pSession);
		virtual ~GKeypadService();

		bool SendEnableCmdToSPOT();
		//bool SendNonSecureEnableCmdToSPOT();
		bool SendEnableCmdForSecAudToSPOT();
		bool SendDisableCmdToSPOT();
		bool ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize);
		bool KeyMapping(UINT8 u8KeyPressed, eKeyValue& eNormalizeKey);


		static GKeypadService* mst_pSoleInstance;
		static GMutex          mst_createInstanceMutex;
		static const UINT8     mst_aryu8EnableKBCmd[KB_ENABLE_CMD_LEN];
		static const UINT8     mst_aryu8EnableKBCmdVGD[KB_ENABLE_CMD_LEN];
		static const UINT8     mst_aryu8DisableKBCmd[KB_DISABLE_CMD_LEN];
		static const char*     mst_arypcKeysLabel[eKeyYes + 1];

		enum eStatus           m_eStatus;
		enum eSAResult         m_eSAResult;

		enum eSATestStatus     m_NonSecureStatus;
		enum eSATestStatus     m_SecureStatus;


		UINT8                  mst_aryu8KeysStatistics[eMaxKeys];
		static const char*     m_aryu8SAResultStrings[eSALast + 1];
		GMutex                 m_instanceMutex;
		GSPOTSession*          m_pSession;
		GAbsKeyboardManager*   m_pManager;
};

inline GKeypadService* GKeypadService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GKeypadService* GKeypadService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GKeypadService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GKeypadService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

inline bool GKeypadService::isSoftKeyCode(UINT8 u8KeyCode)
{
	return (u8KeyCode >= eKeyL1 && u8KeyCode <= eKeyR4);
}

inline bool GKeypadService::isAuxKeyCode(UINT8 u8KeyCode)
{
	return (u8KeyCode >= eKeyP1 && u8KeyCode <= eKeyP10);
}

inline bool GKeypadService::isADASoftKeyCode(UINT8 u8KeyCode)
{
	return  (u8KeyCode >= eKeyAL1 && u8KeyCode <= eKeyAR4);
}

inline bool GKeypadService::isKeypadKeyCode(UINT8 u8KeyCode)
{
	return (u8KeyCode == eKeyYes || u8KeyCode <= eKeyNo);
}

inline bool GKeypadService::KeyTranslate(UINT8 u8KeyPressed, UINT8 & u8NormalizeKey)
{
	bool      blret;
	eKeyValue eValue = eKey00;

	blret = KeyMapping(u8KeyPressed, eValue);
	u8NormalizeKey = UINT8(eValue);
	return blret;
}

#endif

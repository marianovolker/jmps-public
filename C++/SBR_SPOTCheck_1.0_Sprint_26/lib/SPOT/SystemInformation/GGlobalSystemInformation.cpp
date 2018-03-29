#include <string.h>
#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

using namespace GSpotDefs;


GMutex                    GGlobalSystemInformation::mst_CreationMutex;
GGlobalSystemInformation* GGlobalSystemInformation::mst_pSoleInstance = NULL;

UINT8  GGlobalSystemInformation::mst_aryu8AllHardwareInfoCommand[HWINFO_CMD_LEN] =
			{0x00, 0x07, 0x01, 0x00, 0x10, 0x02, 0x00};

UINT8  GGlobalSystemInformation::mst_aryu8AllSoftwareInfoCommand[HWINFO_CMD_LEN] =
			{0x00, 0x07, 0x01, 0x00, 0x10, 0x03, 0x00};

UINT8  GGlobalSystemInformation::mst_aryu8PrinterOPTHardwareConfigurationCommand[OPT_HW_CONFIG_CMD_LEN] =
			{0x00, 0x07, 0x04, 0x00, 0x10, 0x02, 0x00 };

UINT8  GGlobalSystemInformation::mst_aryu8OTIStatusInfoCommand[OPT_STATUS_INFO_CMD_LEN] =
			{0x00, 0x07, 0x04, 0x00, 0x10, 0x01, 0xff };

UINT8  GGlobalSystemInformation::mst_aryu8EnableClessCommand[8] =
			{0x00, 0x08, 0x04, 0x00, 0x50, 0x01 ,0x00 ,0x00 };

UINT8  GGlobalSystemInformation::mst_aryu8DisableClessCommand[6] =
			{0x00, 0x06, 0x04, 0x00, 0x50, 0x05 };

const char* GGlobalSystemInformation::mst_CpcEmbeddedIPAddress = LOOPBACK_SPOT_IP;

GGlobalSystemInformation* GGlobalSystemInformation::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock m(mst_CreationMutex);

	if( mst_pSoleInstance == NULL )
	{
		mst_pSoleInstance = new GGlobalSystemInformation(pSession);
	}

	return mst_pSoleInstance;
}

void GGlobalSystemInformation::DestroyInstance()
{
	GMutexLock m(mst_CreationMutex);
	if( mst_pSoleInstance )
	{
		mst_pSoleInstance->DestroyInternalReferences();
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

GGlobalSystemInformation::GGlobalSystemInformation(GSPOTSession* pSession) :
	m_blEmbeddedSPC(IS_EMBEDDED_APP),
	m_pSession(pSession),
	m_eSPOTModel(eUnknown),
	m_Condition(&m_Mutex)
{
	memset(&m_HardwareInfo, 0, sizeof(m_HardwareInfo));
	memset(&m_SoftwareInfo, 0, sizeof(m_SoftwareInfo));
	memset(&m_OTI_Info, 0, sizeof(m_OTI_Info));
	m_SoftwareInfo.m_pcFirmwareVersions = new char[eFwrInfoLength * FWR_VERSION_NO_LEN];
	m_SoftwareInfo.m_pcSoftwareVersions = new char[eSwfInfoLength * SWR_VERSION_NO_LEN];
}

GGlobalSystemInformation::~GGlobalSystemInformation()
{
}

void GGlobalSystemInformation::DestroyInternalReferences()
{
	if(m_HardwareInfo.m_CardReaderInfo.m_pcDevModel)
	{
		delete[] m_HardwareInfo.m_CardReaderInfo.m_pcDevModel;
		m_HardwareInfo.m_CardReaderInfo.m_pcDevModel = NULL;
	}

	if(m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber)
	{
		delete[] m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber;
		m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber = NULL;
	}

	if(m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber)
	{
		delete[] m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber;
		m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber = NULL;
	}

	if(m_HardwareInfo.m_DisplayInfo.m_pcDevModel)
	{
		delete[] m_HardwareInfo.m_DisplayInfo.m_pcDevModel;
		m_HardwareInfo.m_DisplayInfo.m_pcDevModel = NULL;
	}

	if(m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber)
	{
		delete[] m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber;
		m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber = NULL;
	}

	if(m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber)
	{
		delete[] m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber;
		m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber = NULL;
	}

	m_HardwareInfo.m_SPOTInfo.m_pcIPAddress = NULL;

	if(m_SoftwareInfo.m_pcSoftwareVersions)
	{
		delete m_SoftwareInfo.m_pcSoftwareVersions;
		m_SoftwareInfo.m_pcSoftwareVersions = NULL;
	}

	if(m_SoftwareInfo.m_pcFirmwareVersions)
	{
		delete m_SoftwareInfo.m_pcFirmwareVersions;
		m_SoftwareInfo.m_pcFirmwareVersions = NULL;
	}
}

bool GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(GSPOTMessage* pMsg)
{
	if(!pMsg)
		return false; // no message is supplied

	if( pMsg->GetAppId() != eSPOT_PP_ANSWER )
		return false; // unexpected application ID

	if( pMsg->GetCommandCode() != eSPOT_PP_SYSTEM )
		return false; // unexpected command category

	UINT8 u8SubCmd = pMsg->GetSubCommandCode();

	if( u8SubCmd == eSPOT_PP_SYS_HWR_INFO || u8SubCmd == eSPOT_PP_SYS_SWR_INFO )
		return true; // unexpected command code

	return false;
}

bool GGlobalSystemInformation::isHardwareInfoMessageReply(GSPOTMessage* pMsg)
{
	if( pMsg->GetSubCommandCode() == eSPOT_PP_SYS_HWR_INFO )
		return true; // expected command code

	return false;
}

bool GGlobalSystemInformation::isSoftwareInfoMessageReply(GSPOTMessage* pMsg)
{
	if( pMsg->GetSubCommandCode() == eSPOT_PP_SYS_SWR_INFO )
		return true; // expected command code

	return false;
}

bool GGlobalSystemInformation::isOPTHardwareConfigMessageReply(GSPOTMessage* pMsg)
{
	if(!pMsg)
		return false; // no message is supplied

	if( pMsg->GetAppId() != eSPOT_OPT_ANSWER )
		return false; // unexpected application ID

	if( pMsg->GetCommandCode() != eSPOT_OPT_SYSTEM )
		return false; // unexpected command category

	if( pMsg->GetSubCommandCode() != eSPOT_OPT_SYS_HWR_INFO )
		return false; // unexpected command code

	return true;
}

bool GGlobalSystemInformation::isOPTStatusInfoRequestMessageReply(GSPOTMessage* pMsg)
{
	if(!pMsg)
		return false; // no message is supplied

	if( pMsg->GetAppId() != eSPOT_OPT_ANSWER )
		return false; // unexpected application ID

	if( pMsg->GetCommandCode() != eSPOT_OPT_SYSTEM )
		return false; // unexpected command category

	if( pMsg->GetSubCommandCode() != eSPOT_OPT_SYS_STATUS_INFO )
		return false; // unexpected command code

	return true;
}

bool GGlobalSystemInformation::SendHwrInfoCommandToSPOT()
{
	GSPOTMessage oMsg(mst_aryu8AllHardwareInfoCommand);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GGlobalSystemInformation::SendSwrInfoCommandToSPOT()
{
	GSPOTMessage oMsg(mst_aryu8AllSoftwareInfoCommand);
	return m_pSession->SendSPOTCommand( &oMsg );
}

/**
 * Sends command to SPOT to query printer hardware information
 */
bool GGlobalSystemInformation::SendOPTHwrConfigCommandToSPOT()
{
	GSPOTMessage oMsg(mst_aryu8PrinterOPTHardwareConfigurationCommand);
	return m_pSession->SendSPOTCommand( &oMsg );
}

/**
 * Sends command to SPOT to query system information, like OTI info c-less reader
 */
bool GGlobalSystemInformation::SendOPTSystemInfoCommandToSPOT()
{
	GSPOTMessage oMsg1(mst_aryu8EnableClessCommand);
	m_pSession->SendSPOTCommand( &oMsg1 );

	GSPOTMessage oMsg2(mst_aryu8OTIStatusInfoCommand);
	m_pSession->SendSPOTCommand( &oMsg2 );

	GSPOTMessage oMsg3(mst_aryu8DisableClessCommand);
	return m_pSession->SendSPOTCommand( &oMsg3 );
}

bool GGlobalSystemInformation::SendSyncHwrInfoCommandToSPOT()
{
	GMutexLock oLock(m_Mutex);
	if( !SendHwrInfoCommandToSPOT() )
		return false;
	//waits up to 5s the response
	return (m_Condition.Wait(5000) != 0);
}

bool GGlobalSystemInformation::SendSyncSwrInfoCommandToSPOT()
{
	GMutexLock oLock(m_Mutex);
	if( !SendSwrInfoCommandToSPOT() )
		return false;
	return (m_Condition.Wait(5000) != 0);
}

bool GGlobalSystemInformation::SendSyncOPTHwrConfigCommandToSPOT()
{
	GMutexLock oLock(m_Mutex);
	if( !SendOPTHwrConfigCommandToSPOT() )
		return false;
	return (m_Condition.Wait(5000) != 0);
}

bool GGlobalSystemInformation::SendSyncOPTSystemInfoCommandToSPOT()
{
	GMutexLock oLock(m_Mutex);
	if( !SendOPTSystemInfoCommandToSPOT() )
		return false;
	// Only wait for mst_aryu8OTIStatusInfoCommand
	return (m_Condition.Wait(5000) != 0);
}

void GGlobalSystemInformation::ReceivedCommandFromSPOT()
{
	GMutexLock oLock(m_Mutex);
	m_Condition.Signal(); // wake up thread
}

int GGlobalSystemInformation::ParseHrwInfoReply(GSPOTMessage* pMsg)
{
	int iret = ReceiveHrwInfoReply(pMsg);
	ReceivedCommandFromSPOT();
	return iret;
}

int GGlobalSystemInformation::ReceiveHrwInfoReply(GSPOTMessage* pMsg)
{

	LOG(eSpotProtocol, eDebug, "ParseHrwInfoReply() - Data\n");


	int len = pMsg->GetDataLen();
	if(len < 8)
		return 0; // bad reply message

	UINT8 *pu8Data = pMsg->GetData();

	if(pu8Data[0] != 0x00) // ACK code not OK
	{
		return 0;
	}

	int nItems = pu8Data[1];
	if(!nItems) // null item qty
	{
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 0, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2; // byte pointer to scan items
	char*  pc = NULL;

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID =  *pb++;
		iItemLen = *pb++; // after this pb points to the item's data

		if(iItemID == eSPOT_SERIALNUMBER)
		{
			UpdateSPOTSerialNumber(iItemLen, pb);
			iProcessedItems++;
		}

		else if(iItemID >= eSPOT_SM_SERIALNUMBER && iItemID <= eSPOT_SM_HWRID)
			iProcessedItems += UpdateSecureModuleInfo(iItemID, iItemLen, pb);

		else if(iItemID >= eSPOT_DY_DEVTYPE && iItemID <= eSPOT_DY_DEVHWRCONFIG)
			iProcessedItems += UpdateDisplayInfo(iItemID, iItemLen, pb);

		else if(iItemID >= eSPOT_KB_SERIALNUMBER && iItemID <= eSPOT_KB_DEVTYPE)
			iProcessedItems += UpdateKeypadInfo(iItemID, iItemLen, pb);

		else if(iItemID >= eSPOT_CR_SERIALNUMBER && iItemID <= eSPOT_CR_DEVMODEL)
			iProcessedItems += UpdateCardReaderInfo(iItemID, iItemLen, pb);

		pb += iItemLen; // now points to the following item's TLV structure
		iItemNo++;
	}

	return iProcessedItems;
}

int GGlobalSystemInformation::ParseSwfInfoReply(GSPOTMessage* pMsg)
{
	int iret = ReceiveSwfInfoReply(pMsg);
	ReceivedCommandFromSPOT();
	return iret;
}

int GGlobalSystemInformation::ReceiveSwfInfoReply(GSPOTMessage* pMsg)
{
	LOG(eSpotProtocol, eDebug, "ParseSwfInfoReply() - Data:\n");
	GConversion::LogBytesAsASCIIHexString(pMsg->GetData(), pMsg->GetDataLen());
	int len = pMsg->GetDataLen();
	if(len < 8)
		return 0; // bad reply message

	UINT8 *pu8Data = pMsg->GetData();

	if(pu8Data[0] != 0x00) // ACK code not OK
	{
		return 0;
	}

	int nItems = pu8Data[1];
	if(!nItems) // null item qty
	{
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 0, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2; // byte pointer to scan items

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID =  *pb++;
		iItemLen = *pb++; // after this pb points to the item's data

		if(iItemID < SWINFO_BASE_ENUM || iItemID >= FWINFO_BASE_ENUM + eFwrInfoLength)
		{
			pb += iItemLen; // now points to the following item's TLV structure
			iItemNo++;
			continue; // unrecognized ID
		}

		if(iItemID >= FWINFO_BASE_ENUM)
		{
			UpdateFirmwareInfoItem(iItemID, iItemLen, pb);
			iProcessedItems++;
		}
		else
		{
			UpdateSoftwareInfoItem(iItemID, iItemLen, pb);
			iProcessedItems++;
		}

		pb += iItemLen; // now points to the following item's TLV structure
		iItemNo++;
	}

	return iProcessedItems;
}

void GGlobalSystemInformation::WriteCharField(UINT8* pSrc, char* pcField,
                                              int iSrcLen, int iFieldSize)
{
	WriteByteField(pSrc, reinterpret_cast<UINT8*>(pcField), iSrcLen, iFieldSize);
	if(iSrcLen >= iFieldSize)
	{
		pcField[iFieldSize - 1] = '\0'; // null terminated string is guaranteed even though
	}                                 // it implies truncating data (should not happen)
}

void GGlobalSystemInformation::WriteByteField(UINT8* pSrc, UINT8* pField,
                                              int iSrcLen, int iFieldSize)
{
	int iSafeLen = ( iSrcLen > iFieldSize ? iFieldSize : iSrcLen);
	memcpy(pField, pSrc, iSafeLen);

	if(iSafeLen < iFieldSize) // zero padding on remainder of the field
		memset(pField + iSafeLen, 0, iFieldSize - iSafeLen);
}

void GGlobalSystemInformation::UpdateSPOTSerialNumber(int iItemLen, UINT8* pData)
{
	if(m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber == NULL)
		m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber = new char[SERIALNO_LEN];

	WriteCharField(pData, m_HardwareInfo.m_SPOTInfo.m_pcSerialNumber, iItemLen, SERIALNO_LEN);
}

int GGlobalSystemInformation::UpdateSecureModuleInfo(int iItemID, int iItemLen, UINT8* pData)
{
	if( iItemID == int(eSPOT_SM_SERIALNUMBER) )
	{
		if(m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber == NULL)
			m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber = new char[SERIALNO_LEN];

		WriteCharField(pData, m_HardwareInfo.m_SecurityModuleInfo.m_pcSerialNumber, iItemLen, SERIALNO_LEN);
		return 1; // one item processed
	}

	if( iItemID == int(eSPOT_SM_HWRID) )
	{
		m_HardwareInfo.m_SecurityModuleInfo.m_aryu8HwrID[0] = *pData;
		m_HardwareInfo.m_SecurityModuleInfo.m_aryu8HwrID[HWR_ID_LEN - 1] = *(pData + 1);
		return 1; // one item processed
	}

	return 0; // unknown item
}

int GGlobalSystemInformation::UpdateDisplayInfo(int iItemID, int iItemLen, UINT8* pData)
{
	switch(iItemID)
	{
		case eSPOT_DY_DEVTYPE:
			m_HardwareInfo.m_DisplayInfo.m_u8DevType = *pData;
			break;

		case eSPOT_DY_DEVFUNCTIONS:
			WriteByteField(pData, m_HardwareInfo.m_DisplayInfo.m_aryu8DevFunctions, iItemLen, DEV_FUNCTIONS_LEN);
			break;

		case eSPOT_DY_DEVMODEL:
			if(m_HardwareInfo.m_DisplayInfo.m_pcDevModel == NULL)
				m_HardwareInfo.m_DisplayInfo.m_pcDevModel = new char[DESCRIPTION_LEN];

			WriteCharField(pData, m_HardwareInfo.m_DisplayInfo.m_pcDevModel, iItemLen, DESCRIPTION_LEN);
			if( !strncmp(m_HardwareInfo.m_DisplayInfo.m_pcDevModel, "MCF5", 4) )
				m_eSPOTModel = eM3;
			else if( !strncmp(m_HardwareInfo.m_DisplayInfo.m_pcDevModel, "OMAP", 4) )
				m_eSPOTModel = eM5;
			break;

		case eSPOT_DY_DEVHWRCONFIG:
			m_HardwareInfo.m_DisplayInfo.m_DevHwrConfig.m_u16Xres = 256 * pData[2] + pData[3];
			m_HardwareInfo.m_DisplayInfo.m_DevHwrConfig.m_u16Yres = 256 * pData[0] + pData[1];
			m_HardwareInfo.m_DisplayInfo.m_DevHwrConfig.m_u8ColorDepth =  pData[4];
			break;

		default:
			return 0; // unknown item
	}

	return 1; // one item processed
}

int GGlobalSystemInformation::UpdateKeypadInfo(int iItemID, int iItemLen, UINT8* pData)
{
	switch(iItemID)
	{
		case eSPOT_KB_SERIALNUMBER:
			if(m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber == NULL)
				m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber = new char[SERIALNO_LEN];

			WriteCharField(pData, m_HardwareInfo.m_KeypadInfo.m_pcSerialNumber, iItemLen, SERIALNO_LEN);
			break;

		case eSPOT_KB_HWRID:
			m_HardwareInfo.m_KeypadInfo.m_aryu8HwrID[0] = *pData;
			m_HardwareInfo.m_KeypadInfo.m_aryu8HwrID[HWR_ID_LEN - 1] = *(pData + 1);
			break;

		case eSPOT_KB_DEVTYPE:
			m_HardwareInfo.m_KeypadInfo.m_u8DevType = *pData;
			break;

		default:
			return 0; // unknown item
	}

	return 1; // one item processed
}

int GGlobalSystemInformation::UpdateCardReaderInfo(int iItemID, int iItemLen, UINT8* pData)
{
	switch(iItemID)
	{
		case eSPOT_CR_SERIALNUMBER:
			if(m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber == NULL)
				m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber = new char[SERIALNO_LEN];

			WriteCharField(pData, m_HardwareInfo.m_CardReaderInfo.m_pcSerialNumber, iItemLen, SERIALNO_LEN);
			break;

		case eSPOT_CR_HWRID:
			m_HardwareInfo.m_CardReaderInfo.m_aryu8HwrID[0] = *pData;
			m_HardwareInfo.m_CardReaderInfo.m_aryu8HwrID[HWR_ID_LEN - 1] = *(pData + 1);
			break;

		case eSPOT_CR_DEVTYPE:
			m_HardwareInfo.m_CardReaderInfo.m_u8DevType = *pData;
			break;

		case eSPOT_CR_DEVFUNCTIONS:
			WriteByteField(pData, m_HardwareInfo.m_CardReaderInfo.m_aryu8DevFunctions, iItemLen, DEV_FUNCTIONS_LEN);
			break;

		case eSPOT_CR_DEVMODEL:
			if(m_HardwareInfo.m_CardReaderInfo.m_pcDevModel == NULL)
				m_HardwareInfo.m_CardReaderInfo.m_pcDevModel = new char[DESCRIPTION_LEN];

			WriteCharField(pData, m_HardwareInfo.m_CardReaderInfo.m_pcDevModel, iItemLen, DESCRIPTION_LEN);
			break;

		default:
			return 0; // unknown item
	}

	return 1; // one item processed
}

void GGlobalSystemInformation::UpdateSoftwareInfoItem(int iItemID, int iItemLen, UINT8* pData)
{
	int idx = (iItemID - SWINFO_BASE_ENUM) * SWR_VERSION_NO_LEN;

	WriteCharField(pData, m_SoftwareInfo.m_pcSoftwareVersions + idx, iItemLen, SWR_VERSION_NO_LEN);
}

void GGlobalSystemInformation::UpdateFirmwareInfoItem(int iItemID, int iItemLen, UINT8* pData)
{
	int idx = (iItemID - FWINFO_BASE_ENUM) * FWR_VERSION_NO_LEN;

	WriteCharField(pData, m_SoftwareInfo.m_pcFirmwareVersions + idx, iItemLen, FWR_VERSION_NO_LEN);
}

int GGlobalSystemInformation::ParseHardwareConfigReply(GSPOTMessage* pMsg)
{
	int iret = ReceiveHardwareConfigReply(pMsg);
	ReceivedCommandFromSPOT();
	return iret;
}

/**
 *
 *  Receive message : OPT Hardware configuration   0x10, 0x02
 *
 * Similar to above case but now each device has different parameters set. This is managed by a variable length
 * sequence of tagged value. That is a sequence of 1byte for parameter tag and n bytes for parameter value
 * (where depends on the parameter type).

 * AckCode[1]
 * See 6.4.1
 * NumEntries[1]
 * Entries[NumEntries]
 * TagId[1]
 * TagLen[1]
 * TagData[TagLen]
 * Tag coding (id.s) and meaning are explained in chapter
 * @param GSPOTMessage the message answer to parse
 * */
int GGlobalSystemInformation::ReceiveHardwareConfigReply(GSPOTMessage* pMsg)
{
	int len = pMsg->GetDataLen();
	if(len < 8)
		return 0; // bad reply message

	UINT8 *pu8Data = new UINT8[len];
	memset(pu8Data, 0, len);
	pMsg->GetData(pu8Data, len);
	if(pu8Data[0] != 0x00) // ACK code not OK
	{
		delete[] pu8Data;
		return 0;
	}

	int nItems = pu8Data[1];
	if(!nItems) // null item qty
	{
		delete[] pu8Data;
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 0, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2; // byte pointer to scan items
	char*  pc = NULL;

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID =  *pb++;
		iItemLen = *pb++;

		if(iItemID >= eSPOT_PR_SERIALNUMBER && iItemID <= eSPOT_PR_DEVMODEL)
		{
			iProcessedItems += UpdatePrinterInfo(iItemID, iItemLen, pb);
		}

		pb += iItemLen; // now points to the following item's TLV structure
		iItemNo++;
	}

	delete[] pu8Data;
	return iProcessedItems;
}

int GGlobalSystemInformation::UpdatePrinterInfo(int iItemID, int iItemLen, UINT8* pData)
{
	switch(iItemID)
	{
		case eSPOT_PR_SERIALNUMBER:
			if(m_HardwareInfo.m_PrinterInfo.m_pcSerialNumber == NULL)
				m_HardwareInfo.m_PrinterInfo.m_pcSerialNumber = new char[OPT_SERIALNO_LEN];

			WriteCharField(pData, m_HardwareInfo.m_PrinterInfo.m_pcSerialNumber, iItemLen, OPT_SERIALNO_LEN);
			break;

		case eSPOT_PR_DEVTYPE:
			m_HardwareInfo.m_PrinterInfo.m_u8DeviceType = *pData;
			break;

		case eSPOT_PR_DEVFUNCTIONS:
			WriteByteField(pData, m_HardwareInfo.m_PrinterInfo.m_aryu8DevFunctions, iItemLen, DEV_FUNCTIONS_LEN);
			break;

		case eSPOT_PR_DEVMODEL:
			if(m_HardwareInfo.m_PrinterInfo.m_pcDevModel == NULL)
				m_HardwareInfo.m_PrinterInfo.m_pcDevModel = new char[OPT_SERIALNO_LEN];

			WriteCharField(pData, m_HardwareInfo.m_PrinterInfo.m_pcDevModel, iItemLen, OPT_SERIALNO_LEN);
			break;

		default:
			return 0; // unknown item
	}

	return 1; // one item processed
}

const char* GGlobalSystemInformation::GetSoftwareInfo(int index)
{
	if(index < eMinSwfIndex || index > eMaxSwfIndex)
		return NULL;

	return m_SoftwareInfo.m_pcSoftwareVersions + index * SWR_VERSION_NO_LEN;
}

const char* GGlobalSystemInformation::GetFirmwareInfo(int index)
{
	if(index < eMinFwrIndex || index > eMaxFwrIndex)
		return NULL;

	return m_SoftwareInfo.m_pcFirmwareVersions + index * FWR_VERSION_NO_LEN;
}


int GGlobalSystemInformation::ParseOPTStatusInfoRequestReply(GSPOTMessage* pMsg)
{
	int iret = ReceiveOPTStatusInfoRequestReply(pMsg);
	ReceivedCommandFromSPOT();
	return iret;
}

int GGlobalSystemInformation::ReceiveOPTStatusInfoRequestReply(GSPOTMessage* pMsg)
{
	int len = pMsg->GetDataLen();

	UINT8 *pu8Data = new UINT8[len];
	memset(pu8Data, 0, len);
	pMsg->GetData(pu8Data, len);
	if (pu8Data[0] != 0x00) // ACK code not OK
	{
		delete[] pu8Data;
		return 0;
	}

	int nItems = pu8Data[1];
	if (!nItems) // null item qty
	{
		delete[] pu8Data;
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 4, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2; // byte pointer to scan items
	char* pc = NULL;

	while (iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID = *pb++;

		if (iItemID >= eSPOT_SS_PRINTER && iItemID <= eSPOT_SS_BATTINPUT4)
		{
			iProcessedItems += UpdateOPTStatusInfo(iItemID, iItemLen, pb);
		}

		pb += iItemLen; // now points to the following item's TLV structure
		iItemNo++;
	}

	delete[] pu8Data;
	return iProcessedItems;
}

int GGlobalSystemInformation::UpdateOPTStatusInfo(int iItemID, int iItemLen, UINT8* pData)
{
	switch (iItemID)
	{
		case eSPOT_SS_OTI:
			// Take first message data only, or the update from "disabled" to "enabled"
			if (m_OTI_Info.m_u8Status == 0 || m_OTI_Info.m_u8Status == 3 && *pData == 4)
			{
				m_OTI_Info.m_u8Status = *pData;
			}
			if (m_OTI_Info.m_u8StatusExt == 0x0)
				m_OTI_Info.m_u8StatusExt = *(pData + 1);
			if (m_OTI_Info.m_u8ErrorCode == 0x0)
				m_OTI_Info.m_u8ErrorCode = *(pData + 2);
		break;

		default:
			return 0; // unknown item
	}

	return 1; // one item processed
}

/*
 * GResourcesService.cpp
 *
 *  Created on: 29/07/2013
 *      Author: Sebastian Lipchak
 */

#include "GResourcesService.h"
#include "Common/Util/DEBUGOUT.h"
#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"
#include "Common/Util/GCRC32.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "Common/Util/GConversion.h"
#include "SPOT/Utils/GFileDownloadRequestHelperM3.h"
#include "SPOT/Utils/GFileDownloadRequestHelperM5.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Config/GSPOTCheckIni.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace GSpotDefs;

const UINT8  GResourcesService::mst_aryu8FileDeleteCmd[DY_FILE_DELETE_CMD_LEN] =
             {
                 0x00, 0x08, /* cmdLength */
                 0x01, 0x00, eSPOT_PP_RESOURCES, SPOT_PP_RM_FILE_DELETE,
                 0x00, 0x00  /* fileType, fileID */
             };

const char * GResourcesService::mst_arypcStatusLabel[] =
             {
                 "eStatusInitial",
                 "eStatusWaitingFileDeleteAnswer",
                 "eStatusWaitingFileUploadAnswer",
                 "eStatusWaitingFileBlockUploadAnswer",
                 "eStatusFileDone",
                 "eStatusError"
             };

const char * GResourcesService::mst_arypcEventLabel[] =
             {
                 "eEventSendFileUpload",
                 "eEventFileUploadAnswer",
                 "eEventSendFileBlock",
                 "eEventFileBlockAnswer",
                 "eEventSendFileDelete",
                 "eEventFileDeleteAnswer",
                 "eEventError",
                 "eLastEvent"
             };

GResourcesService*  GResourcesService::mst_pSoleInstance = NULL;
GMutex              GResourcesService::mst_createInstanceMutex;

const int           GResourcesService::mst_CiResMaxFiles = RES_MAX_FILES;
const int           GResourcesService::mst_CiMaxSlots    = RES_SCR_MAX_SLOTS;
const UINT          GResourcesService::mst_CuFileChunk   = RES_CHUNK_SIZE;


GResourcesService::GResourcesService(GSPOTSession* pSession) :
				m_pSession(pSession),
				m_eStatus(eStatusInitial),
				m_pActiveFile(NULL),
				m_u8Block(1),
				m_u32FileOffset(0),
				m_iCurrentFile(0),
				m_eStatusWholeDownload(eStatusWholeInitial),
				m_eOperation(eOperationDeletingDownloadingResources),
				m_Condition(&m_Mutex),
				m_SlotsFree()

{
	for (int i = 0; i < mst_CiMaxSlots; ++i)  m_SlotsFree.push_back(i);

	memset(m_sResourcesToUploadList, 0, sizeof(m_sResourcesToUploadList));

	LOG(eResourcesService, eInfo, "Resource Service started.\n");
}

GResourcesService::~GResourcesService()
{
	for(int i = 0; i < mst_CiResMaxFiles; ++i)
		if( m_sResourcesToUploadList[i] )
		{
			delete m_sResourcesToUploadList[i];
			m_sResourcesToUploadList[i] = NULL;
		}

	LOG(eResourcesService, eInfo, "Resource Service stopped.\n");
}

GResourcesService* GResourcesService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GResourcesService(pSession);
	}

	return mst_pSoleInstance;
}


void GResourcesService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

bool GResourcesService::FindAvailableSlots()
{
	return SendBrowseCmd();
}

void GResourcesService::InitializeResourcesWithFreeSlots()
{
	const char cPathSeparator = PATH_SEPARATOR;
	string strResourcesDefault = RESOURCES_DEFAULT_PATHNAME;
	string strPinpadColorDefault = RESOURCES_PINPAD_COLOR_DEFAULT_PATHNAME;
	string strResourcesFullPath;
	if (GGlobalSystemInformation::GetInstance()->isM3Unit())
	{
		// Load M3 keymatrix resource.

		strResourcesFullPath = strResourcesDefault + "m3" + cPathSeparator + strPinpadColorDefault;
		//add separator
		if (strResourcesFullPath[strResourcesFullPath.length() - 1] != cPathSeparator) strResourcesFullPath += cPathSeparator;

		m_sResourcesToUploadList[0] = new sResourceToUpload;
		//key matrix
		m_sResourcesToUploadList[0]->strFullFileName = strResourcesFullPath + "id236_2_PinpadM3Keymatrix15.bin";
		m_sResourcesToUploadList[0]->eType = eScreenFile;

		if (!m_SlotsFree.empty())
		{
			m_sResourcesToUploadList[0]->u8SPOTResourceId = m_SlotsFree.front();
			LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. free= %d \n", m_sResourcesToUploadList[0]->u8SPOTResourceId);
			m_SlotsFree.pop_front();
		}
		else // double check, should never happen.
		{
			m_eStatusWholeDownload = eStatusNoSlotsFree;
		}

		m_sResourcesToUploadList[0]->strShortName = "PinpadM3Keymatrix15.bin";
	}
	else // Load M5 keymatrix resource.
	{
		//for each resource for keypad

		strResourcesFullPath = strResourcesDefault + "m5" + cPathSeparator + strPinpadColorDefault;
		//add separator
		if (strResourcesFullPath[strResourcesFullPath.length() - 1] != cPathSeparator) strResourcesFullPath += cPathSeparator;

		m_sResourcesToUploadList[0] = new sResourceToUpload;
		//key matrix
		m_sResourcesToUploadList[0]->strFullFileName = strResourcesFullPath + "id236_4_PinpadM5Keymatrix16.bin";
		m_sResourcesToUploadList[0]->eType = eScreenFile;

		if (!m_SlotsFree.empty())
		{
			m_sResourcesToUploadList[0]->u8SPOTResourceId = m_SlotsFree.front();
			LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. free= %d \n", m_sResourcesToUploadList[0]->u8SPOTResourceId);
			m_SlotsFree.pop_front();
		}
		else // double check, should never happen.
		{
			m_eStatusWholeDownload = eStatusNoSlotsFree;
		}

		m_sResourcesToUploadList[0]->strShortName = "PinpadM5Keymatrix16.bin";
		//load M5 resources
	}
	m_sResourcesToUploadList[1] = new sResourceToUpload;
	//Soft keys
	m_sResourcesToUploadList[1]->strFullFileName = strResourcesFullPath + "id237_0_PinpadSoftkeys4x2.bin";
	m_sResourcesToUploadList[1]->eType = eScreenFile;
	if (!m_SlotsFree.empty())
	{
		m_sResourcesToUploadList[1]->u8SPOTResourceId = m_SlotsFree.front();
		LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. free= %d \n", m_sResourcesToUploadList[1]->u8SPOTResourceId);
		m_SlotsFree.pop_front();
	}
	else // double check, should never happen.
	{
		m_eStatusWholeDownload = eStatusNoSlotsFree;
	}
	m_sResourcesToUploadList[1]->strShortName = "PinpadSoftkeys4x2.bin";
	m_sResourcesToUploadList[2] = new sResourceToUpload;
	//Aux keys
	m_sResourcesToUploadList[2]->strFullFileName = strResourcesFullPath + "id238_0_PinpadAuxkeys442.bin";
	m_sResourcesToUploadList[2]->eType = eScreenFile;
	if (!m_SlotsFree.empty())
	{
		m_sResourcesToUploadList[2]->u8SPOTResourceId = m_SlotsFree.front();
		LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. free= %d \n", m_sResourcesToUploadList[2]->u8SPOTResourceId);
		m_SlotsFree.pop_front();
	}
	else // double check, should never happen.
	{
		m_eStatusWholeDownload = eStatusNoSlotsFree;

	}

	m_sResourcesToUploadList[2]->strShortName = "PinpadAuxkeys442.bin";
}

bool GResourcesService::InitializeResourcesFromConfigFile()
{
	LOG(eResourcesService, eDebug, "******************* GResourcesService::InitializeResourcesFromConfigFile() ************************\n");

	UINT8 u8IdPinpadM3Keymatrix15FromIni = 0xFF;

	//both
	UINT8 u8IdPinpadSoftkeys4x2FromIni = 0xFF;
	UINT8 u8IdPinpadAuxkeys442FromIni = 0xFF;

	u8IdPinpadM3Keymatrix15FromIni = GSPOTCheckIni::GetSoleInstance()->GetKeymatrixScreenResID();
	u8IdPinpadSoftkeys4x2FromIni   = GSPOTCheckIni::GetSoleInstance()->GetSoftkeyScreenResID();
	u8IdPinpadAuxkeys442FromIni    = GSPOTCheckIni::GetSoleInstance()->GetAuxkeyScreenResID();

	//M5
	//TODO get id from ini file in the future
	UINT8 u8IdPinpadM5Keymatrix16FromIni = u8IdPinpadM3Keymatrix15FromIni;


	m_sResourcesToUploadList[0] = new sResourceToUpload;
	m_sResourcesToUploadList[1] = new sResourceToUpload;
	m_sResourcesToUploadList[2] = new sResourceToUpload;

	memset(m_sResourcesToUploadList[0], 0, sizeof(sResourceToUpload));
	memset(m_sResourcesToUploadList[1], 0, sizeof(sResourceToUpload));
	memset(m_sResourcesToUploadList[2], 0, sizeof(sResourceToUpload));

	if (GGlobalSystemInformation::GetInstance()->isM3Unit())
	{
		//load M3 resources
		//key matrix

		m_sResourcesToUploadList[0] = new sResourceToUpload;
		m_sResourcesToUploadList[0]->eType = eScreenFile;
		m_sResourcesToUploadList[0]->u8SPOTResourceId = u8IdPinpadM3Keymatrix15FromIni; //get id from ini
		m_sResourcesToUploadList[0]->strShortName = "PinpadM3Keymatrix15.bin";

		LOG(eResourcesService, eDebug, "InitializeResourcesFromConfigFile. id from ini= %d \n", m_sResourcesToUploadList[0]->u8SPOTResourceId);

	}
	else // SPOT_M5
	{

		 m_sResourcesToUploadList[0] = new sResourceToUpload;
		 //key matrix
		 m_sResourcesToUploadList[0]->eType = eScreenFile;
		 m_sResourcesToUploadList[0]->u8SPOTResourceId = u8IdPinpadM5Keymatrix16FromIni; //note :  same as M3
		 m_sResourcesToUploadList[0]->strShortName = "PinpadM5Keymatrix16.bin";

		if(GGlobalSystemInformation::GetInstance()->isVGDUnit())
		{
			 m_u8SecureAudioResId = GSPOTCheckIni::GetSoleInstance()->GetSecureAudioScreenResID();
		}

		 LOG(eResourcesService, eDebug, "InitializeResourcesFromConfigFile. id from ini= %d \n",m_sResourcesToUploadList[0]->u8SPOTResourceId );

	}

	//Soft keys

	m_sResourcesToUploadList[1] = new sResourceToUpload;
	m_sResourcesToUploadList[1]->eType = eScreenFile;
	m_sResourcesToUploadList[1]->u8SPOTResourceId = u8IdPinpadSoftkeys4x2FromIni; //get id from ini file
	m_sResourcesToUploadList[1]->strShortName = "PinpadSoftkeys4x2.bin";

	LOG(eResourcesService, eDebug, "InitializeResourcesFromConfigFile. id from ini= %d \n", m_sResourcesToUploadList[1]->u8SPOTResourceId);

	//Aux keys
	m_sResourcesToUploadList[2] = new sResourceToUpload;
	m_sResourcesToUploadList[2]->eType = eScreenFile;
	m_sResourcesToUploadList[2]->u8SPOTResourceId = u8IdPinpadAuxkeys442FromIni; //get id from ini file
	m_sResourcesToUploadList[2]->strShortName = "PinpadAuxkeys442.bin";

	LOG(eResourcesService, eDebug, "InitializeResourcesFromConfigFile. id from ini= %d \n", m_sResourcesToUploadList[2]->u8SPOTResourceId);

	m_eStatusWholeDownload = eStatusDownloadCompleted;
	return true;
}

bool GResourcesService::Initialize()
{
	LOG(eResourcesService, eDebug, "******************* GResourcesService::Initialize() ************************\n");

	//check if is running embedded / inside the spot unit M3
	//if (GGlobalSystemInformation::GetInstance()->isEmbeddedSPC())
	int iRunningMode = GSPOTCheckIni::GetSoleInstance()->GetRunningMode();
	if (iRunningMode == eEmbedded || iRunningMode == eDummyEmbedded)
	{
		//get AvailableSlots from ini file
		//initialize slots.

		LOG(eResourcesService, eDebug, "***** GResourcesService::EMBEDDED MODE *****\n");
		return InitializeResourcesFromConfigFile();

	}
	else
	{
		LOG(eResourcesService, eDebug, "***** GResourcesService::EXTERNAL MODE  ******\n");

		GMutexLock oLock(m_Mutex);

		FindAvailableSlots();

		//wait up to 30s for the upload process to finish
		return (m_Condition.Wait(30000) != 0);
	}

}

bool GResourcesService::SendCmdToSPOT(GSPOTMessage* pMessage)
{
	if (m_pSession == NULL || pMessage == NULL)
	{
		return false;
	}

	return m_pSession->SendSPOTCommand(pMessage);
}

void GResourcesService::FileUpload(std::string strFullFileName,
                eSPOTFileTypes eType, UINT8 u8SPOTResourceId)
{
	LOG(eResourcesService, eDebug, "File Upload [ %s  ].\n",
	                strFullFileName.c_str());
	UINT8 u8AckCode;
	UINT32 u32Crc = 0;
	struct stat fileInfo;

	//check is file exist
	if (!CheckFile(&strFullFileName, &fileInfo))
	{
		LOG(eResourcesService, eFatal, "File [ %s  ] doesn't exist.\n",
		                strFullFileName.c_str());
		m_eStatusWholeDownload = eStatusWholeError;
		NextFile();
		return;
	}

	m_pActiveFile->pfile = fopen(m_pActiveFile->strFullFileName.c_str(), "r");

	if (!m_pActiveFile->pfile)
	{
		LOG(eResourcesService, eError, "Could not open file for Upload.\n");
		m_eStatusWholeDownload = eStatusWholeError;

		NextFile();
		return;
	}

	UINT32 u32SizeToUpload;

	if (GGlobalSystemInformation::GetInstance()->isM3Unit())
	{
		u32SizeToUpload = fileInfo.st_size - GFileDownloadRequestHelperM3::GetResmarkSize();
	}
	else
	{
		u32SizeToUpload = fileInfo.st_size - GFileDownloadRequestHelperM5::GetResmarkSize();

	}

	LOG(eResourcesService, eDebug, "File size with resmark [ %u  ].\n", (UINT32 ) fileInfo.st_size);
	LOG(eResourcesService, eDebug, "File size without resmark [ %u  ].\n", u32SizeToUpload);

	SendFileUploadRequest(eType, u8SPOTResourceId, u32SizeToUpload, u32Crc);
}

/**
 *
 * @title File Download Request
 * (Master sends a file to SPOT)
 * It has to be noticed that, when
 * (FileId ,FileType) refer to an
 * existing file, it is necessary to
 * first delete the old file and then
 * download the new one.
 *
 * 0x00, 0x01
 *
 * FileType[1] b7-b4 = SPOT local application that owns this type of file:
 * 0x00 = reserved
 * 0x01 = pinpad application
 * 0x02 = EMV application
 * 0x03 = Setup/Download application
 * 0x04 = OPT application
 * 0x05 = acquirer application
 * 0x06-0x0F = reserved for future applications
 * b3-b0 = File types client specific (see the tables with files types at each application-
 * APDU section of this document).
 *
 * FileId [1] File unique identification number (see files types tables for values usage for various
 * application)
 *
 * FileSz[4] File size.
 *
 * CRC[4] CRC32 on whole file (see appendix)
 *
 * AuthenType[1] 0x00 = none
 * 0x01 = PIN-resource authenticator (SPOT M3/M4 only)
 * 0x02 = NotPIN-resource authenticator (SPOT M3/M4 only)
 * 0x03 = PIN-resource RSA authenticator (SPOT M4/M5 only)
 * 0x04 = NotPIN-resource RSA authenticator (SPOT M4/M5 only)

 * Authent[size] Optional secret authenticator. Content and size depend on AuthenType.
 * AuthenType=0x00 It consists of four zeroes (0x00000000).
 * AuthenType=0x01,0x02 It consists in a four bytes ANSI 9.19 MAC calculated in secure
 * room as explained in ref.[4].
 * AuthenType=0x03,0x04 It consists in a 248 bytes RSA-certificate calculated in secure
 * room as explained in ref.[4].

 Description[16] Ascii description of the file.
 */

bool GResourcesService::SendFileUploadRequest(eSPOTFileTypes eType,
                UINT8 u8FileId, UINT32 u32FileSize, UINT32 u32Crc)
{
	if (m_pSession == NULL)
		return false;

	GSPOTMessage oMsg(eSPOT_PP_CATEGORY, eSPOT_PP_RESOURCES, SPOT_PP_RM_FILE_TRANSFER_REQUEST, NULL, 0);

	//Development

	string strResourceFile = m_pActiveFile->strFullFileName;

	string strPPNunformat(GGlobalSystemInformation::GetInstance()->GetSPOTInfo().m_pcSerialNumber);

	string strPPN = strPPNunformat.substr(strPPNunformat.size() - 8, 8);

	LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: PPN = %s \n", strPPN.c_str());

	//get the file size
	struct stat st;
	stat(strResourceFile.c_str(), &st);
	UINT32 u32Size = st.st_size;

	if (GGlobalSystemInformation::GetInstance()->isM3Unit())
	{
		GFileDownloadRequestHelperM3 oDownloadRequestHelper;

		oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str(), strPPN.c_str());

		//oDownloadRequestHelper.GetFileId();
		m_pActiveFile->u32FileSizeNoResmark = oDownloadRequestHelper.GetFileSize();

		//	//test files has been hacked with this value
		UINT32 u32TestCRC = oDownloadRequestHelper.GetCRC();

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: oDownloadRequestHelper.GetCRC = %u \n", u32TestCRC);
		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: CRC = %u \n", u32Crc);

		UINT8 u8AunthenType = oDownloadRequestHelper.GetAuthenType();

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: Authen Type = %u \n", u8AunthenType);

		//.lic file is not present then the auth is gotten from resource itself
		UINT32 u32TestAuth = oDownloadRequestHelper.GetAuthentication();



		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: Authen = %u \n", u32TestAuth)

		char szFileDescription[17];
		strncpy(szFileDescription,oDownloadRequestHelper.GetDescription(),sizeof(szFileDescription)-1);
		szFileDescription[16] = 0;

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: Description = %s \n", szFileDescription);
		//Production

		//File type
		UINT8 u8FileType = 0x10; // the high nibble represent a resource for a PinPad app
		u8FileType |= eType;      //the low nibble represent the file type

		//File type
		oMsg.AppendByte(u8FileType);
		//file id
		oMsg.AppendByte(u8FileId);

		//file size
		oMsg.AppendDWord(u32FileSize);

		//CRC value
		oMsg.AppendDWord(u32TestCRC);

		//AuthenType
		//put Ansi 9.19 mac or RSA signature from secure room
		oMsg.AppendByte(u8AunthenType);

		//Authent
		oMsg.AppendDWord(u32TestAuth);

		//Description
		UINT16 u16Description = 16;
		oMsg.Append((UINT8*)m_pActiveFile->strShortName.c_str(), u16Description);
	}
	else
	{
		GFileDownloadRequestHelperM5 oDownloadRequestHelper;

		oDownloadRequestHelper.ProcessResourceFile(strResourceFile.c_str());

		//oDownloadRequestHelper.GetFileId();
		m_pActiveFile->u32FileSizeNoResmark = oDownloadRequestHelper.GetFileSize();

		//	//test files has been hacked with this value
		UINT32 u32TestCRC = oDownloadRequestHelper.GetCRC();

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: oDownloadRequestHelper.GetCRC = %u \n", u32TestCRC);
		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: CRC = %u \n", u32Crc);

		UINT8 u8AunthenType = oDownloadRequestHelper.GetAuthenType();

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: Authen Type = %u \n", u8AunthenType);

		UINT8 aryAuthentication[GFileDownloadRequestHelperM5::CAuthLen];
		//.lic file is not present then the auth is gotten from resource itself
		oDownloadRequestHelper.GetAuthentication(aryAuthentication, sizeof(aryAuthentication));

		char szFileDescription[17];
		strncpy(szFileDescription,oDownloadRequestHelper.GetDescription(),sizeof(szFileDescription)-1);
		szFileDescription[16] = 0;

		LOG(eResourcesService, eDebug, "***SendFileUploadRequest:: Description = %s \n", szFileDescription);

		//Production

		//File type
		UINT8 u8FileType = 0x10; // the high nibble represent a resource for a PinPad app
		u8FileType |= eType;      //the low nibble represent the file type

		//File type
		oMsg.AppendByte(u8FileType);
		//file id
		oMsg.AppendByte(u8FileId);

		//file size
		oMsg.AppendDWord(u32FileSize);

		//CRC value
		oMsg.AppendDWord(u32TestCRC);

		//AuthenType
		//put Ansi 9.19 mac or RSA signature from secure room
		oMsg.AppendByte(u8AunthenType);

		//Authent
		oMsg.Append(aryAuthentication,sizeof(aryAuthentication));

		//Description
		UINT16 u16Description = 16;
		oMsg.Append((UINT8*)m_pActiveFile->strShortName.c_str(), u16Description);

	}

	UINT8 aryu8CreateCmd[oMsg.GetDataLen()];
	memset(aryu8CreateCmd, 0, oMsg.GetDataLen());

	oMsg.GetData(aryu8CreateCmd, oMsg.GetDataLen());

	LOG(eResourcesService, eDebug, "Sending  File Download request message: \n");

	bool blret = SendCmdToSPOT(&oMsg);
	LOG(eResourcesService, eDebug, "GResourcesService::SendFileUploadRequest SendCmdToSPOT - %s\n",
	                                blret ? "OK" : "FAIL");
	return blret;
}

bool GResourcesService::SendBrowseCmd()
{
	LOG(eResourcesService, eDebug, "GResourcesService::SendBrowseCmd \n");

	if (m_pSession == NULL)
		return false;

	GSPOTMessage oMsg(eSPOT_PP_CATEGORY, eSPOT_PP_RESOURCES, SPOT_PP_RM_FILE_BROWSE, NULL, 0);

	UINT8 u8FileType = 0x10;
	u8FileType |= eScreenFile;

	oMsg.AppendByte(u8FileType);

	LOG(eResourcesService, eDebug, "SendBrowseCmd. Sending  file browse message: \n");

	UINT8 aryu8CreateCmd[DY_BROWSE_CMD_LEN+6];
	memset(aryu8CreateCmd,0, sizeof(aryu8CreateCmd));
	UINT16 u16Len     = oMsg.GetTransmitBuffer(aryu8CreateCmd);
	UINT16 u16Datalen = oMsg.GetDataLen();
	UINT16 u16MsgLen  = oMsg.GetMessageLen();

	LOG(eResourcesService, eDebug, "SendBrowseCmd. Sending  file browse message with len: %d, datlen = %d, Message len = %d\n", u16Len, u16Datalen, u16MsgLen);
	GConversion::LogBytesAsASCIIHexString(aryu8CreateCmd, sizeof(aryu8CreateCmd));

	bool blret = SendCmdToSPOT(&oMsg);
	LOG(eResourcesService, eDebug, "GResourcesService::SendBrowseCmd. SendCmdToSPOT - %s\n",
	    blret ? "OK" : "FAIL");

	return blret;

}

bool GResourcesService::SendDeleteFileRequest(eSPOTFileTypes eType, UINT8 u8FileId)
{
	LOG(eResourcesService, eDebug, "GResourcesService::SendDeleteFileRequest \n");

	if (m_pSession == NULL)
		return false;

	UINT8 aryu8CreateCmd[DY_FILE_DELETE_CMD_LEN];

	memcpy(aryu8CreateCmd, mst_aryu8FileDeleteCmd, sizeof(aryu8CreateCmd));

	UINT8 u8FileType = 0x10;
	u8FileType |= eType;

	int pos = 6;

	//File type
	aryu8CreateCmd[pos++] = u8FileType;

	//File id
	aryu8CreateCmd[pos++] = u8FileId;

	LOG(eResourcesService, eDebug, "SendDeleteFileRequest. Sending  delete message: \n");

	GSPOTMessage oMsg(aryu8CreateCmd);
	bool blret = SendCmdToSPOT(&oMsg);
	LOG(eResourcesService, eDebug, "GResourcesService::SendDeleteFileRequest. SendCmdToSPOT - %s\n",
	                                blret ? "OK" : "FAIL");
	return blret;
}

bool GResourcesService::SendBlockUploadRequest(UINT32 u32Offset, UINT8* pBlock, UINT16 u16BlockSize)
{
	if (m_pSession == NULL)
		return false;

	GSPOTMessage oMsg(eSPOT_PP_CATEGORY, eSPOT_PP_RESOURCES, SPOT_PP_RM_FILE_TRANSFER, NULL, 0);

	oMsg.AppendDWord(u32Offset);
	oMsg.AppendWord(u16BlockSize);
	oMsg.Append(pBlock, u16BlockSize);

	LOG(eResourcesService, eDebug, "Sending  File Block Download message: \n");

	bool blret = SendCmdToSPOT(&oMsg);
	LOG(eResourcesService, eDebug, "GResourcesService::SendBlockUploadRequest. SendCmdToSPOT - %s\n",
	                               blret ? "OK" : "FAIL");
	return blret;
}

bool GResourcesService::CheckFile(std::string* strFileName,
                struct stat* fileinfo)
{
	struct stat buf;
	struct stat* pbuf = &buf;

	if (fileinfo)
		pbuf = fileinfo;
	else
		pbuf = &buf;

	return (stat(strFileName->c_str(), pbuf) == 0);
}

void GResourcesService::ProcessEventReceived(eEvents e, sDataEvent* pEventData)
{
	switch (m_eStatus)
	{
		//======================================================================================================
		case eStatusInitial:
			{
			switch (e)
			{

				case eEventSendFileUpload:
					NextState(eEventSendFileUpload, eStatusInitial);

				case eEventFileUploadAnswer:
					NextState(eEventFileUploadAnswer, eStatusInitial);
				break;

				case eEventFileBlockAnswer:
					NextState(eEventFileUploadAnswer, eStatusInitial);
				break;

				case eEventSendFileDelete:
					if (m_pActiveFile)
					{
						NextState(eEventSendFileDelete, eStatusWaitingFileDeleteAnswer);
						SendDeleteFileRequest(m_pActiveFile->eType, m_pActiveFile->u8SPOTResourceId);
					}
				break;

				case eEventFileDeleteAnswer:
					NextState(eEventFileDeleteAnswer, eStatusInitial);

				break;

				case eEventError:

				break;

				default:
					LOG(eResourcesService, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;

			}
		}
		break;
			//======================================================================================================

			//======================================================================================================
		case eStatusWaitingFileDeleteAnswer:
			{
			switch (e)
			{
				case eEventFileDeleteAnswer:

					//process fileDelete Answer
					if (m_eOperation == eOperationDeletingResources)
					{
						NextState(eEventFileDeleteAnswer, eStatusFileDone);
						NextFile();
					}
					else // eOperationDeletingDownloadingResources
					{
						//Delete was Ok, so we send file upload request.
						if (m_pActiveFile)
						{
							//clean offset
							m_u8Block = 1;
							m_u32FileOffset = 0;

							NextState(eEventFileDeleteAnswer, eStatusWaitingFileUploadAnswer);
							FileUpload(m_pActiveFile->strFullFileName, m_pActiveFile->eType, m_pActiveFile->u8SPOTResourceId);
						}
					}

				break;

				case eEventFileUploadAnswer:
					NextState(eEventFileUploadAnswer, eStatusWaitingFileDeleteAnswer);
				break;

				case eEventError:

				break;

				default:
					LOG(eResourcesService, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;

			}

		}
		break;
			//======================================================================================================

			//======================================================================================================
		case eStatusWaitingFileUploadAnswer:
			{
			switch (e)
			{

				case eEventFileUploadAnswer:

					if (pEventData && (pEventData->size >= 1) && m_pActiveFile)
					{
						UINT8 u8AckCode = ((UINT8*) pEventData->data)[0];

						//if there was an error
						if (u8AckCode != eSPOT_NO_ERROR && u8AckCode != eSPOT_DOWNLOAD_COMPLETED)
						{
							LOG(eResourcesService, eError, "eEventFileUploadAnswer. Unexpected error during upload.\n");

							LogAckCode(u8AckCode);
							//log error code and go to Error state
							NextFile();

							m_eStatusWholeDownload = eStatusWholeError;

							break;
						}

						if (u8AckCode == eSPOT_NO_ERROR)
						{
							NextState(eEventFileUploadAnswer, eStatusWaitingFileBlockUploadAnswer);
							SendNextBlock();
						}
					}

				break;

				case eEventFileBlockAnswer:
					NextState(eEventFileUploadAnswer, eStatusWaitingFileUploadAnswer);
				break;

				case eEventError:

				break;

				default:
					LOG(eResourcesService, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;

			}
		}
		break;
			//======================================================================================================

			//======================================================================================================

		case eStatusWaitingFileBlockUploadAnswer:
			{
			switch (e)
			{

				case eEventFileBlockAnswer:

					//process block upload answer

					if (pEventData && pEventData->size >= 5 && m_pActiveFile)
					{

						UINT8 u8AckCode = ((UINT8*) pEventData->data)[0];
						UINT32 u32OffsetReceived = 0;
						UINT8* pu8Buff = &((UINT8*) pEventData->data)[1];

						memcpy(&u32OffsetReceived, pu8Buff, sizeof(UINT32));
						u32OffsetReceived = GConversion::BigEndianToHost(u32OffsetReceived);

						LOG(eResourcesService, eDebug, "Offset Received:  %u.\n", u32OffsetReceived);

						if (u8AckCode != eSPOT_NO_ERROR && u8AckCode != eSPOT_DOWNLOAD_COMPLETED)
						{
							LOG(eResourcesService, eError, "eEventFileBlockAnswer.Unexpected error during upload.\n");

							//see if this is the last block
							if (u32OffsetReceived == m_pActiveFile->u32FileSizeNoResmark)
							{
								LOG(eResourcesService, eError, "File Download completed WITH ERRROR.File Upload ended. last block sent.\n");

							}
							//flag whole process error
							m_eStatusWholeDownload = eStatusWholeError;
							NextFile();

							break;
						}

						//process answer
						if (u32OffsetReceived == m_pActiveFile->u32FileSizeNoResmark && u8AckCode == eSPOT_DOWNLOAD_COMPLETED)
						{
							LOG(eResourcesService, eInfo, "File Download completed OK. filetype:  %d, fileID:  %d, Path: %s.\n", m_pActiveFile->eType, m_pActiveFile->u8SPOTResourceId,
							                m_pActiveFile->strFullFileName.c_str());

							NextState(eEventFileBlockAnswer, eStatusFileDone);
							NextFile();

						}
						else
						{

							NextState(eEventFileBlockAnswer, eStatusWaitingFileBlockUploadAnswer);
							SendNextBlock();
						}
					}

				break;

				case eEventFileUploadAnswer:

				break;

				case eEventError:

				break;

				default:
					LOG(eResourcesService, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;

			}
		}

		break;
			//======================================================================================================

	}

	if (pEventData != NULL)
	{
		delete pEventData;
	}

}

void GResourcesService::NextState(eEvents e, eStatus s)
{
	LOG(eResourcesService, eDebug, "[ResourcesService][current_status[%s] ==>> event[%s] ==>> next_status[%s]]\n", mst_arypcStatusLabel[m_eStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s]);
	m_eStatus = s;
}

bool GResourcesService::DeleteKeypadResourcesToSPOT()
{
	LOG(eResourcesService, eDebug, "DeleteKeypadResourcesToSPOT....\n");

	if ( m_eStatusWholeDownload == eStatusNoSlotsFree )
	{
		LOG(eResourcesService, eDebug, "DeleteKeypadResourcesToSPOT: No need to delete resources with no slots free.\n");
		return false;
	}

	//check if is running embedded / inside the spot unit
	//TODO check what happens in M5 Unit in the future

	int iRunningMode =GSPOTCheckIni::GetSoleInstance()->GetRunningMode();
	//if (GGlobalSystemInformation::GetInstance()->isEmbeddedSPC())
	if(iRunningMode == eEmbedded || iRunningMode == eDummyEmbedded)
	{
		LOG(eResourcesService, eDebug, "DeleteKeypadResourcesToSPOT: No need to delete resources in embedded mode\n");
		return false;
	}

	GMutexLock oLock(m_Mutex);

	m_iCurrentFile = 0;

	//set service operation to do
	m_eOperation = eOperationDeletingResources;
	m_eStatusWholeDownload = eStatusWholeInitial;
	m_eStatus = eStatusInitial;

	m_pActiveFile = m_sResourcesToUploadList[m_iCurrentFile];

	ProcessEventReceived(eEventSendFileDelete, NULL);

	//wait up to 10s for the delete process to finish
	return (m_Condition.Wait(10000) != 0);
}

bool GResourcesService::UploadKeypadResourcesToSPOT()
{
	LOG(eResourcesService, eDebug, "UploadKeypadResourcesToSPOT....\n");

	//check if is running embedded / inside the spot unit
	//TODO check what happens in M5 Unit in the future

	int iRunningMode =GSPOTCheckIni::GetSoleInstance()->GetRunningMode();
	//if (GGlobalSystemInformation::GetInstance()->isEmbeddedSPC())
	if(iRunningMode == eEmbedded || iRunningMode == eDummyEmbedded)
	{
		LOG(eResourcesService, eDebug, "UploadKeypadResourcesToSPOT: No need to download resources in embedded mode\n");
		return true;
	}

	if ( m_eStatusWholeDownload == eStatusNoSlotsFree)
	{
		return false;
	}

	GMutexLock oLock(m_Mutex);

	m_SlotsFree.clear();
	for (int i=0; i< mst_CiMaxSlots; ++i) m_SlotsFree.push_back(i);

	m_iCurrentFile = 0;

	//set service operation to do
	m_eOperation = eOperationDeletingDownloadingResources;
	m_eStatusWholeDownload = eStatusWholeInitial;
	m_pActiveFile = m_sResourcesToUploadList[m_iCurrentFile];
	m_eStatus = eStatusInitial;
	ProcessEventReceived(eEventSendFileDelete, NULL);

	//wait up to 30s for the upload process to finish
	return (m_Condition.Wait(30000) != 0);
}

void GResourcesService::SendNextBlock()
{
	LOG(eResourcesService, eDebug, "SendNextBlock.\n");

	UINT8 aryu8Block[mst_CuFileChunk];
	INT16 s16Read;

	FILE* pfile = m_pActiveFile->pfile;

	if (!feof(pfile))
	{

		if (m_u32FileOffset < m_pActiveFile->u32FileSizeNoResmark)
		{
			fseek(pfile, m_u32FileOffset, SEEK_SET);
			if (m_pActiveFile->u32FileSizeNoResmark - m_u32FileOffset < mst_CuFileChunk)
			{
				s16Read = fread(aryu8Block, 1, m_pActiveFile->u32FileSizeNoResmark - m_u32FileOffset, pfile);
			}
			else
			{
				s16Read = fread(aryu8Block, 1, mst_CuFileChunk, pfile);
			}

			if (SendBlockUploadRequest(m_u32FileOffset, aryu8Block, s16Read))
			{
				++m_u8Block;
				m_u32FileOffset += s16Read;
			}
			else
			{
				LOG(eResourcesService, eError, "Error sending block # %d file with offset: %d\n", m_u8Block);
			}
		}

		if (ferror(pfile))
		{
			LOG(eResourcesService, eError, "SendNextBlock(): Unexpected error during upload.\n");
			fclose(pfile);
			pfile = NULL;
			m_eStatusWholeDownload = eStatusWholeError;
			return;
		}
	}
}

bool GResourcesService::isResourcesReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if (!pMsg)
	{
		return false; // no message is supplied
	}

	if (!blOnlySubCmd) // this flag avoids to analyze the following, if already tested true
	{
		if (pMsg->GetAppId() != eSPOT_PP_ANSWER)
		{
			return false; // unexpected application ID
		}

		if (pMsg->GetCommandCode() != eSPOT_PP_RESOURCES)
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();

	if (iSubCmd == eSPOT_PP_RM_FILE_TRANSFER_REQUEST || iSubCmd == eSPOT_PP_RM_FILE_TRANSFER || iSubCmd == eSPOT_PP_RM_FILE_DELETE || iSubCmd == eSPOT_PP_RM_FILE_BROWSE  )
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

bool GResourcesService::ProcessResourcesReply(GSPOTMessage* pMsg)
{
	bool   blret      = false;
	UINT8* pu8MsgData = NULL;
	UINT16 u16DataLen = 0;
	int    iCmdCode   = pMsg->GetSubCommandCode();

	u16DataLen = pMsg->GetDataLen();
	if (u16DataLen == 0)
	{
		return false;
	}

	pu8MsgData = pMsg->GetData();

	if (pu8MsgData == NULL)
	{
		return false;
	}

	UINT8 u8AckCode = pu8MsgData[0];

	switch (iCmdCode)
	{
		case eSPOT_PP_RM_FILE_TRANSFER_REQUEST:

			LOG(eResourcesService, eDebug, "File send request answer received\n");
			LOG(eResourcesService, eDebug, "Answer. AckCode [%02x]\n", u8AckCode);
			LogAckCode(u8AckCode);

			blret = ProcessDataEvent(eEventFileUploadAnswer, pu8MsgData, u16DataLen);

			blret = true;
		break;

		case eSPOT_PP_RM_FILE_TRANSFER:

			LOG(eResourcesService, eDebug, "File block answer received\n");
			LOG(eResourcesService, eDebug, "Answer. AckCode [%02x] \n", u8AckCode);
			LogAckCode(u8AckCode);

			blret = ProcessDataEvent(eEventFileBlockAnswer, pu8MsgData, u16DataLen);
			blret = true;

		break;

		case eSPOT_PP_RM_FILE_DELETE:

			LOG(eResourcesService, eDebug, "File Delete Answer received \n");
			LOG(eResourcesService, eDebug, "Answer. AckCode [%02x] \n", u8AckCode);
			LogAckCode(u8AckCode);

			if (u8AckCode == eSPOT_NO_ERROR)
			{
				LOG(eResourcesService, eInfo, "Delete DONE OK. file type [%d] id [%d] \n ", m_pActiveFile->eType, m_pActiveFile->u8SPOTResourceId);
			}
			else
			{
				LOG(eResourcesService, eInfo, "Delete FAIL, probably file not in SPOT. file type [%d] id [%d] \n ", m_pActiveFile->eType, m_pActiveFile->u8SPOTResourceId);
			}

			ProcessEventReceived(eEventFileDeleteAnswer, NULL);

		break;

		case eSPOT_PP_RM_FILE_BROWSE:

			LOG(eResourcesService, eDebug, "File Browse Answer received \n");
			LOG(eResourcesService, eDebug, "Answer. AckCode [%02x] \n", u8AckCode);
			LogAckCode(u8AckCode);

			if (u8AckCode == eSPOT_NO_ERROR)
			{
				LOG(eResourcesService, eInfo, "File Browse DONE OK.\n");
			}
			else
			{
				LOG(eResourcesService, eFatal, "File Browse FAIL\n");
			}

			ProcessFileBrowseAnswer(pMsg);

		break;

		default:
			break;
	}

	return blret;
}

bool GResourcesService::ProcessFileBrowseAnswer(GSPOTMessage* pMsg)
{
	LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. File Browse Answer decode\n");

	int len = pMsg->GetDataLen();
	if( len < DY_BROWSE_REPLY_MIN_DATALEN )
		return false; // bad reply message

	UINT8 *pu8Data = pMsg->GetData();

	if(pu8Data[0] != eSPOT_NO_ERROR) // ACK code not OK
	{
		return false;
	}

	int nFileType = pu8Data[1]; // FileType

	LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. File type = 0x%02x \n", nFileType );

	int nItems = pu8Data[2]; // NumEntries
	if( nItems + mst_CiResMaxFiles >= mst_CiMaxSlots ) // no place for SPOTCheck resources ?
	{
		m_eStatusWholeDownload = eStatusNoSlotsFree;
		LOG(eResourcesService, eError,
		    "ProcessFileBrowseAnswer: NOT ENOUGH FREE RESOURCE SLOTS : %d\n",
		    mst_CiMaxSlots - nItems);
		return false;
	}

	int iItemNo = 0, iFileId = 0;
	UINT8* pb = pu8Data + 3; // byte pointer to scan items

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iFileId =  *pb++;

		LOG(eResourcesService, eDebug, "ProcessFileBrowseAnswer. File slot used= %d \n",iFileId );

		// remove slot from the list due is used.
		m_SlotsFree.remove(iFileId);

		pb += DY_BROWSE_ENTRY_LEN - 1 ; // now points to the following item
		iItemNo++;
	}

	InitializeResourcesWithFreeSlots();

	// Signal the finish of File Browse parsing.
	m_Mutex.Lock();
	m_Condition.Signal();
	m_Mutex.Unlock();

	if(m_eStatusWholeDownload != eStatusNoSlotsFree )
	{
		return true;
	}
	else
	{
		return false;
	}

}


bool GResourcesService::ProcessDataEvent(eEvents e, UINT8* pMsgData, UINT16 u16DataSize)
{
	const char* pcMethod = "GResourceService::ProcessDataEvent() -- ";

	GMutexLock ml(m_StateChangeMutex);

	sDataEvent* pData = new sDataEvent;
	pData->data = (void *) pMsgData;
	pData->size = u16DataSize;

	ProcessEventReceived(e, pData);

	return true;
}

void GResourcesService::NextFile()
{
	const char *pcLogLegend = NULL;

	//close current file
	if (m_pActiveFile->pfile != NULL)
	{
		struct stat fileInfo;

		//check is file exist
		if (CheckFile(&m_pActiveFile->strFullFileName, &fileInfo))
		{
			LOG(eResourcesService, eInfo, "File [ %s  ] will be closed.\n",
			                m_pActiveFile->strFullFileName.c_str());
			fclose(m_pActiveFile->pfile);
			m_pActiveFile->pfile = NULL;
		}
	}

	//move to next file
	m_iCurrentFile++;

	if (m_sResourcesToUploadList[m_iCurrentFile] != NULL && m_iCurrentFile < mst_CiResMaxFiles)
	{
		NextState(eEventSendFileDelete, eStatusInitial);

		m_pActiveFile = m_sResourcesToUploadList[m_iCurrentFile];
		ProcessEventReceived(eEventSendFileDelete, NULL);
	}
	else // all files have been processed (uploaded or deleted)
	{
		if( m_eOperation == eOperationDeletingResources )
		{
			pcLogLegend = "****==== Delete resources finished.\n";
			LOG(eResourcesService, eInfo, pcLogLegend);
		}

		// upload finished

		NextState(eLastEvent, eStatusFileDone);
		// FLAG finish of process
		if (m_eStatusWholeDownload != eStatusWholeError)
			m_eStatusWholeDownload = eStatusDownloadCompleted;

		if (isLastUploadProcessOk())
		{
			pcLogLegend = "****========= Whole Upload Process: OK\n";
		}
		else
		{
			pcLogLegend = "****========= Whole Upload Process: ERROR\n";
		}


		LOG(eResourcesService, eInfo, pcLogLegend);

		//Signal the condition to unblock the UploadKeypadResourcesToSPOT
		m_Mutex.Lock();
		m_Condition.Signal();
		m_Mutex.Unlock();
	}
}

void GResourcesService::LogAckCode(UINT8 u8AckCode)
{
	switch (u8AckCode)
	{
		case eSPOT_NO_ERROR:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_NO_ERROR\n");
		break;

		case eSPOT_DOWNLOAD_COMPLETED:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_DOWNLOAD_COMPLETED\n");
		break;

		case eSPOT_ERR_NOT_ENOUGH_SPACE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_NOT_ENOUGH_SPACE\n");
		break;

		case eSPOT_ERR_BLOCK_OUT_OF_SEQUENCE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_BLOCK_OUT_OF_SEQUENCE\n");
		break;

		case eSPOT_ERR_OVERRUN:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_OVERRUN\n");
		break;

		case eSPOT_ERR_BAD_SIGNATURE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_BAD_SIGNATURE\n");
		break;

		case eSPOT_ERR_BLOCK_WRITE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_BLOCK_WRITE\n");
		break;
		case eSPOT_ERR_CANNOT_ADD_RESOURCE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_CANNOT_ADD_RESOURCE\n");
		break;

		case eSPOT_ERR_CANNOT_DELETE_RESOURCE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_CANNOT_DELETE_RESOURCE\n");
		break;

		case eSPOT_ERR_CANNOT_CREATE_TMP_FILE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_CANNOT_CREATE_TMP_FILE\n");
		break;

		case eSPOT_ERR_CANNOT_READ_RESOURCE:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_CANNOT_READ_RESOURCE\n");
		break;

		case eSPOT_ERR_BLOCK_READ:
			LOG(eResourcesService, eDebug, "Error Description: eSPOT_ERR_BLOCK_READ\n");
		break;

		default:
			LOG(eResourcesService, eDebug, "Error Description: unknown\n");

		break;
	}
}


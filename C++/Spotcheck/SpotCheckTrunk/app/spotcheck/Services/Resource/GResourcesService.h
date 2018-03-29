/**

 © 2013 Gilbarco Inc.
 Confidential and Proprietary
 *
 @file GResourcesService.h
 @author Sebastian Lipchak
 @date July 29 2013
 @copyright © 2013 Gilbarco Inc. Confidential and Proprietary

 */

#ifndef GRESOURCESSERVICE_H_
#define GRESOURCESSERVICE_H_

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <GMutex.h>
#include <GCondition.h>
#include <list>

enum SPOT_PP_RESOURCES_SUBCMDS
{
	SPOT_PP_RM_FILE_TRANSFER_REQUEST = 0x01,
	SPOT_PP_RM_FILE_TRANSFER = 0x02,
	SPOT_PP_RM_FILE_BROWSE = 0x03,
	SPOT_PP_RM_FILE_DELETE = 0x04,
	SPOT_PP_RM_UPLOAD_TRANSFER_REQUEST = 0x05,
	SPOT_PP_RM_UPLOAD_TRANSFER = 0x06,
};

const int DY_FILE_UPLOAD_REQUEST_CMD_LEN = 17;
const int DY_FILE_BLOCK_REQUEST_CMD_LEN = 13;
const int DY_FILE_DELETE_CMD_LEN = 8;
const int DY_BROWSE_CMD_LEN = 7;
const int DY_BROWSE_REPLY_MIN_DATALEN = 3;
const int DY_BROWSE_ENTRY_LEN = 6+16;

const int RES_MAX_FILES      =    3;
const int RES_SCR_MAX_SLOTS  =  255;
const int RES_CHUNK_SIZE     = 2048;

class GResourcesService
{
	public:

		enum SPOT_PP_RESOURCES_SUBCMDS
		{
			SPOT_PP_RM_FILE_TRANSFER_REQUEST = 0x01,
			SPOT_PP_RM_FILE_TRANSFER = 0x02,
			SPOT_PP_RM_FILE_BROWSE = 0x03,
			SPOT_PP_RM_FILE_DELETE = 0x04,
			SPOT_PP_RM_UPLOAD_TRANSFER_REQUEST = 0x05,
			SPOT_PP_RM_UPLOAD_TRANSFER = 0x06,
		};

		typedef enum
		{
			eMessageFile = 0x00,
			eImageFile = 0x01,
			eFontFile = 0x02,
			eLogFile = 0x03,
			eReservedFile = 0x04,
			eScreenFile = 0x05

		} eSPOTFileTypes;

		enum eStatus
		{
			eStatusInitial,
			eStatusWaitingFileDeleteAnswer,
			eStatusWaitingFileUploadAnswer,
			eStatusWaitingFileBlockUploadAnswer,
			eStatusFileDone,
			eStatusError,

			eLastStatus = (eStatusError + 1)
		};

		enum eOperation
		{
			eOperationDeletingDownloadingResources,
			eOperationDeletingResources
		};

		enum eStatusWholeDownload
		{
			eStatusWholeInitial,
			eStatusDownloadCompleted,
			eStatusNoSlotsFree,
			eStatusWholeError
		};

		enum eEvents
		{
			eEventSendFileUpload,
			eEventFileUploadAnswer,
			eEventSendFileBlock,
			eEventFileBlockAnswer,
			eEventSendFileDelete,
			eEventFileDeleteAnswer,
			eEventError,

			eLastEvent = (eEventError + 1)
		};

		struct sDataEvent
		{
			void* data;
			UINT16 size;
		};

		struct sResourceToUpload
		{
			sResourceToUpload() :
				strFullFileName(), eType(eScreenFile), u32FileSizeNoResmark(0),
				strShortName(), pfile(NULL), u8SPOTResourceId(0)
			{
			}

			std::string     strFullFileName;
			eSPOTFileTypes  eType;
			UINT32          u32FileSizeNoResmark;
			std::string     strShortName;
			FILE*           pfile;
			UINT8           u8SPOTResourceId;
		};


		inline static GResourcesService* GetService();
		       static GResourcesService* CreateInstance(GSPOTSession* pSession);
		       static void               DestroyInstance();
		       static bool               isResourcesReply(GSPOTMessage* pMsg,
		                                                  bool blOnlySubCmd = false );

		inline bool isLastUploadProcessOk();
		inline bool isLastUploadProcessFinished();
		       bool SendCmdToSPOT(GSPOTMessage* pMessage);
		       bool ProcessResourcesReply(GSPOTMessage* pMsg);
		       bool ProcessDataEvent(eEvents e, UINT8* pMsgData, UINT16 u16DataSize);
		       bool UploadKeypadResourcesToSPOT();
		       bool DeleteKeypadResourcesToSPOT();
		       bool Initialize();
		inline int  GetResourceId(int i);
		inline int  GetResourceIdForKeyMatrix();
		inline int  GetResourceIdForSoftKeys();
		inline int  GetResourceIdForAuxKeys();
		inline int  GetResourceIdForSecureAudio();


	private:

		GResourcesService(GSPOTSession* pSession);
		virtual ~GResourcesService();

		void FileUpload(std::string strFullFileName, eSPOTFileTypes eType,
		                UINT8 u8SPOTResourceId);

		bool SendFileUploadRequest(eSPOTFileTypes eType, UINT8 u8FileId,
		                           UINT32 u32FileSize, UINT32 u32Crc);
		bool SendBrowseCmd();


		bool SendDeleteFileRequest(eSPOTFileTypes eType, UINT8 u8FileId);
		bool SendBlockUploadRequest(UINT32 u32Offset, UINT8* pBlock,
		                            UINT16 u16BlockSize);

		bool CheckFile(std::string* strFileName, struct stat* fileinfo);
		void NextState(eEvents e, eStatus s);
		void ProcessEventReceived(eEvents e, sDataEvent* pEventData);
		void SendNextBlock();
		void LogAckCode(UINT8 u8AckCode);
		void NextFile();
		bool FindAvailableSlots();
		bool ProcessFileBrowseAnswer(GSPOTMessage* pMsg);
		bool InitializeResourcesFromConfigFile();
		void InitializeResourcesWithFreeSlots();

		// Class constants
		static const UINT           mst_CuFileChunk;   // = RES_CHUNK_SIZE
		static const int            mst_CiResMaxFiles; // = RES_MAX_FILES
		static const int            mst_CiMaxSlots;    // = RES_SCR_MAX_SLOTS

		static const char *         mst_arypcStatusLabel[eLastStatus];
		static const char *         mst_arypcEventLabel[eLastEvent+1];
		static const UINT8          mst_aryu8FileDeleteCmd[DY_FILE_DELETE_CMD_LEN];

		static GResourcesService*   mst_pSoleInstance;
		static GMutex               mst_createInstanceMutex;

		GMutex                      m_StateChangeMutex;
		GSPOTSession*               m_pSession;
		enum eStatus                m_eStatus;
		sResourceToUpload*          m_pActiveFile;
		UINT8                       m_u8SecureAudioResId;

		UINT8                       m_u8Block;
		UINT32                      m_u32FileOffset;
		sResourceToUpload*          m_sResourcesToUploadList[RES_MAX_FILES];
		int                         m_iCurrentFile;
		enum eStatusWholeDownload   m_eStatusWholeDownload;
		enum eOperation             m_eOperation;
		GMutex                      m_Mutex;
		GCondition                  m_Condition;
		std::list<int>              m_SlotsFree;
};

inline GResourcesService* GResourcesService::GetService()
{
	GMutexLock ml(mst_createInstanceMutex);

	return mst_pSoleInstance;
}

inline bool GResourcesService::isLastUploadProcessOk()
{
	return (m_eStatusWholeDownload == eStatusDownloadCompleted);
}

inline bool GResourcesService::isLastUploadProcessFinished()
{
	return (m_eStatusWholeDownload != eStatusWholeInitial);
}

inline int GResourcesService::GetResourceId(int i)
{
	if( m_sResourcesToUploadList[i] != NULL)
		return m_sResourcesToUploadList[i]->u8SPOTResourceId;
	else
		return -1; // Error: Ids for resources haven't been defined.
}

inline int GResourcesService::GetResourceIdForKeyMatrix()
{
	return GetResourceId(0);
}

inline int GResourcesService::GetResourceIdForSoftKeys()
{
	return GetResourceId(1);
}

inline int GResourcesService::GetResourceIdForAuxKeys()
{
	return GetResourceId(2);
}

inline int GResourcesService::GetResourceIdForSecureAudio()
{
	return m_u8SecureAudioResId;
}

#endif /* GRESOURCESSERVICE_H_ */

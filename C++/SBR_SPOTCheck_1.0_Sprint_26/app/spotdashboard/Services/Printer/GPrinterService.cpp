#include <string.h>

#include "GPrinterService.h"

#include "Common/Util/GConversion.h"

using namespace GSpotDefs;

const UINT8  GPrinterService::mst_aryu8PRTStatusCmd[7] = {0x00, 0x07, 0x04, 0x00, 0x10, 0x01, 0x01};


GPrinterService* GPrinterService::mst_pSoleInstance = NULL;
GMutex           GPrinterService::mst_InstanceMutex;


GPrinterService::GPrinterService(GSPOTSession* pSession)
	: m_pSession(pSession)
{
	LOG(ePrinter, eInfo, "Printer Service Started.\n");
}

GPrinterService::~GPrinterService()
{
	LOG(ePrinter, eInfo, "Printer Service Stopped.\n");
}

bool GPrinterService::isPrinterReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if(!pMsg)
	{
		return false; // no message is supplied
	}

	if( ! blOnlySubCmd ) // this flag avoids to analyze the following, if already tested true
	{
		if( pMsg->GetAppId() != eSPOT_OPT_ANSWER )
		{
			return false; // unexpected application ID
		}

		if( pMsg->GetCommandCode() != eSPOT_OPT_PRINTER )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd >= eSPOT_OPT_PRN_TEXTDATA  &&  iSubCmd <= eSPOT_OPT_PRN_JOBCANCEL )
	{
		return true;
	}
	else
	{
		return false; // unexpected message type
	}
}

bool GPrinterService::ProcessOPTSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock ml(m_instanceMutex);

	const char* pcMethod = "GPrinterService::ProcessOPTSystemStatusNotification() - ";
	if(u8Status == eSPOT_PR_ERROR_STATE || u8Error > 0)
	{
		LOG(ePrinter, eError, "%sstatus = %u, ERROR = %u .\n", pcMethod, u8Status, u8Error);
		return false;
	}
	  // not ready but transitory situation
	else if(u8Status == eSPOT_PR_BUSY_STATE || u8Status == eSPOT_PR_STARTUP_STATE)
	{
		const char* pcState = "busy";
		if(u8Status == eSPOT_PR_STARTUP_STATE)
			pcState = "starting up";

		LOG(ePrinter, eNotice, "%sPrinter %s, extended status %u .\n", pcMethod, pcState, u8StatusEx);
		return true;
	}
	else if(u8Status == eSPOT_PR_READY_STATE )
	{
		LOG(ePrinter, eDebug, "%sPrinter is back to READY state now.\n", pcMethod);
	}

	return true;
}

bool GPrinterService::ProcessPrinterReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	int iSubCmd = pMsg->GetSubCommandCode();
	const char* pcMethod = "GPrinterService::ProcessPrinterReply() - ";

	if(iSubCmd >= eSPOT_OPT_PRN_TEXTDATA && iSubCmd <= eSPOT_OPT_PRN_JOBCANCEL && iSubCmd != eSPOT_OPT_PRN_JOBSTATUS)
	{
		UINT16 u16MsgLen = pMsg->GetDataLen();
		if(u16MsgLen < 5)
		{
			LOG(ePrinter, eError, "Message length too short !!!!!\n");
			return false;
		}

		UINT8* pu8Data = pMsg->GetData();
		if(pu8Data == NULL)
		{
			LOG(ePrinter, eError, "Corrupt message with no data !!!!!\n");
			return false;
		}

		UINT8 u8Buffer[40];
		char  acBuffer[128];
		UINT  ulen = pMsg->GetTransmitBuffer(u8Buffer);
		GConversion::BytesToASCIIHexString(acBuffer, u8Buffer, ulen);
		LOG(ePrinter, eDebug, "%s Message Received:\n%s\n", pcMethod, acBuffer);
	}
	else
	{
		LOG(ePrinter, eTrace, "Printer Reply Subcommand %d , message processing not implemented yet.\n", iSubCmd);
	}

	return blret;
}

bool GPrinterService::QueryPrinterStatus()
{
	const char*  pcMethod = "GPrinterService::QueryPrinterStatus() - ";
	LOG(ePrinter, eTrace, "%s%s", pcMethod, "SPOT Printer Query Status message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8PRTStatusCmd, sizeof(mst_aryu8PRTStatusCmd));

	UINT8* pu8PRTStatusCmd = const_cast<UINT8*>(mst_aryu8PRTStatusCmd);

	GSPOTMessage oMsg(pu8PRTStatusCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}


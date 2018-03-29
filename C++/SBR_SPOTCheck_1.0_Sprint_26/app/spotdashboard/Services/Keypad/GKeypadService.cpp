#include <unistd.h>
#include <cstring>

#include "GKeypadService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

#include "SPOT/SPOTProtocolInterface/GAckCodesPinpadApplications.h"

using namespace GSpotDefs;

GKeypadService* GKeypadService::mst_pSoleInstance = NULL;
GMutex GKeypadService::mst_createInstanceMutex;

const UINT8  GKeypadService::mst_aryu8EnableKBCmd [] =
             { 0x00, 0x14, 0x01, 0x00, 0x40, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00 };

const UINT8  GKeypadService::mst_aryu8DisableKBCmd[] =
             { 0x00, 0x06, 0x01, 0x00, 0x40, 0x02 };


GKeypadService::GKeypadService(GSPOTSession* pSession):
                m_pSession(pSession)
{
	LOG(eKeypad, eInfo, "Keypad Service started.\n");
}

GKeypadService::~GKeypadService()
{
	LOG(eKeypad, eInfo, "Keypad Service stopped.\n");
}

bool GKeypadService::isKeypadReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if (!pMsg)
	{
		return false; // no message is supplied
	}

	if (!blOnlySubCmd) // this flag avoids to analyze the following, if already tested true
	{
		if (pMsg->GetAppId() != eSPOT_PP_ANSWER)
			return false; // unexpected application ID

		if (pMsg->GetCommandCode() != eSPOT_PP_KEYBOARD)
			return false; // unexpected command category
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if (iSubCmd == eSPOT_PP_KB_ENABLE || iSubCmd == eSPOT_PP_KB_DISABLE)
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

bool GKeypadService::ProcessKeypadReply(GSPOTMessage* pMsg)
{
	UINT8	u8Ack = ( pMsg->GetData() == NULL ? 0x80 : *(pMsg->GetData()) );
	char	arycError[256];

	memset(arycError, 0x00, sizeof(arycError));
	snprintf(arycError, (sizeof(arycError)-1), "Keypad reply: [%s].", GAckCodesPinpadApplications::GetOrCreateInstance()->GetDescription(u8Ack));
	LOG(eKeypad, (u8Ack == 0)?eDebug:eError, "Keypad reply %s .\n", arycError);

	m_oRulesProcessor.ProcessServiceNotification(u8Ack);

	return true;
}

bool GKeypadService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock ml(m_instanceMutex);

	const char* pcMethod = "GKeypadService::ProcessSystemStatusNotification() -- ";
	LOG(eKeypad, eDebug, "%s st %02X, stex %02X, error %02X \n", pcMethod,
	    int(u8Status), int(u8StatusEx), int(u8Error));

	if( u8Status == 0x00 )
	{
		LOG(eKeypad, eNotice, "Keypad device reported DISABLED.\n");
	}

	else if( u8Status == 0x01 )
	{
		LOG(eKeypad, eNotice, "Keypad device reported ENABLED.\n");
	}

	else if( u8Status == 0x02 )
	{
		LOG(eKeypad, eWarning, "Keypad device reported BUSY.\n");
	}

	else if( u8Status == 0x03 )
	{
		LOG(eKeypad, eError, "Keypad reported recoverable READER ERROR state, error 0x%02X.\n", u8Error);
	}

	else if( u8Status == 0x04 )
	{
		LOG(eKeypad, eError, "Keypad reported INOPERATIVE FATAL ERROR state, error 0x%02X.\n", u8Error);
	}

	else // other not common errors
	{
		LOG(eKeypad, eError,
		    "Keypad reported state 0x%02X and error 0x%02X.\n",
		    u8Status, u8Error);
	}


	m_oRulesProcessor.ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);

	return true;
}

bool GKeypadService::SendEnableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GKeypadService::SendEnableCmdToSPOT() - ";
	LOG(eKeypad, eTrace, "%s%s", pcMethod, "SPOT Keypad Enable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8EnableKBCmd, sizeof(mst_aryu8EnableKBCmd));

	UINT8* pu8EnableKBCmd = const_cast<UINT8*>(mst_aryu8EnableKBCmd);
	GSPOTMessage oMsg(pu8EnableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GKeypadService::SendDisableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GKeypadService::SendDisableCmdToSPOT() - ";
	LOG(eKeypad, eTrace, "%s%s", pcMethod, "SPOT Keypad Disable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8DisableKBCmd, sizeof(mst_aryu8DisableKBCmd));

	UINT8* pu8DisableKBCmd = const_cast<UINT8*>(mst_aryu8DisableKBCmd);
	GSPOTMessage oMsg(pu8DisableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

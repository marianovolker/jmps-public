#include <unistd.h>
#include <cstring>

#include "GCardReaderService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

#include "SPOT/SPOTProtocolInterface/GAckCodesPinpadApplications.h"

using namespace GSpotDefs;

GCardReaderService* GCardReaderService::mst_pSoleInstance = NULL;
GMutex GCardReaderService::m_createInstanceMutex;

const UINT8  GCardReaderService::mst_aryu8EnableCRCmd			[]	= {0x00, 0x06, 0x01, 0x00, 0x20, 0x01};
const UINT8  GCardReaderService::mst_aryu8DisableCRCmd			[]	= {0x00, 0x06, 0x01, 0x00, 0x20, 0x02};


//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################
bool GCardReaderService::isCardReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if(!pMsg)
	{
		return false; // no message is supplied
	}

	if( ! blOnlySubCmd ) // this flag avoids to analyze the following, if already tested true
	{
		if( pMsg->GetAppId() != eSPOT_PP_ANSWER )
		{
			return false; // unexpected application ID
		}

		if( pMsg->GetCommandCode() != eSPOT_PP_CARDREADER )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd == eSPOT_PP_CR_ENABLE  ||  iSubCmd == eSPOT_PP_CR_DISABLE || iSubCmd == eSPOT_PP_CR_READ )
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

bool GCardReaderService::ProcessCardReaderReply(GSPOTMessage* pMsg)
{
	UINT8	u8Ack = ( pMsg->GetData() == NULL ? 0xFF : *(pMsg->GetData()) );
	char	arycError[256];

	memset(arycError, 0x00, sizeof(arycError));
	snprintf(arycError, sizeof(arycError) - 1,
	         "Card Reader reply: [%02X] - [%s].", u8Ack,
	         GAckCodesPinpadApplications::GetOrCreateInstance()->GetDescription(u8Ack));

	LOG(eCardReader, (u8Ack == 0)? eDebug : eError, "%s .\n", arycError);

	m_oRulesProcessor.ProcessServiceNotification(u8Ack);

	return true;
}

bool GCardReaderService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock ml(m_instanceMutex);

	LOG(eCardReader, eDebug, "[st %02X, stex %02X, error %02X]\n", int(u8Status), int(u8StatusEx), int(u8Error) );

	if(u8Status == 0x00)
	{
		LOG( eCardReader, eNotice, "Card Reader device reported DISABLED.\n" );
	}

	else if(u8Status == 0x01)
	{
		LOG( eCardReader, eNotice, "Card Reader device reported ENABLED.\n" );
	}

	else if(u8Status == 0x02)
	{
		LOG(eCardReader, eWarning, "Card reader reported a busy state.\n");
	}

	else if(u8Status == 0x03)
	{
		LOG(eCardReader, eError, "Card reader reported recoverable READER ERROR state, error 0x%02X.\n", u8Error);
	}

	else if(u8Status == 0x04)
	{
		LOG(eCardReader, eError, "Card reader reported INOPERATIVE FATAL ERROR state, error 0x%02X.\n", u8Error);
	}

	else // other not common errors
	{
		LOG(eCardReader, eError,
		    "Card reader reported state 0x%02X and error 0x%02X.\n",
		    u8Status, u8Error);
	}

	m_oRulesProcessor.ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);

	return true;
}


bool GCardReaderService::SendEnableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GCardReaderService::SendEnableCmdToSPOT() - ";
	LOG(eCardReader, eTrace, "%s%s", pcMethod, "SPOT card reader Enable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8EnableCRCmd, sizeof(mst_aryu8EnableCRCmd));

	UINT8* pu8EnableCRCmd = const_cast<UINT8*>(mst_aryu8EnableCRCmd);
	GSPOTMessage oMsg(pu8EnableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GCardReaderService::SendDisableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GCardReaderService::SendDisableCmdToSPOT() - ";
	LOG(eCardReader, eTrace, "%s%s", pcMethod, "SPOT card reader Disable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8DisableCRCmd, sizeof(mst_aryu8DisableCRCmd));

	UINT8* pu8DisableCRCmd = const_cast<UINT8*>(mst_aryu8DisableCRCmd);
	GSPOTMessage oMsg(pu8DisableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}


//##########################################################################################
//##########################################################################################
// Private methods
//##########################################################################################
//##########################################################################################

GCardReaderService::GCardReaderService(GSPOTSession* pSession)
	: m_pSession(pSession)
{
	LOG(eCardReader, eInfo, "Card Reader Service started.\n");
}

GCardReaderService::~GCardReaderService()
{
	LOG(eCardReader, eInfo, "Card Reader Service stopped.\n");
}


#include <unistd.h>
#include <cstring>

#include "GContactlessReaderService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

#include <GSystem.h>

using namespace GSpotDefs;

GContactlessReaderService* GContactlessReaderService::mst_pSoleInstance = NULL;
GMutex GContactlessReaderService::m_createInstanceMutex;

const UINT8  GContactlessReaderService::mst_aryu8EnableCLRCmd	[8]	= {0x00, 0x08, 0x04, 0x00, 0x50, 0x01, 0x00, 0x00};
const UINT8  GContactlessReaderService::mst_aryu8DisableCLRCmd	[6]	= {0x00, 0x06, 0x04, 0x00, 0x50, 0x05};


//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################
bool GContactlessReaderService::isContactlessReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
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

		if( pMsg->GetCommandCode() != eSPOT_OPT_CLESS )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd == eSPOT_OPT_CLR_ENABLE  ||  iSubCmd == eSPOT_OPT_CLR_DISABLE || iSubCmd == eSPOT_OPT_CLR_CARD_DATA )
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

bool GContactlessReaderService::ProcessContactlessReaderReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	UINT8*      pu8MsgData = NULL;
	UINT16      u16DataLen = 0;
	int         iCmdCode = pMsg->GetSubCommandCode();

	switch( iCmdCode )
	{
		case eSPOT_OPT_CLR_ENABLE:
			LOG( eContactlessReader, eDebug, "[Contactless Reader device reported ENABLED]\n" );
			blret = true;
		break;

		case eSPOT_OPT_CLR_DISABLE:
			LOG( eContactlessReader, eDebug, "[Contactless Reader device reported DISABLED]\n" );
			blret = true;
		break;

		case eSPOT_OPT_CLR_CARD_DATA:
			u16DataLen = pMsg->GetDataLen();
			if(u16DataLen == 0)
			{
				break;
			}
			pu8MsgData = new UINT8[u16DataLen + 4];
			if(pu8MsgData == NULL)
			{
				break;
			}
			if( ! pMsg->GetData(pu8MsgData, u16DataLen) )
			{
				delete[] pu8MsgData;
				break;
			}
			delete[] pu8MsgData;
		break;

		default:
		break;
	}

	return blret;
}

bool GContactlessReaderService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock ml(m_instanceMutex);

	LOG(eContactlessReader, eDebug, "[st %02X, stex %02X, error %02X]\n", int(u8Status), int(u8StatusEx), int(u8Error) );

	if( u8Status == 0x04 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		LOG( eContactlessReader, eDebug, "[Contactless Reader device reported ENABLED]\n" );

		return true;
	}

	if( u8Status == 0x03 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		LOG( eContactlessReader, eDebug, "[Contactless Reader device reported DISABLED]\n" );

		return true;
	}

	if( u8Error != 0x00 )
	{
		LOG( eContactlessReader, eDebug, "[Contactless Reader device reported an ERROR [0x%x]]\n", u8Error );

		return true;
	}

	return false;
}

bool GContactlessReaderService::SendEnableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GContactlessReaderService::SendEnableCmdToSPOT() - ";
	LOG(eContactlessReader, eTrace, "%s%s", pcMethod, "SPOT ContactlessReader Enable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8EnableCLRCmd, sizeof(mst_aryu8EnableCLRCmd));

	UINT8* pu8EnableCRCmd = const_cast<UINT8*>(mst_aryu8EnableCLRCmd);
	GSPOTMessage oMsg(pu8EnableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GContactlessReaderService::SendDisableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GContactlessReaderService::SendDisableCmdToSPOT() - ";
	LOG(eContactlessReader, eTrace, "%s%s", pcMethod, "SPOT ContactlessReader Disable message has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8DisableCLRCmd, sizeof(mst_aryu8DisableCLRCmd));

	UINT8* pu8DisableCRCmd = const_cast<UINT8*>(mst_aryu8DisableCLRCmd);
	GSPOTMessage oMsg(pu8DisableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}





//##########################################################################################
//##########################################################################################
// Private methods
//##########################################################################################
//##########################################################################################

GContactlessReaderService::GContactlessReaderService(GSPOTSession* pSession)
	: m_pSession(pSession)
{
	LOG(eContactlessReader, eInfo, "Contactless Reader Service started.\n");
}

GContactlessReaderService::~GContactlessReaderService()
{
	LOG(eContactlessReader, eInfo, "Contactless Reader Service stopped.\n");
}


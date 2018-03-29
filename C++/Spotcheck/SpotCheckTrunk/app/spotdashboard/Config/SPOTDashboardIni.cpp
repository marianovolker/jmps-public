#include "SPOTDashboardIni.h"

#include "Common/Logger/ConfigLabels.h"
#include "Common/Util/GIniFileHandler.h"
#include "Common/Util/GString.h"
#include "GSystem.h"
#include <GMutex.h>
#include "GSocket.h"

#include "SPOT/SPOTProtocolInterface/GAckCodesPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysMainPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysSecurityModulePinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysCardReaderPinpadApplications.h"
#include "SPOT/SPOTProtocolInterface/GSpotSysKeyboardPinpadApplications.h"

#include <iostream>


GMutex SPOTDashboardIni::mst_Mutex;
SPOTDashboardIni* SPOTDashboardIni::mst_pInstance = NULL;

const int SPOTDashboardIni::INI_BUFFER_SIZE_KEYVALUE = 192;
const char * SPOTDashboardIni::INI_DASHBOARD_CONFIG_DEFAULT_FILE = "./SPOTDashboard.ini";


SPOTDashboardIni::~SPOTDashboardIni()
{
}

SPOTDashboardIni::SPOTDashboardIni()
{
}

bool SPOTDashboardIni::Initialize()
{
	GIniFileHandler oIni(INI_DASHBOARD_CONFIG_DEFAULT_FILE);
	char arycBuffer[INI_BUFFER_SIZE_KEYVALUE];
	bool bAskIP(false);

	m_strIPAddress = DEFAULT_SPOT_IP;

	if (!oIni.IsReadable())
	{
		// If it can't read the .ini then applies defaults.
		GSystem::Perror( "ERROR in GGlobalSystemInformation: Initialization file [%s] not found. Using defaults.\n", INI_DASHBOARD_CONFIG_DEFAULT_FILE );
		bAskIP = true;
	}
	else
	{
		oIni.GetPrivateProfileString( GENERAL_SECTION_NAME, ASK_SPOT_IP_KEY_NAME, "no", arycBuffer, sizeof(arycBuffer) );

		GString strAskIP(arycBuffer);
		strAskIP.ToLower();
		bAskIP = ( (strAskIP=="yes")?(true):(false) );

		if( !bAskIP )
		{
			oIni.GetPrivateProfileString( GENERAL_SECTION_NAME, SPOT_IP_KEY_NAME, DEFAULT_SPOT_IP, arycBuffer, sizeof(arycBuffer) );
			std::string strIpOnFile(arycBuffer);
			if ( GSocket::ValidateIpAddress(strIpOnFile.c_str()) )
			{
				m_strIPAddress = strIpOnFile;
			}
			else
			{
				GSystem::Perror( "ERROR in GGlobalSystemInformation: SPOT ip is not well formated. using default ip %s.\n", DEFAULT_SPOT_IP );
			}
		}
	}

	LoadPropietaryCodes("Pinpad.AckCodes");

	LoadPropietaryCodes("Pinpad.Main.States");
	LoadPropietaryCodes("Pinpad.Main.StatesEx");
	LoadPropietaryCodes("Pinpad.Main.Errors");

	LoadPropietaryCodes("Pinpad.Security.States");
	LoadPropietaryCodes("Pinpad.Security.StatesEx");
	LoadPropietaryCodes("Pinpad.Security.Errors");

	LoadPropietaryCodes("Pinpad.Keypad.States");
	LoadPropietaryCodes("Pinpad.Keypad.StatesEx");
	LoadPropietaryCodes("Pinpad.Keypad.Errors");

	LoadPropietaryCodes("Pinpad.CardReader.States");
	LoadPropietaryCodes("Pinpad.CardReader.StatesEx");
	LoadPropietaryCodes("Pinpad.CardReader.Errors");

	return AskIP(bAskIP);
}

bool SPOTDashboardIni::AskIP(bool bAskIP)
{
	if( bAskIP )
	{
		short nRetries(3);
		bool bIsValidIP(false);

		while( (--nRetries) >= 0 )
		{
			std::cout << "Please, insert the SPOT IP Address: ";
			std::cin  >> m_strIPAddress;

			if ( GSocket::ValidateIpAddress(m_strIPAddress.c_str()) )
			{
				bIsValidIP = true;
				break;
			}
		}

		if ( !bIsValidIP )
		{
			GSystem::Perror( "ERROR in GGlobalSystemInformation: SPOT ip is not well formated. Exiting application...\n" );
			return false;
		}
	}

	return true;
}

bool SPOTDashboardIni::LoadPropietaryCodes(const char * czIniSection)
{
	GIniFileHandler oIni(INI_DASHBOARD_CONFIG_DEFAULT_FILE);
	char arycBuffer[INI_BUFFER_SIZE_KEYVALUE];
	char arycBufferKey[INI_BUFFER_SIZE_KEYVALUE];

	std::string sKey;
	unsigned int iKey = 0;

	if( oIni.IsReadable() )
	{
		int iLen = oIni.GetPrivateProfileString( czIniSection, NULL, "", arycBuffer, sizeof(arycBuffer) );
		for( int i=0; i<iLen; i++ )
		{
			if( arycBuffer[i] != 0 )
			{
				sKey += arycBuffer[i];
			}
			else
			{
				if( sscanf(sKey.c_str(), "0x%02x", &iKey) )
				{
					oIni.GetPrivateProfileString( czIniSection, sKey.c_str(), "", arycBufferKey, sizeof(arycBufferKey) );

					if( !SetDescriptionToAppropriateInternalMap(czIniSection, iKey, (const char*)arycBufferKey) )
					{
						GSystem::Perror( "Warning, the key [0x%02x] was not found in the %s internal map.", iKey, czIniSection );
					}
				}
				else
				{
					GSystem::Perror( "Warning, Discarding the key [%s] because it's bad formatted, loading %s internal map.", sKey.c_str(), czIniSection );
				}

				sKey.clear();
			}
		}
	}
	return true;
}

bool SPOTDashboardIni::SetDescriptionToAppropriateInternalMap(const char * czIniSection, UINT8 uiKey, const char * czNewDescription)
{
	GString sIniSection(czIniSection);
	sIniSection.ToLower();
	bool bSetStatesDescription(false);

	if(sIniSection=="pinpad.ackcodes")
	{
		bSetStatesDescription=GAckCodesPinpadApplications::GetOrCreateInstance()->SetDescription(uiKey, czNewDescription);
	}

	else if(sIniSection=="pinpad.main.states")
	{
		bSetStatesDescription=GSpotSysMainPinpadApplications::GetOrCreateInstance()->SetStatesDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.main.statesex")
	{
		bSetStatesDescription=GSpotSysMainPinpadApplications::GetOrCreateInstance()->SetStatesExDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.main.errors")
	{
		bSetStatesDescription=GSpotSysMainPinpadApplications::GetOrCreateInstance()->SetErrorsDescription(uiKey, czNewDescription);
	}

	else if(sIniSection=="pinpad.security.states")
	{
		bSetStatesDescription=GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->SetStatesDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.security.statesex")
	{
		bSetStatesDescription=GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->SetStatesExDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.security.errors")
	{
		bSetStatesDescription=GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->SetErrorsDescription(uiKey, czNewDescription);
	}


	else if(sIniSection=="pinpad.keypad.states")
	{
		bSetStatesDescription=GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->SetStatesDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.keypad.statesex")
	{
		bSetStatesDescription=GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->SetStatesExDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.keypad.errors")
	{
		bSetStatesDescription=GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->SetErrorsDescription(uiKey, czNewDescription);
	}


	else if(sIniSection=="pinpad.cardreader.states")
	{
		bSetStatesDescription=GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->SetStatesDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.cardreader.statesex")
	{
		bSetStatesDescription=GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->SetStatesExDescription(uiKey, czNewDescription);
	}
	else if(sIniSection=="pinpad.cardreader.errors")
	{
		bSetStatesDescription=GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->SetErrorsDescription(uiKey, czNewDescription);
	}


	return bSetStatesDescription;
}

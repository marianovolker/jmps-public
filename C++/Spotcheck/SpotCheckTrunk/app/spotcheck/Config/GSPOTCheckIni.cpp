#include "GSPOTCheckIni.h"

#include "Common/Util/GIniFileHandler.h"
#include "Common/Util/GString.h"
#include "GSystem.h"
#include <GMutex.h>
#include "GSocket.h"

#include <iostream>

GMutex GSPOTCheckIni::mst_Mutex;
GSPOTCheckIni* GSPOTCheckIni::mst_pInstance = NULL;

const int GSPOTCheckIni::INI_BUFFER_SIZE_KEYVALUE = 192;

GSPOTCheckIni::GSPOTCheckIni()
{
	m_u8KeypadResId = m_u8SoftkeyResId = m_u8AuxkeyResId = 0xFF;
	m_iRunningMode = eDummyEmbedded;
}

GSPOTCheckIni::~GSPOTCheckIni()
{
}

bool GSPOTCheckIni::Initialize()
{
	GIniFileHandler oIni(INI_SPOTCHECK_CONFIG_DEFAULT_FILE);
	char arycBuffer[INI_BUFFER_SIZE_KEYVALUE];
	bool blAskIP(false);


	if(IS_EXTERNAL_APP)
	{
		m_iRunningMode = eExternal;
	}
/*	else
	{
		m_iRunningMode = eEmbedded;
	}
*/
	m_strIPAddress = DEFAULT_SPOT_IP;

	if (!oIni.IsReadable())
	{
		// If it can't read the .ini then applies defaults.
		GSystem::Perror( "ERROR in GSPOTCheckIni: Initialization file [%s] not found. Using defaults.\n", INI_SPOTCHECK_CONFIG_DEFAULT_FILE );
		//if(blEmbedded)
		if(m_iRunningMode != eExternal)
			return false; // Fatal error situation for embedded SPC.

		blAskIP = true;
	}
	else // SPOTCheck.ini can be read
	{
		oIni.GetPrivateProfileString( GENERAL_SECTION_NAME, ASK_SPOT_IP_KEY_NAME,
				                              "no", arycBuffer, sizeof(arycBuffer) );

		GString strAskIP(arycBuffer);
		strAskIP.ToLower();

		if(m_iRunningMode!= eExternal)
		{
			m_iRunningMode = oIni.GetPrivateProfileInt(GENERAL_SECTION_NAME, RUNNING_MODE, eDummyEmbedded);
		}

		m_iRomfsNumber = oIni.GetPrivateProfileInt(GENERAL_SECTION_NAME, ROMFS_NUMBER, 0);

		if((m_iRunningMode == eInjected || m_iRunningMode ==eDummyInjected) && ((m_iRomfsNumber <5 )||(m_iRomfsNumber >8 ))) //available ROMfs slots for SPOTCheck
		{
			GSystem::Perror( "ERROR in GSPOTCheckIni: RomfsNumber %d is not a valid Romfs number for SPOTCheck", m_iRomfsNumber);
			return false;

		}

		
		//if(blEmbedded) //embedded
		if(m_iRunningMode == eEmbedded || m_iRunningMode == eDummyEmbedded)
		{
			int iResId = 0;
			const char* pcIniError = "ERROR: Section [%s] or Key \"%s\" haven't been found in SPOTCheck.ini; %s test shall fail!!!";

			iResId = oIni.GetPrivateProfileInt( RESOURCES_SECTION_NAME, M3KEYMATRIX_SCR_KEY_NAME, -1 );
			if(iResId == -1)
				GSystem::Perror(pcIniError, RESOURCES_SECTION_NAME, M3KEYMATRIX_SCR_KEY_NAME, "Keypad");
			else
				m_u8KeypadResId = UINT8( UINT(iResId) );

			iResId = oIni.GetPrivateProfileInt( RESOURCES_SECTION_NAME, SOFTKEYMATRIX_SCR_KEY_NAME, -1 );
			if(iResId == -1)
				GSystem::Perror(pcIniError, RESOURCES_SECTION_NAME, SOFTKEYMATRIX_SCR_KEY_NAME, "Keypad");
			else
				m_u8SoftkeyResId = UINT8( UINT(iResId) );

			iResId = oIni.GetPrivateProfileInt( RESOURCES_SECTION_NAME, AUXKEYMATRIX_SCR_KEY_NAME, -1 );
			if(iResId == -1)
				GSystem::Perror(pcIniError, RESOURCES_SECTION_NAME, AUXKEYMATRIX_SCR_KEY_NAME, "Keypad");
			else
				m_u8AuxkeyResId = UINT8( UINT(iResId) );

			// Do not ask or read ip in embedded mode.
			// blAskIP = false ; // Uncomment this for Linux "embedded" simulation.

			// Delete the following two lines for external connection under simulation.
			m_strIPAddress = LOOPBACK_SPOT_IP;
			return true;
		}
		else if( m_iRunningMode == eInjected )
		{
			m_strIPAddress = LOOPBACK_SPOT_IP;
			return true;

		}
		else// not embedded or injected
		{
			blAskIP = ( strAskIP == "yes" ? true : false );
		}

		if( blAskIP == false )
		{
			oIni.GetPrivateProfileString( GENERAL_SECTION_NAME, SPOT_IP_KEY_NAME, DEFAULT_SPOT_IP, arycBuffer, sizeof(arycBuffer) );
			std::string strIpOnFile(arycBuffer);
			if ( GSocket::ValidateIpAddress(strIpOnFile.c_str()) )
			{
				m_strIPAddress = strIpOnFile;
				return true;
			}
			else
			{
				GSystem::Perror( "ERROR in GSPOTCheckIni: SPOT ip is not well formated. using default ip %s.\n", DEFAULT_SPOT_IP );
				return false;
			}
		}
	}

	return AskIP(blAskIP);
}

bool GSPOTCheckIni::ReadSecureAudioSCRId()
{
	int iResId = 0;
	const char* pcIniError = "ERROR: Section [%s] or Key \"%s\" haven't been found in SPOTCheck.ini; %s test shall fail!!!";
	GIniFileHandler oIni(INI_SPOTCHECK_CONFIG_DEFAULT_FILE);
	iResId = oIni.GetPrivateProfileInt( RESOURCES_SECTION_NAME, SECUREAUDIO_SCR_KEY_NAME, -1 );
		if(iResId == -1)
			GSystem::Perror(pcIniError, RESOURCES_SECTION_NAME, SECUREAUDIO_SCR_KEY_NAME, "SecureAudio");
		else
			m_u8SecureAudioResId = UINT8( UINT(iResId) );

	return true;
}

bool GSPOTCheckIni::AskIP(bool bAskIP)
{
	if( bAskIP )
	{
		short nRetries(3);
		bool bIsValidIP(false);

		while( (--nRetries) >= 0 )
		{
			std::cout << "Please, type the SPOT IP Address: ";
			std::cin  >> m_strIPAddress;

			if ( GSocket::ValidateIpAddress(m_strIPAddress.c_str()) )
			{
				bIsValidIP = true;
				break;
			}
		}

		if ( !bIsValidIP )
		{
			GSystem::Perror( "ERROR in GSPOTCheckIni: SPOT ip is not well formated. Exiting application...\n" );
			return false;
		}
	}

	return true;
}

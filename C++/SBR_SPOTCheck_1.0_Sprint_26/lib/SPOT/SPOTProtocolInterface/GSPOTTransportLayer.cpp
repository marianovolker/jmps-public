///////////////////////////////////////////////////////////
//  GSPOTTransportLayer.cpp
//  Implementation of the Class GSPOTTransportLayer
//  Created on:      08-Mar-2007 10:51:18 AM
///////////////////////////////////////////////////////////
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"
#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "GSystem.h"

GSPOTTransportLayer* GSPOTTransportLayer::mst_pInstance = NULL;
GMutex GSPOTTransportLayer::mst_InstanceMutex;

GSPOTTransportLayer::GSPOTTransportLayer(const char* ip_addr):
				m_pSocket(NULL), m_ui16Port(SPOT_SOCKET_PORT), m_pSession(NULL),
				m_bThreadRunning(false), m_strIPAddr(ip_addr)
{
}

GSPOTTransportLayer::~GSPOTTransportLayer()
{
	CloseConnection();
}

void GSPOTTransportLayer::CreateInstance(const char* ip_addr)
{
	GMutexLock ml(mst_InstanceMutex);
	if (mst_pInstance == NULL)
	{
		mst_pInstance = new GSPOTTransportLayer(ip_addr);
	}
}

void GSPOTTransportLayer::DestroySoleInstance()
{
	GMutexLock ml(mst_InstanceMutex);
	if (mst_pInstance)
	{
		delete mst_pInstance;
		mst_pInstance = NULL;
	}
}

GSPOTTransportLayer* GSPOTTransportLayer::GetSoleInstance()
{
	GMutexLock ml(mst_InstanceMutex);
	return mst_pInstance;
}

void GSPOTTransportLayer::SetSession(GSPOTSession* pSession)
{
	if (!m_pSession)
	{
	    m_pSession = pSession;
	}
}

bool GSPOTTransportLayer::OpenConnection()
{
	GMutexLock ml(m_Mutex);

	bool bConnected = false;
	UINT8 u8attempts = 0;

	if (m_pSocket != NULL)
	{
		LOG(eSpotProtocol, eFatal, "GSPOTTransportLayer::OpenConnection - Invalid Socket state\n");
		return false;
	}

	m_pSocket = new GSocket(eTCP);

	// try at most 3 times to connect to SPOT
	while ((!bConnected) && (u8attempts < 3))
	{
		// Trace connecting activity no matter console logging level.
		printf("Info: Connecting to [%s:%d]\n", m_strIPAddr.c_str(), m_ui16Port);

		bConnected = m_pSocket->Connect(m_strIPAddr.c_str(), m_ui16Port);
		u8attempts++;
	}

	if (!bConnected)
	{
		LOG(eSpotProtocol, eFatal, "GSPOTTransportLayer::OpenConnection - Could not connect.\n");
		if (m_pSocket)
		{
			m_pSocket->Close();
			delete m_pSocket;
			m_pSocket = NULL;
		}
	}
	return bConnected;
}

void GSPOTTransportLayer::CloseConnection()
{
	GMutexLock ml(m_Mutex);

	if (m_pSocket == NULL)
	{
		return;
	}

	m_pSocket->Close();
	delete m_pSocket;
	m_pSocket = NULL;
}

void GSPOTTransportLayer::Start()
{
	GMutexLock ml(m_Mutex);
	m_bThreadRunning = true;
	GTask::Start(eJoinable);
}

void GSPOTTransportLayer::Stop()
{
	GMutexLock ml(m_Mutex);
	m_bThreadRunning = false;
}

bool GSPOTTransportLayer::IsRunning()
{
	GMutexLock ml(m_Mutex);
	return m_bThreadRunning;
}

bool GSPOTTransportLayer::IsRunningAndConnected()
{
	GMutexLock ml(m_Mutex);
	return m_bThreadRunning && m_pSocket->IsConnected();
}

bool GSPOTTransportLayer::SendMessage(GSPOTMessage* pMsg)
{
	GMutexLock ml(m_Mutex);

	if (!m_pSocket)
	{
	    return false;
	}

	if (!pMsg)
	{
	    return false;
	}

	UINT8* pMsgBuff = new UINT8[pMsg->GetMessageLen()];

	if (pMsg->GetTransmitBuffer(pMsgBuff) > 0)
	{
		UINT16 BytesSent = m_pSocket->Write(reinterpret_cast<const char*>(pMsgBuff), pMsg->GetMessageLen(), SPOT_TXMSG_TIMEOUT);
		if (BytesSent != pMsg->GetMessageLen())
		{
			LOG(eSpotProtocol, eError, "GSPOTTransportLayer::SendMessage - Did not send expected number of bytes.\n");
			delete[] pMsgBuff;
			return false;
		}
	}

	delete[] pMsgBuff;
	return true;
}

void GSPOTTransportLayer::NotifyMessage(GSPOTMessage* pMsg)
{
	if (m_pSession)
	{
		m_pSession->NotifyMessage(pMsg);
	}
}

void GSPOTTransportLayer::NotifyError(eSPOTTransportLayerError eError)
{
	if (m_pSession)
	{
	    m_pSession->NotifyError(eError);
	}
}

void GSPOTTransportLayer::Run()
{
	eSPOT_MSG_RESULT eResult(eSPOT_MESSAGE_ERROR);
	UINT16 u16BytesRead(0);
	UINT8 pRawBuffer[SPOT_MAXMSGLEN];

	m_Mutex.Lock();
	if (!m_pSocket)
	{
		m_Mutex.Unlock();
		LOG(eSpotProtocol, eFatal, "GSPOTTransportLayer::Run - invalid socket state.");
		return;
	}
	m_Mutex.Unlock();

	LOG(eSpotProtocol, eNotice, "GSPOTTransportLayer::Run - SPOT reader Thread started!\n");

	while (IsRunningAndConnected())
	{
		eResult = ReadMessage(pRawBuffer, u16BytesRead, SPOT_RXMSG_TIMEOUT);

		if (eResult == eSPOT_DATA_RECEIVED)
		{
			// The purpose of the next line of code is for printing and debugging and just to be used at debugging.
			GConversion::LogBytesAsASCIIHexString((const UINT8*) pRawBuffer, u16BytesRead);

			GSPOTMessage* pMessage = new GSPOTMessage(pRawBuffer);

			if (pMessage)
			{
				//message sent to upper layer
				NotifyMessage(pMessage);
			}
			else
			{
				LOG(eSpotProtocol, eError, "GSPOTTransportLayer::Run - GSPOTMessage not created successfully\n");
				GSystem::Sleep(WAIT_ERROR_GUARD);
			}

			delete pMessage;
		}
		else if (eResult == eSPOT_NO_DATA_RECEIVED)
		{
			GSystem::Sleep(WAIT_ERROR_GUARD);
		}
		else if (eResult == eSPOT_MESSAGE_ERROR)
		{
			LOG(eSpotProtocol, eError, "GSPOTTransportLayer::Run - eSPOT_MESSAGE_ERROR\n");
			NotifyError(eIncomingMessageError);
		}
		else
		{
			LOG(eSpotProtocol, eFatal, "GSPOTTransportLayer::Run - Connection Error\n");
			NotifyError(eConnectionError);
			//after notifying upper level kill the thread
			break;
		}
	}

	//just in case socket has been closed
	m_Mutex.Lock();
	m_bThreadRunning = false;
	m_Mutex.Unlock();

	LOG(eSpotProtocol, eNotice, "GSPOTTransportLayer::Run - Finished.\n");
}

eSPOT_MSG_RESULT GSPOTTransportLayer::ReadMessage(UINT8* pui8Buffer, UINT16& ui16BytesRead, UINT32 ui8Timeout)
{
	const unsigned int CHeaderLength(sizeof(UINT16));
	UINT16 ui16Length(0);
	UINT16 ui16Offset(0);
	const int iTotalRetries(4);
	int iRetries(iTotalRetries);

	if (!IsRunning())
	{
		return eSPOT_NO_DATA_RECEIVED;
	}

	if (!IsRunningAndConnected())
	{
		return eSPOT_CONNECTION_ERROR;
	}

	ui16BytesRead = 0;
	// loop until data is available for reading
	do
	{
		// obtain the whole message length
		// if message length field hasn't been read yet, only the 2 first bytes must be read
		// else the whole message is read
		UINT16 ui16Len = ui16Offset < CHeaderLength ? CHeaderLength : ui16Length;

		m_Mutex.Lock();
		long lRet = 0L;
		if (m_pSocket)
		{
			lRet = m_pSocket->Read( (((char*) (pui8Buffer)) + ui16Offset), (ui16Len - ui16Offset), ui8Timeout );
		}
		m_Mutex.Unlock();

		if (lRet == eSocketClosed || lRet == eSocketError)
		{
			if (!IsRunning())
			{
				return eSPOT_NO_DATA_RECEIVED;
			}

			LOG(eSpotProtocol, eError, "GSPOTTransportLayer::ReadMessage - Socket error\n");

			return eSPOT_CONNECTION_ERROR;
		}
		else if (lRet == eSocketTimeout)
		{
			// check if received bytes are all of the length
			if (ui16Offset != ui16Length)
			{
				LOG(eSpotProtocol, eError, "GSPOTTransportLayer::ReadMessage - bad received SPOT message length TIMEOUT NOT READ ALL MESSAGE\n");

				if( iRetries > 0 )
				{
					iRetries--;
					LOG(eSpotProtocol, eWarning, "GSPOTTransportLayer::ReadMessage - Applying a workaround at retry [%d].\n", (iTotalRetries-iRetries));
					LOG(eSpotProtocol, eWarning, "GSPOTTransportLayer::ReadMessage - [length = %d], [ui16Offset = %d].\n", ui16Length, ui16Offset);
					GSystem::Sleep(800);
					continue;
				}
			}

			return eSPOT_NO_DATA_RECEIVED;
		}

		// offset is updated
		ui16Offset += lRet;

		// first time, only the first two bytes are read
		if (ui16Offset == CHeaderLength)
		{
			// field 'length' of the SPOT message has just been read: set length accordingly
			ui16Length = UINT16(pui8Buffer[0] << 8) | UINT16(pui8Buffer[1]);
			// check the appropriate message length for the read value from the buffer
			if (ui16Length < TPDU_HEADER_LENGTH || ui16Length > SPOT_MAXMSGLEN)
			{

				LOG(eSpotProtocol, eError, "GSPOTTransportLayer::ReadMessage - bad received SPOT message length, ui16Offset = %d \n", ui16Offset);
				LOG(eSpotProtocol, eError, "GSPOTTransportLayer::ReadMessage - bad received SPOT message length = %d\n", ui16Length);

				return eSPOT_MESSAGE_ERROR;

			}
		}
	}
	while (ui16Offset < ui16Length);

	ui16BytesRead = ui16Length;

	return eSPOT_DATA_RECEIVED;
}


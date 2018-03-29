///////////////////////////////////////////////////////////
//  GSPOTSession.cpp
//  Implementation of the Class GSPOTSession
//  Created on:      08-Mar-2007 10:51:14 AM
///////////////////////////////////////////////////////////

// SPOT related files
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"
#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"

// DEBUGOUT
#include "Common/Util/DEBUGOUT.h"

#include <string.h>

using namespace GSpotDefs;

GMutex GSPOTSession::mst_Mutex;
GSPOTSession* GSPOTSession::mst_pSPOTSession(NULL);
const UINT16 GSPOTSession::CMilliPerSecond(1000);
int GSPOTSession::CLIENT_ID(0x82);

void GSPOTSession::CreateInstance(GSPOTTransportLayer* pSPOTTransportLayer)
{
   GMutexLock lock(mst_Mutex);

   if( pSPOTTransportLayer != NULL && mst_pSPOTSession == NULL )
   {
      mst_pSPOTSession = new GSPOTSession(pSPOTTransportLayer);
      pSPOTTransportLayer->SetSession(mst_pSPOTSession);
   }
}

void GSPOTSession::Destroy()
{
   GMutexLock lock(mst_Mutex);
   if( mst_pSPOTSession != NULL )
   {
      delete mst_pSPOTSession;
      mst_pSPOTSession = NULL;
   }
}

GSPOTSession* GSPOTSession::GetSoleInstance()
{
   GMutexLock lock(mst_Mutex);
   return mst_pSPOTSession;
}

GSPOTSession::GSPOTSession()
  : m_eSPOTSessionState(eSPOTDisconnected),
    m_pSPOTTransportLayer(NULL),
    m_ui16PingTimeout(CKEEP_SESSION_ALIVE_TIMEOUT)
{
   m_pWatchSession = new GSimpleCommand<GSPOTSession>(this, &GSPOTSession::WatchSession);
   m_pWatchSessionTimer = new GTimer(m_pWatchSession, CRECONNECT_SESSION_TIMEOUT, false, false);
}

GSPOTSession::GSPOTSession(GSPOTTransportLayer* pSPOTTransportLayer)
  : m_eSPOTSessionState(eSPOTDisconnected),
    m_pSPOTTransportLayer(pSPOTTransportLayer),
    m_ui16PingTimeout(CKEEP_SESSION_ALIVE_TIMEOUT)
{
   m_pWatchSession = new GSimpleCommand<GSPOTSession>(this, &GSPOTSession::WatchSession);
   m_pWatchSessionTimer = new GTimer(m_pWatchSession, CRECONNECT_SESSION_TIMEOUT, false, false);
}

GSPOTSession::~GSPOTSession()
{
   if (m_pWatchSessionTimer)
   {
      delete m_pWatchSessionTimer;
      m_pWatchSessionTimer = NULL;
   }

   if( m_pWatchSession )
   {
      delete m_pWatchSession;
      m_pWatchSession = NULL;
   }
}

void GSPOTSession::SetPingTimeout(UINT8 ui8Sec)
{
   m_ui16PingTimeout = ui8Sec * CMilliPerSecond;
}

bool GSPOTSession::Connect(bool bReconnect)
{
   if( m_eSPOTSessionState != eSPOTDisconnected && m_eSPOTSessionState != eSPOTConnectionError )
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Connect - Invalid session state\n");
      return false;
   }

   if (!m_pSPOTTransportLayer)
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Connect - Transport Layer ptr is null\n");
      return false;
   }

   if( !m_pSPOTTransportLayer->OpenConnection() )
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Connect - Could not open connection with SPOT\n");
      if ( bReconnect ) StartWatchSessionTimer();
      return false;
   }

   m_eSPOTSessionState = eSPOTConnecting;

   // Now it tries to login into SPOT...
   if( !Login() )
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Connect - Could not login into SPOT\n");
      m_pSPOTTransportLayer->Stop();
      m_eSPOTSessionState = eSPOTDisconnected;
      if ( bReconnect ) StartWatchSessionTimer();
      return false;
   }

   //We are connected to SPOT
   m_eSPOTSessionState = eSPOTConnected;
   m_pWatchSessionTimer->SetInterval(m_ui16PingTimeout);
   //This method can be called from WatchSession and timer should be running
   StartWatchSessionTimer();

   m_pSPOTTransportLayer->Start();
   LOG(eSpotProtocol, eInfo, "GSPOTSession::Connect - Session connected Ok\n");
   return true;
}

void GSPOTSession::Disconnect(bool bTerminateConnection)
{
   if( m_eSPOTSessionState != eSPOTConnected && m_eSPOTSessionState != eSPOTConnectionError )
   {
      LOG(eSpotProtocol, eError, "GSPOTSession::Disconnect - Invalid session state\n");
      return;
   }   

   if (!m_pSPOTTransportLayer)
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Disconnect - Transport Layer ptr is null\n");
      return;
   }

   m_eSPOTSessionState = eSPOTDisconnecting;
   m_pSPOTTransportLayer->Stop();
   m_eSPOTSessionState = eSPOTDisconnected;

   if( bTerminateConnection )
   {
      m_pWatchSessionTimer->Stop();
   }
   LOG(eSpotProtocol, eInfo, "GSPOTSession::Disconnect - Session ended Ok\n");
}

bool GSPOTSession::IsConnected()
{
   return (m_eSPOTSessionState == eSPOTConnected);
}

void GSPOTSession::KeepSessionAlive()
{
   if (!m_pSPOTTransportLayer)
   {
     LOG(eSpotProtocol, eFatal, "GSPOTSession::KeepSessionAlive - Transport Layer ptr is null\n");
     return;
   }

   GSPOTMessage* pMessage = new GSPOTMessage(eSPOT_SERVICE_CATEGORY, eSPOT_SERVICE_PING);
  
   // push ping message to transport layer object
   if( !m_pSPOTTransportLayer->SendMessage(pMessage) )
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::KeepSessionAlive - Could not send PING message to SPOT\n");
   }

   delete pMessage;
}

bool GSPOTSession::Login()
{
   if (!m_pSPOTTransportLayer)
   {
     LOG(eSpotProtocol, eFatal, "GSPOTSession::Login - Transport Layer ptr is null\n");
     return false;
   }

   GSPOTMessage* pMessage = new GSPOTMessage(eSPOT_SERVICE_CATEGORY, eSPOT_SERVICE_LOGIN);

   //Populate login message
   pMessage->AppendByte(CLIENT_ID); // SPOTCheck client id, view http://twiki.gso.gilbarco.com/cgi-bin/twiki/view/Payment/SPOTProtocolClientIDUse
   pMessage->AppendWord(0);           				//RSA Key management disable
   pMessage->AppendByte(CPING_TIMEOUT / CMilliPerSecond);	//timeout in seconds

   if( !m_pSPOTTransportLayer->SendMessage(pMessage) )
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Login - Could not send LOGIN message to SPOT\n");
      delete pMessage;
      return false;
   }

   delete pMessage;
   return true;
}

void GSPOTSession::Logout()
{
   if (!m_pSPOTTransportLayer)
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::Logout - Transport Layer ptr is null\n");
      return;
   }

   GSPOTMessage* pMessage = new GSPOTMessage(eSPOT_SERVICE_CATEGORY, eSPOT_SERVICE_LOGOUT);

   if( !m_pSPOTTransportLayer->SendMessage(pMessage) )
   {
      LOG(eSpotProtocol, eError, "GSPOTSession::Logout - Could not send LOGOUT message to SPOT\n");
   }

   delete pMessage;
}

void GSPOTSession::StartWatchSessionTimer()
{
   if( m_pWatchSessionTimer->IsRunning() )
   {
      m_pWatchSessionTimer->Reset();
   }
   else
   {
      m_pWatchSessionTimer->Start();
   }
}

void GSPOTSession::AddSessionListener(GAbsSPOTSessionListener* Listener)
{
   m_SessionListenerList.push_back(Listener);
}

bool GSPOTSession::SendSPOTCommand(GSPOTMessage* pMsg)
{
   bool retval = false;
   if (!m_pSPOTTransportLayer)
   {
      LOG(eSpotProtocol, eFatal, "GSPOTSession::SendSPOTMessage - Transport Layer ptr is null\n");
      return false;
   }

   if( !m_pSPOTTransportLayer->SendMessage(pMsg) )
   {
      LOG(eSpotProtocol, eError, "GSPOTSession::SendSPOTMessage - Could not send message to SPOT\n");
      return false;
   }
   return true;
}

void GSPOTSession::NotifyError(eSPOTTransportLayerError eTLError)
{
   if( eTLError != eConnectionError )
   {
      NotifySessionError(eTLError); // non fatal error (link to SPOT still available)
   }
   else
   {
      m_eSPOTSessionState = eSPOTConnectionError;
      m_pWatchSessionTimer->SetInterval(CRECONNECT_SESSION_TIMEOUT);
      m_pWatchSessionTimer->Reset();
      LOG(eSpotProtocol, eFatal, "GSPOTSession::NotifyError - Closing session...\n");
   }
}

void GSPOTSession::WatchSession()
{
   if ( m_eSPOTSessionState == eSPOTConnected )
   {
      // if we are connected send the ping (in KeepSessionAlive)
      KeepSessionAlive();
      m_pWatchSessionTimer->Reset();
   }
}

void GSPOTSession::NotifyMessage(GSPOTMessage* pMsg)
{
   // send msg to listeners, who will copy message and place in their queues for handling
   std::list<GAbsSPOTSessionListener*>::iterator iter(m_SessionListenerList.begin());
   for( ; iter != m_SessionListenerList.end(); ++iter )
   {
      GAbsSPOTSessionListener* pListener = (*iter);
      pListener->Push(pMsg);
   }
}

void GSPOTSession::NotifySessionError(eSPOTTransportLayerError eError)
{
   std::list<GAbsSPOTSessionListener*>::iterator iter(m_SessionListenerList.begin());
   for( ; iter != m_SessionListenerList.end(); ++iter )
   {
      GAbsSPOTSessionListener* pListener = (*iter);
      pListener->NotifyError(eError);
   }
}


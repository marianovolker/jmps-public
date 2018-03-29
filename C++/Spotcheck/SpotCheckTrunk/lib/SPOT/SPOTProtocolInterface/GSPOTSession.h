/**
* @file GSPOTSession.h
* @brief interface for class GSPOTSession.
* It provides communication to all SPOT services (i.e. Card Reader, Beeper, Keypad) with the only
* exception of the PassThrough service which has its own ProtocolHandler
*
* @date 08-Mar-2007 10:51:10 AM
*/

#ifndef GSPOTSession_h
#define GSPOTSession_h 1

#include <GSystem.h>
#include <GTypes.h>
#include <GMutex.h>
#include "Common/Util/GTimer.h"
#include "Common/Util/GSimpleCommand.h"
#include "SPOT/SPOTProtocolInterface/GAbsSPOTSessionListener.h"
#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"

#include <list>

class GSPOTTransportLayer;

class GSPOTSession
{
  public:
    static void CreateInstance(GSPOTTransportLayer* pSPOTTransportLayer);
    static void Destroy();
    static GSPOTSession* GetSoleInstance();

    inline static void SetSessionID(int iSessionID){CLIENT_ID=iSessionID;};

    /**
     * @param bReconnect enable background task to keep trying to reconnect
     * @return true/false - success or fail
     */
    bool Connect(bool bReconnect = true);
    void Disconnect(bool bTerminateConnection = true);
    bool IsConnected();

    void NotifyMessage(GSPOTMessage* pMsg);
    void NotifyError(GSpotDefs::eSPOTTransportLayerError eError);
    
    /**
     * @brief Callback method to process incoming message from SPOT
     */
    void WatchSession();
    void SetPingTimeout(UINT8 ui8Sec);
    void AddSessionListener(GAbsSPOTSessionListener* pListener);
    bool SendSPOTCommand(GSPOTMessage* pMsg);

protected:
    GSPOTSession();
    GSPOTSession(GSPOTTransportLayer* pSPOTTransportLayer);
    virtual ~GSPOTSession();

    void KeepSessionAlive();
    bool Login();
    void Logout();
    void StartWatchSessionTimer();
    void SetSessionStateToError();
    /*!
     * \brief [Notifies listeners of SPOT communication error]
     *
     * This method notifies all listeners a non fatal error situation in
     * messaging to/from SPOT (transport layer).
     */
    void NotifySessionError(GSpotDefs::eSPOTTransportLayerError eError);

    static const UINT16 CMilliPerSecond;
    static GSPOTSession* mst_pSPOTSession;
    static GMutex mst_Mutex;

    GSpotDefs::eSPOTConnectionState m_eSPOTSessionState;
    GSPOTTransportLayer* m_pSPOTTransportLayer;

    GSimpleCommand<GSPOTSession>* m_pWatchSession;
    GTimer* m_pWatchSessionTimer;

    UINT16 m_ui16PingTimeout;
    std::list<GAbsSPOTSessionListener*> m_SessionListenerList;

private:
	static int CLIENT_ID;
};

#endif // GSPOTSession_h

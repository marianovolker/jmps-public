///////////////////////////////////////////////////////////
//  GSPOTTransportLayer.h
//  Implementation of the Class GSPOTTransportLayer
//  Created on:      08-Mar-2007 10:51:18 AM
///////////////////////////////////////////////////////////

#ifndef GSPOTTransportLayer_h
#define GSPOTTransportLayer_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"
#include "GSocket.h"
#include "GTask.h"
#include <string>

using namespace GSpotDefs;

class GSPOTTransportLayer : public GTask
{
  public:
    static void CreateInstance(const char* ip_addr);
    static void DestroySoleInstance();
    static GSPOTTransportLayer* GetSoleInstance();
    /**
    * After creating the TransportLayer the reference to the session must be set
    * @param pSession
    */
    void SetSession(GSPOTSession* pSession);
    bool OpenConnection();
    void CloseConnection();
    bool SendMessage(GSPOTMessage* pMsg);
    bool ReceiveMessage(GSPOTMessage* pRcvMsg);
    eSPOT_MSG_RESULT ReadMessage(UINT8* pui8Buffer, UINT16& ui16BytesRead, UINT32 ui8Timeout);
    void Start();
    void Stop();
    bool IsRunning();
    bool IsRunningAndConnected();

    void NotifyError(GSpotDefs::eSPOTTransportLayerError eError);
    void NotifyMessage(GSPOTMessage* pMsg);

  protected:
    GSPOTTransportLayer(const char* ip_addr);
    virtual ~GSPOTTransportLayer();
    void Run();

    static GSPOTTransportLayer*  mst_pInstance;
    static GMutex mst_InstanceMutex;

    GSocket* m_pSocket;
    UINT16 m_ui16Port;
    GSPOTSession* m_pSession;
    bool m_bThreadRunning;
    std::string m_strIPAddr;
    GMutex m_Mutex;
};

#endif // GSPOTTransportLayer_h


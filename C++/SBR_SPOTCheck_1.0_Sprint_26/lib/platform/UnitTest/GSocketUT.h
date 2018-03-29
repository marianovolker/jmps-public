//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#ifndef GSocketUT_h
#define GSocketUT_h

#include <GTask.h>
#include <GSocket.h>
#include <gtest/gtest.h>
#include <queue>

using namespace std;

class GSocketServerEndPoint : public GTask
{
public:
	typedef enum
	{
		eIdle,
		eConnected,
		eClosed
	}eState;

	GSocketServerEndPoint();
	virtual ~GSocketServerEndPoint(void);

	eState GetState();
	void SetSocket(GSocket* pSocket);

	void SetId(int id);
	int GetId();

	void Stop();
private:
	void Run();

	// Prohibit GSocketServerEndPoint copy.
	GSocketServerEndPoint(const GSocketServerEndPoint &right);
	GSocketServerEndPoint& operator=(const GSocketServerEndPoint &right);

	GMutex m_mutex;
	GSocket* m_pSocket;
	eState m_eState;
	int m_iId;
};

class GSocketServer : public GTask
{
public:
	typedef enum
	{
		eIdle		= int(1),
		eStarted	= int(2),
		eFinished 	= int(3),
	}eState;

	GSocketServer();
	virtual ~GSocketServer(void);

	/**
	 * It push a socket end point into the server to be used to accept incoming connections
	 * @param pClient
	 */
	void Push(GSocketServerEndPoint* pClient);

	void SetUseEndPointClientQueue(bool value);

	void Start();

	/**
	 * after stopping the server nothing else can be done with it
	 */
	void Stop();

	eState GetState();
private:
	void Run();

	// Prohibit GSocketServer copy.
	GSocketServer(const GSocketServer &right);
	GSocketServer& operator=(const GSocketServer &right);

	//client queue
	queue<GSocketServerEndPoint*> m_Clients;

	//socket class
	GSocket* m_pSocketServer;
	bool m_bUseQueue;
	eState m_eState;

	GMutex m_gMutex;
	GCondition m_gCondition;
};

//Unit test class
class GSocketUT : public testing::Test
{
public:
	GSocketUT();
	virtual ~GSocketUT();

	void SetUp();
	void TearDown();

	static const int CPort;
};


#endif /* GSocketUT_h */

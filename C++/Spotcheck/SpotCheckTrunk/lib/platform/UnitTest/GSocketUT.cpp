//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#include "GSocketUT.h"
#include <GTask.h>
#include <GSocket.h>
#include <iostream>
#include <string>
#include <vector>
#include "GStopWatch.h"
#include "GSystem.h"

using namespace std;


const int GSocketUT::CPort = 5000;

/*
 *  class GSocketServerEndPoint
 */
GSocketServerEndPoint::GSocketServerEndPoint()
 : m_pSocket(NULL),
   m_eState(eIdle),
   m_iId(-1)
{
}

GSocketServerEndPoint::~GSocketServerEndPoint(void)
{
	Stop();
	Join();
}

GSocketServerEndPoint::eState GSocketServerEndPoint::GetState()
{
	GMutexLock lock(m_mutex);
	return m_eState;
}

void GSocketServerEndPoint::SetSocket(GSocket* pSocket)
{
	m_pSocket = pSocket;
}

void GSocketServerEndPoint::SetId(int id)
{
	m_iId = id;
}

int GSocketServerEndPoint::GetId()
{
	return m_iId;
}

void GSocketServerEndPoint::Run()
{
	char szBuffer[512];
	bool bExit(false);

	m_mutex.Lock();
	m_eState = eConnected;
	m_mutex.Unlock();

	while( !bExit )
	{
		int iread = m_pSocket->Read(szBuffer, sizeof(szBuffer), 2);
		if( iread == eSocketTimeout )
		{
		}
		else if( iread == eSocketClosed )
		{
			bExit = true;
		}
		else if( iread == eSocketError )
		{
			bExit = true;
		}
		else if( iread > 0 )
		{
			//implement a simple echo
			m_pSocket->Write(reinterpret_cast<const char*>(szBuffer), iread, 10);
		}
	};
	m_pSocket->Close();

	m_mutex.Lock();
	m_eState = eClosed;
	m_mutex.Unlock();

	delete m_pSocket;
	m_pSocket = NULL;
}

void GSocketServerEndPoint::Stop()
{
	if( m_pSocket )
	{
		m_pSocket->Close();
	}
}


/*
 *  class GSocketServerTask
 */
GSocketServer::GSocketServer()
	:	GTask(),
		m_pSocketServer(NULL),
		m_bUseQueue(false),
		m_eState(eIdle),
		m_gMutex(),
		m_gCondition(&m_gMutex)
{

}

GSocketServer::~GSocketServer()
{
	Stop();
	Join();
}

void GSocketServer::Start()
{
	GMutexLock lock(m_gMutex);
	GTask::Start(eJoinable);
	while (m_eState == eIdle)
	{
		m_gCondition.Wait();
	}
}

void GSocketServer::Stop()
{
	GMutexLock lock(m_gMutex);
	if( m_pSocketServer )
		m_pSocketServer->Close();
}

void GSocketServer::Push(GSocketServerEndPoint* pClient)
{
	GMutexLock lock(m_gMutex);
	m_Clients.push(pClient);
}

void GSocketServer::SetUseEndPointClientQueue(bool value)
{
	GMutexLock lock(m_gMutex);
	m_bUseQueue = value;
}

GSocketServer::eState GSocketServer::GetState()
{
	GMutexLock lock(m_gMutex);
	return m_eState;
}

void GSocketServer::Run()
{
	bool bExit(false);
	int iId(0);

	{
		GMutexLock lock(m_gMutex);

		m_pSocketServer = new GSocket;

		if( !m_pSocketServer->Listen(GSocketUT::CPort) )
		{
			cout << "GSocketServerTask::Run fail to open socket in listen mode" << endl;
			return;
		}

		m_eState = eStarted;
		m_gCondition.Signal();
	}
	while( !bExit )
	{
		GSocket* pSocket = m_pSocketServer->Accept();

		if( pSocket )
		{
			GMutexLock lock(m_gMutex);
			eTaskRunMode eMode(eDetached);
			GSocketServerEndPoint* pEndPoint(NULL);
			if( m_bUseQueue && !m_Clients.empty() )
			{
				eMode = eJoinable;
				pEndPoint = m_Clients.front();
				m_Clients.pop();
			}
			else if( !m_bUseQueue )
			{
				pEndPoint = new GSocketServerEndPoint;
			}
			if( pEndPoint )
			{
				++iId;
				pEndPoint->SetSocket(pSocket);
				pEndPoint->SetId(iId);
				pEndPoint->Start(eMode);
			}
		}
		else
		{
			bExit = true;
		}
	};
	{
		GMutexLock lock(m_gMutex);
		m_eState = eFinished;
		delete m_pSocketServer;
		m_pSocketServer = NULL;
	}
}

/*
 *  class GSocketUT
 */
GSocketUT::GSocketUT()
{
}

GSocketUT::~GSocketUT()
{
}

void GSocketUT::SetUp()
{

}

void GSocketUT::TearDown()
{

}

TEST_F(GSocketUT, TestConnect)
{
	GSocket *pSocket(new GSocket);

	EXPECT_FALSE(pSocket->Connect("127.0.0.1", 5000));

	delete pSocket;
	pSocket = NULL;
}

TEST_F(GSocketUT, TestListen)
{
	GSocket *pSocket(new GSocket);

	EXPECT_TRUE(pSocket->Listen(5000));

	delete pSocket;
	pSocket = NULL;
}

TEST_F(GSocketUT, TestCloseServer)
{
	GSocketServer* pSocketServer(new GSocketServer);

	pSocketServer->SetUseEndPointClientQueue(false);

	ASSERT_EQ(GSocketServer::eIdle, pSocketServer->GetState());

	pSocketServer->Start();

	ASSERT_EQ(GSocketServer::eStarted, pSocketServer->GetState());

	pSocketServer->Stop();

	GSystem::Sleep(100);

	ASSERT_EQ(GSocketServer::eFinished, pSocketServer->GetState());

	delete pSocketServer;
	pSocketServer = NULL;
}


TEST_F(GSocketUT, TestEchoMessage)
{
	GSocketServer* pSocketServer(new GSocketServer);

	pSocketServer->SetUseEndPointClientQueue(false);
	pSocketServer->Start();

	GSocket* pSocketClient(new GSocket);
	ASSERT_TRUE(pSocketClient->Connect("127.0.0.1", GSocketUT::CPort));

	char szBuffer[512];
	string strMsg("echo message");
	for( int i=0; i<5; ++i )
	{
		ASSERT_EQ(strMsg.length(), pSocketClient->Write(strMsg.c_str(), strMsg.length(), 5));
		GSystem::Sleep(100);
		int iread = pSocketClient->Read(szBuffer, sizeof(szBuffer), 5);
		ASSERT_EQ(iread, strMsg.length());
	}

	ASSERT_TRUE(pSocketClient->Close());
	delete pSocketClient;
	pSocketClient = NULL;

	delete pSocketServer;
	pSocketServer = NULL;
}


TEST_F(GSocketUT, TestMultipleClients)
{
	GSocketServer* pSocketServer(new GSocketServer);

	pSocketServer->SetUseEndPointClientQueue(false);
	pSocketServer->Start();

	vector<GSocket*> vClients;
	for( int i = 0; i < 10; ++i )
	{
		GSocket* pSocketClient(new GSocket);
		vClients.push_back(pSocketClient);

		ASSERT_TRUE(pSocketClient->Connect("127.0.0.1", GSocketUT::CPort));
	}

	for( int i = 0; i < 10; ++i )
	{
		char szBuffer[512];
		string strMsg("X");
		for( int i=0; i<5; ++i )
		{
			ASSERT_EQ(strMsg.length(), vClients[i]->Write(strMsg.c_str(), strMsg.length(), 5));
			GSystem::Sleep(100);
			int iread = vClients[i]->Read(szBuffer, sizeof(szBuffer), 5);
			ASSERT_EQ(iread, strMsg.length());
		}
	}

	for( int i = 0; i < 10; ++i )
	{
		GSocket* pSocketClient(vClients[i]);
		ASSERT_TRUE(pSocketClient->Close());
		delete pSocketClient;
		pSocketClient = NULL;
	}

	delete pSocketServer;
	pSocketServer = NULL;
}


TEST_F(GSocketUT, TestReadTimeout)
{
    const int CTimeout = 100; // 100 miliseconds

	GSocketServer* pSocketServer(new GSocketServer);

	pSocketServer->Start();

	GSocket* pSocketClient(new GSocket);

	ASSERT_TRUE(pSocketClient->Connect("127.0.0.1", GSocketUT::CPort));

	char szBuffer[512];
	GStopWatch oStopWatch;

	oStopWatch.StartTimer();
	int iread = pSocketClient->Read(szBuffer, sizeof(szBuffer), CTimeout);
	oStopWatch.StopTimer();

	ASSERT_EQ(iread, eSocketTimeout);

	long dElapsedTime = oStopWatch.GetElapsedMilliseconds();
	long dDiffTimeout = dElapsedTime - CTimeout;

	if( dDiffTimeout < 0 )
	{
		dDiffTimeout *= -1;
	}

	ASSERT_LE(dDiffTimeout, 10); // 10 miliseconds;

	ASSERT_TRUE(pSocketClient->Close());
	delete pSocketClient;
	pSocketClient = NULL;

	delete pSocketServer;
	pSocketServer = NULL;
}


TEST_F(GSocketUT, TestDisconnection)
{
	GSocketServer* pSocketServer(new GSocketServer);

	//set server to use pre existing end point clients
	//in this mode ServerEndPoint task is created as joinable
	pSocketServer->SetUseEndPointClientQueue(true);

	//create and add end point into the server
	GSocketServerEndPoint* pSocketEndPoint(new GSocketServerEndPoint);
	pSocketServer->Push(pSocketEndPoint);
	pSocketServer->Start();

	//server end point must be in idle state
	ASSERT_EQ(GSocketServerEndPoint::eIdle, pSocketEndPoint->GetState());

	GSocket* pSocketClient(new GSocket);

	ASSERT_TRUE(pSocketClient->Connect("127.0.0.1", GSocketUT::CPort));
	GSystem::Sleep(500);

	//check end point state
	ASSERT_EQ(GSocketServerEndPoint::eConnected, pSocketEndPoint->GetState());

	//check client state
	ASSERT_TRUE(pSocketClient->IsConnected());

	//close client
	ASSERT_TRUE(pSocketClient->Close());

	ASSERT_FALSE(pSocketClient->IsConnected());

	delete pSocketClient;
	pSocketClient = NULL;

	GSystem::Sleep(2500);
	//end point must be in close state
	ASSERT_EQ(GSocketServerEndPoint::eClosed, pSocketEndPoint->GetState());

	pSocketServer->Stop();

	delete pSocketEndPoint;
	pSocketEndPoint = NULL;

	delete pSocketServer;
	pSocketServer = NULL;
}

TEST_F(GSocketUT, TestWriteOverClosedSocket)
{
	GSocketServer* pSocketServer(new GSocketServer);

	pSocketServer->SetUseEndPointClientQueue(true);

	GSocketServerEndPoint* pSocketEndPoint(new GSocketServerEndPoint);
	pSocketServer->Push(pSocketEndPoint);
	pSocketServer->Start();
	GSystem::Sleep(200);

	//server end point must be in idle state
	ASSERT_EQ(GSocketServerEndPoint::eIdle, pSocketEndPoint->GetState());

	GSocket* pSocketClient(new GSocket);

	ASSERT_TRUE(pSocketClient->Connect("127.0.0.1", GSocketUT::CPort));
	GSystem::Sleep(500);

	//check end point state
	ASSERT_EQ(GSocketServerEndPoint::eConnected, pSocketEndPoint->GetState());

	//close server end point before client write data
	pSocketEndPoint->Stop();
	GSystem::Sleep(200);

	string strmsg("dummy message");
	ASSERT_EQ(eSocketClosed, pSocketClient->Write(strmsg.c_str(), strmsg.length(), 1));

	//close client
	ASSERT_TRUE(pSocketClient->Close());
	delete pSocketClient;
	pSocketClient = NULL;
	GSystem::Sleep(2500);

	//end point must be in close state
	ASSERT_EQ(GSocketServerEndPoint::eClosed, pSocketEndPoint->GetState());

	pSocketServer->Stop();

	delete pSocketEndPoint;
	pSocketEndPoint = NULL;

	delete pSocketServer;
	pSocketServer = NULL;
}

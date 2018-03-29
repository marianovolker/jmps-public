//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#include <winsock2.h>

#include <GSocketImpl.h>
#include <GSocket.h>
#include <iostream>

using namespace std;

int GSocketImpl::m_iInstanceCount = 0;
GMutex GSocketImpl::m_Mutex;

GSocketImpl::GSocketImpl( eSocketType type )
 : m_SocketType(type),
   m_lFD(-1)
{
	long lSocktype(0);
	bool bInvalidSocketType(false);

	// Set the actual BSD socket type.
	switch ( m_SocketType )
	{
		case eTCP:
			lSocktype = SOCK_STREAM;
		break;

		case eUDP:
			lSocktype = SOCK_DGRAM;
		break;

		default:
			bInvalidSocketType = true;
		break;
	}

	if( !bInvalidSocketType && Initialize() )
	{
		// Create the socket endpoint.
		m_lFD = socket(PF_INET, lSocktype, 0);
		if ( m_lFD == INVALID_SOCKET )
		{
			cout << "GSocketImpl::socket fail." << endl;
			m_lFD = -1;
		}
	}
}

GSocketImpl::GSocketImpl( long lFD )
{
	m_lFD = lFD;
	Initialize();
}

GSocketImpl::~GSocketImpl ()
{
	Close();
	Uninitialize();
}

bool GSocketImpl::Initialize()
{
	GMutexLock lock(m_Mutex);
	if( m_iInstanceCount == 0 )
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
		    cout << "WSAStartup failed with error: " << err << endl;
		    return false;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			cout << "The Winsock version dll that was found is [" << ((int)LOBYTE(wsaData.wVersion)) << "].[" << ((int)HIBYTE(wsaData.wVersion)) << "]." << endl;
		}
		else
		{
			cout << "The Winsock version 2.2 dll was found." << endl;
		}
	}
	++m_iInstanceCount;
	return true;
}

bool GSocketImpl::Uninitialize()
{
	GMutexLock oMutex(m_Mutex);

	if( m_iInstanceCount <= 0 )
	{
		return false;
	}

	--m_iInstanceCount;
	if( m_iInstanceCount == 0 )
	{
		WSACleanup();
	}
	return true;
}

long GSocketImpl::Read( char *szBuffer, long lLength, long lTimeout )
{
	long lretval(eSocketError);

	if ( m_lFD == -1 )
	{
		return eSocketError;
	}

	if ( lTimeout >= 0 )
	{
		fd_set fds;
		struct timeval tv;

		// Initialize the set of file descriptors.
		FD_ZERO( &fds );
		FD_SET( m_lFD, &fds );

		// First converted to microseconds.
		long lusTimeOut = lTimeout * 1000;
		tv.tv_sec  = (lusTimeOut/1000000L);
		tv.tv_usec = (lusTimeOut%1000000L);

		lretval = select( m_lFD+1, &fds, NULL, NULL, &tv );

		if( lretval < 0 )
		{
			return eSocketError;
        }
		else if( lretval == 0 )
		{
			return eSocketTimeout;
        }
	}

    lretval = recvfrom(m_lFD, (char*)(szBuffer), lLength, 0, NULL, NULL);

    if( lretval == 0 )
    {
    	return eSocketClosed;
    }

    else if( lretval < 0 )
    {
    	return eSocketError;
    }

    return lretval;
}

long GSocketImpl::Write( const char* szBuffer, long lLength, long lTimeout )
{
	long lretval(eSocketError);

	if ( m_lFD == -1 )
	{
		return eSocketError;
	}

	//validate if socket has been closed before sending any data
	{
		fd_set fds;
		struct timeval tv;

		// Initialize the set of file descriptors.
		FD_ZERO( &fds );
		FD_SET( m_lFD, &fds );

		// Set up the timeout to zero I don't want to spend any time on this
		tv.tv_sec  = 0;
		tv.tv_usec = 0;

		lretval = select( m_lFD+1, &fds, 0, NULL, &tv );
		if( lretval < 0 )
		{
			return eSocketError;
		}
		else if( lretval > 0 )
		{
			char szBuffer[128];
			//it means the file descriptor has been signed for reading
			//just check if the socket has been closed without taking any data from the incoming buffer
			lretval = recvfrom(m_lFD, (char*)(szBuffer), sizeof(szBuffer), MSG_PEEK, NULL, NULL);
			if( lretval == 0 )
			{
				return eSocketClosed;
			}
		}
	}

	if( lTimeout >= 0 )
	{
		fd_set fds;
		struct timeval tv;

		// Initialize the set of file descriptors.
		FD_ZERO( &fds );
		FD_SET( m_lFD, &fds );

		// First converted to microseconds.
		long lusTimeOut = lTimeout * 1000;
		tv.tv_sec  = (lusTimeOut/1000000L);
		tv.tv_usec = (lusTimeOut%1000000L);

		lretval = select( m_lFD+1, 0, &fds, NULL, &tv );

		if( lretval < 0 )
		{
			return eSocketError;
        }
		else if( lretval == 0 )
		{
			return eSocketTimeout;
        }
	}

	lretval = send( m_lFD, szBuffer, lLength, 0 );

	if( lretval < 0 )
		return eSocketError;

	return lretval;
}

bool GSocketImpl::Close()
{
	if ( m_lFD == -1 )
	{
		return false;
	}

	// try to shutdown connection first
	if( shutdown(m_lFD, SD_BOTH) != 0)
	{
		//if there are no active connection on the socket the shutdown fails with error code WSAENOTCONN
		int iError = WSAGetLastError();
		if( iError != WSAENOTCONN )
		{
			return false;
		}
	}

	if ( closesocket( m_lFD ) != 0 )
	{
		return false;
	}
	else
	{
		m_lFD = -1;
		return true;
	}
}

bool GSocketImpl::Listen( long lPort, long lBacklog )
{
	// Make sure socket was initialized properly.
	if ( m_lFD == -1 )
	{
		return false;
	}

	// Set up the server's sockaddr struct.
	m_Sockaddr.sin_family = AF_INET;
	m_Sockaddr.sin_port = htons( lPort );
	memset(&(m_Sockaddr.sin_addr), 0, sizeof(struct in_addr));

	/*
	SO_REUSEADDR - This socket option tells the kernel that even if this port is busy (in
	the TIME_WAIT state), go ahead and reuse it anyway.  If it is busy,
	but with another state, you will still get an address already in use
	error.  It is useful if your server has been shut down, and then
	restarted right away while sockets are still active on its port.  You
	should be aware that if any unexpected data comes in, it may confuse
	your server, but while this is possible, it is not likely.
	*/
	char optval(1); //enable the option
	if ( setsockopt( m_lFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0 )
	{
		closesocket(m_lFD);
		m_lFD = -1;
		return false;
	}

	int rc = bind( m_lFD, (struct sockaddr*)&m_Sockaddr, sizeof(struct sockaddr_in) );
	if ( rc != 0 )
	{
		cout << "GSocketImpl::Listen fail to bind socket in listen mode, return code: "<<rc << endl;
		return false;
	}

	rc = listen( m_lFD, lBacklog );
	if ( rc != 0 )
	{
		cout << "GSocketImpl::Listen fail to listen, return code: "<<rc << endl;
		return false;
	}

	return true;
}

GSocket* GSocketImpl::Accept ()
{
	// Make sure socket was initialized properly.
	if ( m_lFD == -1 )
	{
		return NULL;
	}

	long lNewFD = accept( m_lFD, 0, 0 );
	if ( lNewFD == -1 )
	{
		return NULL;
	}
	else
	{
		GSocketImpl* pSocket = new GSocketImpl(lNewFD);
		return reinterpret_cast<GSocket*>(pSocket);
	}
}

bool GSocketImpl::Connect( const char* szHost, long lPort )
{
	if ( m_lFD == -1 )
	{
		return false;
	}

	memset( &m_Sockaddr, 0, sizeof(struct sockaddr_in) );

	m_Sockaddr.sin_family = AF_INET;
	m_Sockaddr.sin_port = htons(lPort);
	m_Sockaddr.sin_addr.s_addr = inet_addr( szHost );

	if( connect(m_lFD, (struct sockaddr *)&m_Sockaddr, sizeof(struct sockaddr_in)) >= 0 )
	{
		return true;
	}
	return false;
}

bool GSocketImpl::Connect( long lIPAddr, long lPort )
{
	if ( m_lFD == -1 )
	{
		return false;
	}

	memset(&m_Sockaddr, 0, sizeof(struct sockaddr_in));

	m_Sockaddr.sin_family = AF_INET;
	m_Sockaddr.sin_port = htons(lPort);
	m_Sockaddr.sin_addr.s_addr = htons( lIPAddr );

	if( connect(m_lFD, (struct sockaddr *) &m_Sockaddr, sizeof(struct sockaddr_in)) >= 0 )
	{
		return true;
	}
	return false;
}

bool GSocketImpl::ValidateIpAddress( const char* szIpAddress )
{
    unsigned long result = inet_addr(szIpAddress);
    return (result != INADDR_NONE && result != INADDR_ANY);
}

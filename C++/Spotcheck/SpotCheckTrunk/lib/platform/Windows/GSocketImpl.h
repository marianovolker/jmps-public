//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#ifndef GSocketImpl_h
#define GSocketImpl_h

#include <GSocketTypes.h>
#include <GMutex.h>
#include <winsock.h>

typedef struct sockaddr_in SockaddrIn;

class GSocket;
class GSocketImpl
{
protected:
	GSocketImpl( eSocketType type);

	GSocketImpl( long lFD );

	virtual ~GSocketImpl();

	long Read( char *szBuffer, long lLength, long lTimeout );

	long Write( const char* szBuffer, long lLength, long lTimeout );

	bool Close();

	bool Listen( long lPort, long lBacklog = 32 );

	GSocket* Accept();

	bool Connect( const char* szHost, long lPort );

	bool Connect( long lPAddr, long lPort );

	bool IsConnected() { return (m_lFD != -1); }

	static bool ValidateIpAddress( const char* szIpAddress );

private:
	// Prohibit GSocketImpl copy.
	GSocketImpl(const GSocketImpl &right);
	GSocketImpl& operator=(const GSocketImpl &right);

private:
	bool Initialize();
	bool Uninitialize();

	/*!
	 * Set to whether or not the socket is intended for TCP or UDP style
	 * communication.
	 */
	eSocketType m_SocketType;

	/*!
	 * File descriptor for underlying BSD socket instance.
	 */
	long m_lFD;

	/*!
	 * Holds the BSD-style sockaddr_in struct that holds details about the socket
	 * endpoint.
	 * it has been defined as a void* to avoid use of type sockaddr_in
	 */
	SockaddrIn m_Sockaddr;

	static int m_iInstanceCount;
	static GMutex m_Mutex;
};

#endif

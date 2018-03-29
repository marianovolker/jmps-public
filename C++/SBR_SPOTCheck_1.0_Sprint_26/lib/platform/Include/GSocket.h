//Copyright Gilbarco Inc. 2011
//Gilbarco Proprietary and Confidential

#ifndef GSocket_h
#define GSocket_h

#include <GSocketImpl.h>
#include <GSocketTypes.h>

/**
 * @class GSocket
 * @brief Class to provide an OO wrapper for a BSD Socket style network communication
 * endpoint.  Just like the concept of a BSD socket, this class supports both
 * client and server implementations.  Although the API doesn't differ greatly
 * from the BSD implementation.
 */
class GSocket : private GSocketImpl
{
public:
	/**
	 * Constructor for GSocket class.  Must provide whether the socket is intended
	 * for TCP- or UDP-style communication.
	 */
	GSocket( eSocketType type = eTCP ) : GSocketImpl(type)
	{
	}
	/**
	 * This form of GSocket() is essentially a copy constructor - it takes a file
	 * descriptor for an existing underlying socket instance and creates a GSocket
	 * instance around it.
	 */
	GSocket( long lFD ) : GSocketImpl(lFD)
	{
	}

	virtual ~GSocket(void)
	{
	}

	/**
	* @brief Read Reads data from the socket
	*
	* @param[in] [szBuffer] [buffer to store the data read from the socket]
	* @param[in] [lLength] [lenght of szBuffer]
	* @param[in] [lTimeout] [timeout to wait for read operation in miliseconds]
	* @return >0 length bytes read
	* @return <0 an error defined on the eSocketErrorCodes enum {eSocketTimeout, eSocketClosed, eSocketError}
	 */
	long Read( char *szBuffer, long lLength, long lTimeout )
	{
		return GSocketImpl::Read(szBuffer, lLength, lTimeout);
	}

	/**
	 * @brief Delivers up to lLength characters from buffer szBuffer to the
	 * opposite endpoint of the socket, or less if timeout lTimeout expires
	 * before it completes.
	 * @param[in] [szBuffer] [buffer to store the data read from the socket]
	 * @param[in] [lLength] [lenght of szBuffer]
	 * @param[in] [lTimeout] [timeout to wait for read operation in miliseconds]
	 * @return >0 bytes written in the socket
	 * @return <0 an error defined on the eSocketErrorCodes enum {eSocketTimeout, eSocketClosed, eSocketError}
	 */
	long Write( const char* szBuffer, long lLength, long lTimeout )
	{
		return GSocketImpl::Write(szBuffer, lLength, lTimeout);
	}

	/**
	 * @brief Shuts down the socket endpoint.  This method typically should not be
	 * directly invoked, since no method is currently provided to re-open the
	 * socket.  The destructor will call Close() when the GSocket object is
	 * destroyed.
	 */
	bool Close()
	{
		return GSocketImpl::Close();
	}

	/**
	 * @brief it is equivalent to the BSD-style socket call for the server
	 * endpoint.  It takes two parameters: one for port number being listened to;
	 * the second defines the backlog of incoming connections the server socket
	 * should allow before turning clients away.
	 * */
	bool Listen( long lPort, long lBacklog = 32 )
	{
		return GSocketImpl::Listen(lPort, lBacklog);
	}

	/**
	* @brief Returns a new (cloned) GSocket instance when an connection is made
	* to an incoming request.  This allows listen() to go back to waiting on the
	* existing GSocket endpoint for additional incoming requests.
	*/
	GSocket* Accept()
	{
		return GSocketImpl::Accept();
	}

	/**
	 * @brief [connect client to a given host:port]
	 * @param[in] szHost IPv4 address in ASCII format
	 * @param[in] lPort
	 * @param[out] true is connection is successfully
	 */
	bool Connect( const char* szHost, long lPort )
	{
		return GSocketImpl::Connect(szHost, lPort);
	}

	/**
	 * @brief [connect client to a given host:port]
	 * @param[in] 32 bits IP address like in struct in_addr
	 * @param[in] lPort
	 * @param[out] true is connection is successfully
	 */
	bool Connect( long lPAddr, long lPort )
	{
		return GSocketImpl::Connect(lPAddr, lPort);
	}

	bool IsConnected()
	{
		return GSocketImpl::IsConnected();
	}

	/**
	 * @brief [validate the ip address]
	 * @param[in] szIpAddress IPv4 address in ASCII format
	 * @param[out] true if is correct
	 */
	static bool ValidateIpAddress( const char* szIpAddress )
	{
		return GSocketImpl::ValidateIpAddress(szIpAddress);
	}

private:
	// Prohibit GSocket copy.
	GSocket(const GSocket &right);
	GSocket& operator=(const GSocket &right);
};

#endif

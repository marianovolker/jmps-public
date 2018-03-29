/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SSLClient.h
 * @author gparis
 * @date Friday, September 26, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */
#ifndef SSLCLIENT_H_
#define SSLCLIENT_H_

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


/**
 * @class  SSLClient
 * @brief  A client entity having all the stuff to communicate with a remote server.
 * @author Guillermo Paris
 * @date   2014-09-26
 * @since  0.0.0
 */
class SSLClient
{
  public:
               /**
                * @brief Type of server certificate validation.
                *
                * eNone    Means no verification at all.
                * eSubject Means just the subject part of the certificate is validated.
                * eRFC2818 Means full validation according to RFC2818 standard normative.
                */
    enum        eServerVerificationType { eNone, eSubject, eRFC2818 };

                /** @brief Default Constructor. */
                SSLClient();
                /** @brief Destructor. */
                ~SSLClient();

                /** @brief Client initialization */
    bool        init();
                /** @brief Initiates the first connection (unencrypted) to the server. */
    bool        connect();
                /** @brief Sets no delay for TCP connections. */
    void        setNoTCPDelay();
                /** @brief Performs the security handshake to the remote server. */
    void        handshake();

               /**
                * @brief Sets the verification type to apply to server's certificate(s).
                * @param e The server certificate(s) verification type.
                */
    void        setServerVerification(eServerVerificationType e);

               /**
                * @brief Gets the server hostname or IP address.
                *
                * @return Server's hostname or IP address.
                */
    const char* getHostNameOrIP();

               /**
                * @brief Sets the server hostname or IP address.
                *
                * @param pcNameOrIP Server's hostname or IP address.
                */
    void        setHostNameOrIP(const char* pcNameOrIP);


  private:

    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket>          SSLsocket;
    typedef boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>  ResolverIterator;

         /**
          * @brief Verifies the server's certificate(s) just using the subject part only.
          *
          * @return @b true if OK, @b false if identity could not be verified.
          *
          * @param blPreverified  boolean indicating if the certificate has passed the preverification.
          * @param [In|Out] vctx  the verification context passed by reference.
          *
          * This verify callback can be used to check whether the certificate that is
          * being presented is valid for the peer. This method will only check the subject part of
          * the certificate(s).
          * Note that this callback is called once for each certificate in the certificate chain,
          * starting from the root certificate authority.
          *
          */
    bool  verifyCertificateBySubject(bool blPreverified, boost::asio::ssl::verify_context& vctx);

         /**
          * @brief Verifies the server's certificate(s) just using the subject part only.
          *
          * @return @b true if OK, @b false if identity could not be verified.
          *
          * @param blPreverified  boolean indicating if the certificate has passed the preverification.
          * @param [In|Out] vctx  the verification context passed by reference.
          *
          * This verify callback can be used to check whether the certificate that is
          * being presented is valid for the peer. For example, RFC 2818 describes
          * the steps involved in doing this for HTTPS. Consult the OpenSSL
          * documentation for more details. Note that the callback is called once
          * for each certificate in the certificate chain, starting from the root
          * certificate authority.
          *
          */
    bool  verifyCertificateByRFC2818(bool blPreverified, boost::asio::ssl::verify_context& vctx);

    std::string                m_strHostNameOrIP;  ///< Server name or IP.
    boost::asio::ssl::context  m_SSLcontext;       ///< SSL context for operations.
    boost::asio::io_service    m_ioService;        ///< Service entity for socket/networking operations.
    ResolverIterator           m_resolverIterator; ///< Iterator over endpoint set arising from query resolution.
    SSLsocket *                m_pSSLsocket;       ///< SSL socket high level entity (uses a real TCP socket)

    boost::asio::ip::tcp::resolver*        m_pResolver;///< Resolver entity for IP/host name query resolution.
    boost::asio::ip::tcp::resolver::query* m_pQuery;   ///< Query entity consisting of hostname/IP and service name.
};

#endif /* SSLCLIENT_H_ */

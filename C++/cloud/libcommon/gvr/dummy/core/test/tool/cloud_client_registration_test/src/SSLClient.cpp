/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SSLClient.cpp
 * @author gparis
 * @date Friday, September 26, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <boost/bind.hpp>
#include "SSLClient.h"

namespace ssl = boost::asio::ssl;
using boost::asio::ip::tcp;

SSLClient::SSLClient()
  : m_SSLcontext(ssl::context::sslv23),
    m_pSSLsocket(NULL), m_pResolver(NULL), m_pQuery(NULL)
{
}

SSLClient::~SSLClient()
{
  if( m_pQuery != NULL )
  {
    delete m_pQuery;
  }

  if( m_pResolver != NULL )
  {
    delete m_pResolver;
  }

  if( m_pSSLsocket != NULL )
  {
    delete m_pSSLsocket;
  }
}

bool  SSLClient::init()
{
  using boost::asio::ip::tcp;
  namespace ssl = boost::asio::ssl;

  // Set the authority certificate.
  m_SSLcontext.load_verify_file("certificates/ca.crt");

  printf("\nTRACE-Client: conf 0.\n");

  // Open a socket and connect it to the remote host.
  m_pSSLsocket = new SSLsocket(m_ioService, m_SSLcontext);
  m_pResolver  = new tcp::resolver(m_ioService);
  m_pQuery     = new tcp::resolver::query(m_strHostNameOrIP, "https");

  printf("\nTRACE-Client: conf 1.\n");

  try
  {
    m_resolverIterator = m_pResolver->resolve(*m_pQuery);
    return true;
  }
  catch(boost::system::system_error& se)
  {
    return false;
  }
}

bool SSLClient::connect()
{
  ResolverIterator  it, itend;

  it = boost::asio::connect(m_pSSLsocket->lowest_layer(), m_resolverIterator);
  return ( it == itend ? false : true );
}

void SSLClient::setNoTCPDelay()
{
  m_pSSLsocket->lowest_layer().set_option(tcp::no_delay(true));
}

void SSLClient::setServerVerification(eServerVerificationType e)
{
  switch(e)
  {
    case eRFC2818:
      m_pSSLsocket->set_verify_mode(ssl::verify_peer);
      m_pSSLsocket->set_verify_callback( boost::bind(&SSLClient::verifyCertificateByRFC2818, this, _1, _2) );
      break;

    case eSubject:
      m_pSSLsocket->set_verify_mode(ssl::verify_peer);
      m_pSSLsocket->set_verify_callback( boost::bind(&SSLClient::verifyCertificateBySubject, this, _1, _2) );
      break;

    case eNone:
    default:
      m_pSSLsocket->set_verify_mode(ssl::verify_none);
      break; // no verification is requested, so there is not call back.
  }
}

void SSLClient::handshake()
{
  m_pSSLsocket->handshake(SSLsocket::client);
}


bool SSLClient::verifyCertificateBySubject(bool blPreverified, boost::asio::ssl::verify_context& vctx)
{
    // In this example we will simply print the certificate's subject name.
    char acSubjectName[256];
    X509* pX509Certificate = X509_STORE_CTX_get_current_cert( vctx.native_handle() );
    X509_NAME_oneline( X509_get_subject_name(pX509Certificate), acSubjectName, 256 );
    std::cout << "Verifying " << acSubjectName << "\n";

    return blPreverified; // All is OK if the current certificate was verified OK.
}

bool SSLClient::verifyCertificateByRFC2818(bool blPreverified, boost::asio::ssl::verify_context& vctx)
{
    if( ! blPreverified )
      return false; // The pre-verification of the current certificate failed.

	ssl::rfc2818_verification  oVerifier(m_strHostNameOrIP);
	return oVerifier(blPreverified, vctx);
}

const char* SSLClient::getHostNameOrIP()
{
	return m_strHostNameOrIP.c_str();
}

void SSLClient::setHostNameOrIP(const char* pcNameOrIP)
{
	m_strHostNameOrIP = pcNameOrIP;
}



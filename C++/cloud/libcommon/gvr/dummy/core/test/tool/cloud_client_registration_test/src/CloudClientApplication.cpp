/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudClientApplication.cpp
 * @author gparis
 * @date Tuesday, September 16, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <errno.h>
#include "CloudClientApplication.h"

CloudClientApplication::CloudClientApplication()
{
	m_blRunning = m_blConnected = false;
	m_iLastError = 0;
	m_pConfiguration = NULL;
}

CloudClientApplication::CloudClientApplication( CloudClientApplicationConfiguration* pConf )
{
	m_blRunning = m_blConnected = false;
	m_iLastError = 0;
	m_pConfiguration = pConf;
}

CloudClientApplication::~CloudClientApplication()
{
  if( m_pConfiguration != NULL )
  {
    delete m_pConfiguration;
  }
}

bool CloudClientApplication::connectTLSForRegistration()
{
	printf("\nTRACE-Client: ZERO.\n");

	try
	{
		bool blrc = m_SSLclient.connect();

		if( ! blrc )
		{
			printf("\nERROR: Could not connect to registration  server. (%d) %s\n", errno, strerror(errno));
			m_blConnected = false;
			return false;
		}

		printf("\nTRACE-Client: ONE.\n");
		m_SSLclient.setNoTCPDelay();

		printf("\nTRACE-Client: TWO.\n");
		m_SSLclient.setServerVerification(SSLClient::eSubject);

		printf("\nTRACE-Client: THREE.\n");
		// Perform SSL handshake and verify the remote host's certificate.
		m_SSLclient.handshake();

		printf("\nTRACE-Client: FOUR.\n");
	}
	catch(boost::system::system_error& se)
	{
		printf("\nERROR: SSL operation error: %s.\n", se.what());
		m_blConnected = false;
		return false;
	}

	// ... read and write as normal ...

	m_blConnected = true;
	return true;
}

/** Initializes the application. */
bool CloudClientApplication::init()
{
  m_iLastError = 0;
  m_SSLclient.setHostNameOrIP("127.0.0.1");
  bool blrc = m_SSLclient.init();
  if( !blrc )
  {
    m_blConnected = false;
    printf("\nERROR: Could not resolve registration server IP.\n");
  }

  return blrc;
}

/** Starts running the application. */
bool CloudClientApplication::start()
{
  printf("\nNOTICE: Cloud Client application starting ...\n");
  if( ! connectTLSForRegistration() )
  {
    printf("ERROR: Cloud Client application could not connect to the server. Aborted!\n");
    return false;
  }

  m_blRunning = true;
  printf("INFO: Cloud Client application started.\n");
  return true;
}

/** Stops running the application. */
bool CloudClientApplication::stop()
{
  if( ! m_blRunning )
  {
	  printf("WARNING: Cloud Client application is already stopped.\n");
	  return false;
  }

  printf("NOTICE: Cloud Client application is being stopped ...\n");
  m_blRunning = false;
  printf("INFO: Cloud Client application stopped.\n");
  return true;
}

int CloudClientApplication::exit()
{
  return m_iLastError;
}

bool CloudClientApplication::configure( CloudClientApplicationConfiguration* pConf )
{
  if(pConf == NULL)
  {
    return false; // bad parameter
  }

  if(m_blRunning)
  {
    stop();

    if(m_pConfiguration != NULL)
    {
      delete m_pConfiguration;  // get rid of the old configuration
    }

    m_pConfiguration = pConf; // configure the application

    start();

    return true;
  }

  if(m_pConfiguration != NULL)
  {
	  delete m_pConfiguration;  // get rid of the old configuration
  }

  m_pConfiguration = pConf; // configure the application

  return true;
}




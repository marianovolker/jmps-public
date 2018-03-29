/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudClientApplication.h
 * @author gparis
 * @date Tuesday, September 16, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUD_CLIENT_APPLICATION_H_
#define _CLOUD_CLIENT_APPLICATION_H_

#include "CloudClientApplicationConfiguration.h"
#include "SSLClient.h"

/**
 * @class CloudClientApplication
 *
 * @brief  This class is designed to execute the business logic of a trivial cloud client application.
 *         This means connecting to a remote host for mutual authentication and registration.
 *
 * @author Guillermo Paris
 * @date   2014-09-16
 * @since  0.0.0
 */
class CloudClientApplication
{
  public:

	/** @brief Default application constructor. */
    CloudClientApplication();

    /** @brief Constructs and configures the application in one shot. */
    CloudClientApplication( CloudClientApplicationConfiguration* );

    /** @brief Destructor. */
    ~CloudClientApplication();

   /**
    * Configures application after construction.
    *
    * @brief Configures/Re-configures the application. If the application is
    *        already running, this will stop, re-configure and start again the application.
    */
    bool    configure( CloudClientApplicationConfiguration* );

    /** @brief Initializes the application. */
    bool    init();

    /** @brief Starts running the application. */
    bool    start();

   /** @brief Stops running the application. */
    bool    stop();

   /**
    * Exits the application.
    *
    * @brief Exits the application returning to the OS the last error code.
    * @return The last error code.
    */
    int     exit();


  private:

    /** @brief Connects to remote host, do the handshake and establish the TLS link. */
    bool    connectTLSForRegistration();

    bool                                   m_blRunning;     ///< flag indicating the app is running or not.
    bool                                   m_blConnected;   ///< flag indicating the app is connected or not.
    int                                    m_iLastError;    ///< integer error code to return to OS.
    SSLClient                              m_SSLclient;     ///< SSL client object.
    CloudClientApplicationConfiguration*   m_pConfiguration;///< pointer to app configuration object.
};

#endif


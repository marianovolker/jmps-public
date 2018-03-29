/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file cloud_client_registration_tool.cpp
 * @author gparis
 * @date Friday, September 12, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstdio>
#include <unistd.h>
#include <cloud_client_registration_tool.h>
#include "CloudClientApplication.h"

/**
 * @brief      Cloud Client Registration Tool Main entry point.
 * @param [in] argc Amount of arguments passed to the dummy tool command line.
 * @param [in] argv List of arguments passed to the dummy tool command line.
 * @return     Command line expected result code. Dummy Tool always return 0, which means ended successfully.
 *
 * @since 1.0.0
 */
int main (int argc, const char **argv)
{
  // library initialization
  gvr::dummy::do_dummy();

  // log initialization :)
  printf("\n");

  CloudClientApplicationConfiguration*  pConfiguration;

  // Application configuration.
  pConfiguration = new CloudClientApplicationConfiguration(argc, argv);
  pConfiguration->setFoo(0xCAFECAFE, "cafe cafe !");

  CloudClientApplication cloudApp( pConfiguration );

  //Load the site registration request configuration
  if( pConfiguration->loadSiteRegistrationConfig("configuration/registration_device.xml"))
  {
    if( cloudApp.init() )
    {
      if( cloudApp.start() )
      {
        sleep(4);
        cloudApp.stop();
      }
    }
   }

  return cloudApp.exit();
}


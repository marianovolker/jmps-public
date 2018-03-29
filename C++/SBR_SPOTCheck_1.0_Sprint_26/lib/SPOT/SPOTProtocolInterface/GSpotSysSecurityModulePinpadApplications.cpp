/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysSecurityModulePinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include "GSpotSysSecurityModulePinpadApplications.h"

GSpotSysSecurityModulePinpadApplications* GSpotSysSecurityModulePinpadApplications::m_pInstance = NULL;
GMutex GSpotSysSecurityModulePinpadApplications::mst_InstanceMutex;

GSpotSysSecurityModulePinpadApplications::GSpotSysSecurityModulePinpadApplications():GAbsSpotSysPinpadApplications()
{
	populateStatesMap();
	populateStateExtensionsMap();
	populateErrorsMap();
}

GSpotSysSecurityModulePinpadApplications::~GSpotSysSecurityModulePinpadApplications()
{
}

void GSpotSysSecurityModulePinpadApplications::populateStatesMap()
{
	mapStates.insert ( std::make_pair( 0x00, itemType("SPOT_SM_IDLE_STATE"        , "IDLE: sem enabled, waiting for a service request." ) ) );
	mapStates.insert ( std::make_pair( 0x01, itemType("SPOT_SM_BUSY_STATE"        , "BUSY: sem in use, processing a request."           ) ) );
	mapStates.insert ( std::make_pair( 0x02, itemType("SPOT_SM_ERROR_STATE"       , "ERROR: sem in error, waiting for error recovery"   ) ) );
	mapStates.insert ( std::make_pair( 0x03, itemType("SPOT_SM_INOPERATIVE_STATE" , "INOPERATIVE: sem in fatal error, cannot operate"   ) ) );
	mapStates.insert ( std::make_pair( 0x04, itemType("SPOT_SM_STARTUP"           , "STARTUP: sem initializing device"         		   	) ) );
	mapStates.insert ( std::make_pair( 0x05, itemType("SPOT_SM_DALLASDOWNLOAD"    , "DOWNLOAD: sem downloading new firmware"   		   	) ) );
}

void GSpotSysSecurityModulePinpadApplications::populateStateExtensionsMap()
{
	mapStateExtensions.insert ( std::make_pair( 0x80, itemType("SPOT_SM_SENSORARMED_EVENT", "Installation sensor activated" ) ) );
}

void GSpotSysSecurityModulePinpadApplications::populateErrorsMap()
{
	mapErrors.insert ( std::make_pair( 0x01, itemType("SPOT_SM_DRIVERINITERROR"          , "Error initializing driver"                                                ) ) );
	mapErrors.insert ( std::make_pair( 0x02, itemType("SPOT_SM_DEVICECONNECTIONTIMEOUT"  , "Device connection timeout (warning)"                                      ) ) );
	mapErrors.insert ( std::make_pair( 0x03, itemType("SPOT_SM_DEVICECONNECTIONERROR"    , "Device connection failure (fatal error) – device could be tampered"       ) ) );
	mapErrors.insert ( std::make_pair( 0x04, itemType("SPOT_SM_BADSOFTWAREVERSION"       , "Bad device firmware version"                                              ) ) );
	mapErrors.insert ( std::make_pair( 0x05, itemType("SPOT_SM_DEVICENOARMEDERROR"       , "Installation sensor not activated: cannot operate"                        ) ) );
	mapErrors.insert ( std::make_pair( 0x06, itemType("SPOT_SM_NOSMKERROR"               , "Software Master Key not loaded into the device"                           ) ) );
	mapErrors.insert ( std::make_pair( 0x07, itemType("SPOT_SM_ENCRYPTIONERROR"          , "Error in encrypted communication: SMK not matching"                       ) ) );
	mapErrors.insert ( std::make_pair( 0x09, itemType("SPOT_SM_SECURITYMODULEERROR"      , "Security module command failed"                                           ) ) );
	mapErrors.insert ( std::make_pair( 0x0a, itemType("SPOT_SM_KEYDBERROR"               , "Key database error"                                                       ) ) );
	mapErrors.insert ( std::make_pair( 0x7f, itemType("SPOT_SM_GENERICERROR"             , "Generic error or timely frw cryptographic-check failed (SPOT M3c-2.1)"    ) ) );
}

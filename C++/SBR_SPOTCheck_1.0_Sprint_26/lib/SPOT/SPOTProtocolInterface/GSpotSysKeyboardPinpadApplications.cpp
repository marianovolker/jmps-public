/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysKeyboardPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#include "GSpotSysKeyboardPinpadApplications.h"

GSpotSysKeyboardPinpadApplications* GSpotSysKeyboardPinpadApplications::m_pInstance = NULL;
GMutex GSpotSysKeyboardPinpadApplications::mst_InstanceMutex;

GSpotSysKeyboardPinpadApplications::GSpotSysKeyboardPinpadApplications():GAbsSpotSysPinpadApplications()
{
	populateStatesMap();
	populateStateExtensionsMap();
	populateErrorsMap();
}

GSpotSysKeyboardPinpadApplications::~GSpotSysKeyboardPinpadApplications()
{
}

void GSpotSysKeyboardPinpadApplications::populateStatesMap()
{
	mapStates.insert ( std::make_pair( 0x00, itemType("SPOT_KB_DISABLED_STATE"    , "DISABLED: keyboard disabled, user input is not accepted."	) ) );
	mapStates.insert ( std::make_pair( 0x01, itemType("SPOT_KB_ENABLED_STATE"     , "ENABLED: keyboard enabled, waiting for user input."      	) ) );
	mapStates.insert ( std::make_pair( 0x02, itemType("SPOT_KB_BUSY_STATE"        , "BUSY: keyboard in use, processing the user input."       	) ) );
	mapStates.insert ( std::make_pair( 0x03, itemType("SPOT_KB_ERROR_STATE"       , "ERROR: keyboard in error, waiting for error recovery"    	) ) );
	mapStates.insert ( std::make_pair( 0x04, itemType("SPOT_KB_INOPERATIVE_STATE" , "INOPERATIVE: keyboard in fatal error, cannot operate"    	) ) );
	mapStates.insert ( std::make_pair( 0x05, itemType("SPOT_KB_STARTUP"           , "STARTUP: keyboard initializing device"						) ) );
	mapStates.insert ( std::make_pair( 0x06, itemType("SPOT_KB_DALLASDOWNLOAD"    , "DOWNLOAD: keyboard downloading new firmware" 			 	) ) );
}

void GSpotSysKeyboardPinpadApplications::populateStateExtensionsMap()
{
	mapStateExtensions.insert ( std::make_pair( 0x80, itemType("SPOT_KB_SENSORARMED_EVENT", "Installation sensor activated" ) ) );
}

void GSpotSysKeyboardPinpadApplications::populateErrorsMap()
{
	mapErrors.insert ( std::make_pair( 0x01, itemType("SPOT_KB_DRIVERINITERROR"          , "Error initializing driver"                                          ) ) );
	mapErrors.insert ( std::make_pair( 0x02, itemType("SPOT_KB_DEVICECONNECTIONTIMEOUT"  , "Device connection timeout (warning)"                                ) ) );
	mapErrors.insert ( std::make_pair( 0x03, itemType("SPOT_KB_DEVICECONNECTIONERROR"    , "Device connection failure (fatal error) – device could be tampered" ) ) );
	mapErrors.insert ( std::make_pair( 0x04, itemType("SPOT_KB_BADSOFTWAREVERSION"       , "Bad device firmware version"                                        ) ) );
	mapErrors.insert ( std::make_pair( 0x05, itemType("SPOT_KB_DEVICENOARMEDERROR"       , "Installation sensor not activated: cannot operate"                  ) ) );
	mapErrors.insert ( std::make_pair( 0x06, itemType("SPOT_KB_NOSMKERROR"               , "Software Master Key not loaded into the device"                     ) ) );
	mapErrors.insert ( std::make_pair( 0x07, itemType("SPOT_KB_ENCRYPTIONERROR"          , "Error in encrypted communication: SMK not matching"                 ) ) );
	mapErrors.insert ( std::make_pair( 0x09, itemType("SPOT_KB_ KEYBOARDERROR"           , "Keyboard command failed"                                            ) ) );
	mapErrors.insert ( std::make_pair( 0x7f, itemType("SPOT_KB_GENERICERROR"             , "Generic error"                                                      ) ) );
}

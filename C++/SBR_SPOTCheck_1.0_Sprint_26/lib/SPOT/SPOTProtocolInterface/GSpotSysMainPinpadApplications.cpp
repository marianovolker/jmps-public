/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysMainPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#include "GSpotSysMainPinpadApplications.h"

GSpotSysMainPinpadApplications* GSpotSysMainPinpadApplications::m_pInstance = NULL;
GMutex GSpotSysMainPinpadApplications::mst_InstanceMutex;

GSpotSysMainPinpadApplications::GSpotSysMainPinpadApplications(): GAbsSpotSysPinpadApplications()
{
	populateStatesMap();
	populateStateExtensionsMap();
	populateErrorsMap();
}

GSpotSysMainPinpadApplications::~GSpotSysMainPinpadApplications()
{
}

void GSpotSysMainPinpadApplications::populateStatesMap()
{
	mapStates.insert ( std::make_pair( 0x01, itemType("SPOT_STARTUP_STATE"     , "STARTUP : the system initializing all devices."                             ) ) );
	mapStates.insert ( std::make_pair( 0x02, itemType("SPOT_INOPERATIVE_STATE" , "INOPERATIVE: the system is in error and cannot operate."                    ) ) );
	mapStates.insert ( std::make_pair( 0x03, itemType("SPOT_OFFLINE_STATE"     , "OFFLINE: no remote clients (master programs on external PC) are connected." ) ) );
	mapStates.insert ( std::make_pair( 0x04, itemType("SPOT_ONLINE_STATE"      , "ONLINE: at least a remote client connected. System is ready to operate."    ) ) );
	mapStates.insert ( std::make_pair( 0x05, itemType("SPOT_BUSY_STATE"        , "BUSY: the system is processing user input."                                 ) ) );
	mapStates.insert ( std::make_pair( 0x06, itemType("SPOT_SERVICE_STATE"     , "SERVICE: the system operates a configuration/setup procedure."              ) ) );
}

void GSpotSysMainPinpadApplications::populateStateExtensionsMap()
{
	mapStateExtensions.insert ( std::make_pair( 0x00, itemType("SYS_VIDEO_SECURE"   , "SPOT shows SECURE screen (M5 only)"                            ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x01, itemType("SYS_VIDEO_UNSECURE" , "SPOT shows EXTERNAL screen (M5 only)"                          ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x02, itemType("SYS_VIDEO_EXTERNAL" , "SPOT shows NON-SECURE screen (M5 only)"                        ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x03, itemType("SYS_VIDEO_UNKNOWN"  , "The screen shown is not determined 2 (M5 only)"                ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x04, itemType("SYS_BUSY_KEYB"      , "Pinpad is busy since a secure pin input procedure is ongoing"  ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x05, itemType("SYS_BUSY_CR"        , "Pinpad is busy since a get EMV signature operation is ongoing" ) ) );
}

void GSpotSysMainPinpadApplications::populateErrorsMap()
{
	mapErrors.insert ( std::make_pair( 0x00, itemType("SPOT_SYS_NOERROR", "No error (M5 only)"                                                              ) ) );
	mapErrors.insert ( std::make_pair( 0x01, itemType("SPOT_SYS_ERROR"	, "SYSTEM is in error due to errors on one or more critical subsystems 4 (M5 only)" ) ) );
}

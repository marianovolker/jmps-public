/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSpotSysCardReaderPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include "GSpotSysCardReaderPinpadApplications.h"


GSpotSysCardReaderPinpadApplications* GSpotSysCardReaderPinpadApplications::m_pInstance = NULL;
GMutex GSpotSysCardReaderPinpadApplications::mst_InstanceMutex;

GSpotSysCardReaderPinpadApplications::GSpotSysCardReaderPinpadApplications():GAbsSpotSysPinpadApplications()
{
	populateStatesMap();
	populateStateExtensionsMap();
	populateErrorsMap();
}

GSpotSysCardReaderPinpadApplications::~GSpotSysCardReaderPinpadApplications()
{
}

void GSpotSysCardReaderPinpadApplications::populateStatesMap()
{
	mapStates.insert ( std::make_pair( 0x00, itemType("SPOT_CR_DISABLED_STATE"    , "DISABLED: reader disabled, cards are not accepted."  ) ) );
	mapStates.insert ( std::make_pair( 0x01, itemType("SPOT_CR_ENABLED_STATE"     , "ENABLED: reader enabled, waiting for card insertion.") ) );
	mapStates.insert ( std::make_pair( 0x02, itemType("SPOT_CR_BUSY_STATE"        , "BUSY: reader in use, processing the inserted card."  ) ) );
	mapStates.insert ( std::make_pair( 0x03, itemType("SPOT_CR_ERROR_STATE"       , "ERROR: reader in error, waiting for error recovery"  ) ) );
	mapStates.insert ( std::make_pair( 0x04, itemType("SPOT_CR_INOPERATIVE_STATE" , "INOPERATIVE: reader in fatal error, cannot operate"  ) ) );
	mapStates.insert ( std::make_pair( 0x05, itemType("SPOT_CR_STARTUP"           , "STARTUP: reader initializing device"       		  ) ) );
	mapStates.insert ( std::make_pair( 0x06, itemType("SPOT_CR_DALLASDOWNLOAD"    , "DOWNLOAD: reader downloading new firmware" 		  ) ) );
}

void GSpotSysCardReaderPinpadApplications::populateStateExtensionsMap()
{
	mapStateExtensions.insert ( std::make_pair( 0x01, itemType("SPOT_CR_CARDIN_EVENT"      , "Card inside the reader (under reading head or, after ejection, available in reader inlet). This bit can be used to check if the customer has picked up its card after reading." ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x02, itemType("SPOT_CR_ICCIN_EVENT"       , "The card is endowed with a chip. This bit is set only by the Panasonic reader (1)." ) ) );
	mapStateExtensions.insert ( std::make_pair( 0x80, itemType("SPOT_CR_SENSORARMED_EVENT" , "Installation sensor activated"                                                      ) ) );
}

void GSpotSysCardReaderPinpadApplications::populateErrorsMap()
{
	mapErrors.insert ( std::make_pair( 0x01, itemType("SPOT_CR_DRIVERINITERROR"          , "Error initializing driver"                            ) ) );
	mapErrors.insert ( std::make_pair( 0x02, itemType("SPOT_CR_DEVICECONNECTIONTIMEOUT"  , "Device connection timeout (warning)"                  ) ) );
	mapErrors.insert ( std::make_pair( 0x03, itemType("SPOT_CR_DEVICECONNECTIONERROR"    , "Device connection failure - device could be tampered" ) ) );
	mapErrors.insert ( std::make_pair( 0x04, itemType("SPOT_CR_BADSOFTWAREVERSION"       , "Bad device firmware version"                          ) ) );
	mapErrors.insert ( std::make_pair( 0x05, itemType("SPOT_CR_DEVICENOARMEDERROR"       , "Installation sensor not activated: cannot operate"    ) ) );
	mapErrors.insert ( std::make_pair( 0x06, itemType("SPOT_CR_NOSMKERROR"               , "Software Master Key not loaded into the device"       ) ) );
	mapErrors.insert ( std::make_pair( 0x07, itemType("SPOT_CR_ENCRYPTIONERROR"          , "Error in encrypted communication: SMK not matching"   ) ) );
	mapErrors.insert ( std::make_pair( 0x08, itemType("SPOT_CR_READERCONNECTIONERROR"    , "Reader not responding"                                ) ) );
	mapErrors.insert ( std::make_pair( 0x09, itemType("SPOT_CR_READERERROR"              , "Reader command failed"                                ) ) );
	mapErrors.insert ( std::make_pair( 0x0A, itemType("SPOT_CR_ICCERROR"                 , "ICC command failed"                                   ) ) );
	mapErrors.insert ( std::make_pair( 0x7F, itemType("SPOT_CR_GENERICERROR"             , "Generic error"                                        ) ) );
}

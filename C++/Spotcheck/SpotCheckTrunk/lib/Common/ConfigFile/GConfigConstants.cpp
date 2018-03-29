#include "GConfigConstants.h"


const std::string GConfigConstants::CRoot                            ("/config");


// CRIND
const std::string GConfigConstants::CCrindType                       (CRoot + "/CRIND/Type");
const std::string GConfigConstants::CCrindMode                       (CRoot + "/CRIND/Mode");
const std::string GConfigConstants::CCrindId                         (CRoot + "/CRIND/Id");

// SPOT/UPM
const std::string GConfigConstants::CSpotIPAddress                   (CRoot + "/SPOT/IPAddress");

// POS
const std::string GConfigConstants::CPOSInterface                    (CRoot + "/POS/Interface");
const std::string GConfigConstants::CPOSDev                          (CRoot + "/POS/Dev");
const std::string GConfigConstants::CPOSBaudRate                     (CRoot + "/POS/BaudRate");

// Pump
const std::string GConfigConstants::CPumpId                          (CRoot + "/Pump/Id");
const std::string GConfigConstants::CPumpDev                         (CRoot + "/Pump/Dev");

// Display
const std::string GConfigConstants::CDisplayMode                     (CRoot + "/Display/Mode");
const std::string GConfigConstants::CDisplayPredictorAvailable       (CRoot + "/Display/PredictorAvailable");
const std::string GConfigConstants::CDisplayPredictorPersistenceFile (CRoot + "/Display/PredictorPersistenceFile");
const std::string GConfigConstants::CDisplayPinLookUpFile            (CRoot + "/Display/PinLookUpFile");
const std::string GConfigConstants::CDisplayNonPinLookUpFile         (CRoot + "/Display/NonPinLookUpFile");
const std::string GConfigConstants::CDisplaySecureScreenRangesPath   (CRoot + "/Display/SecureScreenRangesPath");
const std::string GConfigConstants::CDisplayShowRotatingClock        (CRoot + "/Display/ShowRotatingClock");
const std::string GConfigConstants::CDisplayPersonalityProfilePath   (CRoot + "/Display/Personality/ProfilePath");
const std::string GConfigConstants::CDisplayPersonalityProfile       (CRoot + "/Display/Personality/Profile");
const std::string GConfigConstants::CDisplayPersonalityImageSet      (CRoot + "/Display/Personality/ImageSet");

// Printer

// Cash
const std::string GConfigConstants::CCashEnabled                     (CRoot + "/Cash/Enabled");
const std::string GConfigConstants::CCashDev                         (CRoot + "/Cash/Dev");

// Scanner
const std::string GConfigConstants::CScannerEnabled                  (CRoot + "/Scanner/Enabled");
const std::string GConfigConstants::CScannerDev                      (CRoot + "/Scanner/Dev");

// TRIND
const std::string GConfigConstants::CTRINDEnabled                    (CRoot + "/TRIND/Enabled");
const std::string GConfigConstants::CTRINDDev                        (CRoot + "/TRIND/Dev");

// LOG
const std::string GConfigConstants::CLogTraceEnabled                 (CRoot + "/Log/TraceEnabled");
const std::string GConfigConstants::CLogMaxLogFileSize               (CRoot + "/Log/MaxLogFileSize");

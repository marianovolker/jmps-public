#ifndef GConfigConstants_h
#define GConfigConstants_h

#include <string>

class GConfigConstants
{
public:
	// CRIND
	static const std::string CCrindType;
	static const std::string CCrindMode;
	static const std::string CCrindId;

	// SPOT/UPM
	static const std::string CSpotIPAddress;

	// POS
	static const std::string CPOSInterface;
	static const std::string CPOSDev;
	static const std::string CPOSBaudRate;

	// Pump
	static const std::string CPumpId;
	static const std::string CPumpDev;

	// Display
	static const std::string CDisplayMode;
	static const std::string CDisplayPredictorAvailable;
	static const std::string CDisplayPredictorPersistenceFile;
	static const std::string CDisplayPinLookUpFile;
	static const std::string CDisplayNonPinLookUpFile;
    static const std::string CDisplaySecureScreenRangesPath;
	static const std::string CDisplayShowRotatingClock;
	static const std::string CDisplayPersonalityProfilePath;
	static const std::string CDisplayPersonalityProfile;
	static const std::string CDisplayPersonalityImageSet;

	// Printer

	// Cash
	static const std::string CCashEnabled;
	static const std::string CCashDev;

	// Scanner
	static const std::string CScannerEnabled;
	static const std::string CScannerDev;

	// TRIND
	static const std::string CTRINDEnabled;
	static const std::string CTRINDDev;

	// LOG
	static const std::string CLogTraceEnabled;
	static const std::string CLogMaxLogFileSize;

private:
	static const std::string CRoot;
};

#endif


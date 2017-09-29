//! GISCommandStatusEnum 
/*!
  Implementation of the Class GISCommandStatusEnum
*/

#if !defined(GISCommandStatusEnum_h)
#define GISCommandStatusEnum_h

#include <QString>

class GISCommandStatusEnum
{

public:
	
	typedef enum
	{
		eNoErrors 											= 0,
		eCommandError 										= 1,
		eDisplayNumberAlreadyUsed 							= 2,
		eScreenNumberAlreadyUsed 							= 3,
		eSegmentNumberAlreadyUsed 							= 4,
		eFieldNumberAlreadyUsed 							= 5,
		eInvalidDisplayNumberOrDisplayNumberDoesNotExist 	= 6,
		eInvalidScreenNumberOrScreenNumberDoesNotExist 		= 7,
		eInvalidSegmentNumberOrSegmentNumberDoesNotExist 	= 8,		
		eInvalidFieldNumberOrFieldNumberDoesNotExist 		= 9,
		eInvalidXPositionOrXRangeExceeded 					= 10,
		eInvalidYPositionOrYRangeExceeded 					= 11,
		eInvalidAttribute 									= 12,		
		eFontNumberUndefined 								= 13,		
		eFontNumberAlreadyUsed 								= 14,
		eVariableLengthDataContainsNonDisplayableCharacters = 15,
		eVideoWindowSizeError 								= 16,		
		eVideoMediaPlayerError 								= 17,
		eVideoNotAvailable 									= 18,
		eDisplayObjectSpaceExceeded 						= 19,
		eDisplayFrameBufferSapceExceeded 					= 20,
		eBusyProcessingACommand 							= 21,
		eDiagnosticTestsComplete 							= 22,
		eInvalidDownloadType 								= 23,
		eBacklightConditionNotDetected 						= 24,
		eSpeechProcessingNotAvailable 						= 25,
		eFlashRAMWriteReadError 							= 26,
		eFlashRAMNotAvailable 								= 27,
		eFlashRAMSapceExceeded 								= 28,
		eImageDecompressionComplete 						= 29,		
		eApplicationSaveComplete 							= 30,
		eDisplayAndVolumeAdjustmentsComplete 				= 31,		
		eInvalidColorOrColorRegister 						= 32,
		eAllExpectedPacketsReceived 						= 33,
		eFontNumberSpecifiedNotExpected 					= 34,
		eSegmentNumberSpecifiedNotExpected 					= 35,
		ePacketNumber1Expected 								= 36,		
		eVariableLengthDataContainsInvalidLength			= 37,
		eHighTemperature 									= 49,
		eVideoSwitchedOn 									= 50,
		
		eLastError

	} eCommandStatusEnum;
	
	GISCommandStatusEnum( );
	GISCommandStatusEnum( eCommandStatusEnum e );
	virtual ~GISCommandStatusEnum();
	
	void SetError( eCommandStatusEnum e );
	eCommandStatusEnum GetError( );
	QString toString();

private:
	
	static const char* strCommandStatusData[eLastError+1];
	eCommandStatusEnum m_eCommandStatusEnumValue;

};
#endif

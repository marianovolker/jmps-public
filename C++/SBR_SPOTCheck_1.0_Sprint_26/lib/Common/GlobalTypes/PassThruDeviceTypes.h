//	===========================================================================
//
//	File Name:		PassThruDeviceTypes.h
// 
//  Description:	This file holds all the common & global define types for
//					PassThru Device & Service specific 
//					
//	Date Created:	12/15/2006
//
//	Author:			Vinod Kolapuram
//
//	===========================================================================


#ifndef PassThruDeviceTypes_h 
#define PassThruDeviceTypes_h 1





// PAC added
/*
It is defined at GFingerprintSensorService. But it should be moved to this place

typedef enum { eLocalPromptEventMsgSize=2,eLocalPromptEventCode=97, eEntryIndexInLocalPromptTableForMsgLiftFinger=7, ePassThruDataEventId=98, 
			   eCode_PassThruDataToSensorOrPassThruDataToTheApp= 0x3F,
			   eMaxPassthruDataPacketSizeToTheApp = 244, eMaxTotalPassThruDataSizeFromSensor = 1024, eMaxPassThruDataPacketSizeFromApp = 200, 
			   eMaxPassThruTotalDataSizeFromApp = 1024, eMaxInitFailureRetryCount = 3 } eFingerprintSensorServiceConstants ;
*/
#define eMaxPassthruDataPacketSizeToTheApp 244


// PassThru Device Ids
typedef enum
{
	eFPSensor   = 0x3e,
  eEMVKernel  = 0x3f
} eDeviceIDs;


// PAC Added
// DeviceID response code
/*
There is a enum defined at GFingerprintSensorService that should be moved to this place.
  and I also believe that it can be split in two different enumeration
	typedef enum { eImageResponseOrDeviceStatusIndicator = 0x31, ePassthruDataFromSensor = 0x33, eImageCaptureOK = 0x20, eImageCaptureFail = 0x21, eNoScanner = 0x22, eScannerError = 0x23, eNoFinger = 0x24, eBiometricsSysOK = 0x30 ,eBiometricsSysOffline = 0x31,
					  eBimetricsSysDown = 0x32, eBiometricsHWDown = 0x33, eUnknownCommandFromSensor=0x3F }eFpSensorResponseOrDeviceStatusCodes ;

*/
enum eDeviceResponseCode
{
// Response codes for FingerPrint
  eFPSensorImageResponse          = 0x31, // it should replace GFingerprintSensorService::eImageResponseOrDeviceStatusIndicator
  eFPSensorPassthruDataResponse   = 0x33, // it should replace GFingerprintSensorService::ePassthruDataFromSensor
// Response codes for EMV Kernel
  eEMVKernelPassthruDataResponse  = 0x40,
};



// FPSensor PassThru Events
typedef enum
{
	eLocalPromptEvent = 0x61,
	ePassThruEventID = 0x62,
	ePassThruError = 0x63
} ePassThruEvents;

// FPSensor PassThru Commands
typedef enum
{
	eEnableSensorCmd = 0x30,
	ePassThruDataCmd = 0x32,
	eEnableFingerprintServiceCmd = 0x34,
	eEnableFHCmd = 0x20,
	eDisableFHCmd = 0x21
} ePassThruCommands;


//PassThru Errors
typedef enum
{
	eUnknownPassThruDevice = 0x48,
	eUnknownPassThruCmd = 0x49,
  eInvalidPassThruPacket = 0x4A,
  ePassThruPacketTimeout = 0x4B
} eErrorIDs;


// FPSensor PassThru Response
typedef enum
{
	ePassThruResp = 0x3F
} ePassThruResponseID;


// Hash defines

#define ERROR_BLOCK_SIZE 4   // Used in ReportPassThruError() and memory allocation for error block

#define START_OF_PAY_LOAD_BYTE 5 // Used to inform FPSensorService the start of payload data

#define FP_SENSOR_COMMAND_BYTE 4 // This is the fifth byte in the parameter block which has the command type value

#define FH_APP_COMMAND_BYTE 5  // This is the sixth byte in the parameter block which has the FH Enable 20 or FH Disable 21

#define PASS_THRU_DEVICE_ID_BYTE 3 // This is the fourth byte in the parameter block which has the passThru device type value

// Debug defines for testing the data integrity and proper flow
//#define FP_SENSOR_DEBUG_FLAG 1

#endif


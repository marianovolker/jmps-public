///////////////////////////////////////////////////////////
//  eSPOTMessageId.h
//  Implementation of the Class eSPOTMessageId
//  Created on:      08-Mar-2007 10:51:26 AM
///////////////////////////////////////////////////////////
typedef enum
{
    eSPOTFirstMessageId = 0x0F0000,
    eSPOTUnknown = eSPOTFirstMessageId,
                                               //App   Cmd   Subcmd
//App Interface commands
    eSPOTLoginRequest,                         //0x00  0x01
    eSPOTLogoutRequest,                        //0x00  0x02
    eSPOTPingRequest,                          //0x00  0x03
    eSPOTLockRequest,                          //0x00  0x04
    eSPOTUnLockRequest,                        //0x00  0x05
//App Interface answers
    eSPOTLoginResponse,                        //0x80  0x01
    eSPOTLogoutResponse,                       //0x80  0x02
    eSPOTPingResponse,                         //0x80  0x03
    eSPOTLockResponse,                         //0x80  0x04
    eSPOTUnLockResponse,                       //0x80  0x05


//PinPad App commands and answers

    eSPOTSoftwareConfigurationRequest,         //0x01  0x10  0x03
    eSPOTSoftwareConfigurationResponse,        //0x81  0x10  0x03
    //Resource commands
    eSPOTFileUploadRequest,                    //0x01  0x00  0x01
    eSPOTFileBlockUploadRequest,               //0x01  0x00  0x02
    eSPOTFileDeleteRequest,                    //0x01  0x00  0x04
    //Resource answers
    eSPOTFileUploadResponse,                   //0x81  0x00  0x01
    eSPOTFileBlockUploadResponse,              //0x81  0x00  0x02
    eSPOTFileDeleteResponse,                   //0x81  0x00  0x04

    //Pinpad System Status request
    eSPOTPinpadSystemStatusRequest,            //0x01  0x10  0x01
    //Pinpad System Hardware Info & Configuration request
    eSPOTPinpadHardwareInfoRequest,            //0x01  0x10  0x02
    //Pinpad System Set Date/Time request
    eSPOTPinpadSetDateTimeRequest,	       //0x01  0x10  0x05
    //Pinpad System Get Date/Time request
    eSPOTPinpadGetDateTimeRequest,	       //0x01  0x10  0x06
    //Pinpad System Status answer
    eSPOTPinpadSystemStatus,                   //0x81  0x10  0x01
    //Pinpad System Hardware Configuration & Info answer
    eSPOTPinpadHardwareInfoAnswer,             //0x81  0x10  0x02
    //Pinpad System Get Date/Time answer
    eSPOTPinpadGetDateTimeResponse,	       //0x81  0x10  0x06

    //Card Reader Commands
    eSPOTCardReaderEnableRequest,              //0x01  0x20  0x01
    eSPOTCardReaderDisableRequest,             //0x01  0x20  0x02
    eSPOTCardReaderChipResetRequest,           //0x01  0x20  0x07
    //Card Reader answers
    eSPOTCardReaderEnableResponse,             //0x81  0x20  0x01
    eSPOTCardReaderDisableResponse,            //0x81  0x20  0x02
    eSPOTCardReaderChipResetResponse,          //0x81  0x20  0x07
    //Card Reader Events
    eSPOTTrackDataEvent,                       //0x81  0x20  0x03

    //Display Management commands
    eSPOTSelectResourceMessageFile,            //0x01  0x30  0x01
    eSPOTCreateWindowRequest,                  //0x01  0x30  0x03
    eSPOTDestroyWindowRequest,                 //0x01  0x30  0x04
    eSPOTDisplayStringRequest,                 //0x01  0x30  0x05
    eSPOTShowWindowRequest,                    //0x01  0x30  0x07
    eSPOTHideWindowRequest,                    //0x01  0x30  0x08
    eSPOTCreateWindowFromFileRequest,          //0x01  0x30  0x0A
    eSPOTSwitchSecureMediaConnector,           //0x01  0x30  0x11
    //Display Management answers
    eSPOTCreateWindowResponse,                 //0x81  0x30  0x03
    eSPOTDestroyWindowResponse,                //0x81  0x30  0x04
    eSPOTDisplayStringResponse,                //0x81  0x30  0x05
    eSPOTShowWindowResponse,                   //0x81  0x30  0x07
    eSPOTHideWindowResponse,                   //0x81  0x30  0x08
    eSPOTCreateWindowFromFileResponse,         //0x81  0x30  0x0A
    eSPOTSwitchSecureMediaResponse,   		   //0x81  0x30  0x11

    //Beep command
    eSPOTBeepRequest,                          //0x01  0x30  0x09
    //Beep answer
    eSPOTBeepResponse,                         //0x81  0x30  0x09

    //Keyboard and security commands
    eSPOTCustomerInputEnableRequest,           //0x01  0x40  0x01
    eSPOTCustomerInputAbortRequest,            //0x01  0x40  0x02
    eSPOTDataEncryptionRequest,                //0x01  0x40  0x03
	eSPOTApplicationkeydownloadRequest,        //0x01  0x40  0x04
	eSPOTKeyDerivationRequest,                //0x01  0x40  0x05
    //Keyboard and security answers, it has input data as well
    eSPOTCustomerInputResponse,                //0x81  0x40  0x01
    eSPOTDataEncryptionResponse,               //0x81  0x40  0x03
	eSPOTKeyDerivationResponse,                //0x81  0x40  0x05

//OPT App commands and answers

    //Term I/O command
    eSPOTSoftKeysRequest,                      //0x04  0x30  0x01

    //Control I/O command
    eSPOTControlIORequest,                     //0x04  0x30  0x02
    eSPOTControlIOResponse,                    //0x84  0x30  0x02

    //RF Card Reader Commands
    eSPOTRFCardReaderEnableRequest,            //0x04  0x50  0x01
    eSPOTRFReaderSetMIFAREReadTable,           //0x04  0x50  0x03
    eSPOTRFCardReaderDisableRequest,           //0x04  0x50  0x05
    //Term I/O answer, it has key data as well
    eSPOTSoftKeysResponse,                     //0x84  x030  0x01

    //OPT Status Request
    eSPOTOPTStatusRequest,                     //0x04  0x10  0x01
    //OPT Status Answer
    eSPOTOPTStatusResponse,                    //0x84  0x10  0x01

    //RF Card Reader Answers
    eSPOTRFCardReaderEnableResponse,           //0x84  0x50  0x01
    eSPOTRFCardReaderDisableResponse,          //0x84  0x50  0x05
    //RF Card Reader Events
    eSPOTRFTrackDataEvent,                     //0x84  0x50  0x04
    //0x84  0x30  0x01


// EFT App Commands

    // EFT KMPB Requests
    eSPOTGenerateKEK,                          //0x05  0x10  0x01
    eSPOTLoadKMPB,                             //0x05  0x10  0x02
    // EFT KMPB Responses
    eSPOTReturnKEK,                            //0x85  0x10  0x01
    eSPOTLoadComplete,                         //0x85  0x10  0x02

    eSPOTLastMessageId
} eSPOTMessageIdTypes;

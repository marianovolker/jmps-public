/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @brief This file contains all the unit test that check the message structure for different types
 *        of messages, without mocking any other part of the system (session, etc)
 *
 * @file MessageStructure_unittest.h
 * @author Guillermo Paris
 * @date Wednesday, March 04, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#ifndef MESSAGESTRUCTURE_UNITTEST_H_
#define MESSAGESTRUCTURE_UNITTEST_H_

#include <gvr/communication/spot/SpotMessageLogin.h>
#include <gvr/communication/spot/SpotMessageLoginResponse.h>
#include <gvr/communication/spot/SpotMessagePing.h>
#include <gvr/communication/spot/SpotMessagePingResponse.h>
#include <gvr/communication/spot/SpotMessageReset.h>
#include <gvr/communication/spot/SpotMessageSystemStatus.h>
#include <gvr/communication/spot/SpotMessageSystemStatusResponse.h>
#include <gvr/communication/spot/SpotMessageHardwareInfo.h>
#include <gvr/communication/spot/SpotMessageHardwareInfoResponse.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfo.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfoResponse.h>
#include <gvr/communication/spot/SpotMessageSoftwareInfo.h>
#include <gvr/communication/spot/SpotMessageSoftwareInfoResponse.h>
#include <gvr/communication/spot/SpotMessageOptSwInfo.h>
#include <gvr/communication/spot/SpotMessageOptSwInfoResponse.h>
#include <gvr/communication/spot/SpotMessagePackageInfo.h>
#include <gvr/communication/spot/SpotMessagePackageInfoResponse.h>
#include <gvr/communication/spot/SpotMessageDebianInfo.h>
#include <gvr/communication/spot/SpotMessageDebianInfoResponse.h>
#include <gvr/communication/spot/SpotMessagePackageActivation.h>
#include <gvr/communication/spot/SpotMessagePackageActivationResponse.h>
#include <gvr/communication/spot/SpotMessagePackageDownload.h>
#include <gvr/communication/spot/SpotMessagePackageDownloadResponse.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownload.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransfer.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransferResponse.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackage.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackageResponse.h>
#include <gvr/communication/spot/SpotMessageFileDownload.h>
#include <gvr/communication/spot/SpotMessageFileDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageFileDelete.h>
#include <gvr/communication/spot/SpotMessageFileDeleteResponse.h>
#include <gvr/communication/spot/SpotMessageFileBlockDownload.h>
#include <gvr/communication/spot/SpotMessageFileBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotMessageNewEphemeralKey.h>
#include <gvr/communication/spot/SpotMessageNewEphemeralKeyResponse.h>
#include <gvr/communication/spot/SpotMessageEphemeralEncrypt.h>
#include <gvr/communication/spot/SpotMessageEphemeralEncryptResponse.h>
#include <gvr/communication/spot/SpotMessageFileUpload.h>
#include <gvr/communication/spot/SpotMessageFileUploadResponse.h>
#include <gvr/communication/spot/SpotMessageFileBlockUpload.h>
#include <gvr/communication/spot/SpotMessageFileBlockUploadResponse.h>

#endif /* MESSAGESTRUCTURE_UNITTEST_H_ */

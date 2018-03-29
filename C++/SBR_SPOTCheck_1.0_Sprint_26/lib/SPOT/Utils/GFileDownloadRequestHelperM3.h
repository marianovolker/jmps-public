/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFileDownloadRequestHelper.h
 * @author Pablo Crespo
 * @date July 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GFileDownloadRequestHelperM3_h
#define GFileDownloadRequestHelperM3_h

#include <GTypes.h>

typedef struct st_Resource_CertM3 ResourceCertM3;

class GFileDownloadRequestHelperM3
{
public:
	GFileDownloadRequestHelperM3();
	virtual ~GFileDownloadRequestHelperM3();
	
	// Given a resource file this method get the resource information from the resMark
	// then in a second step it looks for the Authentication field within the corresponding .lic file
	// szPPN serial number has to be provided in the following form "00022000"
	// if szPPNN is set to NULL the .lic files are omitted
	bool ProcessResourceFile(const char* szResourceFileName, const char* szPPN);
	// file unique identification number
	UINT8 GetFileId();
	// file size without the resmark record
	UINT32 GetFileSize();
	// file CRC defined in the resmark
	UINT32 GetCRC();	
	// AuthType defined in the resmark
	UINT8 GetAuthenType();
	// Get the ANSI 9.19 MAC from the reskmark or from the lic file if present
	UINT32 GetAuthentication();
	// Get Description from the resmark
	char* GetDescription();
	//returns the size of the reskmark field in the resource file
	static UINT16 GetResmarkSize();


private:
	ResourceCertM3* m_stResourceCert;
	UINT32 m_u32FileSize;
	UINT32 m_u32Crc;
	UINT32 m_u32Auth;
};

#endif // GFileDownloadRequestHelperM3_h

/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFileDownloadRequestHelperM5.h
 * @author Pablo Crespo
 * @date July 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GFileDownloadRequestHelperM5_h
#define GFileDownloadRequestHelperM5_h

#include <GTypes.h>

typedef struct st_Resource_CertM5 ResourceCertM5;

class GFileDownloadRequestHelperM5
{
public:
	static const UINT8 CAuthLen = 248;
	GFileDownloadRequestHelperM5();
	virtual ~GFileDownloadRequestHelperM5();
	
	// Given a resource file this method get the resource information from the resMark
	bool ProcessResourceFile(const char* szResourceFileName);
	// file unique identification number
	UINT8 GetFileId();
	// file size without the resmark record
	UINT32 GetFileSize();
	// file CRC defined in the resmark
	UINT32 GetCRC();	
	// AuthType defined in the resmark
	UINT8 GetAuthenType();
	// Get RSA certificate 
	UINT8* GetAuthentication(UINT8* pBuffer, UINT8 u8BufferLen);
	// Get Description from the resmark
	char* GetDescription();
	//returns the size of the reskmark field in the resource file
	static UINT16 GetResmarkSize();

private:
	ResourceCertM5* m_stResourceCert;
	UINT32 m_u32FileSize;
	UINT32 m_u32Crc;
};

#endif // GFileDownloadRequestHelperM5_h

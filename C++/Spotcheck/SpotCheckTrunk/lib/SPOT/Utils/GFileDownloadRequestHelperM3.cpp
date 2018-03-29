/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFileDownloadRequestHelperM3.h
 * @author Pablo Crespo
 * @date July 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include "GFileDownloadRequestHelperM3.h"
#include <stdio.h>
#include <string.h>
#include "LicFileUtils.h"
#include "Common/Util/DEBUGOUT.h"
 
#define RESMARK "/resMark"

#pragma pack(1)
struct st_Resource_CertM3
{
	char szResMarkId[8];
	UINT8 u8FileId;
	UINT8 u8AuthenType; //for m3 0, 01 or 02 are the only valid options
	UINT8 aryCRC[4];
	UINT8 aryMac[4];	//on a production resource the mac is really part of the .lic file
	char szFileDescription[16];
};
#pragma pack()

GFileDownloadRequestHelperM3::GFileDownloadRequestHelperM3()
 : m_u32FileSize(0), m_u32Crc(0), m_u32Auth(0)
{
	m_stResourceCert = new ResourceCertM3;
	memset(m_stResourceCert, 0, 	sizeof(ResourceCertM3));
}

GFileDownloadRequestHelperM3::~GFileDownloadRequestHelperM3()
{
	if( m_stResourceCert != NULL )
	{
		delete m_stResourceCert;
		m_stResourceCert = NULL;
	}
}

bool GFileDownloadRequestHelperM3::ProcessResourceFile(const char* szResourceFileName, const char* szPPN)
{
	//clean up members
	memset(m_stResourceCert, 0, 	sizeof(ResourceCertM3));
	m_u32FileSize = 0;
	m_u32Crc = 0;
	m_u32Auth = 0;
	
	//first step is to get the resource information from the resmark
	FILE* resource_fd = fopen( szResourceFileName ,"rb");
	if( resource_fd == NULL )
	{
		LOG(eUnCategorized, eError, "File [%s] could not be opened.", szResourceFileName);
		return false;
	}

	//set the cursor at the end of the file to read the resmark field
	int iOffset = sizeof(ResourceCertM3);
	fseek(resource_fd, -iOffset, SEEK_END);
	fread(m_stResourceCert, iOffset, 1, resource_fd);

	//if the resmark field cannot be found there was a problem
	if(strncmp(m_stResourceCert->szResMarkId, RESMARK, sizeof(m_stResourceCert->szResMarkId)) != 0)
	{
		LOG(eUnCategorized, eError, "File [%s] without RESMARK record.", szResourceFileName);
		fclose(resource_fd);
		return false;
	}

	//prepare Crc and Mac
	m_u32Crc = ((m_stResourceCert->aryCRC[0] << 24)|(m_stResourceCert->aryCRC[1] << 16)|(m_stResourceCert->aryCRC[2] <<  8)| m_stResourceCert->aryCRC[3]);
	m_u32Auth = ((m_stResourceCert->aryMac[0] << 24) | (m_stResourceCert->aryMac[1] << 16) | (m_stResourceCert->aryMac[2] <<  8) | m_stResourceCert->aryMac[3]);

	if( szPPN != NULL && strlen(szPPN) != 0 )
	{
		// if file .lic does not exist the function fails
		// now prepare pointers and data
		struct LicFileData *pslfd;
		int iNE;
		LicFileStatus lfs=LicFileFetch(szResourceFileName, szPPN, pslfd, &iNE);
		// if a .lic file was found then override the authentication
		if( lfs == eFileResource && pslfd != NULL )
		{
			m_u32Auth = ((pslfd->byArr[0] << 24) | (pslfd->byArr[1] << 16) | (pslfd->byArr[2] <<  8) | pslfd->byArr[3] );
		}
		LicFileFree(pslfd);
	}
	m_u32FileSize = ftell(resource_fd) - sizeof(ResourceCertM3);
	fclose(resource_fd);

	return true;
}
	
UINT8 GFileDownloadRequestHelperM3::GetFileId()
{
	return m_stResourceCert->u8FileId;
}

UINT32 GFileDownloadRequestHelperM3::GetFileSize()
{
	return m_u32FileSize;
}
	
UINT32 GFileDownloadRequestHelperM3::GetCRC()
{
	return m_u32Crc;
}

UINT8 GFileDownloadRequestHelperM3::GetAuthenType()
{
	return m_stResourceCert->u8AuthenType;
}


UINT32 GFileDownloadRequestHelperM3::GetAuthentication()
{
	return m_u32Auth;
}

char* GFileDownloadRequestHelperM3::GetDescription()
{
	return m_stResourceCert->szFileDescription;
}

UINT16 GFileDownloadRequestHelperM3::GetResmarkSize()
{
	return sizeof(ResourceCertM3);
}
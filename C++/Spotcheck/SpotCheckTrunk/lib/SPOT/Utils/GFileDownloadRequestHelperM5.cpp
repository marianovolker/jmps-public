/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GFileDownloadRequestHelperM5.h
 * @author Pablo Crespo
 * @date July 30 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#include "GFileDownloadRequestHelperM5.h"
#include <stdio.h>
#include <string.h>
#include "LicFileUtils.h"
#include "Common/Util/DEBUGOUT.h"
 
#define RESMARK "/resMark"

#pragma pack(1)
struct st_Resource_CertM5
{
	char szResMarkId[8];
	UINT8 u8FileId;
	UINT8 u8AuthenType; //for m5 0, 03 or 04 are the only valid options
	UINT8 aryCRC[4];
	UINT8 aryRSACertificate[GFileDownloadRequestHelperM5::CAuthLen];
	char szFileDescription[16];
};
#pragma pack()

GFileDownloadRequestHelperM5::GFileDownloadRequestHelperM5()
 : m_u32FileSize(0), m_u32Crc(0)
{
	m_stResourceCert = new ResourceCertM5;
	memset(m_stResourceCert, 0, sizeof(ResourceCertM5));
}

GFileDownloadRequestHelperM5::~GFileDownloadRequestHelperM5()
{
	if( m_stResourceCert != NULL )
	{
		delete m_stResourceCert;
		m_stResourceCert = NULL;
	}
}

bool GFileDownloadRequestHelperM5::ProcessResourceFile(const char* szResourceFileName)
{
	//clean up members
	memset(m_stResourceCert, 0, sizeof(ResourceCertM5));
	m_u32FileSize = 0;
	m_u32Crc = 0;
	
	//first step is to get the resource information from the resmark
	FILE* resource_fd = fopen( szResourceFileName ,"rb");
	if( resource_fd == NULL )
	{
		LOG(eUnCategorized, eError, "File [%s] could not be opened.", szResourceFileName);
		return false;
	}

	//set the cursor at the end of the file to read the resmark field
	int iOffset = sizeof(ResourceCertM5);
	fseek(resource_fd, -iOffset, SEEK_END);
	fread(m_stResourceCert, iOffset, 1, resource_fd);

	//if the resmark field cannot be found there was a problem
	if(strncmp(m_stResourceCert->szResMarkId, RESMARK, sizeof(m_stResourceCert->szResMarkId)) != 0)
	{
		LOG(eUnCategorized, eError, "File [%s] without RESMARK record.", szResourceFileName);
		fclose(resource_fd);
		return false;
	}

	//get CRC
	m_u32Crc = ((m_stResourceCert->aryCRC[0] << 24)|(m_stResourceCert->aryCRC[1] << 16)|(m_stResourceCert->aryCRC[2] <<  8)| m_stResourceCert->aryCRC[3]);

	m_u32FileSize = ftell(resource_fd) - sizeof(ResourceCertM5);
	fclose(resource_fd);

	return true;
}
	
UINT8 GFileDownloadRequestHelperM5::GetFileId()
{
	return m_stResourceCert->u8FileId;
}

UINT32 GFileDownloadRequestHelperM5::GetFileSize()
{
	return m_u32FileSize;
}
	
UINT32 GFileDownloadRequestHelperM5::GetCRC()
{
	return m_u32Crc;
}

UINT8 GFileDownloadRequestHelperM5::GetAuthenType()
{
	return m_stResourceCert->u8AuthenType;
}

UINT8* GFileDownloadRequestHelperM5::GetAuthentication(UINT8* pBuffer, UINT8 u8BufferLen)
{
	if( u8BufferLen < CAuthLen )
		return NULL;

	memcpy(pBuffer, m_stResourceCert->aryRSACertificate, CAuthLen);
	return pBuffer;
}

char* GFileDownloadRequestHelperM5::GetDescription()
{
	return m_stResourceCert->szFileDescription;
}

UINT16 GFileDownloadRequestHelperM5::GetResmarkSize()
{
	return sizeof(ResourceCertM5);
}

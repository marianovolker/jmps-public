#include <stdio.h>
#include "Common/Util/GMD5.h"

GMD5::GMD5()
{
	m_bInitialized = false;
}

GMD5::~GMD5()
{
	unsigned char md[MD5_DIGEST_LENGTH];

	if( m_bInitialized )
	{
		md5_finish(&m_Context, md);
	}
}

UINT8* GMD5::GetHashValue(UINT8* pu8BufferToHash, UINT32 u32BufferLength)
{
	unsigned char md[MD5_DIGEST_LENGTH];
	md5_state_t state;

	md5_init(&state);
	md5_append(&state, (md5_byte_t*)pu8BufferToHash, u32BufferLength);
	md5_finish(&state, md);
	GetHexHashValue(md, m_u8HashValue);
	return m_u8HashValue;
}

UINT32 GMD5::GetHashLength()
{
	return (UINT32)(MD5_DIGEST_LENGTH*2);
}


void GMD5::Init()
{
	if( !m_bInitialized )
	{
		m_bInitialized = true;
		md5_init(&m_Context);
	}
}

void GMD5::UpdateBuffer(UINT8* puBufferChunk, UINT32 u32ChunkLength)
{
	if( m_bInitialized )
	{
		md5_append(&m_Context, (md5_byte_t*)puBufferChunk, u32ChunkLength);
	}
}

UINT8* GMD5::GetHashValue()
{
	unsigned char md[MD5_DIGEST_LENGTH];

	if( m_bInitialized )
	{
		md5_finish(&m_Context, md);
		m_bInitialized = false;
		GetHexHashValue(md, m_u8HashValue);
		return m_u8HashValue;
	}
	return NULL;
}

void GMD5::GetHexHashValue(unsigned char* pHash, UINT8* pHexHash)
{
	int i;

	for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{		
		sprintf((char*)&pHexHash[i*2], "%02x", pHash[i]);
	}
	pHexHash[i*2] = 0;
}

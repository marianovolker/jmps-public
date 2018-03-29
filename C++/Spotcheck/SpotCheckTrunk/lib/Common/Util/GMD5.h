//	=============================================================================
//
//	File: GMD5
//
//	Description: Specialization of GAbsHashAlgorithm. It implemets MD5 algorithm
//
//	Created: 10/18/2006
//
//	Author: Pablo Crespo
//
//	=============================================================================

#ifndef GMD5_h
#define GMD5_h 1

#include "GTypes.h"
#include "Common/Util/GAbsHashAlgorithm.h"

//it is a proprietary implementation of md5
//It was used to avoid the necessity of include openssl to crosstool
#include "Common/Util/md5.h"

#define MD5_DIGEST_LENGTH 16
class GMD5 : public GAbsHashAlgorithm
{
public:
	GMD5();
	virtual ~GMD5();

	/**
	It returns a hash value generated from a raw buffer
	*/
	UINT8* GetHashValue(UINT8* pu8BufferToHash, UINT32 u32BufferLength);
	/**
	It returns the hash value length in hex
	*/
	UINT32 GetHashLength();


	/**
	It must be called before call UpdateBuffer
	*/	
	void Init();
	/**
	It can be called repeatedly with chunks of the buffer. It must be used when the buffer to hash
	can't be held in memory 
	*/
	void UpdateBuffer(UINT8* puBufferChunk, UINT32 u32ChunkLength);
	/**
	It gets the hash value for the Buffer that was updated using UpdateBuffer
	*/	
	UINT8* GetHashValue();
	//
private:
	
	void GetHexHashValue(unsigned char* pHash, UINT8* pHexHash);	

	UINT8 m_u8HashValue[MD5_DIGEST_LENGTH*2+1];
	md5_state_t m_Context;
	bool m_bInitialized;
};

#endif

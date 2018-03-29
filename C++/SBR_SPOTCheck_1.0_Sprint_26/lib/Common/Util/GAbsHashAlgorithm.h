//	=============================================================================
//
//	File: GAbsHashAlgorithm
//
//	Description: Interface for Hash Algorithms
//
//	Created: 10/18/2006
//
//	Author: Pablo Crespo
//
//	=============================================================================

#ifndef GAbsHashAlgorithm_h
#define GAbsHashAlgorithm_h 1

#include "GTypes.h"

class GHashAlgorithmFactory;

class GAbsHashAlgorithm
{
public:
	/**
	It returns a hash value generated from a raw buffer
	*/
	virtual UINT8* GetHashValue(UINT8* pu8BufferToHash, UINT32 u32BufferLength) = 0;
	/**
	It returns the hash value length
	*/
	virtual UINT32 GetHashLength() = 0;
	
	/**
	It must be called before call UpdateBuffer
	*/	
	virtual void Init() = 0;
	/**
	It can be called repeatedly with chunks of the buffer. It must be used when the buffer to hash
	can't be held in memory 
	*/
	virtual void UpdateBuffer(UINT8* puBufferChunk, UINT32 u32ChunkLength) = 0;
	/**
	It gets the hash value for the Buffer that was updated using UpdateBuffer
	*/	
	virtual UINT8* GetHashValue() = 0;
	
	//
	friend class GHashAlgorithmFactory;
protected:
	GAbsHashAlgorithm(){;};
	virtual ~GAbsHashAlgorithm(){;};
};

#endif

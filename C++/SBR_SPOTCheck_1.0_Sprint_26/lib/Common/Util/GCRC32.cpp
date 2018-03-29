#include "Common/Util/GCRC32.h"

UINT32 GCRC32::Calculate(UINT8 *pu8Buffer, UINT32 u32Len, UINT32 u32Crc)
{
	UINT8* p = pu8Buffer;

	u32Crc = CRC32_INITIAL_VALUE;
	while( u32Len > 0 )
	{
		u32Crc = GCRC32::Update(*p, u32Crc);
		++p;
		--u32Len;
	}
	return (u32Crc ^ CRC32_FINAL_XOR_VALUE);
}

UINT32 GCRC32::Update(UINT8 u8, UINT32 u32Crc)
{
	return ((u32Crc >> 8) & 0x00FFFFFFFF) ^ g_aryu32crc32[(u32Crc ^ u8) & 0xFF];
}

void GCRC32::Initiate(UINT32* u32Crc)
{
	*u32Crc = CRC32_INITIAL_VALUE;
}

void GCRC32::Update(UINT8 *pu8Buffer, UINT32 u32Len, UINT32* u32Crc)
{
	UINT8* p = pu8Buffer;

	while( u32Len > 0 )
	{
		*u32Crc = GCRC32::Update(*p, *u32Crc);
		++p;
		--u32Len;
	}
}

void GCRC32::Finalize(UINT32* u32Crc)
{
	(*u32Crc) = (*u32Crc) ^ CRC32_FINAL_XOR_VALUE;
}

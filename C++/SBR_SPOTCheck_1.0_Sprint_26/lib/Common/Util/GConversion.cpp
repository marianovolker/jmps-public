// GConversion.cpp

#include <ctype.h>
#include <string.h>

// GSystem
#include "GSystem.h"
// GConversion
#include "Common/Util/GConversion.h"
// DEBUGOUT
#include "Common/Util/DEBUGOUT.h"


// Class GConversion
void GConversion::UIToA (UINT32 u32Val, char* pcBuffer)
{
	char	arycRevNum[10];		// Max 10 digits in a UINT32
	INT32	s32DigCnt		= 0;

	do {
		// Convert to ASCII and store.
		arycRevNum[s32DigCnt++] = (u32Val % 10) + '0';

		// Advance to the next digit.
		u32Val /= 10;
	} while (u32Val > 0);

	// Reverse the digits
	UINT32 u32BuffPos	= 0;

	while (--s32DigCnt >= 0)
	{
		pcBuffer[u32BuffPos++] = arycRevNum[s32DigCnt];
	}

	pcBuffer[u32BuffPos] = '\0';
}

UINT8 GConversion::ASCIIHexDigitToDecimal (UINT8 byASCIIDigit)
{

	if((byASCIIDigit >= '0') && (byASCIIDigit <= '9'))
		return(byASCIIDigit - '0');
	else if ((byASCIIDigit >= 'A') && (byASCIIDigit <= 'F'))
		return(byASCIIDigit - 'A' + 10);
	else if ((byASCIIDigit >= 'a') && (byASCIIDigit <= 'f'))
		return(byASCIIDigit - 'a' + 10);
	else
	{
		LOG(eCommon, eError,"GConversion::ASCIIHexDigitToDecimal() - Invalid hex digit, returning 0\n");
		return(0);
	}
}

UINT8 GConversion::ASCIIHexByteToDecimal (UINT8* pbyASCIIHexByte)
{
	return( (UINT8) ( ASCIIHexDigitToDecimal(pbyASCIIHexByte[0]) * 16 ) +
		ASCIIHexDigitToDecimal(pbyASCIIHexByte[1]) );
}

UINT32 GConversion::ASCIIHexStringToBytes(UINT8* out_pu8Buffer, const char* szHexData)
{
	bool   blLastNibble = false;
	int    index = 0;
	char   c = '\0';
	const char*  psz = szHexData;
	UINT8  bh = 0, bl = 0;

	while( (c = *psz++) != '\0' )
	{
		if( !isHexDigit(c) )
			continue; // Any non hex digit is skipped (this allows to have separators between bytes!)

		if( !blLastNibble )  // first nibble
		{
			bh = HexNibbleToByte(c);
			blLastNibble = true;
		}
		else // last nibble
		{
			bl = HexNibbleToByte(c);
			blLastNibble = false;
			out_pu8Buffer[index++] =  UINT8(16 * bh + bl);
		}
	}

	if(blLastNibble) // last digit was alone ( no matching low nibble digit )
		out_pu8Buffer[index] =  bh;

	return (UINT32)index;
}


char* GConversion::BytesToASCIIHexString(char* pcbuffer, const UINT8* pu8Buffer, const UINT16 u8BufferLen)
{
	if (!pcbuffer) return NULL;

	int i = 0, n = 0;

	for (; i < u8BufferLen; i++)
	{
		n += sprintf(pcbuffer + n, "%02X ", pu8Buffer[i]);
	}

	return pcbuffer;
}

void GConversion::LogBytesAsASCIIHexString(const UINT8* pu8Buffer, const UINT16 u8BufferLen)
{
	UINT16 u16Len = ((u8BufferLen*3)+1);

	char* pcDataAux = new char[u16Len];

	memset(pcDataAux, 0x00, u16Len);

	BytesToASCIIHexString (pcDataAux, pu8Buffer, u8BufferLen);

	LOG(eCommon, eDebug, "[%s]\n", pcDataAux);

	delete[] pcDataAux;
}

UINT8 GConversion::HexNibbleToByte(char cDigit)
{
	if((cDigit >= '0') && (cDigit <= '9'))
		return(cDigit - '0');
	else if ((cDigit >= 'A') && (cDigit <= 'F'))
		return(cDigit - 'A' + 10);
	else if ((cDigit >= 'a') && (cDigit <= 'f'))
		return(cDigit - 'a' + 10);
	else
		return(0xFF);  // Invalid digit
}

bool GConversion::isHexDigit(char cDigit)
{
	if((cDigit >= '0') && (cDigit <= '9'))
		return true;
	else if ((cDigit >= 'A') && (cDigit <= 'F'))
		return true;
	else if ((cDigit >= 'a') && (cDigit <= 'f'))
		return true;
	else
		return false;
}

INT8 GConversion::ConvertToASCII (INT8 cByteToConvert, bool bUpperNibble)
{
	INT8 cResult;

	if (bUpperNibble)
	{
		// mask off lower nibble and shift upper nibble to
		// lower nibble
		cResult = (cByteToConvert & 0xF0) >> 4;
	}
	else
	{
		// mask off upper nibble
		cResult = cByteToConvert & 0x0F;
	}

	// convert the nibble to an ascii character
	if (cResult >= 0 && cResult <= 9)
	{
		// if a digit (0-9)
		cResult += 0x30;
	}
	else
	{
		// if a character (a-f)
		cResult += 0x37;  //NOTE not sure if this is to be upper or lower case characters assumed upper.
	}
	return cResult;
}

bool GConversion::ConvertToDecimalASCII (UINT32 u32Number, UINT8* pu8ASCIIBuffer, UINT8 u8ASCIIBufferLength)
{
   int nIndex = u8ASCIIBufferLength - 1;
   while (u32Number)
   {
      if (nIndex < 0)
      {
         // u32Number is too big to fit in pu8ASCIIBuffer
         LOG(eUnCategorized, eError, "GConversion::ConvertToDecimalAscii() - Number too large for buffer, conversion failed\n");
         return false;
      }
      UINT8 u8DecimalDigit = u32Number % 10;
		pu8ASCIIBuffer[nIndex--] = u8DecimalDigit + 0x30;
      u32Number /= 10;
   }
   // Pad the remaining bytes with 0's
   while (nIndex >= 0)
		pu8ASCIIBuffer[nIndex--] = 0x30;

   return true;
}

UINT16 GConversion::BuildUINT16 (UINT8* pbyData)
{
    return (UINT16) ( *pbyData )
        |  ( ( (UINT16) *(pbyData + 1) ) << 8 );
}

UINT32 GConversion::BuildUINT32 (UINT8* pbyData)
{
    return (UINT32) ( *pbyData )
        |  ( ( (UINT32) *(pbyData + 1) ) << 8 )
        |  ( ( (UINT32) *(pbyData + 2) ) << 16 )
        |  ( ( (UINT32) *(pbyData + 3) ) << 24 );
}

UINT16 GConversion::HostToLittleEndian(UINT16 Data)
{
   UINT16 LEData;

   // Conversion needed if host is big endian.
   if (GSystem::HostIsBigEndian())
   {
      LEData = ((Data << 8) & 0xFF00) | ((Data >> 8) & 0x00FF);
   }
   else
   {
      LEData = Data;
   }

   return LEData;
}

UINT16 GConversion::LittleEndianToHost(UINT16 Data)
{
   UINT16 LEData;

   // Conversion needed if host is big endian.
   if (GSystem::HostIsBigEndian())
   {
      LEData = ((Data << 8) & 0xFF00) | ((Data >> 8) & 0x00FF);
   }
   else
   {
      LEData = Data;
   }

   return LEData;
}

UINT32 GConversion::HostToLittleEndian(UINT32 Data)
{
   UINT32 LEData;

   // Conversion needed if host is big endian.
   if (GSystem::HostIsBigEndian())
   {
      LEData = ((Data << 24) & 0xFF000000) | ((Data <<  8) & 0x00FF0000) |
               ((Data >>  8) & 0x0000FF00) | ((Data >> 24) & 0x000000FF);
   }
   else
   {
      LEData = Data;
   }

   return LEData;
}

UINT32 GConversion::LittleEndianToHost(UINT32 Data)
{
   UINT32 LEData;

   // Conversion needed if host is big endian.
   if (GSystem::HostIsBigEndian())
   {
      LEData = ((Data << 24) & 0xFF000000) | ((Data <<  8) & 0x00FF0000) |
               ((Data >>  8) & 0x0000FF00) | ((Data >> 24) & 0x000000FF);
   }
   else
   {
      LEData = Data;
   }

   return LEData;
}


UINT16 GConversion::HostToBigEndian(UINT16 Data)
{
   UINT16 BEData;

   // Conversion needed if host is little endian.
   if (GSystem::HostIsLittleEndian())
   {
      BEData = ((Data << 8) & 0xFF00) | ((Data >> 8) & 0x00FF);
   }
   else
   {
      BEData = Data;
   }

   return BEData;
}

UINT16 GConversion::BigEndianToHost(UINT16 Data)
{
   UINT16 BEData;

   // Conversion needed if host is little endian.
   if (GSystem::HostIsLittleEndian())
   {
      BEData = ((Data << 8) & 0xFF00) | ((Data >> 8) & 0x00FF);
   }
   else
   {
      BEData = Data;
   }

   return BEData;
}

UINT32 GConversion::HostToBigEndian(UINT32 Data)
{
   UINT32 BEData;

   // Conversion needed if host is little endian.
   if (GSystem::HostIsLittleEndian())
   {
      BEData = ((Data << 24) & 0xFF000000) | ((Data <<  8) & 0x00FF0000) |
               ((Data >>  8) & 0x0000FF00) | ((Data >> 24) & 0x000000FF);
   }
   else
   {
      BEData = Data;
   }

   return BEData;
}

UINT32 GConversion::BigEndianToHost(UINT32 Data)
{
   UINT32 BEData;

   // Conversion needed if host is little endian.
   if (GSystem::HostIsLittleEndian())
   {
      BEData = ((Data << 24) & 0xFF000000) | ((Data <<  8) & 0x00FF0000) |
               ((Data >>  8) & 0x0000FF00) | ((Data >> 24) & 0x000000FF);
   }
   else
   {
      BEData = Data;
   }

   return BEData;
}

void GConversion::ConvertHexBufferToDecimalBuffer(const UINT8* pu8HexBuffer, UINT32 u32HexBufferLength, UINT8* pu8OutDecimalBuffer)
{
  UINT32 u32DecodedDataLength = u32HexBufferLength / 2;
  UINT8 u8HI, u8LO;
  int iHI, iLO;
  for (int i=0; i < u32DecodedDataLength; i++)
  {
    u8HI = pu8HexBuffer[i * 2];
    u8LO = pu8HexBuffer[i * 2 + 1];
    iHI = isdigit(u8HI) ? (u8HI - '0') : ((toupper(u8HI) - 'A') + 10);
    iLO = isdigit(u8LO) ? (u8LO - '0') : ((toupper(u8LO) - 'A') + 10);
    pu8OutDecimalBuffer[i] = (UINT8) (iLO + (iHI * 16));
  }
}

void GConversion::ConvertDecimalBufferToHexBuffer(const UINT8* pu8DecimalBuffer, UINT32 u32DecimalBufferLength, UINT8* pu8OutHexBuffer)
{
  int iHI, iLO;
  for (int i=0; i < u32DecimalBufferLength; i++)
  {
    iHI = pu8DecimalBuffer[i] / 16;
    iLO = pu8DecimalBuffer[i] % 16;
    pu8OutHexBuffer[i * 2] = (iHI < 10) ? ('0' + iHI) : ('A' + (iHI - 10));
    pu8OutHexBuffer[i * 2 + 1] = (iLO < 10) ? ('0' + iLO) : ('A' + (iLO - 10));

  }
}

void GConversion::xtoa (unsigned long val,
                        char *buf,
                        unsigned radix,
                        int is_neg)
{
        char *p;                /* pointer to traverse string */
        char *firstdig;         /* pointer to first digit */
        char temp;              /* temp char */
        unsigned digval;        /* value of digit */

        p = buf;

        if (is_neg) {
            /* negative, so output '-' and negate */
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}


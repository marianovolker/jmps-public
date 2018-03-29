#include "Common/Util/GEncoder.h"
#include "Common/Util/DEBUGOUT.h"
#include <string.h>

/*
Encoding table for non ascii characters
Char	Crind Code	UTF8 Code
====  ==========  =========
�	    AD          C2A1
�	    9B	        C2A2
�	    9C	        C2A3
�	    9D	        C2A5
P	    9E	        C2A6
f	    9F	        C2A7
�	    A6	        C2AA
�	    A7	        C2BA
�	    AC	        C2BC
�	    AB	        C2BD
�	    A8	        C2BF
�	    8E	        C382
�	    92	        C386
�	    80	        C387
�     90          C389
�	    A5	        C391
�	    99	        C396
�	    9A	        C39B
�	    85	        C3A0
�	    A0	        C3A1
�	    83	        C3A2
�	    84	        C3A4
�	    86	        C3A5
�	    91	        C3A6
�	    87	        C3A7
�	    8A	        C3A8
�	    82	        C3A9
�	    88	        C3AA
�	    89	        C3AB
�	    A1	        C3AD
�	    8C	        C3AE
�	    8B	        C3AF
�	    A4	        C3B1
�	    95	        C3B2
�	    A2	        C3B3
�	    93	        C3B4
�	    94	        C3B6
�	    97	        C3B9
�	    A3	        C3BA
�	    96	        C3BB
�	    98	        C3BF
*/
const int GEncoder::CUTF8EncodedLength = 2;
bool GEncoder::m_bInit = false;
EncoderMap GEncoder::m_Crind2UTF8Index;
EncoderMap::iterator GEncoder::m_iEnd;
GMutex GEncoder::m_mutex;

const char g_Crind2UTF8Encoding[][GEncoder::CUTF8EncodedLength] = { {0xC2,0xA1}, {0xC2,0xA2}, {0xC2,0xA3},
                                          {0xC2,0xA5}, {0xC2,0xA6}, {0xC2,0xA7},
                                          {0xC2,0xAA}, {0xC2,0xBA}, {0xC2,0xBC},
                                          {0xC2,0xBD}, {0xC2,0xBF}, {0xC3,0x82},
                                          {0xC3,0x86}, {0xC3,0x87}, {0xC3,0x89},
                                          {0xC3,0x91}, {0xC3,0x96}, {0xC3,0x9B},
                                          {0xC3,0xA0}, {0xC3,0xA1}, {0xC3,0xA2},
                                          {0xC3,0xA4}, {0xC3,0xA5}, {0xC3,0xA6},
                                          {0xC3,0xA7}, {0xC3,0xA8}, {0xC3,0xA9},
                                          {0xC3,0xAA}, {0xC3,0xAB}, {0xC3,0xAD},
                                          {0xC3,0xAE}, {0xC3,0xAF}, {0xC3,0xB1},
                                          {0xC3,0xB2}, {0xC3,0xB3}, {0xC3,0xB4},
                                          {0xC3,0xB6}, {0xC3,0xB9}, {0xC3,0xBA},
                                          {0xC3,0xBB}, {0xC3,0xBF} };


GEncoder::GEncoder()
{
	m_mutex.Lock();
	if( !m_bInit )
	{
		m_Crind2UTF8Index[0xAD] = 0;
		m_Crind2UTF8Index[0x9B] = 1;
		m_Crind2UTF8Index[0x9C] = 2;
		m_Crind2UTF8Index[0x9D] = 3;
		m_Crind2UTF8Index[0x9E] = 4;
		m_Crind2UTF8Index[0x9F] = 5;
		m_Crind2UTF8Index[0xA6] = 6;
		m_Crind2UTF8Index[0xA7] = 7;
		m_Crind2UTF8Index[0xAC] = 8;
		m_Crind2UTF8Index[0xAB] = 9;
		m_Crind2UTF8Index[0xA8] = 10;
		m_Crind2UTF8Index[0x8E] = 11;
		m_Crind2UTF8Index[0x92] = 12;
		m_Crind2UTF8Index[0x80] = 13;
		m_Crind2UTF8Index[0x90] = 14;
		m_Crind2UTF8Index[0xA5] = 15;
		m_Crind2UTF8Index[0x99] = 16;
		m_Crind2UTF8Index[0x9A] = 17;
		m_Crind2UTF8Index[0x85] = 18;
		m_Crind2UTF8Index[0xA0] = 19;
		m_Crind2UTF8Index[0x83] = 20;
		m_Crind2UTF8Index[0x84] = 21;
		m_Crind2UTF8Index[0x86] = 22;
		m_Crind2UTF8Index[0x91] = 23;
		m_Crind2UTF8Index[0x87] = 24;
		m_Crind2UTF8Index[0x8A] = 25;
		m_Crind2UTF8Index[0x82] = 26;
		m_Crind2UTF8Index[0x88] = 27;
		m_Crind2UTF8Index[0x89] = 28;
		m_Crind2UTF8Index[0xA1] = 29;
		m_Crind2UTF8Index[0x8C] = 30;
		m_Crind2UTF8Index[0x8B] = 31;
		m_Crind2UTF8Index[0xA4] = 32;
		m_Crind2UTF8Index[0x95] = 33;
		m_Crind2UTF8Index[0xA2] = 34;
		m_Crind2UTF8Index[0x93] = 35;
		m_Crind2UTF8Index[0x94] = 36;
		m_Crind2UTF8Index[0x97] = 37;
		m_Crind2UTF8Index[0xA3] = 38;
		m_Crind2UTF8Index[0x96] = 39;
		m_Crind2UTF8Index[0x98] = 40;
		m_iEnd = m_Crind2UTF8Index.end();
		m_bInit = true;
	}
	m_mutex.Unlock();
}

GEncoder::~GEncoder()
{
}

void GEncoder::Encode2UTF8(const char* pString, string *str)
{
	UINT16 u16Length = strlen(pString);
	UINT16 u16Pointer = 0;
	char szCode[8];
	map<UINT8, UINT8>::iterator iter;

	m_mutex.Lock();
	for( UINT16 u16Index = 0; u16Index < u16Length; ++u16Index )
	{
		UINT8 u8Char = UINT8(pString[u16Index]);
		if( u8Char > 0x7F )
		{
			iter = m_Crind2UTF8Index.find(u8Char);
			if( iter == m_iEnd )
			{
				LOG(ePOSInterface, eError, "%s: Character %x wasn't found!!!\n", __PRETTY_FUNCTION__, int(u8Char));
				str->append(1, '#'); //dummy character
			}
			else
			{
				memcpy(szCode, g_Crind2UTF8Encoding[iter->second], CUTF8EncodedLength);
				szCode[CUTF8EncodedLength] = 0;
				str->append(szCode);
			}
		}
		else
		{
			str->append(1, pString[u16Index]);
		}
	}
	m_mutex.Unlock();
}

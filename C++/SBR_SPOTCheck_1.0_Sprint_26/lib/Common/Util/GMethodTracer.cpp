#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "Common/Util/GMethodTracer.h"
#include "Common/Util/DEBUGOUT.h"

const char* pcEmpty = "\0\0\0"; // 4 bytes empty string

#ifndef _DISABLE_DEBUGOUT

const char Gmtr::ms_acArgSeparator[4]  = ", ";;
const char Gmtr::ms_acustomTag[4]      = "###";
const char Gmtr::ms_actypeLetters[32]  = "FfGgEePpSsCcBbHhIiLlXxQq";

void Gmtr::BasicMemberInitialization()
{
	m_blretval = false;
	m_uTextLength = m_uMethodNameLength = m_uOuterBufferLength = 0;
	m_pcOuterDumpBuffer = NULL;
}

Gmtr::Gmtr(const char* pcMethod) // for 0 parameters method trace
{
	BasicMemberInitialization();
	int n = BeginMethodName(pcMethod);
	EndMethodName(n, true);
}

Gmtr::Gmtr(int nparam, const char* pcMethod, const char* pcformat, ...)
{
	char  acFormat[128];

	acFormat[0] =  m_acText[0] = '\0';

	BasicMemberInitialization();

	if(!GetTrueFormat(nparam, acFormat, pcformat)) return;

	int npos = BeginMethodName(pcMethod);

	va_list  arglist;
	va_start(arglist, pcformat);
	npos += vsprintf(m_acText + npos, acFormat, arglist);
	va_end(arglist);

	EndMethodName(npos, true);
}

Gmtr::~Gmtr()
{
	if(!m_blretval)
		sprintf(m_acText + m_uMethodNameLength, "() - Return point");

	if(m_pcOuterDumpBuffer)
		delete m_pcOuterDumpBuffer;
}


void Gmtr::SetReturnValue(int nparam, const char* pcformat, ...)
{
	int   n = m_uMethodNameLength;
	char  acFormat[128];

	acFormat[0] = '\0';

	if(!GetTrueFormat(nparam, acFormat, pcformat)) return;

	n += sprintf(m_acText + n, "() - Return Value");
	if(nparam > 1)
		m_acText[n++] = 's';

	m_acText[n++] = ':';
	m_acText[n++] = ' ';

	m_acText[n]   = '\0'; // terminator for the string m_acText

	va_list  arglist;
	va_start(arglist, pcformat);
	n += vsprintf(m_acText + n, acFormat, arglist);
	va_end(arglist);

	m_acText[n] = '\0';
	m_uTextLength = n; // total text length
	m_blretval = true;
}

char* Gmtr::GetTrueFormat(int nparam, char* pcoTrueFormat,
                          const char* pciSimpleFormat)
{
	if(!pciSimpleFormat || !pcoTrueFormat || nparam < 1) return NULL;

	*pcoTrueFormat = '\0'; // output format parameter initialization

	bool    blCustom = strncmp(pciSimpleFormat, ms_acustomTag, 3) ? false : true;
	int     i = 1;
	char    *pc;

	if(blCustom)
	{
		const char* pcfmt = pciSimpleFormat + 3;

		if(2 > strlen(pcfmt)) // no custom string format, at least a "%s" is needed
		  return pcoTrueFormat;

		strcpy(pcoTrueFormat, pcfmt);
	}
	else
	{
		char *pcfmt, acfmtspec[FMT_SPEC_LEN];

		pcfmt = const_cast<char*>(pciSimpleFormat);
		while(i <= nparam) // populate pcoTrueFormat here
		{
			pcfmt = GetParameterFormatSpec(acfmtspec, pcfmt, i == nparam);
			if(!pcfmt)
				break; // end of input format string suddenly reached

			if(acfmtspec[0] =='?')
				continue; // field format split by unknown char(s)

			strcat(pcoTrueFormat, acfmtspec);// concatenates all parameter true formats
			i++; // process next field
		}

		if(!pcfmt) // too few parameters in input format string
			if( (pc = strrchr(pcoTrueFormat, ',')) )
				*pc = '\0'; // last redundant ',' removed
	}

	return pcoTrueFormat;
}

char* Gmtr::GetParameterFormatSpec(char  acFmtSpec[FMT_SPEC_LEN],
                                   char* pcFmtStr, bool bllast /* = false */)
{
	bool  blcaps, blhex, blunsig, blwidth;
	int   i = 1;
	char  *pc;

	blcaps = blhex = blunsig = blwidth = false;
	pc = pcFmtStr;
	acFmtSpec[0] = '%'; // for sure, this will be the first char in format string

	while(isdigit(*pc) || *pc == '.')
	{
		if(*pc == '\0') break;
		else pc++;
	}
	if(*pc == '\0') return NULL; // end of format stream

	i = pc - pcFmtStr; // position of the first non digit char

	if(i > 0) // field lenght spec is present
	{
		strncpy(&acFmtSpec[1], pcFmtStr, i);
		blwidth = true;
	}

	i++; // due to initial '%' in acFmtSpec[]

	if(*pc == 'u' || *pc == 'U') // unsigned flag
	{
		blunsig = true;
		pc++;
	}

	if(*pc == 'x' || *pc == 'X') // hexadecimal flag
	{
		blunsig = blhex = true;
		if(*pc == 'X')
			blcaps = true;

		pc++;
	}

	if(!strchr(ms_actypeLetters, *pc))// unknown type character inside format frame
	{
		acFmtSpec[0] = '?';
		acFmtSpec[1] = '\0';
		return ++pc; // it points to the next unread character in the format stream
	}

	switch(*pc)
	{
	case 'b':
	case 'B':
		acFmtSpec[i++] = (*pc == 'b' ? 'd' : 'X');
		break;

	case 'C': // reserved for multibyte character set
	case 'S':
		acFmtSpec[i++] = 'l';
		acFmtSpec[i++] = (*pc == 'C' ? 'c' : 's');
		break;

	case 'c':
	case 'e':
	case 'E':
	case 'g':
	case 'G':
	case 's':
		acFmtSpec[i++] = *pc;
		break;

	case 'f':
	case 'F':
	case 'p':
	case 'P':
		acFmtSpec[i++] = tolower(*pc);
		break;

	case 'h':
	case 'H':
	case 'l':
	case 'L':
		acFmtSpec[i++] = tolower(*pc);
	case 'i':
	case 'I':
		if(blunsig)
			acFmtSpec[i++] = blhex ? (blcaps ? 'X' : 'x') : 'u';
		else // decimal signed numbers
			acFmtSpec[i++] = 'd';
		break;

	case 'q':
	case 'Q':
		acFmtSpec[i++] = 'l';
		acFmtSpec[i++] = 'l';
		if(blunsig)
			acFmtSpec[i++] = blhex ? (blcaps ? 'X' : 'x') : 'u';
		else // decimal signed numbers
			acFmtSpec[i++] = 'd';
		break;

	default: // preventive: it should never enter here. Unknown character
		acFmtSpec[0] = '?';
		acFmtSpec[1] = '\0';
		return ++pc; // it points to the next unread character in the format stream
		break;
	}

	if(!bllast) // if it isn't the last parameter
	{
		acFmtSpec[i++] = ','; // separator between parameters
		acFmtSpec[i++] = ' ';
	}

	acFmtSpec[i] = '\0';
	return ++pc;
}

int Gmtr::BeginMethodName(const char* pcMethod)
{
	int n;

	if(pcMethod && *pcMethod != '\0')
	{
		char* pc = strchr(const_cast<char*>(pcMethod), '(');
		if(pc)
		{
			m_uMethodNameLength = pc - pcMethod;
			n = m_uMethodNameLength + 1;
			strncpy(m_acText, pcMethod, n);// copy 1st part of the name until '(' inclusive
		}
		else // no opening parenthesis found
		{
			n = strlen(pcMethod);
			m_uMethodNameLength = short(n);
			strcpy(m_acText, pcMethod);
			m_acText[n++] = '(';
		}
	}
	else // no method name is given
	{
		n = 4;
		m_uMethodNameLength = 3;
		m_acText[0] = '?';
		m_acText[1] = '?';
		m_acText[2] = '?';
		m_acText[3] = '(';
	}

	m_acText[n]   = '\0'; // terminator for the string m_acText
	return n;
}

void Gmtr::EndMethodName(int npos, bool blDbgOut /* = false */)
{
	m_acText[npos++] = ')';
	m_acText[npos] = '\0';
	m_uTextLength = npos; // total text length
}

char* Gmtr::GetProperBuffer(unsigned short int ulen)
{
	if(ulen > TIPICAL_DATA_LEN) // printed data won't fit into the internal buffer
	{
		if(m_pcOuterDumpBuffer) // external buffer already exist
		{
			if(ulen > m_uOuterBufferLength) // printed data won't fit into used buffer
			{
				delete m_pcOuterDumpBuffer;
				m_pcOuterDumpBuffer = new char[3 * ulen + 4];
				m_uOuterBufferLength = (m_pcOuterDumpBuffer != NULL ? ulen : 0);
			}
		}
		else // external buffer hasn't been created yet
		{
			m_pcOuterDumpBuffer = new char[3 * ulen + 4];
			m_uOuterBufferLength = (m_pcOuterDumpBuffer != NULL ? ulen : 0);
		}
		return m_pcOuterDumpBuffer;
	}
	else // printed data fits into the internal buffer
		return m_acInnerDumpBuffer;
}

const char* Gmtr::HexDump(const unsigned char *pu8Data,
                          const unsigned short u16dataLen)
{
	char* pcDumpBuffer = GetProperBuffer(u16dataLen);

	if(pcDumpBuffer)
		return (const char*) Gmtr::HexDump(pcDumpBuffer, pu8Data, u16dataLen);
	else
		return pcEmpty; // no buffer available, no printed data
}


char* Gmtr::HexDump(char* pcBuffer, const unsigned char *pu8Data,
                    const unsigned short u16dataLen /* = TIPICAL_DATA_LEN */)
{
	if(!pcBuffer) return NULL;

	for(unsigned int i = 0, n = 0; i < u16dataLen; i++)
		n += sprintf(pcBuffer + n, "%02X ", *(pu8Data + i));

	return pcBuffer;
}

#else // _DISABLE_DEBUGOUT

Gmtr::Gmtr(const char* pcMethod) // for 0 parameters method trace
{
}

Gmtr::Gmtr(int nparam, const char* pcMethod, const char* pcformat, ...)
{
}

Gmtr::~Gmtr()
{
}

void Gmtr::SetReturnValue(int nparam, const char* pcformat, ...)
{
}

const char*  Gmtr::HexDump(const unsigned char *pu8Data,
                           const unsigned short u16dataLen)
{
	return pcEmpty;
}

char* Gmtr::HexDump(char* pcBuffer, const unsigned char *pu8Data,
                    const unsigned short u8dataLen /* = TIPICAL_DATA_LEN */)
{
	return const_cast<char*>(pcEmpty);
}

#endif // _DISABLE_DEBUGOUT


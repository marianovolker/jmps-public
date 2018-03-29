///////////////////////////////////////////////////////////////////////
//  GIniFileHandler.cpp                                              //
//  Implementation of the Class GIniFileHandler                      //
//  Created on:      10-Oct-2007 09:57:22 p.m.                       //
//  Author:             Guillermo Paris (GMP)                        //
///////////////////////////////////////////////////////////////////////

#include "Common/Util/GIniFileHandler.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "Common/Util/DEBUGOUT.h"
#include "GSystem.h"
#include "GTypes.h"

GIniFileHandler::GIniFileHandler(const char* cIniFilename,
                                 bool blWriteEnable /* = false */)
   : m_pcFilename(NULL), m_fp(NULL)
{
	Open(cIniFilename,blWriteEnable);
}

GIniFileHandler::GIniFileHandler()
	:m_pcFilename(NULL),
	 m_fp(NULL)

{}

GIniFileHandler::~GIniFileHandler()
{
	if(m_pcFilename)
		delete [] m_pcFilename;

	if(m_fp)
		fclose(m_fp);
}

void GIniFileHandler::Open(const char* cIniFilename, bool blWriteEnable)
{
	m_fp = fopen(cIniFilename, "rt");
	if(blWriteEnable)
		if( (m_pcFilename = new char[MAX_FILENAME_LENGTH]) != NULL )
		{
			strncpy(m_pcFilename, cIniFilename, MAX_FILENAME_LENGTH - 1);
			m_pcFilename[MAX_FILENAME_LENGTH - 1] = '\0'; // safety
		}
}


bool GIniFileHandler::HasSection(const char* csection)
{
	if(!csection || !m_fp)
		return false;

	bool blfound = false;
	char ctmpbuff[MAX_LINE_LENGTH];

	if(ftell(m_fp) != 0)
		rewind(m_fp);

	while(!blfound && ReadLine(m_fp, ctmpbuff))
	{
		if(*ctmpbuff != '[') // not a section begining
			continue;

		if(MatchString(ctmpbuff + 1, ']', csection) != NULL)
		{
			blfound = true;  //  Required section found
		}
	}
	return blfound;
}

int GIniFileHandler::GetPrivateProfileInt(const char* csection,
                                          const char* ckey, int idefa)
{
	char ctmpbuf[MAX_LINE_LENGTH], *pc;

	GMutexLock lock(m_mutex);

	if(ftell(m_fp) != 0)
		rewind(m_fp);

	pc = FindLine(m_fp, csection, ckey, ctmpbuf, NULL);
	if(pc != NULL)
	{
		idefa = atoi(pc);
	}

	return idefa;
}

int GIniFileHandler::WritePrivateProfileInt(const char* csection,
                                            const char* ckey, int ivalue)
{
	if(m_pcFilename == NULL)
		return 0; // ini file not enabled for writing !!

	char szTemp[32];
	memset(szTemp, 0, 32);

	sprintf(szTemp, "%d", ivalue);
	return(WritePrivateProfileString(csection, ckey, szTemp));
}


int GIniFileHandler::GetPrivateProfileString(const char* csection,
                                             const char* ckey,
                                             const char* cdefa,
                                             char* coutput, int ioutlen)
{
	GMutexLock lock(m_mutex);

	if(ftell(m_fp) != 0)
		rewind(m_fp);

	if(ckey == NULL) // Null key means to bring all the keys in the section
		return GetAllSectionKeys(m_fp, csection, coutput, ioutlen);
	else
	{
		char clineBuffer[MAX_LINE_LENGTH], *pc = NULL;

		pc = FindLine(m_fp, csection, ckey, clineBuffer, NULL);
		strncpy(coutput, pc ? pc : (cdefa ? cdefa : ""), ioutlen - 1); // '\0' trailing padding
		coutput[ioutlen - 1] = '\0';
		return strlen(coutput);
	}
}

int GIniFileHandler::WritePrivateProfileString(const char* csection,
                                               const char* ckey,
                                               const char* coutput)
{
	if(m_pcFilename == NULL)
		return 0; // ini file not enabled for writing !!

	GMutexLock lock(m_mutex);

	FILE *rfp, *wfp;
	std::string cur_name;
	char tmp_name[MAX_FILENAME_LENGTH+9];
	char ctmpbuf[MAX_LINE_LENGTH], *pc;
	int len;

	// check if 'm_pcFilename' is a link file
	if (!GSystem::ReadLink(m_pcFilename, cur_name, MAX_FILENAME_LENGTH))
	{
		cur_name = m_pcFilename;
	}

	if( !(rfp = fopen(cur_name.c_str(),"r")) )  /* If the .ini file doesn't exist */
	{
		if( !(wfp = fopen(cur_name.c_str(),"w")) ) /*  then make one */
		{
			return(0);
		}
		fprintf(wfp,"[%s]\n%s=%s\n", csection, ckey, coutput);
		fclose(wfp);
		return(1);
	}
	snprintf(tmp_name,sizeof(tmp_name), "%s%p", cur_name.c_str(), this); // make temp filename
	wfp = fopen(tmp_name, "w");      // Open a temporary file name to copy to
	if(wfp == NULL)
	{
		fclose(rfp);
		LOG(eCommon,  eError, "WritePrivateProfileString() - RETURNING ZERO "
				"because I couldn't open %s for writing!\n", tmp_name);
		return 0;
	}

	pc = FindLine(rfp, csection, ckey, ctmpbuf, wfp);
	fprintf(wfp, "%s=%s\n", ckey, coutput);
	if(ctmpbuf[0] && pc == NULL)
	{
		fprintf(wfp, "%s\n", ctmpbuf);
	}
	while(ReadLine(rfp, ctmpbuf))
	{
		fprintf(wfp, "%s\n", ctmpbuf);
	}

	fclose(rfp);
	fclose(wfp);
	GSystem::RenameFile(tmp_name, cur_name.c_str());
	return(1);
}

int GIniFileHandler::GetAllSectionKeys(FILE* fr, const char* csection,
                                       char* coutbuffer, int len)
{
	bool blInSection = false;
	int iwritten = 0, ilen;
	char *pcbeg, *pcend, cLineBuffer[MAX_LINE_LENGTH];

	ilen = (len < MAX_KEYLIST_LENGTH) ? len : MAX_KEYLIST_LENGTH; // safety reason
	coutbuffer[0] = '\0';

	while(ReadLine(fr, cLineBuffer))
	{
		if(*cLineBuffer == '[')  // Section mark found ?
		{
		  if(blInSection) // already processing the requested section ?
		  {
			return iwritten;   // another section now begins
		  }
		  else if(MatchString(cLineBuffer + 1, ']', csection) != NULL)
		  {
			blInSection = true; // Required section found
		  }
		}
		else if(blInSection && (pcend = strchr(cLineBuffer, '=')) != NULL && pcend > cLineBuffer)
		{  // Next key found
			pcbeg = cLineBuffer;
			while(*pcbeg == ' ' || *pcbeg == '\t')
			++pcbeg; // remove all leading tabs and spaces

			if(pcbeg < pcend) // only useful characters before '=' are allowed
			{
				--pcend; // now point to the character previous to '='
				while( ((pcend >= cLineBuffer) && (*pcend == ' ')) || (*pcend == '\t') )
				{
					--pcend; // remove all trailing tabs and spaces
				}

				*(++pcend) = '\0'; // the null terminator is not necessary but its safe

				int klen = pcend - pcbeg; // keyname length
				if(ilen >= iwritten + klen) // is there enough room for this key ?
				{
					strncpy(coutbuffer + iwritten, pcbeg, klen);
					iwritten += klen;
					*(coutbuffer + iwritten++) = '\0'; // keyname separator
				}
				else // not enough room for this key
					return iwritten;
			}
		}
	}
	return iwritten;
}

char* GIniFileHandler::FindLine(FILE* fr, const char* csection, const char* ckey,
                                char* cbuffer, FILE* fw)
{
	char* pc;
	bool flag = false;

	while(ReadLine(fr, cbuffer))
	{
		if(*cbuffer == '[')
		{            // Section mark found
			if(flag)
			{
				return NULL;  // Section already found, entry not found
			}
			else if(MatchString(cbuffer + 1, ']', csection) != NULL)
			{
				flag = true;      //Required section found
			}
		}
		else if(flag && (pc = MatchString(cbuffer, '=', ckey)) != NULL)
		{
			return pc + 1;    // Entry found
		}
		if(fw != NULL)
		{
			fprintf(fw, "%s\n", cbuffer);  // Read-write mode, copy to temp file
		}
	}
	if(fw)
	{
		if(!flag)
		{
			fprintf(fw, "[%s]\n", csection);  // Read-write mode, create a section
		}
		cbuffer[0] = '\0';
	}
	return NULL;
}


bool GIniFileHandler::ReadLine(FILE* fr, char* cbuffer)
{
	bool blcomment = false;
	int c;
	int i = 0;

	// Read one line from the source file
	while((c = getc(fr)) != 0x0A)
	{
		if( (c == EOF) || (c == 0xFF) )    // return FALSE on unexpected EOF or getc() returns 0xff
		{
			if(i)
			{
				break;    // Last line in not new-line terminated!
			}
			else // EOF is the first char in the current line ==> this line has no information.
			{
				return false;  // No more lines
			}
		}
		else if(blcomment)
			continue;
		else if(c == '#') // The comment char makes invisible the rest of the line
		{
			blcomment = true;
			continue;
		}

		if(c != '\r' && i < (MAX_LINE_LENGTH - 1))    // Skip carriage-return for DOS compatibility
		{
			cbuffer[i++] = c;
		}
	}

	cbuffer[i] = '\0';
	return true;
}

char* GIniFileHandler::MatchString(char* start, char cend, const char* string)
{
	unsigned int  len = strlen(string);

	if(strncasecmp(start, string, len))
	{
		return NULL;
	}
	for(start += len; *start == ' ' || *start == '\t'; start++);
	return *start == cend ? start : NULL;
}

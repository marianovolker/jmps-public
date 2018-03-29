///////////////////////////////////////////////////////////////////////
//  GIniFileHandler.h                                                //
//  Interface of the Class GIniFileHandler                           //
//  Created on:      10-Oct-2007 09:57:22 p.m.                       //
//  Author:             Guillermo Paris (GMP)                        //
///////////////////////////////////////////////////////////////////////

#ifndef GIniFileHandler_h
#define GIniFileHandler_h

#include <stdio.h>
#include <GMutex.h>

#define MAX_LINE_LENGTH          100
#define MAX_FILENAME_LENGTH      160

// Max. buffer length allowed for getting all the keys of a given section.
const int MAX_KEYLIST_LENGTH  = 1023;

class GIniFileHandler
{
public:
	GIniFileHandler(const char* cIniFilename, bool blWriteEnable = false);
	GIniFileHandler();
	~GIniFileHandler();

	inline bool IsReadable();
	bool HasSection(const char* csection);
	void Open(const char* cIniFilename, bool blWriteEnable = false);
	int GetPrivateProfileInt(const char* csection, const char* ckey,
                             int idefa);
	int WritePrivateProfileInt(const char* section,
                               const char* entry, int value);
	int GetPrivateProfileString(const char* csection, const char* ckey,
                                const char* cdefa, char* coutput, int ioutlen);
	int WritePrivateProfileString(const char* csection, const char* ckey,
                                  const char* coutput);
private:

	static int   GetAllSectionKeys(FILE* fr, const char* csection,
	                               char* coutbuffer, int len);

	static char* FindLine(FILE* fr, const char* csection, const char* ckey,
	                      char* cbuffer, FILE* fw);

	static bool  ReadLine(FILE* fr, char* cbuffer);
	static char* MatchString(char* start, char cend, const char* string);

	char*        m_pcFilename; // only for write enable .ini
	FILE*        m_fp;
	GMutex       m_mutex;
};

inline bool GIniFileHandler::IsReadable()
{
	return m_fp ? true : false;
}

#endif // !defined GIniFileHandler_h

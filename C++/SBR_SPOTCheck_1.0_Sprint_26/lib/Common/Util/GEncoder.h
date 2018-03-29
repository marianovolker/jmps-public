#ifndef GEncoder_h
#define GEncoder_h

#include "GMutex.h"
#include "GTypes.h"
#include <string>
#include <map>

using std::map;
using std::string;

typedef map<UINT8, UINT8> EncoderMap;

class GEncoder
{
public:
	static const int CUTF8EncodedLength;

	GEncoder();
	~GEncoder();
	/*encode a crind string using UTF8*/
	void Encode2UTF8(const char* pString, string *str);

private:
	static bool m_bInit;
	static EncoderMap m_Crind2UTF8Index;
	static EncoderMap::iterator m_iEnd;
	static GMutex m_mutex;
};

#endif

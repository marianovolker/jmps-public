#ifndef GGMethodTracer_h
#define GGMethodTracer_h

#include  <stdio.h>

extern const char* pcEmpty; // empty string placeholder
class  Gmtr;
class  GVoidmtr;

#ifndef _DISABLE_DEBUGOUT
typedef Gmtr GMethodTracer;
#define _FN_  __PRETTY_FUNCTION__
#define GMETHODTRACERBASIC() GMethodTracer theGMethodTracerBasic(_FN_)
#define GMETHODTRACERSTRINGADD(s) GMethodTracer theGMethodTracerBasic(1, _FN_, "s", s)
#else
typedef GVoidmtr GMethodTracer;
#define _FN_  pcEmpty
#define GMETHODTRACERBASIC()
#define GMETHODTRACERSTRINGADD(s)
#endif

#define MAX_METHODTRACE_LEN 255
#define FMT_SPEC_LEN          8
#define TIPICAL_DATA_LEN     16

class Gmtr
{
public:
	Gmtr(const char* pcMethod); // for 0 parameters method trace
	Gmtr(int nparam, const char* pcMethod, const char* pcformat, ...);
	~Gmtr();

	char*                     GetText() { return m_acText; }
	inline short              SetReturnValue(short hret);
	inline unsigned short     SetReturnValue(unsigned short uhret);
	inline int                SetReturnValue(int iret);
	inline unsigned int       SetReturnValue(unsigned int uiret);
	inline long               SetReturnValue(long lret);
	inline unsigned long      SetReturnValue(unsigned long ulret);
	inline long long          SetReturnValue(long long llret);
	inline unsigned long long SetReturnValue(unsigned long long ullret);
	inline double             SetReturnValue(double dret);
	inline char*              SetReturnValue(char* pcret);
	inline void*              SetReturnValue(void* pret);
	void                      SetReturnValue(int nparam, const char* pcformat, ...);

	const char*               HexDump(const unsigned char *pu8Data,
								const unsigned short u16dataLen);

	static char*              HexDump(char* pcBuffer, const unsigned char *pu8Data,
									const unsigned short u16dataLen = TIPICAL_DATA_LEN);

private:

	static char*              GetTrueFormat(int nparam, char* pcTrueFormat,
										  const char* pcSimpleFormat);

	static char*              GetParameterFormatSpec(char  acFmtSpec[FMT_SPEC_LEN],
												   char* pcFmtStr, bool bllast = false);

	static const char         ms_acArgSeparator[4];
	static const char         ms_acustomTag[4];
	static const char         ms_actypeLetters[32];

	void                      BasicMemberInitialization();
	int                       BeginMethodName(const char* pcMethod);
	void                      EndMethodName(int npos, bool blDbgOut = false);
	char*                     GetProperBuffer(unsigned short int length);

	unsigned short int        m_uMethodNameLength;
	unsigned short int        m_uTextLength;
	unsigned short int        m_uOuterBufferLength;

	bool                      m_blretval;
	char*                     m_pcOuterDumpBuffer;
	char                      m_acText[1 + MAX_METHODTRACE_LEN];
	char                      m_acInnerDumpBuffer[3 * TIPICAL_DATA_LEN + 4];
};


inline short Gmtr::SetReturnValue(short hret)
{
	SetReturnValue(1, "h", hret);
	return hret;
}

inline unsigned short Gmtr::SetReturnValue(unsigned short uhret)
{
	SetReturnValue(1, "uh", uhret);
	return uhret;
}

inline int Gmtr::SetReturnValue(int iret)
{
	SetReturnValue(1, "i", iret);
	return iret;
}

inline unsigned int Gmtr::SetReturnValue(unsigned int uiret)
{
	SetReturnValue(1, "ui", uiret);
	return uiret;
}

inline long Gmtr::SetReturnValue(long lret)
{
	SetReturnValue(1, "l", lret);
	return lret;
}

inline unsigned long Gmtr::SetReturnValue(unsigned long ulret)
{
	SetReturnValue(1, "ul", ulret);
	return ulret;
}

inline long long Gmtr::SetReturnValue(long long llret)
{
	SetReturnValue(1, "ll", llret);
	return llret;
}

inline unsigned long long Gmtr::SetReturnValue(unsigned long long ullret)
{
	SetReturnValue(1, "ull", ullret);
	return ullret;
}

inline double Gmtr::SetReturnValue(double dret)
{
	SetReturnValue(1, "e", dret);
	return dret;
}

inline char* Gmtr::SetReturnValue(char* pcret)
{
	SetReturnValue(1, "s", pcret);
	return pcret;
}

inline void* Gmtr::SetReturnValue(void* pret)
{
	SetReturnValue(1, "p", pret);
	return pret;
}

// The following source code is meant only for inclusion in source .cpp files
// where debugging is not allowed.

// For .cpp files where the symbol _DEBUG is undefined

class GVoidmtr
{
public:
	GVoidmtr(const char* pcMethod) {}
	GVoidmtr(int nparam, const char* pcMethod, const char* pcformat, ...) {}
	~GVoidmtr() {}

	char*                     GetText()                                 { return NULL;  }
	inline short              SetReturnValue(short hret)                { return hret;  }
	inline unsigned short     SetReturnValue(unsigned short uhret)      { return uhret; }
	inline int                SetReturnValue(int iret)                  { return iret;  }
	inline unsigned int       SetReturnValue(unsigned int uiret)        { return uiret; }
	inline long               SetReturnValue(long lret)                 { return lret;  }
	inline unsigned long      SetReturnValue(unsigned long ulret)       { return ulret; }
	inline long long          SetReturnValue(long long llret)           { return llret; }
	inline unsigned long long SetReturnValue(unsigned long long ullret) { return ullret;}
	inline double             SetReturnValue(double dret)               { return dret;  }
	inline char*              SetReturnValue(char* pcret)               { return pcret; }
	inline void*              SetReturnValue(void* pret)                { return pret;  }
	void                      SetReturnValue(int nparam, const char* pcformat, ...)    {}

	const char*               HexDump(const unsigned char *pu8Data,
									const unsigned short u16dataLen) { return pcEmpty;}

	static char*              HexDump(char* pcBuffer, const unsigned char *pu8Data,
									const unsigned short u16dataLen = TIPICAL_DATA_LEN)
								{return const_cast<char*>(pcEmpty);}
};

#endif

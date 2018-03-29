//! GLogger 
/*!
// This class define a general log class. 
// The logger inherit from ostringstream the main idea is can use the "shift operators"
// on it; and after that call the Write methos
*/
#ifndef GLogger_H
#define GLogger_H

#include <QString>
#include <QFile>
#include <QMutex>

#include <sstream>
#include <iostream>
#include <string>

#include "GLogConfig.h"
#include "GLogManager.h"
#include "utils/base/src/GException.h"

class GLogger
{
public:	
	GLogger(GLogConfig* pLogConfig) throw(GException);
	virtual ~GLogger();
	
	void Write(GLogConfig::eLevel eLvl, const char* pszFile, int iLineNumber, const char* function, const char* strLog);
	bool OpenFile();
	void CloseFile();
	inline bool HasToWrite(GLogConfig::eLevel eEventLevel) const;

private:
	void WriteLogHeader();
	void BuildPrefixLine(GLogConfig::eLevel eLvl, const char* pszFile, int iLineNumber, const char* function);
	void SetLogConfig(GLogConfig* pLogConfig) throw(GException);

private:
	inline bool _HasToWrite(GLogConfig::eLevel eEventLevel) const;
	
private:
	enum
	{
		CSIZE_PREFIX_LINELOG		= 135,
		CSIZE_LINELOG_TO_RESERVE	= CSIZE_PREFIX_LINELOG + 500
	};

private:
	GLogConfig* 	m_pLogConfig;
	QFile 			m_oLogFile;
	mutable QMutex	m_oMux;
	char 			m_zPrefixBuf[CSIZE_PREFIX_LINELOG];
	QString 		m_strLogLine;
};



inline bool GLogger::_HasToWrite(GLogConfig::eLevel eEventLevel) const
{
	return (eEventLevel <= m_pLogConfig->GetLevel());
}

inline bool GLogger::HasToWrite(GLogConfig::eLevel eEventLevel) const
{
	QMutexLocker oLock(&m_oMux);

	return (eEventLevel <= m_pLogConfig->GetLevel());
}


#define LOG_LEVEL1(message)\
	{\
		GLogger* pLogger(NULL);\
		pLogger=GLogManager::GetDefaultLogger();\
		if( pLogger && pLogger->HasToWrite(GLogConfig::eLevel1) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			pLogger->Write(GLogConfig::eLevel1, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
    }

#define LOG_LEVEL2(message)\
	{\
		GLogger* pLogger(NULL);\
		pLogger=GLogManager::GetDefaultLogger();\
		if( pLogger && pLogger->HasToWrite(GLogConfig::eLevel2) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			pLogger->Write(GLogConfig::eLevel2, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#define LOG_LEVEL3(message)\
	{\
		GLogger* pLogger(NULL);\
		pLogger=GLogManager::GetDefaultLogger();\
		if( pLogger && pLogger->HasToWrite(GLogConfig::eLevel3) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			pLogger->Write(GLogConfig::eLevel3, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}


#define LOG_LEVEL4(message)\
	{\
		GLogger* pLogger(NULL);\
		pLogger=GLogManager::GetDefaultLogger();\
		if( pLogger && pLogger->HasToWrite(GLogConfig::eLevel4) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			pLogger->Write(GLogConfig::eLevel4, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#define _LOG_LEVEL1(logger, message)\
	{\
		if( logger && logger->HasToWrite(GLogConfig::eLevel1) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			logger->Write(GLogConfig::eLevel1, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#define _LOG_LEVEL2(logger, message)\
	{\
		if( logger && logger->HasToWrite(GLogConfig::eLevel2) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			logger->Write(GLogConfig::eLevel2, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#define _LOG_LEVEL3(logger, message)\
	{\
		if( logger && logger->HasToWrite(GLogConfig::eLevel3) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			logger->Write(GLogConfig::eLevel3, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#define _LOG_LEVEL4(logger, message)\
	{\
		if( logger && logger->HasToWrite(GLogConfig::eLevel4) )\
		{\
			std::ostringstream oStream;\
			oStream << qPrintable(QString(message));\
			logger->Write(GLogConfig::eLevel4, __FILE__, __LINE__, __FUNCTION__, oStream.str().data());\
		}\
	}

#endif

#include "GLogger.h"

#include <QMutexLocker>
#include <QDateTime>
#include <QThread>
	
GLogger::GLogger(GLogConfig* pLogConfig) throw(GException)
	: m_pLogConfig(0)
{	
	SetLogConfig(pLogConfig);

	m_strLogLine.reserve(CSIZE_LINELOG_TO_RESERVE);
}

GLogger::~GLogger()
{
	CloseFile();
}

void GLogger::Write(GLogConfig::eLevel eLvl, const char* pszFile, int iLineNumber, const char* function, const char* strLog)
{
	QMutexLocker oLock(&m_oMux);

	if( _HasToWrite(eLvl) )
	{
		BuildPrefixLine(eLvl, pszFile, iLineNumber, function);

		m_strLogLine  = m_zPrefixBuf;
		m_strLogLine += strLog;
		m_strLogLine += "]\n";
		
		// Write the LogLine to file		
		if( m_pLogConfig->GetWriteToFile() && m_oLogFile.isOpen() )
		{
			m_oLogFile.write(qPrintable(m_strLogLine));
			m_oLogFile.flush();
		}
		
		// Write the LogLine to console
		if( m_pLogConfig->GetWriteToConsole() )
		{
			std::cout << qPrintable(m_strLogLine);
		}
	}
}

bool GLogger::OpenFile()
{
	QMutexLocker oLock(&m_oMux);

	bool bOpenSuccess(true);
	
	m_oLogFile.setFileName(m_pLogConfig->GetFullFileName());
	
	if( !m_oLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) )
	{
		bOpenSuccess = false;
	}
	
	return bOpenSuccess;
}

void GLogger::CloseFile()
{
	QMutexLocker oLock(&m_oMux);

	if( m_oLogFile.isOpen() )
	{
		m_oLogFile.flush();
		m_oLogFile.close();
	}
}

void GLogger::WriteLogHeader() 
{
	QString m_strLogLine;

	m_strLogLine += "####################################################\n";
	m_strLogLine += "#  Qt Logger StartUp at ";
	m_strLogLine += QDateTime::currentDateTime().toString("[MM.dd.yyyy hh:mm:ss.zzz] ");
	m_strLogLine += " #\n";
	m_strLogLine += "#   >> Storing Level " + QString::number(m_pLogConfig->GetLevel());
	m_strLogLine += " logs                        #\n";
	m_strLogLine += "####################################################\n\n";
	
	m_oLogFile.write(qPrintable(m_strLogLine));
}

void GLogger::BuildPrefixLine(GLogConfig::eLevel eLvl, const char* pszFile, int iLineNumber, const char* function)
{
	memset(m_zPrefixBuf, 0, CSIZE_PREFIX_LINELOG);
	snprintf(m_zPrefixBuf, CSIZE_PREFIX_LINELOG, "[%s] [L%d] [%llx] [%31s:%04i] [%31s()] ["	, qPrintable(QDateTime::currentDateTime().toString("MM.dd.yyyy hh:mm:ss.zzz"))
																							, eLvl
																							, reinterpret_cast<unsigned long long> (QThread::currentThreadId())
																							, (QString(pszFile).size()<=30)?  (pszFile):(qPrintable(QString(pszFile).right(30)))
																							, iLineNumber
																							, (QString(function).size()<=30)? (function):(qPrintable(QString(function).right(30))) );
}

void GLogger::SetLogConfig(GLogConfig* pLogConfig) throw(GException)
{
	CloseFile();

	if( m_pLogConfig != NULL )
	{
		delete m_pLogConfig;
	}
	m_pLogConfig = pLogConfig;

	if( m_pLogConfig->GetWriteToFile() )
	{
		if(OpenFile())
		{
			WriteLogHeader();
		}
		else
		{
			throw GEXCEPTION("GLogger::Open - Could not open log file");
		}
	}
}

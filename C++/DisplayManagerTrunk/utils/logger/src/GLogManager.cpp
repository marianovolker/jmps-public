#include "GLogManager.h"

#include "GLogger.h"

QMap<QString, GLogger*> GLogManager::m_stoLoggers;
QMutex GLogManager::m_stoMutex;
GLogger* GLogManager::m_stpoDefaultLogger(NULL);

void GLogManager::AddLogger(QString strKey, GLogger* poLogger, bool bSetAsDefault)
{
	m_stoMutex.lock();	
	m_stoLoggers.insert(strKey, poLogger);
	
	if(m_stpoDefaultLogger == NULL || bSetAsDefault)
	{
		m_stpoDefaultLogger = poLogger;	
	}	
	m_stoMutex.unlock();
}

void GLogManager::DelLogger(QString strKey)
{
	m_stoMutex.lock();
	QMap<QString, GLogger*>::iterator i = m_stoLoggers.find(strKey);

	if( i != m_stoLoggers.end() )
	{
		if( m_stpoDefaultLogger == i.value() )
		{
			m_stpoDefaultLogger = NULL;
		}

		delete i.value();
		m_stoLoggers.erase(i);
	}

	m_stoMutex.unlock();
}

GLogger* GLogManager::GetLogger(QString strKey)
{
	QMap<QString, GLogger*>::const_iterator i = m_stoLoggers.find(strKey);
	return (i != m_stoLoggers.end())? i.value() : NULL;	
}


GLogger* GLogManager::GetDefaultLogger()
{
	return m_stpoDefaultLogger;
}

GLogManager::GLogManager()
{
}

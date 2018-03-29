#ifndef CLOGMANAGER_H_
#define CLOGMANAGER_H_

#include <QMutex>
#include <QMap>
#include <QString>

class GLogger;

class GLogManager
{
public:
	static void AddLogger(QString strKey, GLogger* poLogger, bool bSetAsDefault);
	static void DelLogger(QString strKey);
	static GLogger* GetLogger(QString strKey);
	static GLogger* GetDefaultLogger();

private:
	GLogManager();
	
private:
	static QMap<QString, GLogger*> m_stoLoggers;	
	static QMutex m_stoMutex;
	static GLogger* m_stpoDefaultLogger;
};

#endif

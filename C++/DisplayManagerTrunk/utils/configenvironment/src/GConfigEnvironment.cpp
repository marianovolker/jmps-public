#include "GConfigEnvironment.h"

#include <QProcess>

#include "utils/logger/src/GLogger.h"

GConfigEnvironment::GConfigEnvironment()
{
	LOG_LEVEL4 ( "GConfigEnvironment()" );

	Load();
}

GConfigEnvironment::~GConfigEnvironment()
{
	LOG_LEVEL4 ( "~GConfigEnvironment()" );

	m_QMapKeyValue.clear();
}

void GConfigEnvironment::Load() throw (GException)
{
	LOG_LEVEL4("Load()");

	m_QMapKeyValue.clear();

	QStringList aryQListEnvs = QProcess::systemEnvironment();

	while( !aryQListEnvs.isEmpty() )
	{
		QString strLine(aryQListEnvs.takeLast());
	
		QString strLeftSide, strRightSide;

		int iPos = strLine.indexOf("=");
		if( iPos != -1 )
		{
			strLeftSide  = strLine.left(iPos);
			strRightSide = strLine.mid(iPos + 1, strLine.size() - iPos);

			/* remove trailing spaces */
			/* remove leading spaces */
			strLeftSide  = strLeftSide.simplified();
			strRightSide = strRightSide.simplified();

			m_QMapKeyValue.insert(strLeftSide, strRightSide);
		}
	}
}

int GConfigEnvironment::GetInt(const QString& property, int defaultValue) const
{
	LOG_LEVEL4( "GetInt()" );

	QMap<QString,QString>::const_iterator key = m_QMapKeyValue.find(property);
	return (key == m_QMapKeyValue.end()) ? defaultValue : key.value().toInt();
}

bool GConfigEnvironment::GetBool(const QString& property, bool defaultValue) const
{
	LOG_LEVEL4("GetBool()");

	QMap<QString,QString>::const_iterator key = m_QMapKeyValue.find(property);
	return (key == m_QMapKeyValue.end()) ? defaultValue : (key.value() == "true");
}

const QString GConfigEnvironment::GetString(const QString& property, const char* defaultValue) const
{
	LOG_LEVEL4("GetString()");

	QMap<QString,QString>::const_iterator key = m_QMapKeyValue.find(property);
	return (key == m_QMapKeyValue.end()) ? QString(defaultValue) : key.value();
}

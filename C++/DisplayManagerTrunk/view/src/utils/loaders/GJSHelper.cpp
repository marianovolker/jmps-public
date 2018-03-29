#include "GJSHelper.h"

#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"

#include <QVariant>

GJSHelper::GJSHelper()
	:	m_pWebMainFrame(0),
		m_qslJsCalls(),
		m_qslJsLibraries()
{
	LOG_LEVEL4("GJSHelper()");
}

GJSHelper::~GJSHelper()
{
	LOG_LEVEL4("~GJSHelper()");
}

void GJSHelper::LoadJSLibraries( )
{
	while( !m_qslJsLibraries.isEmpty() )
	{
		QString strJavaScriptLibrary(m_qslJsLibraries.takeFirst());

		LOG_LEVEL4(QString("Load Library JS [") + strJavaScriptLibrary + "]");

		ExecuteJavaScript(ReadHtmlFile(strJavaScriptLibrary));
	}
}

void GJSHelper::LoadJSCalls( )
{

	QString strJavaScriptCall("");

	while( !m_qslJsCalls.isEmpty() )
	{
		strJavaScriptCall += m_qslJsCalls.takeFirst();
	}

	if( !strJavaScriptCall.isEmpty() )
	{
		ExecuteJavaScript(strJavaScriptCall);
	}
}

void GJSHelper::ExecuteJavaScript(const QString& strJScript)
{
	QVariant oVarResult = m_pWebMainFrame->evaluateJavaScript(strJScript);

	if( GETCONF_BOOL(eDisplayManagerEnableLoggingOfJavaScript) )
	{
		LOG_LEVEL4(QString("Executing js [") + strJScript + "]");

		if( QString(QMetaType::typeName(oVarResult.type())) == "QVariantMap" )
		{
			LOG_LEVEL4(QString("JS type Result: [") + QMetaType::typeName(oVarResult.type()) + "]" );

			QMap<QString, QVariant> map = oVarResult.toMap();
			QMap<QString, QVariant>::Iterator it = map.begin();
			while( it != map.end() )
			{
				it.value().convert(QVariant::String);
				LOG_LEVEL4(QString("JS result: [") + it.key() + " = " + it.value().toString() + "]");
				++it;
			}
		}
		else
		{
			LOG_LEVEL4(QString("JS type Result: [") + QMetaType::typeName(oVarResult.type()) + "]" );
			LOG_LEVEL4(QString("JS Result: [") + oVarResult.toString() + "]" );
		}
	}
}

QString GJSHelper::ReadHtmlFile(const QString& strPath)
{
	LOG_LEVEL4("ReadHtmlFile()");

	LOG_LEVEL4(QString("File html to read [") + strPath + "]");

	QFile oFile;
	oFile.setFileName(strPath);
	oFile.open(QIODevice::ReadOnly);

	return oFile.readAll();
}

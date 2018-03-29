#include "GConfigReader.h"

#include <QString>
#include <QTextStream>
#include <QFile>

#include "utils/logger/src/GLogger.h"

const QString GConfigReader::CISTALLATION_PATTERN = "XXXREPLACE_";

GConfigReader::GConfigReader(const QString& strFileName) throw (GConfigReaderException)
{
	LOG_LEVEL4 ( "GConfigReader()" );
   
	m_strFileName = strFileName;

	Load(m_strFileName);
}

GConfigReader::~GConfigReader()
{
	LOG_LEVEL4 ( "~GConfigReader()" );

	m_QMapKeyValue.clear();

	m_strFileName.clear();
}

void GConfigReader::Reset()
{
	LOG_LEVEL4 ("Reset()");

	m_QMapKeyValue.clear();

	m_strFileName.clear();
}

void GConfigReader::Load(const QString& strFileName) throw (GConfigReaderException)
{
	LOG_LEVEL3("Load()");

	if( !strFileName.isEmpty() )
	{
		m_strFileName = strFileName;
	}

	QFile oFile(m_strFileName);

	if( !oFile.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		throw GCONFIG_READER_EXCEPTION_DETAILS( GConfigReaderException::MapToInternalError(oFile.error()), (QString("in filename=")+m_strFileName) );
	}

	QTextStream stream(&oFile);

	m_QMapKeyValue.clear();

	QString strLine = stream.readLine();

	while( !strLine.isNull() )
	{
		if( (strLine.startsWith("#") == false ) && (strLine.indexOf(CISTALLATION_PATTERN) == -1) )
		{
			int iPos = strLine.indexOf("=");
			if( iPos != -1 )
			{
				QString strLeftSide  = strLine.left(iPos);
				QString strRightSide = strLine.mid(iPos + 1, strLine.size() - iPos);

				/* remove trailing spaces */
				/* remove leading spaces */
				/* allows for property = stuff, property=stuff etc…. */
				strLeftSide  = strLeftSide.simplified();
				strRightSide = strRightSide.simplified();

				m_QMapKeyValue[strLeftSide] = strRightSide;
				m_lstOriginalFile << (strLeftSide + "=" + strRightSide);

				strLine = stream.readLine();

				continue;
			}
		}

		m_lstOriginalFile << strLine;

		strLine = stream.readLine();
	}

	oFile.close();
}

void GConfigReader::Save(const QString& strOutputFileName) throw (GConfigReaderException)
{
	LOG_LEVEL3("Save()");

	if( !strOutputFileName.isEmpty() )
	{
		m_strFileName = strOutputFileName;
	}

	QFile oFile(m_strFileName);

	if( !oFile.open(QIODevice::WriteOnly | QIODevice::Text) )
	{
		throw GCONFIG_READER_EXCEPTION_DETAILS( GConfigReaderException::MapToInternalError(oFile.error()), (QString("in filename=")+m_strFileName) );
	}

	QTextStream stream(&oFile);

	stream.setFieldAlignment(QTextStream::AlignLeft);

	for( int i = 0; i < m_lstOriginalFile.size(); ++i )
	{
		QString strLine = m_lstOriginalFile.at(i);
		int iPos = strLine.indexOf("=");
		if( iPos != -1 )
		{
			QString strLeftSide  = strLine.left(iPos);
			QString strRightSide = strLine.mid(iPos + 1, strLine.size() - iPos);

			stream.setFieldWidth(60);
			stream << strLeftSide;
			stream.setFieldWidth(1);
			stream << QString("= " + strRightSide) << endl;
		}
		else
		{
			stream.setFieldWidth(1);
			stream << m_lstOriginalFile.at(i) << endl;
		}
	}

	oFile.close();
}

void GConfigReader::Show() const
{
	LOG_LEVEL4("Show()");

	//LOG_LEVEL3( QString("Filename=") + m_strFileName );

	QMap<QString,QString>::const_iterator i= m_QMapKeyValue.constBegin();
	for ( ; i != m_QMapKeyValue.constEnd(); ++i )
	{
	    LOG_LEVEL3( QString(i.key()) + "=" + i.value() );
	}
}

QString GConfigReader::GetFileName() const
{
	return m_strFileName;
}

bool GConfigReader::Set(const QString& strKey, const QString& strNewValue)
{
	LOG_LEVEL4( "Set()" );

	QMap<QString,QString>::const_iterator key = m_QMapKeyValue.find(strKey);
	if( key != m_QMapKeyValue.end() )
	{
		m_QMapKeyValue[strKey] = strNewValue;

		for( int i = 0; i < m_lstOriginalFile.size(); ++i )
		{
			if( m_lstOriginalFile[i].indexOf(strKey) != -1 )
			{
				m_lstOriginalFile[i] = (strKey + "=" + strNewValue);
				break;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

int GConfigReader::GetInt(const QString& strKey, int iDefaultValue) const
{
	LOG_LEVEL4( "GetInt()" );
	QMap<QString,QString>::const_iterator ciKey = m_QMapKeyValue.find(strKey);
	return (ciKey == m_QMapKeyValue.end()) ? iDefaultValue : ciKey.value().toInt();
}

bool GConfigReader::GetBool(const QString& strKey, bool bDefaultValue) const
{
	LOG_LEVEL4("GetBool()");
	QMap<QString,QString>::const_iterator ciKey = m_QMapKeyValue.find(strKey);
	return (ciKey == m_QMapKeyValue.end()) ? bDefaultValue : ((ciKey.value().compare("true", Qt::CaseInsensitive)==0)?(true):(false));
}

const QString GConfigReader::GetString(const QString& strKey, const char* szDefaultValue) const
{
	LOG_LEVEL4("GetString()");
	QMap<QString,QString>::const_iterator ciKey = m_QMapKeyValue.find(strKey);
	return (ciKey == m_QMapKeyValue.end()) ? QString(szDefaultValue) : ciKey.value();
}

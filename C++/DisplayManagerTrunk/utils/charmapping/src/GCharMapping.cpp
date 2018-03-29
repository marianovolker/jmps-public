/*!
 * \file GCharMapping.cpp
 *
 * \author guillermo tomasini
 * \date	09-21-2009
 *
 * utf8 mapping (ascii to utf8) from text file
 */

#include "GCharMapping.h"

#include <QDir>

#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"


GCharMapping::GCharMapping(const QString &mapFilename) throw (GCharMappingException):
	m_pConfig(NULL),
	m_CharVec(eLanguageMappingTableSize, eLanguageMappingNullCharacter)
{
	LOG_LEVEL4("GCharMapping()");

	Init( mapFilename);
}

GCharMapping::GCharMapping ( const GCharMapping &x ):
	m_pConfig(x.m_pConfig),
	m_CharVec(x.m_CharVec)
{
}

GCharMapping & GCharMapping::operator =( const GCharMapping &x )
{
	m_pConfig=x.m_pConfig;
	m_CharVec=x.m_CharVec;

	return *this;
}


GCharMapping::~GCharMapping()
{
	LOG_LEVEL4("~GCharMapping()");

	if( m_pConfig )
	{
		delete m_pConfig;
		m_pConfig=NULL;
	}
}

void GCharMapping::Init(const QString &mapFilename) throw (GCharMappingException)
{
	LOG_LEVEL4("Init()");

	try
	{
		if( m_pConfig )
		{
			delete m_pConfig;
			m_pConfig=NULL;
		}

		m_pConfig = new GConfigReader(mapFilename);

		m_pConfig->Show();
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GConfigReader::Exception [") + e.GetMessage() + "]" );

		throw GCHARMAPPING_EXCEPTION( eCantOpenCharMappingFilename );
	}

	for ( int i=0; i<eLanguageMappingTableSize; i++ )
	{
		QString key;
		key.sprintf("char_%03d", i);

		QString vaux(m_pConfig->GetString(key));

		if ( vaux=="" )
		{
			continue;
		}
		else if ( vaux.endsWith(";") == false )
		{
			LOG_LEVEL3(QString(key) + " without ;");
			continue;
		}

		vaux=vaux.left(vaux.length()-1);

		unsigned int charVal;
		bool ok;
		if ( vaux.startsWith("&#x") )
		{
			QString hex(vaux.right(vaux.length()-3));
			charVal=hex.toInt(&ok, 16);
		}
		else if ( vaux.startsWith( "&#" ) )
		{
			QString dec( vaux.right(vaux.length()-2));
			charVal=dec.toInt(&ok, 10);
		}
		else
		{
			LOG_LEVEL3(QString(key) + " error.");
			continue;
		}

		if (ok)
		{
			LOG_LEVEL4(QString("i: ") + i + ", charVal:" + charVal);

			m_CharVec[i]=QChar(charVal);
		}
		else
		{
			LOG_LEVEL3(QString(key) + ": convertion hex/dec error");
		}
	}
}

void GCharMapping::Reset()
{
	for ( int i=0; i<eLanguageMappingTableSize; i++ )
	{
		m_CharVec[i] = eLanguageMappingNullCharacter;
	}
}

QVector<QChar> & GCharMapping::GetCharVec()
{
	return m_CharVec;
}

QChar & GCharMapping::GetQChar( int i ) 
{
	return m_CharVec[i];
}

#include "GDisplayManagerConfig.h"

#include "utils/configenvironment/src/GConfigEnvironment.h"

const QString GDisplayManagerConfig::CSTRAlmostFailProofPersonalityName = "blue";

GDisplayManagerConfig*  GDisplayManagerConfig::m_pGDisplayManagerConfig = NULL;
QMutex GDisplayManagerConfig::m_mutex;
const QString GDisplayManagerConfig::CSTRConfigEnvironmentPath = "DM_CONFIG_PATH";
const QString GDisplayManagerConfig::CSTRConfigFileName = "displaymanager.properties";

//----------------------------------------------------------------------------------------------------------------------------------
GDisplayManagerConfig::GDisplayManagerConfig():
	m_pConfig(NULL),
	m_strFileName("")
{
	LOG_LEVEL4( "GDisplayManagerConfig()" );

	QString strConfigPath = GConfigEnvironment().GetString(CSTRConfigEnvironmentPath, "./");
	m_strFileName = (strConfigPath.endsWith("/"))? (strConfigPath+CSTRConfigFileName):(strConfigPath+"/"+CSTRConfigFileName);

	m_pConfig = new GConfigReader(m_strFileName);
}

GDisplayManagerConfig::~GDisplayManagerConfig()
{
	LOG_LEVEL4( "~GDisplayManagerConfig()" );

	if( m_pConfig )
	{
		delete m_pConfig;
		m_pConfig=NULL;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------
void GDisplayManagerConfig::Insert( eDisplayManagerConfig eKey, const QString &strName, const QString &strDefaultValue )
{
	LOG_LEVEL4( "Insert()" );

	stDisplayManagerConfig stConfigValue;

	stConfigValue.strName = strName;
	stConfigValue.strDefaultValue = strDefaultValue;
	m_oConfigList.insert(eKey, stConfigValue);
}

QString GDisplayManagerConfig::GetName( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetName()" );

	QMap<eDisplayManagerConfig, stDisplayManagerConfig>::const_iterator ciKey = m_oConfigList.find(eKey);
	return (ciKey == m_oConfigList.end()) ? "" : ciKey.value().strName;
}

QString GDisplayManagerConfig::GetDefaultValue( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetDefaultValue()" );

	QMap<eDisplayManagerConfig, stDisplayManagerConfig>::const_iterator ciKey = m_oConfigList.find(eKey);
	return (ciKey == m_oConfigList.end()) ? "" : ciKey.value().strDefaultValue;
}
//----------------------------------------------------------------------------------------------------------------------------------


// Get/Set Values from Current configuration
//----------------------------------------------------------------------------------------------------------------------------------
bool GDisplayManagerConfig::Set(eDisplayManagerConfig eKey, const QString& strNewValue)
{
	LOG_LEVEL4( "Set()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	return m_pConfig->Set(GetName(eKey), strNewValue);
}

QString GDisplayManagerConfig::GetString( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetString()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	return m_pConfig->GetString(GetName(eKey), qPrintable(GetDefaultValue(eKey)));
}

int GDisplayManagerConfig::GetInt( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetInt()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	return m_pConfig->GetInt(GetName(eKey), GetDefaultValue(eKey).toInt());
}

bool GDisplayManagerConfig::GetBool( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetBool()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	return m_pConfig->GetBool( GetName(eKey), (GetDefaultValue(eKey).compare("true", Qt::CaseInsensitive)==0)?(true):(false) );
}

void GDisplayManagerConfig::ShowCurrentConfiguration( ) const
{
	LOG_LEVEL4( "ShowCurrentConfiguration()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	m_pConfig->Show();
}
//----------------------------------------------------------------------------------------------------------------------------------


// Get/Set Values from filesystem
//----------------------------------------------------------------------------------------------------------------------------------
QString GDisplayManagerConfig::GetStringFromFileSystem( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetStringFromFileSystem()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	GConfigReader oConfig(m_strFileName);

	return oConfig.GetString(GetName(eKey), qPrintable(GetDefaultValue(eKey)));
}

int GDisplayManagerConfig::GetIntFromFileSystem( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetIntFromFileSystem()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	GConfigReader oConfig(m_strFileName);

	return oConfig.GetInt(GetName(eKey), GetDefaultValue(eKey).toInt());
}

bool GDisplayManagerConfig::GetBoolFromFileSystem( eDisplayManagerConfig eKey ) const
{
	LOG_LEVEL4( "GetBoolFromFileSystem()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	GConfigReader oConfig(m_strFileName);

	return oConfig.GetBool( GetName(eKey), (GetDefaultValue(eKey).compare("true", Qt::CaseInsensitive)==0)?(true):(false) );
}

bool GDisplayManagerConfig::SetToFileSystem(eDisplayManagerConfig eKey, const QString& strNewValue)
{
	LOG_LEVEL4( "SetToFileSystem()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	GConfigReader oConfig(m_strFileName);

	bool bResult(oConfig.Set(GetName(eKey), strNewValue));
	if(bResult)
	{
		oConfig.Save();
	}

	return bResult;
}

void GDisplayManagerConfig::ShowConfigurationFromFileSystem( ) const
{
	LOG_LEVEL4( "ShowConfigurationFromFileSystem()" );

	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	GConfigReader oConfig(m_strFileName);

	oConfig.Show();
}
//----------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------
GDisplayManagerConfig * GDisplayManagerConfig::GetInstance() throw(GException)
{
	QMutexLocker oLocker(&GDisplayManagerConfig::m_mutex);

	if( m_pGDisplayManagerConfig == NULL )
	{
		m_pGDisplayManagerConfig = new GDisplayManagerConfig();
		m_pGDisplayManagerConfig->Init();
	}

	if( m_pGDisplayManagerConfig == NULL )
	{
		 throw( GException("GDisplayManagerConfig::GetInstance() made a NULL pointer.") );
	}

	return m_pGDisplayManagerConfig;
}
//----------------------------------------------------------------------------------------------------------------------------------

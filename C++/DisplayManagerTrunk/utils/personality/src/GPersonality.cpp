#include "GPersonality.h"

#include <QDir>
#include <QFileInfo>
#include <QUrl>

#include <unistd.h>

#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"
#include "utils/charmapping/src/GCharMapping.h"

const QString GPersonality::CSTRConfigFileName                      	("personality.properties");
const QString GPersonality::CSTRURLPrefixParameter             			("url_prefix");
const QString GPersonality::CSTRClearScreenParameter          			("clear_screen");
const QString GPersonality::CSTRCustomGraphicPropertyNamePrefix     	("graphic_");
const QString GPersonality::CSTRClockGraphicPropertyNamePrefix      	("graphic_clock_");
const QString GPersonality::CSTRCustomScreenPropertyNamePrefix			("screen_");
const QString GPersonality::CSTRArrowGraphicPropertyNameFrame			("graphic_%1_arrow");
const QString GPersonality::CSTRLeftArrowName							("left");
const QString GPersonality::CSTRRightArrowName							("right");
const QString GPersonality::CSTRCharMappingFileName						("char.mapping.filename");
const QString GPersonality::CSTRCustomGraphicPropertyDimensionNameFrame	("graphic_%1_%2");
const QString GPersonality::CSTRClockGraphicPropertyDimensionNameFrame	("graphic_clock_%1_%2");
const QString GPersonality::CSTRArrowGraphicPropertyDimensionNameFrame	("graphic_%1_arrow_%2");
const QString GPersonality::CSTRDimensionWidthName						("width");
const QString GPersonality::CSTRDimensionHeightName						("height");
const QString GPersonality::CSTROverlapsErrorMarginPixelsName			("overlaps_error_margin_pixels");
const QString GPersonality::CSTRRemoveOverlapsConditionName				("remove_overlaps");

static unsigned int const uikPersonalityLoadFailSleepSeconds = 5;

GPersonality::GPersonality() throw (GPersonalityException):
	m_pConfig(NULL)
{
	LOG_LEVEL4("GPersonality()");

	Init();
}

GPersonality::~GPersonality()
{
	LOG_LEVEL4("~GPersonality()()");

	if( m_pConfig )
	{
		delete m_pConfig;
		m_pConfig=NULL;
	}
}

void GPersonality::Init(QString const & strDefaultResolution, QString const & strDefaultPersonalityName) throw (GPersonalityException)
{
	LOG_LEVEL4("Init()");

	QString strPersonalityBaseDir;
	QString strPersonalityPath;
	QString strPersonalityName;
	QString strPersonalityFileName;

	if( (strPersonalityBaseDir=GETCONF_STR(eDisplayManagerPersonalityDir)).isEmpty() )
	{
		throw GPERSONALITY_EXCEPTION(eInvalidPersonalityDir);
	}

	if( strDefaultPersonalityName.isEmpty() )
	{
		if( (strPersonalityName=GETCONF_STR(eDisplayManagerPersonalityName)).isEmpty() )
		{
			LOG_LEVEL1( GPERSONALITY_EXCEPTION(eInvalidDefaultPersonality).GetMessage() );
			strPersonalityName = GDisplayManagerConfig::CSTRAlmostFailProofPersonalityName;
		}
	}
	else
	{
		strPersonalityName = strDefaultPersonalityName;
	}

	m_strResolution = strDefaultResolution;

	{
		bool bTryPersonalityLoad = true;
		while (bTryPersonalityLoad)
		{
			strPersonalityPath = (strPersonalityBaseDir.endsWith("/"))? (strPersonalityBaseDir):(strPersonalityBaseDir+"/");
			strPersonalityPath += strPersonalityName + "/";
			strPersonalityPath += m_strResolution + "/";
			strPersonalityFileName = strPersonalityPath + CSTRConfigFileName;
			m_strPersonalityDir = strPersonalityPath;

			LOG_LEVEL3(QString("[Personality filename=") + strPersonalityFileName + "]");

			try
			{
				if( m_pConfig )
				{
					delete m_pConfig;
					m_pConfig=NULL;
				}

				m_pConfig = new GConfigReader(strPersonalityFileName);

				m_pConfig->Show();
				bTryPersonalityLoad = false;
			}
			catch(GException & e)
			{
				LOG_LEVEL1( QString("GConfigReader::Exception [") + e.GetMessage() + "]" );

				if (strPersonalityName == GDisplayManagerConfig::CSTRAlmostFailProofPersonalityName)
				{
// This was commented because it sysman keeps retrying when DM exits due to this issue, it will leave the unit unusable. Instead, keep looping indefinitely and try again every X seconds.
//					throw GPERSONALITY_EXCEPTION(eCantOpenPersonalityFilename);
					sleep(uikPersonalityLoadFailSleepSeconds);
				}
				else
				{
					strPersonalityName = GDisplayManagerConfig::CSTRAlmostFailProofPersonalityName;
					if ( strDefaultPersonalityName.isEmpty() )
					{
						if ( !(SETCONF(eDisplayManagerPersonalityName, strPersonalityName)) )
						{
							throw GPERSONALITY_EXCEPTION(eInvalidDefaultPersonality);
						}
					}
				}
			}
		}
	}
}

/*
void GPersonality::Init(QString const & strDefaultResolution, QString const & strDefaultPersonalityName) throw (GPersonalityException)
{
	LOG_LEVEL4("Init()");

	QString strPersonalityBaseDir;
	QString strPersonalityPath;
	QString strPersonalityName;
	QString strPersonalityFileName;

	if( (strPersonalityBaseDir=GETCONF_STR(eDisplayManagerPersonalityDir)).isEmpty() )
	{
		throw GPERSONALITY_EXCEPTION(eInvalidPersonalityDir);
	}

	if( strDefaultPersonalityName.isEmpty() )
	{
		if( (strPersonalityName=GETCONF_STR(eDisplayManagerPersonalityName)).isEmpty() )
		{
			throw GPERSONALITY_EXCEPTION(eInvalidDefaultPersonality);
		}
	}
	else
	{
		strPersonalityName = strDefaultPersonalityName;
	}

	m_strResolution = strDefaultResolution;

	strPersonalityPath = (strPersonalityBaseDir.endsWith("/"))? (strPersonalityBaseDir):(strPersonalityBaseDir+"/");
	strPersonalityPath += strPersonalityName + "/";
	strPersonalityPath += m_strResolution + "/";
	strPersonalityFileName = strPersonalityPath + CSTRConfigFileName;
	m_strPersonalityDir = strPersonalityPath;

	LOG_LEVEL3(QString("[Personality filename=") + strPersonalityFileName + "]");

	try
	{
		if( m_pConfig )
		{
			delete m_pConfig;
			m_pConfig=NULL;
		}

		m_pConfig = new GConfigReader(strPersonalityFileName);

		m_pConfig->Show();
	}
	catch(GException & e)
	{
		LOG_LEVEL1( QString("GConfigReader::Exception [") + e.GetMessage() + "]" );

		throw GPERSONALITY_EXCEPTION(eCantOpenPersonalityFilename);
	}
}
*/

QString GPersonality::GetStrUrlPrefix() const throw (GPersonalityException)
{
	LOG_LEVEL4("GetStrUrlPrefix()");

	QString strUrlPrefix;

	if( (strUrlPrefix=m_pConfig->GetString(CSTRURLPrefixParameter, "")).isEmpty() )
	{
		LOG_LEVEL3( GPERSONALITY_EXCEPTION(eCantFindUrlPrefixInPersonalityFile).GetMessage() );

		//throw GPERSONALITY_EXCEPTION(eCantFindUrlPrefixInPersonalityFile);

		return QString("");
	}

	return strUrlPrefix;
}

QString GPersonality::GetStrClearScreen() const throw (GPersonalityException)
{
	LOG_LEVEL4("GetStrClearScreen()");

	QString strClearScreen;

	if( (strClearScreen=m_pConfig->GetString(CSTRClearScreenParameter, "")).isEmpty() )
	{
		LOG_LEVEL1( GPERSONALITY_EXCEPTION(eCantFindClearScreenInPersonalityFile).GetMessage() );

		throw GPERSONALITY_EXCEPTION(eCantFindClearScreenInPersonalityFile);
	}

	return strClearScreen;
}

QString GPersonality::GetStrGraphicsPrefix( QString const & strPrefix ) const throw (GPersonalityException)
{
	LOG_LEVEL4("GetStrGraphicsPrefix()");

	QString strGraphicsPrefix;

	if( (strGraphicsPrefix=m_pConfig->GetString((QString(CSTRCustomGraphicPropertyNamePrefix)+strPrefix), "")).isEmpty() )
	{
		LOG_LEVEL1( GPERSONALITY_EXCEPTION(eCantFindGraphicInPersonalityFile).GetMessage() );

		throw GPERSONALITY_EXCEPTION(eCantFindGraphicInPersonalityFile);
	}

	return strGraphicsPrefix;
}

QString GPersonality::GetCustomGraphicProperty(QString const & strSuffix) const
{
	LOG_LEVEL4("GetCustomGraphicProperty()");

	return m_pConfig->GetString((QString(CSTRCustomGraphicPropertyNamePrefix)+strSuffix), "");
}

QString GPersonality::GetCustomGraphicProperty(int iSegment) const
{
	LOG_LEVEL4("GetCustomGraphicProperty()");

	return m_pConfig->GetString(QString(CSTRCustomGraphicPropertyNamePrefix)+QString("%1").arg(iSegment), "");

}

QString GPersonality::GetClockGraphicProperty(QString const & strSuffix) const
{
	LOG_LEVEL4("GetGraphicClockProperty()");

	return m_pConfig->GetString((QString(CSTRClockGraphicPropertyNamePrefix)+strSuffix), "");
}

QString GPersonality::GetClockGraphicProperty(int iPhase) const
{
	LOG_LEVEL4("GetGraphicClockProperty()");

	return m_pConfig->GetString(QString(CSTRClockGraphicPropertyNamePrefix)+QString("%1").arg(iPhase), "");
}

QString GPersonality::GetCustomScreenProperty(QString const & strSuffix) const
{
	LOG_LEVEL4("GetCustomScreenProperty()");

	return m_pConfig->GetString((QString(CSTRCustomScreenPropertyNamePrefix)+strSuffix), "");
}

QString GPersonality::GetCustomScreenProperty(int iScreen) const
{
	LOG_LEVEL4("GetCustomScreenProperty()");

	return m_pConfig->GetString(QString(CSTRCustomScreenPropertyNamePrefix)+QString("%1").arg(iScreen), "");
}

QString GPersonality::GetArrowGraphicProperty(GPersonality::eArrowDirection const iArrow) const
{
	LOG_LEVEL4("GetArrowGraphicProperty()");

	return m_pConfig->GetString(QString(CSTRArrowGraphicPropertyNameFrame).arg((iArrow==eLeftArrow)?(CSTRLeftArrowName):(CSTRRightArrowName)), "");

}

QString GPersonality::GetStrGraphicClockPrefix( QString const & strPrefix ) const throw (GPersonalityException)
{
	LOG_LEVEL4("GetStrGraphicClockPrefix()");

	QString strClockGraphicPrefix;

	if( (strClockGraphicPrefix=m_pConfig->GetString((QString(CSTRClockGraphicPropertyNamePrefix)+strPrefix), "")).isEmpty() )
	{
		LOG_LEVEL1( GPERSONALITY_EXCEPTION(eCantFindClockGraphicInPersonalityFile).GetMessage() );

		throw GPERSONALITY_EXCEPTION(eCantFindClockGraphicInPersonalityFile);
	}

	return strClockGraphicPrefix;
}

QString GPersonality::GetPersonalityDir() const
{
	LOG_LEVEL4("GetPersonalityDir()");

	LOG_LEVEL3( QString("Personality Dir [") + m_strPersonalityDir + "]" );

	return m_strPersonalityDir;
}

QVector<QChar> GPersonality::GetPersonalityLanguageMapping( ) const
{
	QString strPathLanguageMappingFile;

	if( (strPathLanguageMappingFile=m_pConfig->GetString(CSTRCharMappingFileName, "")).isEmpty() )
	{
		LOG_LEVEL3( QString("Cannot find language mapping filename [%1] in personality loaded.").arg(CSTRCharMappingFileName) );
	}
	else
    {
    	QFileInfo oFileInfo(strPathLanguageMappingFile);
		if ( oFileInfo.isRelative() )
		{
			oFileInfo.setFile(m_strPersonalityDir + strPathLanguageMappingFile);
		}

		if ( !oFileInfo.exists() )
		{
			LOG_LEVEL3( QString("Language mapping filename [%1] does not exist in personality loaded.").arg(oFileInfo.filePath()) );
		}
		else
		{
			try
			{
				GCharMapping oCharMapping(strPathLanguageMappingFile);

				QVector<QChar> qvResult(oCharMapping.GetCharVec());

				return qvResult;
			}
			catch(GCharMappingException& e)
			{
				LOG_LEVEL3( QString("Loading error [%1] on language mapping filename [%2] in personality loaded.").arg(e.what()).arg(oFileInfo.filePath()) );
			}
		}
    }

	LOG_LEVEL3( "Loading default Language mapping." );

	enum { eLanguageMappingTableSize = 256 };
	enum { eLanguageMappingNullCharacter = 0x0000 };

	QVector<QChar> qvResult(eLanguageMappingTableSize,QChar(eLanguageMappingNullCharacter));

	// Setting default Language Mapping;
	{
		enum {	eLowerBoundPrintableASCIICharacters = 24,
				eHigherBoundPrintableASCIICharacters = 126,
				eLowerBoundPrintableExtendedCharacters = 128,
				eHigherBoundPrintableExtendedCharacters = 173 };
														 // 128  // 129  // 130  // 131  // 132  // 133  // 134  // 135  // 136  // 137
		static QChar const aUnicodeReplacementList[] = { 0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7, 0x00ea, 0x00eb,
														 // 138  // 139  // 140  // 141  // 142  // 143  // 144  // 145  // 146  // 147
														 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5, 0x00c9, 0x00e6, 0x00c6, 0x00f4,
														 // 148  // 149  // 150  // 151  // 152  // 153  // 154  // 155  // 156  // 157
														 0x00f6, 0x00f2, 0x00fb, 0x00f9, 0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5,
														 // 158  // 159  // 160  // 161  // 162  // 163  // 164  // 165  // 166  // 167
														 0x20a7, 0x0192, 0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00b0,
														 // 168  // 169  // 170  // 171  // 172  // 173
														 0x00bf, 0x2122, 0x00ae, 0x00bd, 0x00bc, 0x00a1 };
		for (unsigned int uiI = eLowerBoundPrintableASCIICharacters, uiHigherBound = eHigherBoundPrintableASCIICharacters; uiI <= uiHigherBound; ++uiI)
		{
			qvResult[uiI] = QChar(uiI);
		}
		for (unsigned int uiI = eLowerBoundPrintableExtendedCharacters, uiHigherBound = eHigherBoundPrintableExtendedCharacters; uiI <= uiHigherBound; ++uiI)
		{
			qvResult[uiI] = QChar(aUnicodeReplacementList[static_cast<unsigned int>(uiI-eLowerBoundPrintableExtendedCharacters)]);
		}
	}

	return qvResult;
}

QStringList GPersonality::GetPersonalityList( ) throw (GPersonalityException)
{
	QString strPersonalityBaseDir;
	QString strPersonalityPath;

	if( (strPersonalityBaseDir=GETCONF_STR(eDisplayManagerPersonalityDir)).isEmpty() )
	{
		throw GPERSONALITY_EXCEPTION(eInvalidPersonalityDir);
	}

	strPersonalityPath = (strPersonalityBaseDir.endsWith("/"))? (strPersonalityBaseDir):(strPersonalityBaseDir+"/");

	return QDir(strPersonalityPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}

bool GPersonality::IsAValidPersonality( QString const & strPersonality )
{
	return GPersonality::GetPersonalityList().contains(strPersonality, Qt::CaseInsensitive);
}

int GPersonality::GetCustomGraphicPropertyDimension(GPersonality::eDimension const iDimension, QString const & strSuffix) const
{
	LOG_LEVEL4("GetCustomGraphicPropertyDimension()");
	return m_pConfig->GetInt(QString(CSTRCustomGraphicPropertyDimensionNameFrame).arg(strSuffix).arg((iDimension==eWidth)?(CSTRDimensionWidthName):(CSTRDimensionHeightName)),-1);
}

int GPersonality::GetClockGraphicPropertyDimension(GPersonality::eDimension const iDimension, int iPhase) const
{
	LOG_LEVEL4("GetClockGraphicPropertyDimension()");
	return m_pConfig->GetInt(QString(CSTRClockGraphicPropertyDimensionNameFrame).arg(iPhase).arg((iDimension==eWidth)?(CSTRDimensionWidthName):(CSTRDimensionHeightName)),-1);
}

int GPersonality::GetArrowGraphicPropertyDimension(GPersonality::eDimension const iDimension, GPersonality::eArrowDirection const iArrow) const
{
	LOG_LEVEL4("GetArrowGraphicPropertyDimension()");
	return m_pConfig->GetInt(QString(CSTRArrowGraphicPropertyDimensionNameFrame).arg((iArrow==eLeftArrow)?(CSTRLeftArrowName):(CSTRRightArrowName)).arg((iDimension==eWidth)?(CSTRDimensionWidthName):(CSTRDimensionHeightName)),-1);
}

int GPersonality::GetOverlapsErrorMarginPixels() const
{
	LOG_LEVEL4("GetOverlapsErrorMarginPixels()");
	return m_pConfig->GetInt( GPersonality::CSTROverlapsErrorMarginPixelsName, 0 );
}

bool GPersonality::IsRemoveOverlaps() const
{
	LOG_LEVEL4("IsRemoveOverlaps()");
	return m_pConfig->GetBool( GPersonality::CSTRRemoveOverlapsConditionName, true );
}

bool GPersonality::GetAValidPath(QString & strPath)
{
	LOG_LEVEL4("GetAValidPath()");

	QFileInfo oFileInfo(strPath);

	LOG_LEVEL4( QString("strPath [%1]").arg(strPath) );

	if( strPath.isEmpty() )
	{
		return false;
	}

	if( oFileInfo.isRelative() )
	{
		if( !GetStrUrlPrefix().isEmpty() )
		{
			strPath = GetStrUrlPrefix() + strPath;

			return true;
		}
		else
		{
			strPath = GetPersonalityDir() + strPath;

			oFileInfo.setFile(strPath);
		}
	}

	if( !oFileInfo.exists() )
	{
		if( QUrl(strPath).isValid() )
		{
			return true;
		}

		return false;
	}

	return true;
}

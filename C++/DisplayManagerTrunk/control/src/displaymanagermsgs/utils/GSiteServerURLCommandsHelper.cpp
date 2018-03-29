#include "GSiteServerURLCommandsHelper.h"
#include "utils/media/src/GAbsMediaManager.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/logger/src/GLogger.h"

QString const GSiteServerURLCommandsHelper::CstrAudioMediaTypeURLScheme("audio");
QString const GSiteServerURLCommandsHelper::CstrVideoMediaTypeURLScheme("video");

QChar const GSiteServerURLCommandsHelper::CcQueryValueDelim('=');
QChar const GSiteServerURLCommandsHelper::CcQueryPairDelim('&');

QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyUrl("url");
QString const GSiteServerURLCommandsHelper::CstrQueryItemValueUrlDefault("about:blank");

QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyX("x");
QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyY("y");
QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyWidth("h");
QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyHeight("v");

QString const GSiteServerURLCommandsHelper::CstrCommandVolumeMixer("volume-mixer");
QString const GSiteServerURLCommandsHelper::CstrQueryItemKeyMute("mute");

QString const GSiteServerURLCommandsHelper::CstrMuteTrue("true");
QString const GSiteServerURLCommandsHelper::CstrMuteFalse("false");

GSiteServerURLCommandsHelper::GSiteServerURLCommandsHelper(const QString &strUrl) throw (GException):
m_SiteServerURL(""),
m_strSecondURL(""),
m_strMediaAction(""),
m_eMediaTypeCommand(eNoMediaTypeCommand)
{
	SetSiteServerURL(strUrl);
}


GSiteServerURLCommandsHelper::~GSiteServerURLCommandsHelper()
{
}


QString GSiteServerURLCommandsHelper::GetSecondURL() const
{
	return m_strSecondURL;

}


void GSiteServerURLCommandsHelper::SetSiteServerURL(const QString &strUrl) throw (GException)
{
	LOG_LEVEL4("SetSiteServerURL()");

	m_SiteServerURL = QUrl(QUrl::fromEncoded(strUrl.toAscii(),QUrl::TolerantMode));
	m_SiteServerURL.setQueryDelimiters(CcQueryValueDelim.toAscii(),CcQueryPairDelim.toAscii());
	if (!m_SiteServerURL.isValid())
	{
		throw GEXCEPTION(QString("Invalid URL: %1").arg(m_SiteServerURL.toString()));
	}
}



void GSiteServerURLCommandsHelper::ObtainURL()
{
	LOG_LEVEL4("ObtainURL()");

	if (m_SiteServerURL.hasQueryItem(CstrQueryItemKeyUrl))
	{
		m_strSecondURL = m_SiteServerURL.queryItemValue(CstrQueryItemKeyUrl).trimmed();//???? Cannot imagine the use case in which we get an audio scheme and an URL????
		if (m_strSecondURL.isEmpty())
		{
			m_strSecondURL = CstrQueryItemValueUrlDefault;
		}
	}
}


void GSiteServerURLCommandsHelper::FirstLevelParse()
{
	LOG_LEVEL4("FirstLevelParse()");

	QString sScheme = m_SiteServerURL.scheme();

	if (sScheme == CstrAudioMediaTypeURLScheme)
	{
		m_eMediaTypeCommand = eAudioTypeCommand;
		m_strMediaAction = m_SiteServerURL.authority()+m_SiteServerURL.path();
		ObtainURL();

	} else if (sScheme == CstrVideoMediaTypeURLScheme)
	{
		m_eMediaTypeCommand = eVideoTypeCommand;
		m_strMediaAction = m_SiteServerURL.authority()+m_SiteServerURL.path();
		ObtainURL();
	}
	else
	{
			m_eMediaTypeCommand = eNoMediaTypeCommand;
			m_strSecondURL = m_SiteServerURL.toString();
	}

}

void GSiteServerURLCommandsHelper::ProcessAudioCommand()
{
	LOG_LEVEL3("ProcessAudioCommand()");
	
	QString sAudioCommand(m_strMediaAction.trimmed());

	if ((sAudioCommand == CstrCommandVolumeMixer) || (sAudioCommand.isEmpty()))
	{
                // Use factory to create the correct instance of the MediaManager
                GMediaManagerFactory mediaManagerFactory;
                QSharedPointer<GAbsMediaManager> pMediaManager = mediaManagerFactory.CreateMediaManager();

		QString sMuteValue(m_SiteServerURL.queryItemValue(CstrQueryItemKeyMute).trimmed());
		if (sMuteValue == CstrMuteTrue)
		{
			pMediaManager->MuteVideo();
		}
		else if (sMuteValue == CstrMuteFalse)
		{
			pMediaManager->UnMuteVideo();
		}
	}
}

void GSiteServerURLCommandsHelper::ProcessVideoCommand() throw(GException)
{
	LOG_LEVEL3("ProcessVideoCommand()");

	QString sVideoFile = m_strMediaAction.trimmed();
	if (!sVideoFile.isEmpty())
	{
                // Use factory to create the correct instance of the MediaManager
                GMediaManagerFactory mediaManagerFactory;
                QSharedPointer<GAbsMediaManager> pMediaManager = mediaManagerFactory.CreateMediaManager();

		bool bOk = false;
		int iPosX = m_SiteServerURL.queryItemValue(CstrQueryItemKeyX).trimmed().toInt(&bOk,10);
		int iPosY = m_SiteServerURL.queryItemValue(CstrQueryItemKeyY).trimmed().toInt(&bOk,10);
		int iWidth = m_SiteServerURL.queryItemValue(CstrQueryItemKeyWidth).trimmed().toInt(&bOk,10);
		int iHeight = m_SiteServerURL.queryItemValue(CstrQueryItemKeyHeight).trimmed().toInt(&bOk,10);

		pMediaManager->StartVideo(iWidth, iHeight, iPosX, iPosY,sVideoFile);
	}

}


void GSiteServerURLCommandsHelper::ProcessMediaCommands() throw(GException)
{
	LOG_LEVEL3("ProcessMediaCommands()");

	if (m_eMediaTypeCommand == eAudioTypeCommand)
	{
		ProcessAudioCommand();
	}
	else if (m_eMediaTypeCommand == eVideoTypeCommand)
	{
		ProcessVideoCommand();
	}
}



#include "GURLMsgData.h"

#include <QUrl>
#include <QRegExp>

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"


GURLMsgData::GURLMsgData()
{
	LOG_LEVEL4("GURLMsgData()");
}

GURLMsgData::~GURLMsgData()
{
	LOG_LEVEL4("~GURLMsgData()");
}

QString const & GURLMsgData::GetURL() const
{
	LOG_LEVEL4("GetURL()");

	return m_strURL;
}

void GURLMsgData::SetURL(QString const & strURL)
{
	LOG_LEVEL4("SetURL()");

	m_strURL = strURL;
}

void GURLMsgData::Validate() const throw(GURLException)
{
	LOG_LEVEL4("Validate()");

	LOG_LEVEL4("URL:"+m_strURL);

	{
		static QString const skQueryQueryDelim("?");
		static QString const skQueryPairDelim("&");
		int iDelimQry(m_strURL.indexOf(skQueryQueryDelim));
		int iDelimPr(m_strURL.indexOf(skQueryPairDelim));
		if (((iDelimQry<0) && (iDelimPr>=0)) || (iDelimQry>iDelimPr))
		{
			const_cast<GURLMsgData *>(this)->m_strURL.replace(iDelimPr,skQueryPairDelim.length(),skQueryQueryDelim);
			LOG_LEVEL4("FIXED URL:"+m_strURL);
		}
	}

	if( !QUrl(m_strURL).isValid() )
	{
		throw GURL_EXCEPTION("Invalid URL.");
	}
}

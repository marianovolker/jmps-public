#include "GPersonalityException.h"

const char* GPersonalityException::strPersonalityError[eLastError+1] =
{
	"No Errors",
	"Invalid personality dir configured at configuration file",
	"Invalid default personality configured at configuration file",
	"Invalid view width resolution configured at configuration file",
	"Invalid view height resolution configured at configuration file",
	"Can't open personality filename",
	"Can't find url_prefix variable in personality file",
	"Can't find clearscreen variable in personality file",
	"Can't find graphic_x_x variable in personality file",
	"Can't find clock_graphic_x variable in personality file",

	"Last Error"
};

GPersonalityException::GPersonalityException(GPersonalityException::ePersonalityError e, QString strDetails):
	GException(""),
	m_ePersonalityError(e)
{
	if( (m_ePersonalityError >= eNoErrors) && (m_ePersonalityError <= eLastError) )
	{
		m_strMsg = QString("GPersonalityException(") + strPersonalityError[m_ePersonalityError];

		if( !strDetails.isEmpty() )
		{
			m_strMsg += strDetails;
		}

		m_strMsg += ")";
	}
}

GPersonalityException::~GPersonalityException() throw()
{
}

GPersonalityException::ePersonalityError GPersonalityException::GetError()
{
	return m_ePersonalityError;
}

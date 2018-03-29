#include "GCharMappingException.h"

const char* GCharMappingException::strPersonalityError[eLastError+1] =
{
	"No Errors",
	"Can't open char map filename",

	"Last Error"
};

GCharMappingException::GCharMappingException(GCharMappingException::ePersonalityError e, QString strDetails):
	GException(""),
	m_ePersonalityError(e)
{
	if( (m_ePersonalityError >= eNoErrors) && (m_ePersonalityError <= eLastError) )
	{
		m_strMsg = QString("GCharMappingException(") + strPersonalityError[m_ePersonalityError];

		if( !strDetails.isEmpty() )
		{
			m_strMsg += strDetails;
		}

		m_strMsg += ")";
	}
}

GCharMappingException::~GCharMappingException() throw()
{
}

GCharMappingException::ePersonalityError GCharMappingException::GetError()
{
	return m_ePersonalityError;
}

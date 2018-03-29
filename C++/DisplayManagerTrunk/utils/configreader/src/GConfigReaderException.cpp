#include "GConfigReaderException.h"

const char* GConfigReaderException::strConfigReaderError[eLastError+1] =
{
	"No error occurred.",
	"An error occurred when reading from the file.",
	"An error occurred when writing to the file.",
	"A fatal error occurred.",
	"A resource error occurred.",
	"The file could not be opened.",
	"The operation was aborted.",
	"A timeout occurred.",
	"An unspecified error occurred.",
	"The file could not be removed.",
	"The file could not be renamed.",
	"The position in the file could not be changed.",
	"The file could not be resized.",
	"The file could not be accessed.",
	"The file could not be copied.",

	"Unknow Error"
};

GConfigReaderException::GConfigReaderException(GConfigReaderException::eConfigReaderError e, QString strDetails):
	GException(""),
	m_eConfigReaderError(e),
	m_strDetails(strDetails)
{
	if( (m_eConfigReaderError >= eNoError) && (m_eConfigReaderError <= eLastError) )
	{
		m_strMsg = QString("GConfigReaderException(") + strConfigReaderError[m_eConfigReaderError];

		if( !m_strDetails.isEmpty() )
		{
			m_strMsg += m_strDetails;
		}

		m_strMsg += ")";
	}
}

GConfigReaderException::~GConfigReaderException() throw()
{
}

GConfigReaderException::eConfigReaderError GConfigReaderException::GetError()
{
	return m_eConfigReaderError;
}

GConfigReaderException::eConfigReaderError GConfigReaderException::MapToInternalError(QFile::FileError qtFileError )
{
	switch(qtFileError)
	{
		case QFile::NoError:
			return eNoError;

		case QFile::ReadError:
			return eReadError;

		case QFile::WriteError:
			return eWriteError;

		case QFile::FatalError:
			return eFatalError;

		case QFile::ResourceError:
			return eResourceError;

		case QFile::OpenError:
			return eOpenError;

		case QFile::AbortError:
			return eAbortError;

		case QFile::TimeOutError:
			return eTimeOutError;

		case QFile::UnspecifiedError:
			return eUnspecifiedError;

		case QFile::RemoveError:
			return eRemoveError;

		case QFile::RenameError:
			return eRenameError;

		case QFile::PositionError:
			return ePositionError;

		case QFile::ResizeError:
			return eResizeError;

		case QFile::PermissionsError:
			return ePermissionsError;

		case QFile::CopyError:
			return eCopyError;
	}

	return eLastError;
}

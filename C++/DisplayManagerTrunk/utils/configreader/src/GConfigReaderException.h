//! GConfigReaderException
/*!
  Implementation of the Class GConfigReaderException
*/

#if !defined(GConfigReaderException_h)
#define GConfigReaderException_h

#include <QFile>
#include <stdarg.h>

#include "utils/base/src/GException.h"

#define GCONFIG_READER_EXCEPTION(x)\
	(\
		GConfigReaderException(GConfigReaderException::x, QString(" [at %1:%2]").arg(__FILE__).arg(__LINE__))\
	)

#define GCONFIG_READER_EXCEPTION_DETAILS(x,y)\
	(\
		GConfigReaderException(GConfigReaderException::x, QString(" [at %1:%2 [%3]]").arg(__FILE__).arg(__LINE__).arg(y))\
	)

class GConfigReaderException : public GException
{
	typedef enum
	{
		eNoError 			= 0 ,
		eReadError				,
		eWriteError				,
		eFatalError				,
		eResourceError			,
		eOpenError				,
		eAbortError				,
		eTimeOutError			,
		eUnspecifiedError		,
		eRemoveError			,
		eRenameError			,
		ePositionError			,
		eResizeError			,
		ePermissionsError		,
		eCopyError				,

		eLastError

	} eConfigReaderError;

public:

	GConfigReaderException(eConfigReaderError e, QString strDetails="");
	virtual ~GConfigReaderException() throw();

	static eConfigReaderError MapToInternalError(QFile::FileError qtFileError);
	eConfigReaderError GetError();

private:
	eConfigReaderError m_eConfigReaderError;
	QString m_strDetails;
	static const char* strConfigReaderError[eLastError+1];

};
#endif

//! GURLException
/*!
  Implementation of the Class GURLException
*/

#if !defined(GURLException_h)
#define GURLException_h

#include <stdarg.h>

#include "utils/base/src/GException.h"

#define GURL_EXCEPTION(x)\
	(\
		GURLException(x, QString(" [at %1:%2]").arg(__FILE__).arg(__LINE__))\
	)

#define GURL_EXCEPTION_DETAILS(x,y)\
	(\
		GURLException(x, QString(" [at %1:%2 [%3]]").arg(__FILE__).arg(__LINE__).arg(y))\
	)


class GURLException : public GException
{

public:

	GURLException(const QString &strMsg, QString strDetails="");
	virtual ~GURLException() throw();

};
#endif

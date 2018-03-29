//! GISException 
/*!
  Implementation of the Class GISException
*/

#if !defined(GISException_h)
#define GISException_h

#include <stdarg.h>

#include "utils/base/src/GException.h"
#include "GISCommandStatusEnum.h"

#define GIS_EXCEPTION(x)\
	(\
		GISException(x, QString(" [at %1:%2]").arg(__FILE__).arg(__LINE__))\
	)

#define GIS_EXCEPTION_DETAILS(x,y)\
	(\
		GISException(x, QString(" [at %1:%2 [%3]]").arg(__FILE__).arg(__LINE__).arg(y))\
	)


class GISException : public GException
{

public:
	GISException(GISCommandStatusEnum oCommandStatus, QString strDetails="");
	virtual ~GISException() throw();

	GISCommandStatusEnum GetError();

private:
	GISCommandStatusEnum m_oCommandStatus;

};
#endif

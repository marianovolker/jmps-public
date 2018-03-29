//! GPersonalityException
/*!
  Implementation of the Class GPersonalityException
*/

#if !defined(GPersonalityException_h)
#define GPersonalityException_h

#include "utils/base/src/GException.h"

#define GPERSONALITY_EXCEPTION(x)\
	(\
		GPersonalityException(GPersonalityException::x, QString(" [at %1:%2]").arg(__FILE__).arg(__LINE__))\
	)

#define GPERSONALITY_EXCEPTION_DETAILS(x,y)\
	(\
		GPersonalityException(GPersonalityException::x, QString(" [at %1:%2 [%3]]").arg(__FILE__).arg(__LINE__).arg(y))\
	)

class GPersonalityException : public GException
{
public:
	typedef enum
	{
		eNoErrors 								= 0,
		eInvalidPersonalityDir					= 1,
		eInvalidDefaultPersonality				= 2,
		eInvalidViewWidthResolution				= 3,
		eInvalidViewHeightResolution			= 4,
		eCantOpenPersonalityFilename			= 5,
		eCantFindUrlPrefixInPersonalityFile		= 6,
		eCantFindClearScreenInPersonalityFile	= 7,
		eCantFindGraphicInPersonalityFile		= 8,
		eCantFindClockGraphicInPersonalityFile	= 9,

		eLastError

	} ePersonalityError;

public:
	GPersonalityException(ePersonalityError e, QString strDetails="");
	virtual ~GPersonalityException() throw();

	ePersonalityError GetError();

private:
	ePersonalityError m_ePersonalityError;
	static const char* strPersonalityError[eLastError+1];

};
#endif

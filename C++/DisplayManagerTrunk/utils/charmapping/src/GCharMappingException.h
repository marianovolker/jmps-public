/*!
 * \file GCharMappingException.h
 *
 * \author guillermo tomasini
 * \date	09-21-2009
 *
 * Implementation of the Class GCharMappingException
 */

#if !defined(GCharMappingException_h)
#define GCharMappingException_h

#include "utils/base/src/GException.h"

#define GCHARMAPPING_EXCEPTION(x)\
	(\
		GCharMappingException(GCharMappingException::x, QString(" [at %1:%2]").arg(__FILE__).arg(__LINE__))\
	)

#define GCHARMAPPING_EXCEPTION_DETAILS(x,y)\
	(\
		GCharMappingException(GCharMappingException::x, QString(" [at %1:%2 [%3]]").arg(__FILE__).arg(__LINE__).arg(y))\
	)


/*!
 * \class GCharMappingException
 *
 * \brief exception class
 *
 * \author guillermo tomasini
 * \date	09-21-2009
 */
class GCharMappingException : public GException
{

public:
	typedef enum
	{
		eNoErrors 								= 0,
		eCantOpenCharMappingFilename			= 1,
		
		eLastError

	} ePersonalityError;

public:
	GCharMappingException(ePersonalityError e,
						 QString strDetails="");
	virtual ~GCharMappingException() throw();

	ePersonalityError GetError();

private:
	ePersonalityError m_ePersonalityError;
	static const char* strPersonalityError[eLastError+1];

};
#endif

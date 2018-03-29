// GFatalError.h

#ifndef GFatalError_h
#define GFatalError_h


#include <GFatalErrorImpl.h>


// GFatalError
//
// Application error handler.

class GFatalError : public GFatalErrorImpl
{
public:
	// Constructor
	GFatalError(char const * ckpMessage) : GFatalErrorImpl(ckpMessage)
	{
	}

	// Destructor
	~GFatalError(void)
	{
	}
private:
	// Prohibit GCondition copy.
	GFatalError(const GFatalError &right);
	GFatalError& operator=(const GFatalError &right);
};


#endif

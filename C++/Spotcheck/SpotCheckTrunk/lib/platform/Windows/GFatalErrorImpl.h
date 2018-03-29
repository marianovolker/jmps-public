// GFatalError.h

#ifndef GFatalErrorImpl_H
#define GFatalErrorImpl_H

// GFatalErrorImpl
//
// Windows application error handler.

class GFatalErrorImpl
{
protected:
	// Constructor
	GFatalErrorImpl(char const * ckpMessage);
	// Destructor
	~GFatalErrorImpl(void);
private:
	// Prohibit GCondition copy.
	GFatalErrorImpl(const GFatalErrorImpl &right);
	GFatalErrorImpl& operator=(const GFatalErrorImpl &right);
};


#endif

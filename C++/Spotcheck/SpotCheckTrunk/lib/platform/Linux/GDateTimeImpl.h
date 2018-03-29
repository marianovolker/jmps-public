// GDateTimeImpl.h
//
// GDateTime implementation for Linux.


#ifndef GDateTimeImpl_h
#define GDateTimeImpl_h

#include <GLinux.h>
#include <sys/time.h>

#include <string>
using namespace std;


// GDateTimeImpl

class GDateTimeImpl
{
public:
	// Constructor
	GDateTimeImpl(void);

	// Destructor
	virtual ~GDateTimeImpl(void);

	// Update() - Update to the current date/time.
	void Update(void);

	// GetDateStr() - Get string representation of objects stored date value.
	string GetDateStr(void);

	// GetTimeStr() - Get string representation of objects stored time value.
	string GetTimeStr(void);

private:
	struct timeval m_TimeVal;

	// Prohibit copy.
	GDateTimeImpl(const GDateTimeImpl &right);
	GDateTimeImpl& operator=(const GDateTimeImpl &right);
};


#endif

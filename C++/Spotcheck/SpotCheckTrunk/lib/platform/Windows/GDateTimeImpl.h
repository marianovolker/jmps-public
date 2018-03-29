// GDateTimeImpl.h
//
// GDateTime implementation for Windows.


#ifndef GDateTimeImpl_h
#define GDateTimeImpl_h

#include <GWindows.h>

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
	SYSTEMTIME m_SystemTime;

	// Prohibit copy.
	GDateTimeImpl(const GDateTimeImpl &right);
	GDateTimeImpl& operator=(const GDateTimeImpl &right);
};


#endif

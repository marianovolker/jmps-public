// GDateTime.h


#ifndef GDateTime_h
#define GDateTime_h


#include <GDateTimeImpl.h>


// GDateTime
//
// Standard application DateTime interface.
//
// Note: The current date/time will be recorded at object construction.
//       To update the date/time to the current date/time, use the Update method.

class GDateTime : private GDateTimeImpl
{
public:
	// Constructor
	GDateTime(void) : GDateTimeImpl()
	{
	}

	// Destructor
	~GDateTime(void)
	{
	}

	// Update() - Update to the current date/time.
	void Update(void)
	{
		GDateTimeImpl::Update();
	}

	// GetDateStr() - Get string representation of objects stored date value.
	string GetDateStr(void)
	{
		return GDateTimeImpl::GetDateStr();
	}

	// GetTimeStr() - Get string representation of objects stored time value.
	string GetTimeStr(void)
	{
		return GDateTimeImpl::GetTimeStr();
	}

private:
	// Prohibit GDateTime copy.
	GDateTime(const GDateTime &right);
	GDateTime& operator=(const GDateTime &right);
};


#endif

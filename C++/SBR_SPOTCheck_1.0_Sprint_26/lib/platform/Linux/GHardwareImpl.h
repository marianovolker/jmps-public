// GHardwareImpl.h
//
// GHardware implementation for Linux.

#ifndef GHardwareImpl_h
#define GHardwareImpl_h


#include "GMutex.h"
#include "GHardwareTypes.h"


// GHardwareImpl

class GHardwareImpl
{
protected:
	// Constructor.
	GHardwareImpl(void);

	// Destructor.
	virtual ~GHardwareImpl(void);

	// GetSideId() - Return hardware configured side identification.
	static GHardwareTypes::eSideId GetSideId(void);

	// SetSideId() - If possible, set side id for platform.
	static bool SetSideId(GHardwareTypes::eSideId Id);

	// GetDoorSwitchState() - Return hardware door switch state.
	static GHardwareTypes::eSwitchState GetDoorSwitchState(void);

	// SetDoorSwitchState() - If possible, set door switch state for platform.
	static bool SetDoorSwitchState(GHardwareTypes::eSwitchState State);
	
private:
	// Use exclusion mutex.
	static GMutex m_Mutex;

	static GHardwareTypes::eSideId m_SideId;
	static GHardwareTypes::eSwitchState m_DoorSwitchState;

	// Prohibit copy.
	GHardwareImpl(const GHardwareImpl &right);
	GHardwareImpl& operator=(const GHardwareImpl &right);
};

#endif

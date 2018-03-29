// GHardware.h

#ifndef GHardware_h
#define GHardware_h


#include "GHardwareTypes.h"
#include "GHardwareImpl.h"


// GHardware

class GHardware : private GHardwareImpl
{
public:
	// GetSideId() - Return hardware configured side identification.
	static GHardwareTypes::eSideId GetSideId(void)
	{
		return GHardwareImpl::GetSideId();
	}

	// SetSideId() - If possible, set side id for platform.
	static bool SetSideId(GHardwareTypes::eSideId Id)
	{
		return GHardwareImpl::SetSideId(Id);
	}

	// GetDoorSwitchState() - Return hardware door switch state.
	static GHardwareTypes::eSwitchState GetDoorSwitchState(void)
	{
		return GHardwareImpl::GetDoorSwitchState();
	}
	
	// SetDoorSwitchState() - If possible, set door switch state for platform.
	static bool SetDoorSwitchState(GHardwareTypes::eSwitchState State)
	{
		return GHardwareImpl::SetDoorSwitchState(State);
	}
	
private:
	// Constructor.
	GHardware() : GHardwareImpl()
	{
	}

	// Destructor.
	~GHardware()
	{
	}

	// Prohibit copy.
	GHardware(const GHardware &right);
	GHardware& operator=(const GHardware &right);
};

#endif



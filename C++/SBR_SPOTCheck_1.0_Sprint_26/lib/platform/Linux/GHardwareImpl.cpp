// GHardwareImpl.cpp
//
// GHardware implementation for Windows.

#include "GHardwareImpl.h"


// GHardwareImpl

GMutex GHardwareImpl::m_Mutex;
GHardwareTypes::eSideId GHardwareImpl::m_SideId = GHardwareTypes::eSideUnknown;
GHardwareTypes::eSwitchState GHardwareImpl::m_DoorSwitchState = GHardwareTypes::eSwitchStateUnknown;

// Constructor
GHardwareImpl::GHardwareImpl(void)
{

}

// Destructor
GHardwareImpl::~GHardwareImpl(void)
{

}

// GetSideId() - Return hardware configured side identification.
GHardwareTypes::eSideId GHardwareImpl::GetSideId(void)
{
	GMutexLock Lock(m_Mutex);
	return m_SideId;
}
	
// SetSideId() - If possible, set side id for platform.
bool GHardwareImpl::SetSideId(GHardwareTypes::eSideId Id)
{
	GMutexLock Lock(m_Mutex);
	m_SideId = Id;
	return true;
}

// GetDoorSwitchState() - Return hardware door switch state.
GHardwareTypes::eSwitchState GHardwareImpl::GetDoorSwitchState(void)
{
	GMutexLock Lock(m_Mutex);
	return m_DoorSwitchState;
}

// SetDoorSwitchState() - If possible, set door switch state for platform.
bool GHardwareImpl::SetDoorSwitchState(GHardwareTypes::eSwitchState State)
{
	GMutexLock Lock(m_Mutex);
	m_DoorSwitchState = State;
	return true;
}

/*
 * GSecurityModuleStateRuleProcessor.cpp
 *
 *  Created on: 08/10/2013
 *      Author: gparis
 */
#include "GSecurityModuleStateRuleProcessor.h"

#include "SPOT/SPOTProtocolInterface/GSpotSysSecurityModulePinpadApplications.h"

using namespace GSpotDefs;

// Public methods...

GSecurityModuleStateRuleProcessor::GSecurityModuleStateRuleProcessor()
	: GAbsStateRuleProcessor()
{
}

GSecurityModuleStateRuleProcessor::~GSecurityModuleStateRuleProcessor()
{
}

void GSecurityModuleStateRuleProcessor::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	Clear();

	m_u8Status   = u8Status;
	m_u8StatusEx = u8StatusEx;
	m_u8Error    = u8Error;

	if( (u8StatusEx & 0x80) == 0 )
	{
		m_bDismounted = true;
	}

	if( u8Status == eSPOT_SM_ERROR_STATE && u8Error == eSPOT_SM_GENERICERROR )
	{
		m_bDismounted = true;
	}
	else if( u8Status == eSPOT_SM_INOPERATIVE_STATE )
	{
		m_bOffline = true;
	}

	if( u8Error > 0)
	{
		if( u8Error == eSPOT_SM_DEVICECONNECTIONTIMEOUT ||
			u8Error == eSPOT_SM_DEVICECONNECTIONERROR   ||
			u8Error == eSPOT_SM_DEVICENOARMEDERROR )
		{
			m_bOffline = true;
		}
		else
		{
			m_bOther = true;
			AddNewDescription(GAbsStateRuleProcessor::eStatusType, u8Status);
			AddNewDescription(GAbsStateRuleProcessor::eErrorType,  u8Error);
		}
	}
}

void GSecurityModuleStateRuleProcessor::ProcessServiceNotification(UINT8 u8Ack)
{
	m_u8Ack = u8Ack;
	m_bOther = true;

	AddNewDescription(GAbsStateRuleProcessor::eAckType, u8Ack);
}


// Protected methods..

bool GSecurityModuleStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code)
{
	if( eType == GAbsStateRuleProcessor::eStatusType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetStatesDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eStatusExType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eErrorType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysSecurityModulePinpadApplications::GetOrCreateInstance()->GetErrorsDescription(u8Code)));
	}

	// use the default ack-code table.
	return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code);
}


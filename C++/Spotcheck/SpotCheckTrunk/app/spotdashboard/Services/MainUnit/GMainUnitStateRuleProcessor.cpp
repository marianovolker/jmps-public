/*
 * GMainUnitStateRuleProcessor.cpp
 *
 *  Created on: 07/10/2013
 *      Author: gparis
 */
#include "GMainUnitStateRuleProcessor.h"

#include "SPOT/SPOTProtocolInterface/GSpotSysMainPinpadApplications.h"

using namespace GSpotDefs;

// Public methods...

GMainUnitStateRuleProcessor::GMainUnitStateRuleProcessor()
	: GAbsStateRuleProcessor()
{
}

GMainUnitStateRuleProcessor::~GMainUnitStateRuleProcessor()
{
}

void GMainUnitStateRuleProcessor::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	Clear();

	m_u8Status   = u8Status;
	m_u8StatusEx = u8StatusEx;
	m_u8Error    = u8Error;

	if(u8StatusEx > 0) // Is M5 unit?
	{
		if(u8Error > 0) // only M5 units show this kind of "consolidated error"
		{
			// consolidated warning: "Some device in this SPOT is in error."
			AddNewDescription(GAbsStateRuleProcessor::eErrorType,  u8Error);
		}
	}
}

void GMainUnitStateRuleProcessor::ProcessServiceNotification(UINT8 u8Ack)
{
	m_u8Ack = u8Ack;

	if(u8Ack > eSPOT_NO_ERROR)
	{
		m_bOtherError = true;
		AddNewDescription(GAbsStateRuleProcessor::eAckType, u8Ack);
	}
}


// Protected methods..

bool GMainUnitStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code)
{
	if( eType == GAbsStateRuleProcessor::eStatusType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetStatesDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eStatusExType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eErrorType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysMainPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(u8Code)));
	}

	// use the default ack-code table.
	return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code);
}


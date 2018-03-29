/*
 * GKeypadStateRuleProcessor.cpp
 *
 *  Created on: 01/10/2013
 *      Author: gparis
 */
#include "GKeypadStateRuleProcessor.h"

#include "SPOT/SPOTProtocolInterface/GSpotSysKeyboardPinpadApplications.h"

using namespace GSpotDefs;


// Public methods...

GKeypadStateRuleProcessor::GKeypadStateRuleProcessor()
	: GAbsStateRuleProcessor()
{
}

GKeypadStateRuleProcessor::~GKeypadStateRuleProcessor()
{
}

void GKeypadStateRuleProcessor::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	Clear();

	m_u8Status   = u8Status;
	m_u8StatusEx = u8StatusEx;
	m_u8Error    = u8Error;

	if( (u8StatusEx & 0x80) == 0 )
	{
		m_bDismounted = true;
	}

	if( u8Status < eSPOT_KB_ERROR_STATE )
	{
		return;
	}

	if( u8Status == eSPOT_KB_ERROR_STATE && u8Error == eSPOT_KB_ERROR_STATE )
	{
		m_bDismounted = true;
	}

	else if( u8Status == eSPOT_KB_INOPERATIVE_STATE )
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

void GKeypadStateRuleProcessor::ProcessServiceNotification(UINT8 u8Ack)
{
	m_u8Ack = u8Ack;

	m_bOther = true;
	AddNewDescription(GAbsStateRuleProcessor::eAckType, u8Ack);
}


// Protected methods..

bool GKeypadStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code)
{
	if( eType == GAbsStateRuleProcessor::eStatusType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetStatesDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eStatusExType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eErrorType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysKeyboardPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(u8Code)));
	}

	return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code);
}







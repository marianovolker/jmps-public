/*
 * GCardReaderStateRuleProcessor.cpp
 *
 *  Created on: 01/10/2013
 *      Author: gparis
 */
#include "GCardReaderStateRuleProcessor.h"

#include "SPOT/SPOTProtocolInterface/GSpotSysCardReaderPinpadApplications.h"

using namespace GSpotDefs;

// Public methods...

GCardReaderStateRuleProcessor::GCardReaderStateRuleProcessor()
	: GAbsStateRuleProcessor()
{
}

GCardReaderStateRuleProcessor::~GCardReaderStateRuleProcessor()
{
}

void GCardReaderStateRuleProcessor::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	Clear();

	m_u8Status   = u8Status;
	m_u8StatusEx = u8StatusEx;
	m_u8Error    = u8Error;

	if( u8StatusEx & 0x01 ) // card left inside the reader ?
	{
		AddNewDescription(GAbsStateRuleProcessor::eStatusExType, 0x01);
	}

	if( u8StatusEx & 0x02 ) // card inside is a chip card ?
	{
		AddNewDescription(GAbsStateRuleProcessor::eStatusExType, 0x02);
	}

	if( (u8StatusEx & 0x80) == 0 )
	{
		m_bDismounted = true;
	}

	if( u8Status < eSPOT_CR_ERROR_STATE && u8Error == eSPOT_CR_NOERROR )
	{
		return; // stop processing if there is not further errors.
	}

	if( u8Status == eSPOT_CR_ERROR_STATE && u8Error == eSPOT_CR_GENERICERROR )
	{
		m_bDismounted = true;
		return;
	}

	if( u8Status == eSPOT_CR_INOPERATIVE_STATE )
	{
		if(u8Error == eSPOT_CR_DEVICECONNECTIONTIMEOUT)
			m_bOtherError = true;
		else
			m_bOffline = true;
	}
	else // other errors
	{
		m_bOtherError = true;
	}

	AddNewDescription(GAbsStateRuleProcessor::eStatusType, u8Status);
	AddNewDescription(GAbsStateRuleProcessor::eErrorType,  u8Error);
}

void GCardReaderStateRuleProcessor::ProcessServiceNotification(UINT8 u8Ack)
{
	m_u8Ack = u8Ack;

	if(u8Ack > eSPOT_NO_ERROR)
	{
		m_bOtherError = true;
		AddNewDescription(GAbsStateRuleProcessor::eAckType, u8Ack);
	}
}


// Protected methods..

bool GCardReaderStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code)
{
	if( eType == GAbsStateRuleProcessor::eStatusType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetStatesDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eStatusExType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetStateExtensionsDescription(u8Code)));
	}
	else if( eType == GAbsStateRuleProcessor::eErrorType )
	{
		return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code, std::string(GSpotSysCardReaderPinpadApplications::GetOrCreateInstance()->GetErrorsDescription(u8Code)));
	}

	// Description type is ack code.
	return GAbsStateRuleProcessor::AddNewDescription(eType, u8Code);
}


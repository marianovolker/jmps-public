/*
 * GAbsStateRuleProcessor.cpp
 *
 *  Created on: 01/10/2013
 *      Author: gparis
 */

#include "GAbsStateRuleProcessor.h"

#include "SPOT/SPOTProtocolInterface/GAckCodesPinpadApplications.h"

// Public methods...
GAbsStateRuleProcessor::GAbsStateRuleProcessor()
	: m_bDismounted(false), m_bTampered(false), m_bOffline(false), m_bOther(false),
	  m_u8Status(0x00), m_u8StatusEx(0x00), m_u8Error(0x00), m_u8Ack(0x00)
{
}

GAbsStateRuleProcessor:: ~GAbsStateRuleProcessor()
{
}

const char * GAbsStateRuleProcessor::GetDescription(eCodeType eType, UINT8 u8Code) const
{
	UINT32 key = MakeKey(eType, u8Code);
	MapType::const_iterator i = m_descriptionMap.find(key);
	if (i != m_descriptionMap.end())
	{
		std::string s = (*i).second;
		return s.c_str();
	};
	return "Description not found at dictionary description type.";
}

std::string GAbsStateRuleProcessor::GetDescriptionList() const
{
	std::string s = "";

	MapType::const_iterator i = m_descriptionMap.begin();

	while (i != m_descriptionMap.end())
	{
		s += (*i).second + "\n";

		++i;
	};

	return s;

	
}



// Protected methods...

bool GAbsStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code)
{
	if(eType == eAckType)
	{
		return AddNewDescription(eType, u8Code, std::string(GAckCodesPinpadApplications::GetOrCreateInstance()->GetDescription(u8Code)));
	}

	return false;
}

bool GAbsStateRuleProcessor::AddNewDescription(eCodeType eType, UINT8 u8Code, std::string sDescription)
{
	return m_descriptionMap.insert( std::pair<UINT32, std::string>(MakeKey(eType, u8Code), sDescription) ).second;
}




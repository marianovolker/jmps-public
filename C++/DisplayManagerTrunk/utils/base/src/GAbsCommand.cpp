#include "GAbsCommand.h"

#include "utils/logger/src/GLogger.h"

GAbsCommand::GAbsCommand(GAbsData * pData) 
	: m_pData(pData),
	m_pResponse(NULL)
{
	LOG_LEVEL4("GAbsCommand()");

}

GAbsCommand::~GAbsCommand()
{
	LOG_LEVEL4("~GAbsCommand()");

	if(m_pData != NULL)
	{
		delete m_pData;
		m_pData = NULL;
	}
	
	if(m_pResponse != NULL)
	{
		delete m_pResponse;
		m_pResponse = NULL;
	}	
}

GAbsData * GAbsCommand::GetData()
{
	LOG_LEVEL4("GetData()");

	return m_pData;
}

void GAbsCommand::SetData(GAbsData * pData)
{
	LOG_LEVEL4("SetData()");

	m_pData = pData;
}

GAbsData * GAbsCommand::GetResponse()
{
	LOG_LEVEL4("GetResponse()");

	return m_pResponse;
}

void GAbsCommand::SetResponse(GAbsData * pResponse)
{
	LOG_LEVEL4("SetResponse()");

	m_pResponse = pResponse;
}

#include "GISAbsCommand.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/data/GISCommandResponse.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"
#include <QDebug>

GISAbsCommand::GISAbsCommand()
{
	LOG_LEVEL4("GISAbsCommand()");
}

GISAbsCommand::~GISAbsCommand()
{
	LOG_LEVEL4("~GISAbsCommand()");
}

void GISAbsCommand::Execute() throw(GException)
{
	LOG_LEVEL3("Execute()");

	GISCommandResponse * pResponse = GetResponse();
	if( pResponse )
	{
		delete pResponse;
		pResponse = NULL;
		SetResponse(pResponse);
	}

	pResponse = new GISCommandResponse();
	
	GISAbsData* pGISAbsData = GetData();
	
	pResponse->SetCommand(pGISAbsData->GetCommand().toInt());
	pResponse->SetDisplay(pGISAbsData->GetDisplay());

	SetResponse(pResponse);	//Set pResponse so as to make it available for DoExecute(), in case that it's needed for calling SetData.
	
	try
	{

		pGISAbsData->Validate();

		DoExecute();

		pResponse->SetCommandStatus(GISCommandStatusEnum::eNoErrors);
	}
	catch( GISException e )
	{
		if( e.GetError().GetError() == GISCommandStatusEnum::eApplicationSaveComplete) //! This if is just to clarify that is not an error. 
		{
                        pResponse->SetCommandStatus(GISCommandStatusEnum::eApplicationSaveComplete);
		}
		else
		{
			pResponse->SetCommandStatus(e.GetError());
		}
		LOG_LEVEL3(e.GetMessage());
	}
	

}

GISAbsData * GISAbsCommand::GetData()
{
	LOG_LEVEL4("GetData()");

	return static_cast<GISAbsData*>(GAbsCommand::GetData());
}

void GISAbsCommand::SetData(GISAbsData* pData)
{
	LOG_LEVEL4("SetData()");

	GAbsCommand::SetData(pData);
}

GISCommandResponse* GISAbsCommand::GetResponse()
{
	LOG_LEVEL4("GetResponse()");

	return static_cast<GISCommandResponse*>(GAbsCommand::GetResponse());
}

void GISAbsCommand::SetResponse(GISCommandResponse* pResponse)
{
	LOG_LEVEL4("GetResponse()");

	GAbsCommand::SetResponse(pResponse);
}


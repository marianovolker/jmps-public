#include "control/src/displaymanagermsgs/command/GClockMsgCommand.h"
#include "control/src/displaymanagermsgs/xml/GClockMsgDataXML.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "control/src/displaymanagermsgs/data/GClockMsgData.h"
#include "control/src/displaymanagermsgs/utils/GClockThread.h"
#include "model/src/gdimodel/GGDIModelSingleton.h"

#include "utils/logger/src/GLogger.h"

#include <QMutexLocker>
#include <memory>
#include <QSharedPointer>

GClockMsgCommand::GClockMsgCommand(GAbsData *pData)
	: GAbsMsgCommand(pData)
{
}

GClockMsgCommand::~GClockMsgCommand()
{
}

void GClockMsgCommand::Execute() throw(GException)
{
	LOG_LEVEL4("Execute()");

	std::auto_ptr<GResponseMsgData> pResponse(CreateInitializedResponse(GClockMsgDataXML::CXMLTag, GMsgCmdDataFactory::CInterfaceVersion));

	GResponseMsgData::eResponseCode eCode;

	try
	{
		GClockMsgData *pRequestData = static_cast<GClockMsgData *>(m_pData);
		if (!pRequestData)
		{
			eCode = GResponseMsgData::eError;
			throw GEXCEPTION("Null pointer for GClockMsgData *pRequestData.");
		}

		try 
		{
			pRequestData->Validate();
		}
		catch(GException &ex)
		{
			eCode = GResponseMsgData::eParseError;
			throw;
		}

		GClockMsgData::eActionTypes eType = pRequestData->GetAction();
		GClockThread *pClock;

		if (eType == GClockMsgData::eStart || eType == GClockMsgData::eResume)
		{
			if (!GClockThread::Instance())
			{
				pClock = GClockThread::CreateSoleInstance();
				if (!pClock)
				{
					eCode = GResponseMsgData::eError;
					throw GEXCEPTION("Null pointer for GClockThread.");
				}

				LOG_LEVEL4("Starting Clock rotation...");

				pClock->SetCoordinates(pRequestData->GetXpos(), pRequestData->GetYpos());

				pClock->start();
			}
		}
		else if (eType == GClockMsgData::eStop || eType == GClockMsgData::ePause)
		{
			pClock = GClockThread::Instance();
			if (pClock)
			{
				LOG_LEVEL4("Stopping Clock rotation...");

				pClock->Stop();

				GClockThread::RemoveSoleInstance();
			}
		}
		else
		{
			eCode = GResponseMsgData::eParseError;
			throw GEXCEPTION("Type unsupported.");
		}
		
		pResponse->SetResponseCode(GResponseMsgData::eOk);
	}
	catch(GException &ex)
	{
		LOG_LEVEL1(QString("Exception: ") + ex.GetMessage());

		pResponse->SetResponseCode(eCode);
	}

	SetResponseNamespace(pResponse);

	m_pResponse = pResponse.release();
}

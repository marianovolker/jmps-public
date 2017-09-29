#include "GISDeleteDisplayMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISDeleteDisplayMsgData::m_CName="DeleteDisplayCommand";
const char * GISDeleteDisplayMsgData::m_CCommandId="005";

GISDeleteDisplayMsgData::GISDeleteDisplayMsgData()
{
	LOG_LEVEL4("GISDeleteDisplayMsgData()");
}

GISDeleteDisplayMsgData::~GISDeleteDisplayMsgData()
{
	LOG_LEVEL4("~GISDeleteDisplayMsgData()");
}

void GISDeleteDisplayMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 */
	LOG_LEVEL4("Validate()");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != 1)
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(GetDisplay());
        if( poDisplay == NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
        }
    }
}

QString GISDeleteDisplayMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISDeleteDisplayMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

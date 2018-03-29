#include "GISClearScreenMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISClearScreenMsgData::m_CName="ClearScreenCommand";
const char * GISClearScreenMsgData::m_CCommandId="029";

GISClearScreenMsgData::GISClearScreenMsgData()
{
	LOG_LEVEL4("GISClearScreenMsgData()");
}

GISClearScreenMsgData::~GISClearScreenMsgData()
{
	LOG_LEVEL4("~GISClearScreenMsgData()");
}

void GISClearScreenMsgData::Validate() throw(GException)
{
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

QString GISClearScreenMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISClearScreenMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

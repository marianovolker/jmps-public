#include "GISStatusMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISStatusMsgData::m_CName="StatusCommand";
const char * GISStatusMsgData::m_CCommandId="019";

GISStatusMsgData::GISStatusMsgData()
{
	LOG_LEVEL4("GISStatusMsgData()");
}

GISStatusMsgData::~GISStatusMsgData()
{
	LOG_LEVEL4("~GISStatusMsgData()");
}

void GISStatusMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	*/
	LOG_LEVEL4("Validate()");
	
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

QString GISStatusMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISStatusMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

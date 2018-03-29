#include "GISDeleteDisplayMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


GISDeleteDisplayMsgCommand::GISDeleteDisplayMsgCommand()
{
	LOG_LEVEL4("GISDeleteDisplayMsgCommand()");
}

GISDeleteDisplayMsgCommand::~GISDeleteDisplayMsgCommand()
{
	LOG_LEVEL4("~GISDeleteDisplayMsgCommand()");
}

void GISDeleteDisplayMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();


	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());
        pModel->RemoveDisplay(pGISAbsData->GetDisplay());
    }
}

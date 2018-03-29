#include "GISClearScreenMsgCommand.h"

#include <QSharedPointer>

#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "utils/logger/src/GLogger.h"

GISClearScreenMsgCommand::GISClearScreenMsgCommand()
{
	LOG_LEVEL4("GISClearScreenMsgCommand()");
}

GISClearScreenMsgCommand::~GISClearScreenMsgCommand()
{
	LOG_LEVEL4("~GISClearScreenMsgCommand()");
}

void GISClearScreenMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();


	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        poDisplay->SetCurrentScreen(NULL);

        pModel->ClearView(pGISAbsData->GetSynchronousRequest());

    }
}

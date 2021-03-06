#include "GISDeleteFieldMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


GISDeleteFieldMsgCommand::GISDeleteFieldMsgCommand()
{
	LOG_LEVEL4("GISDeleteFieldMsgCommand()");
}

GISDeleteFieldMsgCommand::~GISDeleteFieldMsgCommand()
{
	LOG_LEVEL4("~GISDeleteFieldMsgCommand()");
}

void GISDeleteFieldMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();
	

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());

        GSegment* poSegment = poScreen->GetSegment(pGISAbsData->GetSegment());

        poSegment->RemoveField(pGISAbsData->GetField());
    //
    //	if( poDisplay->IsCurrentScreen(poScreen) )
    //	{
    //		//Current Screen Changed
    //	}

    }
}

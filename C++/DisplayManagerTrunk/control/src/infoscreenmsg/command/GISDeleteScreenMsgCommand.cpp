#include "GISDeleteScreenMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

GISDeleteScreenMsgCommand::GISDeleteScreenMsgCommand()
{
	LOG_LEVEL4("GISDeleteScreenMsgCommand()");
}

GISDeleteScreenMsgCommand::~GISDeleteScreenMsgCommand()
{
	LOG_LEVEL4("~GISDeleteScreenMsgCommand()");
}

void GISDeleteScreenMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();


	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

   {
        QMutexLocker mtxScope(pModel->GetMutex());
	
        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());

        if( poDisplay->IsCurrentScreen(poScreen) )
        {
            poDisplay->SetCurrentScreen(NULL);
            //Current Screen Changed
        }

        poDisplay->RemoveScreen(pGISAbsData->GetScreen());
   }

}

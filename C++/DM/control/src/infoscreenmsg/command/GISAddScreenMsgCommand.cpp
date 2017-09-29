#include "GISAddScreenMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"

#include "utils/logger/src/GLogger.h"

#include <QMutexLocker>


GISAddScreenMsgCommand::GISAddScreenMsgCommand()
{
	LOG_LEVEL4("GISAddScreenMsgCommand()");
}

GISAddScreenMsgCommand::~GISAddScreenMsgCommand()
{
	LOG_LEVEL4("~GISAddScreenMsgCommand()");
}

void GISAddScreenMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();
	
	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

   {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());
        if( poScreen == NULL )
        {
            poScreen = new GScreen(pGISAbsData->GetScreen(), poDisplay);
            poScreen->SetBGColor(pGISAbsData->GetColor());
            poDisplay->AddScreen(poScreen);
        }
   }

}

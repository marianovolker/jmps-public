#include "GISDisplayScreenMsgCommand.h"

#include "utils/logger/src/GLogger.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include <QMutexLocker>
#include <QDebug>


GISDisplayScreenMsgCommand::GISDisplayScreenMsgCommand()
{
	LOG_LEVEL4("GISDisplayScreenMsgCommand()");
}

GISDisplayScreenMsgCommand::~GISDisplayScreenMsgCommand()
{
	LOG_LEVEL4("~GISDisplayScreenMsgCommand()");
}

void GISDisplayScreenMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

    
	GISAbsData* pGISAbsData = GetData();
	
	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();
    {
        QMutexLocker mtxScope(pModel->GetMutex());
        
        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());
        
        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());
        
        // Set the screen to show as the current one
        poDisplay->SetCurrentScreen(poScreen);
        
        pModel->UpdateView(pGISAbsData->GetSynchronousRequest());
    }
}

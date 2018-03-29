#include "GISSetFieldAttributeMsgCommand.h"

#include <QSharedPointer>

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"

#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


GISSetFieldAttributeMsgCommand::GISSetFieldAttributeMsgCommand()
{
	LOG_LEVEL4("GISSetFieldAttributeMsgCommand()");
}

GISSetFieldAttributeMsgCommand::~GISSetFieldAttributeMsgCommand()
{
	LOG_LEVEL4("~GISSetFieldAttributeMsgCommand()");
}

void GISSetFieldAttributeMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();


	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());

        GSegment* poSegment = poScreen->GetSegment(pGISAbsData->GetSegment());

        GTextField* poField = poSegment->GetField(pGISAbsData->GetField());
        if( poField != NULL )
        {
            poField->SetAttribute(pGISAbsData->GetAttr());
        }
    }

}

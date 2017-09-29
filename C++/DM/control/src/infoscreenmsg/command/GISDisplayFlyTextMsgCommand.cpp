#include "GISDisplayFlyTextMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


GISDisplayFlyTextMsgCommand::GISDisplayFlyTextMsgCommand()
{
	LOG_LEVEL4("GISDisplayFlyTextMsgCommand()");
}

GISDisplayFlyTextMsgCommand::~GISDisplayFlyTextMsgCommand()
{
	LOG_LEVEL4("~GISDisplayFlyTextMsgCommand()");
}

void GISDisplayFlyTextMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();


	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

   {
        QMutexLocker mtxScope(pModel->GetMutex());

        GTextField* poField;
        poField = new GTextField(0, NULL);

        poField->SetXpos(pGISAbsData->GetXpos());
        poField->SetYpos(pGISAbsData->GetYpos());
        poField->SetAttribute(pGISAbsData->GetAttr());
        poField->SetFont(pGISAbsData->GetFont());

        GISCommandBuffer oCommandBuffer(pGISAbsData->GetVLD());

        // Stores the text field, see infoscreen documentation
        poField->SetText(oCommandBuffer.GetNextField());

        pModel->UpdateViewSection(poField,pGISAbsData->GetSynchronousRequest());
   }
}

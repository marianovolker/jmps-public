#include "GISAddSegmentMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"

#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>



GISAddSegmentMsgCommand::GISAddSegmentMsgCommand()
{
	LOG_LEVEL4("GISAddSegmentMsgCommand()");
}

GISAddSegmentMsgCommand::~GISAddSegmentMsgCommand()
{
	LOG_LEVEL4("~GISAddSegmentMsgCommand()");
}

void GISAddSegmentMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());
	
        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());


        GSegment * poSegment = poScreen->GetSegment(pGISAbsData->GetSegment());
        if( poSegment == NULL )
        {
            bool bOk(true);
            GISCommandBuffer oCommandBuffer(pGISAbsData->GetVLD());
            int iHeight = oCommandBuffer.GetNextField().toInt(&bOk);
            int iWidth = oCommandBuffer.GetNextField().toInt(&bOk);

            poSegment = new GSegment(pGISAbsData->GetSegment(), poScreen);

            poSegment->SetBGColor(pGISAbsData->GetColor());
            poSegment->SetXPos(pGISAbsData->GetXpos());
            poSegment->SetYPos(pGISAbsData->GetYpos());
            poSegment->SetHeight(iHeight);
            poSegment->SetWidth(iWidth);
            (pGISAbsData->GetAttr()==GAttributeEnum::eVisible)? (poSegment->SetVisible(true)):(poSegment->SetVisible(false));

            poScreen->AddSegment(poSegment);
        }
    }
}

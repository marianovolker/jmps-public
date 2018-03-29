#include "GISAddFieldMsgCommand.h"

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


GISAddFieldMsgCommand::GISAddFieldMsgCommand()
{
	LOG_LEVEL4("GISAddFieldMsgCommand()");
}

GISAddFieldMsgCommand::~GISAddFieldMsgCommand()
{
	LOG_LEVEL4("~GISAddFieldMsgCommand()");
}

void GISAddFieldMsgCommand::DoExecute() throw (GException)
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

        if( poField == NULL )
        {
            poField = new GTextField(pGISAbsData->GetField(), poSegment);
            if( poField != NULL )
            {


                poField->SetXpos(pGISAbsData->GetXpos());
                poField->SetYpos(pGISAbsData->GetYpos());
                poField->SetAttribute(pGISAbsData->GetAttr());
                poField->SetFont(pGISAbsData->GetFont());

                GISCommandBuffer oCommandBuffer(pGISAbsData->GetVLD());

                // Ignoring the ATRMOD field.
                oCommandBuffer.GetNextField();

                // Ignoring the DICT field.
                oCommandBuffer.GetNextField();

                // Stores the text field, see infoscreen documentation
                poField->SetText(oCommandBuffer.GetNextField());

                poSegment->AddField(poField);
            }

        }

    }

}

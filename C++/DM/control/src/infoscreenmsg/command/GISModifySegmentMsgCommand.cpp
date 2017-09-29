#include "GISModifySegmentMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "utils/logger/src/GLogger.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GISSegmentAttributeEnum.h"
#include <QSharedPointer>
#include <QMutexLocker>


GISModifySegmentMsgCommand::GISModifySegmentMsgCommand()
{
	LOG_LEVEL4("GISModifySegmentMsgCommand()");
}

GISModifySegmentMsgCommand::~GISModifySegmentMsgCommand()
{
	LOG_LEVEL4("~GISModifySegmentMsgCommand()");
}

void GISModifySegmentMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();
	{
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());

        GSegment* poSegment = poScreen->GetSegment(pGISAbsData->GetSegment());

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Make GSegment Clon
		// The rest do it for the clon instead of poSegment
		//	GSegment *poClonedSegment = GSegment::Clone(poSegment);
		//
		//	poClonedSegment->SetBGColor(pGISAbsData->GetColor());
		//	if (pGISAbsData->GetAttr()==GAttributeEnum::eInvisible)
		//	{// If Attr is not zero call old setters and getters
		//		poClonedSegment->SetVisible(false);
		//	}
		//	else
		//	{// GetAttr is Visible otherwise ( all other options where discarded in Validate of MsgData ).
		//
		//		poClonedSegment->SetVisible(true);
		//	}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Java implements this command in the following way, the Spec seems to prefer the one commented above.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        poSegment->SetBGColor(pGISAbsData->GetColor());

        if (pGISAbsData->GetAttr()== GISSegmentAttributeEnum::eInvisible)
        {
        	// If Attr is not zero call old setters and getters
            poSegment->SetVisible(false);
        }
        else
        {
        	// GetAttr is Visible otherwise ( all other options where discarded in Validate of MsgData ).
            poSegment->SetVisible(true);
        }

        GSegment *poClonedSegment = GSegment::Clone(poSegment);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //Call Update section (instead of updateview) with a pointer to the clon (without validation)
        pModel->UpdateViewSection(poClonedSegment,pGISAbsData->GetSynchronousRequest());
	}
}

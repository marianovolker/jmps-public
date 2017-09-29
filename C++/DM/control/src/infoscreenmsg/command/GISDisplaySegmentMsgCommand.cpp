#include "GISDisplaySegmentMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISException.h"
#include "utils/logger/src/GLogger.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GAttributeEnum.h"

#include <QSharedPointer>
#include <QMutexLocker>


GISDisplaySegmentMsgCommand::GISDisplaySegmentMsgCommand()
{
	LOG_LEVEL4("GISDisplaySegmentMsgCommand()");
}

GISDisplaySegmentMsgCommand::~GISDisplaySegmentMsgCommand()
{
	LOG_LEVEL4("~GISDisplaySegmentMsgCommand()");
}

void GISDisplaySegmentMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();
    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GScreen* poScreen = poDisplay->GetScreen(pGISAbsData->GetScreen());

        GSegment* poSegment = poScreen->GetSegment(pGISAbsData->GetSegment());

		//Erase the content of the if and else branches. Leave the empty if commented.
		//Whichever value the attribute has, make a gsegment clon and do what is done to the poSegment in if branch, to the clon
		//	if (pGISAbsData->GetAttr()==GAttributeEnum::eNotErasePreviousLocation)
		//	{// If Attr is not zero call old setters and getters
		//	}
		//	else// GetAttr is eErasePreviousLocation otherwise ( all other options where discarded in Validate of MsgData ).
		//	{
		//	}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	GSegment *poClonedSegment = GSegment::Clone(poSegment);
		//
		//	if (pGISAbsData->GetXpos())
		//		poClonedSegment->SetXPos(pGISAbsData->GetXpos());
		//
		//	if (pGISAbsData->GetYpos())
		//		poClonedSegment->SetYPos(pGISAbsData->GetYpos());
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Java implements this command in the following way, the Spec seems to prefer the one commented above.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (pGISAbsData->GetXpos())
        {
            poSegment->SetXPos(pGISAbsData->GetXpos());
        }

        if (pGISAbsData->GetYpos())
        {
            poSegment->SetYPos(pGISAbsData->GetYpos());
        }

        GSegment *poClonedSegment = GSegment::Clone(poSegment);

		if (poClonedSegment)
		{
			poClonedSegment->SetVisible(true);  
		}
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //Call Update section (instead of updateview) with a pointer to the clon (without validation)
        pModel->UpdateViewSection(poClonedSegment,pGISAbsData->GetSynchronousRequest());
    }
}

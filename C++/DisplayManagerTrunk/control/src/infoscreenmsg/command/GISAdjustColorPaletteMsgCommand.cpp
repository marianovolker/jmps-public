#include "GISAdjustColorPaletteMsgCommand.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"
#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "utils/image/src/GColorPalette.h"

#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


GISAdjustColorPaletteMsgCommand::GISAdjustColorPaletteMsgCommand()
{
	LOG_LEVEL4("GISAdjustColorPaletteMsgCommand()");
}

GISAdjustColorPaletteMsgCommand::~GISAdjustColorPaletteMsgCommand()
{
	LOG_LEVEL4("~GISAdjustColorPaletteMsgCommand()");
}

void GISAdjustColorPaletteMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());
	
        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        GColorPalette* poColorPalette = poDisplay->GetColorPalette();

        GISCommandBuffer oCommandBuffer(pGISAbsData->GetVLD());

        // PALREG field.
        int iPALREG = oCommandBuffer.GetNextField().toInt();

        // RVALUE field.
        int iRVALUE = oCommandBuffer.GetNextField().toInt();

        // GVALUE field.
        int iGVALUE = oCommandBuffer.GetNextField().toInt();

        // BVALUE field.
        int iBVALUE = oCommandBuffer.GetNextField().toInt();

        poColorPalette->SetColor(iPALREG, iRVALUE, iGVALUE, iBVALUE);

    }

}

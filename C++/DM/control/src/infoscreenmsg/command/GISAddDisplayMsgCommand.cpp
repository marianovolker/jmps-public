#include "GISAddDisplayMsgCommand.h"

#include "control/src/infoscreenmsg/base/GISAbsData.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "utils/logger/src/GLogger.h"

#include <QMutexLocker>


GISAddDisplayMsgCommand::GISAddDisplayMsgCommand()
{
	LOG_LEVEL4("GISAddDisplayMsgCommand()");
}

GISAddDisplayMsgCommand::~GISAddDisplayMsgCommand()
{
	LOG_LEVEL4("~GISAddDisplayMsgCommand()");
}

void GISAddDisplayMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISAbsData* pGISAbsData = GetData();

	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        GDisplay* poDisplay = pModel->GetDisplay(pGISAbsData->GetDisplay());

        if ( poDisplay == NULL )
        {
            QList<GDisplay *> lstDisplays = pModel->GetAllDisplays();

            if( lstDisplays.count() == 0 )
            {
                pModel->AddDisplay(new GDisplay(pGISAbsData->GetDisplay()));
            }
        }
    }
}

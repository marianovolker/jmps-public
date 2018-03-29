#include "GISAddDisplayMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISAddDisplayMsgData::m_CName="AddDisplayCommand";
const char * GISAddDisplayMsgData::m_CCommandId="001";

GISAddDisplayMsgData::GISAddDisplayMsgData()
{
	LOG_LEVEL4("GISAddDisplayMsgData()");
}

GISAddDisplayMsgData::~GISAddDisplayMsgData()
{
	LOG_LEVEL4("~GISAddDisplayMsgData()");
}

void GISAddDisplayMsgData::Validate() throw(GException)
{
	/*
	 * 
	  *01 = Command error
	  *02 = Display number already used
	  *21 = Busy processing a command
	*/
	LOG_LEVEL4("Validate()");
	
	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();

    {
        QMutexLocker mtxScope(pModel->GetMutex());

        if ( GetDisplay() == 1 )
        {
            //must be ONE
            GDisplay *poExistingDisplay = pModel->GetDisplay(GetDisplay());
            if ( poExistingDisplay != NULL )
            {//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
                throw GIS_EXCEPTION(GISCommandStatusEnum::eDisplayNumberAlreadyUsed);
            }
        }
        else
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eCommandError);
        }
    }
}

QString GISAddDisplayMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAddDisplayMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}


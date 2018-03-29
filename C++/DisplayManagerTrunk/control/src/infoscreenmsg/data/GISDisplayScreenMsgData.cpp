#include "GISDisplayScreenMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>
#include <QDebug>

const char * GISDisplayScreenMsgData::m_CName="DisplayScreenCommand";
const char * GISDisplayScreenMsgData::m_CCommandId="014";

GISDisplayScreenMsgData::GISDisplayScreenMsgData()
{
	LOG_LEVEL4("GISDisplayScreenMsgData()");
}

GISDisplayScreenMsgData::~GISDisplayScreenMsgData()
{
	LOG_LEVEL4("~GISDisplayScreenMsgData()");
}

void GISDisplayScreenMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	*/
	LOG_LEVEL4("Validate()");

    
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != 1)
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
	}
    
	if ( GetScreen() == 0)
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidScreenNumberOrScreenNumberDoesNotExist);
	}
    
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	QSharedPointer<GISModel> pModel = GISModelSingleton::GetInstance()->GetModel();
    
    {
        QMutexLocker mtxScope(pModel->GetMutex());

       

        GDisplay* poDisplay = pModel->GetDisplay(GetDisplay());

       

        if( poDisplay == NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
        }
        
        GScreen* poScreen = poDisplay->GetScreen(GetScreen());
        
        if( poScreen == NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidScreenNumberOrScreenNumberDoesNotExist);
        }

        
        // Export the screen into the browser's DOM
    }
}

QString GISDisplayScreenMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISDisplayScreenMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

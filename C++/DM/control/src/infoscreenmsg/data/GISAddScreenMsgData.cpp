#include "GISAddScreenMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "utils/image/src/GColorPalette.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISAddScreenMsgData::m_CName="AddScreenCommand";
const char * GISAddScreenMsgData::m_CCommandId="002";

GISAddScreenMsgData::GISAddScreenMsgData()
{
	LOG_LEVEL4("GISAddScreenMsgData()");
}

GISAddScreenMsgData::~GISAddScreenMsgData()
{
	LOG_LEVEL4("~GISAddScreenMsgData()");
}

void GISAddScreenMsgData::Validate() throw(GException)
{
	/*
	 *00 = No errors
	 *03 = Screen number already used
	 *06 = Invalid display number or display number does not exist
	 *32 = Invalid Color or Color Register
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
        if( poScreen != NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eScreenNumberAlreadyUsed);
        }


        if( !((GetColor() > GColorPalette::CFirstRegister) && (GetColor() < GColorPalette::CLastRegister)) )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }
    }

}


QString GISAddScreenMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAddScreenMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

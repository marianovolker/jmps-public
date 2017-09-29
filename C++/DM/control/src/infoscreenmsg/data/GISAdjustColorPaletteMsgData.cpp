#include "GISAdjustColorPaletteMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISAdjustColorPaletteMsgData::m_CName="AdjustColorPaletteCommand";
const char * GISAdjustColorPaletteMsgData::m_CCommandId="010";

GISAdjustColorPaletteMsgData::GISAdjustColorPaletteMsgData()
{
	LOG_LEVEL4("GISAdjustColorPaletteMsgData()");
}

GISAdjustColorPaletteMsgData::~GISAdjustColorPaletteMsgData()
{
	LOG_LEVEL4("~GISAdjustColorPaletteMsgData()");
}

void GISAdjustColorPaletteMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 */
	LOG_LEVEL4("Validate()");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != 1)
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
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

        GColorPalette* poColorPalette = poDisplay->GetColorPalette();
        if( poColorPalette == NULL )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }

        GISCommandBuffer oCommandBuffer(GetVLD());
        bool bOk(true);

        // PALREG field.
        oCommandBuffer.GetNextField().toInt(&bOk);
        if( !bOk )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidColorOrColorRegister));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }

        // RVALUE field.
        oCommandBuffer.GetNextField().toInt(&bOk);
        if( !bOk )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidColorOrColorRegister));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }

        // GVALUE field.
        oCommandBuffer.GetNextField().toInt(&bOk);
        if( !bOk )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidColorOrColorRegister));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }

        // BVALUE field.
        oCommandBuffer.GetNextField().toInt(&bOk);
        if( !bOk )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidColorOrColorRegister));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidColorOrColorRegister);
            LOG_LEVEL2(oError.toString());
        }
   }
}

QString GISAdjustColorPaletteMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAdjustColorPaletteMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

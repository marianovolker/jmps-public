#include "GISDisplayFlyTextMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISDisplayFlyTextMsgData::m_CName="DisplayFlyTextCommand";
const char * GISDisplayFlyTextMsgData::m_CCommandId="028";

GISDisplayFlyTextMsgData::GISDisplayFlyTextMsgData()
{
	LOG_LEVEL4("GISDisplayFlyTextMsgData()");
}

GISDisplayFlyTextMsgData::~GISDisplayFlyTextMsgData()
{
	LOG_LEVEL4("~GISDisplayFlyTextMsgData()");
}

void GISDisplayFlyTextMsgData::Validate() throw(GException)
{
	/*
	 *12 = Invalid Attribute 
	 *13 = Font number undefined
	 *15 = Variable Length Data Contains NonDisplayable Characters
	 */
	LOG_LEVEL4("Validate()");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != 1)
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
	}
	if ( !IsValidFont(GetFont()) )
	{
		//TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
		//throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eFontNumberUndefined));
		GISCommandStatusEnum oError(GISCommandStatusEnum::eFontNumberUndefined);
		LOG_LEVEL2(oError.toString());
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

        int iAtt = GetAttr();
        if( !((iAtt > GAttributeEnum::eBeforeFirstAttrib) && (iAtt < GAttributeEnum::eAfterLastAttrib)) )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidAttribute);
        }
    }
}

QString GISDisplayFlyTextMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISDisplayFlyTextMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

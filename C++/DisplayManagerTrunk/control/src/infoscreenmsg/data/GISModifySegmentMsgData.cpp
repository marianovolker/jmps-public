#include "GISModifySegmentMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GISSegmentAttributeEnum.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISModifySegmentMsgData::m_CName="ModifySegmentCommand";
const char * GISModifySegmentMsgData::m_CCommandId="016";

GISModifySegmentMsgData::GISModifySegmentMsgData()
{
	LOG_LEVEL4("GISModifySegmentMsgData()");
}

GISModifySegmentMsgData::~GISModifySegmentMsgData()
{
	LOG_LEVEL4("~GISModifySegmentMsgData()");
}

void GISModifySegmentMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	 *08 = Invalid segment number or segment number does not exist
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
	if( GetSegment() == 0 )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidSegmentNumberOrSegmentNumberDoesNotExist);
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
        GSegment* poSegment = poScreen->GetSegment(GetSegment());
        if( poSegment == NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidSegmentNumberOrSegmentNumberDoesNotExist);
        }

        if (GetAttr()!= GISSegmentAttributeEnum::eInvisible && GetAttr()!= GISSegmentAttributeEnum::eVisible)
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidAttribute);
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidAttribute);
            LOG_LEVEL2(oError.toString());
        }

    }
}

QString GISModifySegmentMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISModifySegmentMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

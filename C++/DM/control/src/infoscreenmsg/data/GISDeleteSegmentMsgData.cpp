#include "GISDeleteSegmentMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISDeleteSegmentMsgData::m_CName="DeleteSegmentCommand";
const char * GISDeleteSegmentMsgData::m_CCommandId="007";

GISDeleteSegmentMsgData::GISDeleteSegmentMsgData()
{
	LOG_LEVEL4("GISDeleteSegmentMsgData()");
}

GISDeleteSegmentMsgData::~GISDeleteSegmentMsgData()
{
	LOG_LEVEL4("~GISDeleteSegmentMsgData()");
}

void GISDeleteSegmentMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	 *08 = Invalid segment number or segment number does not exist
	*/
	LOG_LEVEL4("Validate()");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != 1 )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
	}
	if ( GetScreen() == 0 )
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
    }
}

QString GISDeleteSegmentMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISDeleteSegmentMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}


#include "GISDeleteFieldMsgData.h"

#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISDeleteFieldMsgData::m_CName="DeleteFieldCommand";
const char * GISDeleteFieldMsgData::m_CCommandId="008";

GISDeleteFieldMsgData::GISDeleteFieldMsgData()
{
	LOG_LEVEL4("GISDeleteFieldMsgData()");
}

GISDeleteFieldMsgData::~GISDeleteFieldMsgData()
{
	LOG_LEVEL4("~GISDeleteFieldMsgData()");
}

void GISDeleteFieldMsgData::Validate() throw(GException)
{
	/*
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	 *08 = Invalid segment number or segment number does not exist
	 *09 = Invalid field number or field number does not exist
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
	if( GetField() == 0 )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidFieldNumberOrFieldNumberDoesNotExist);
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
        GTextField* poField = poSegment->GetField(GetField());
        if( poField == NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidFieldNumberOrFieldNumberDoesNotExist);
        }
   }
}

QString GISDeleteFieldMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISDeleteFieldMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

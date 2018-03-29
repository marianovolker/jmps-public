#include "GISSetFieldAttributeMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>

const char * GISSetFieldAttributeMsgData::m_CName="SetFieldAttributeCommand";
const char * GISSetFieldAttributeMsgData::m_CCommandId="009";

GISSetFieldAttributeMsgData::GISSetFieldAttributeMsgData()
{
	LOG_LEVEL4("GISSetFieldAttributeMsgData()");
}

GISSetFieldAttributeMsgData::~GISSetFieldAttributeMsgData()
{
	LOG_LEVEL4("~GISSetFieldAttributeMsgData()");
}

void GISSetFieldAttributeMsgData::Validate() throw(GException)
{
	/*
	 *05 = Field number already used
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	 *08 = Invalid segment number or segment number does not exist
	 *09 = InvalidFieldNumberOrFieldNumberDoesNotExist
	 *12 = Invalid Attribute
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
	if(	GetField() == 0 )
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
            //NGP 121, GPT 14/10/09
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidFieldNumberOrFieldNumberDoesNotExist);
        }
        int iAtt = GetAttr();
        if( !((iAtt > GAttributeEnum::eBeforeFirstAttrib) && (iAtt < GAttributeEnum::eAfterLastAttrib)) )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidAttribute);
        }
    }
}

QString GISSetFieldAttributeMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISSetFieldAttributeMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

#include "GISAddFieldMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "model/src/infoscreenmodel/GTextField.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>


const char * GISAddFieldMsgData::m_CName="AddFieldCommand";
const char * GISAddFieldMsgData::m_CCommandId="004";

GISAddFieldMsgData::GISAddFieldMsgData()
{
	LOG_LEVEL4("GISAddFieldMsgData()");
}

GISAddFieldMsgData::~GISAddFieldMsgData()
{
	LOG_LEVEL4("~GISAddFieldMsgData()");
}

void GISAddFieldMsgData::Validate() throw(GException)
{
	/*
	 *05 = Field number already used
	 *06 = Invalid display number or display number does not exist
	 *07 = Invalid screen number or screen number does not exist
	 *08 = Invalid segment number or segment number does not exist
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
	if( GetField() == 0 )
	{
		throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidFieldNumberOrFieldNumberDoesNotExist);
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
        if( poField != NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eFieldNumberAlreadyUsed);
        }
        int iAtt = GetAttr();
        if( !((iAtt > GAttributeEnum::eBeforeFirstAttrib) && (iAtt < GAttributeEnum::eAfterLastAttrib)) )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidAttribute);
        }
    }
}

QString GISAddFieldMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAddFieldMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

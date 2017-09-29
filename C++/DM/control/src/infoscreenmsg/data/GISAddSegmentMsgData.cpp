#include "GISAddSegmentMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "control/src/infoscreenmsg/utils/GISCommandBuffer.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "model/src/infoscreenmodel/GDisplay.h"
#include "model/src/infoscreenmodel/GScreen.h"
#include "model/src/infoscreenmodel/GSegment.h"
#include "utils/logger/src/GLogger.h"
#include <QMutexLocker>
#include "model/src/infoscreenmodel/GISSegmentAttributeEnum.h"
#include "config/GDisplayManagerConfig.h"

const char * GISAddSegmentMsgData::m_CName="AddSegmentCommand";
const char * GISAddSegmentMsgData::m_CCommandId="003";

GISAddSegmentMsgData::GISAddSegmentMsgData()
{
	LOG_LEVEL4("GISAddSegmentMsgData()");
}

GISAddSegmentMsgData::~GISAddSegmentMsgData()
{
	LOG_LEVEL4("~GISAddSegmentMsgData()");
}

void GISAddSegmentMsgData::Validate() throw(GException)
{
	/*
	 *03 = Screen number already used
	 *04 = Segment number already used
	 *06 = Invalid display number or display number does not exist
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
        GSegment * poSegment = poScreen->GetSegment(GetSegment());
        if( poSegment != NULL )
        {
            throw GIS_EXCEPTION(GISCommandStatusEnum::eSegmentNumberAlreadyUsed);
        }


        int iAtt = GetAttr();
        if( !((iAtt > GISSegmentAttributeEnum::eBeforeFistAttribute) && (iAtt < GISSegmentAttributeEnum::eAfterLastAttribute)) )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidAttribute);
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidAttribute);
            LOG_LEVEL2(oError.toString());
        }

        int iYPos = GetYpos();
        if( !(iYPos > 0 && iYPos <= GETCONF_NUM(eDisplayManagerViewHeight)) )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidYPositionOrYRangeExceeded));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidYPositionOrYRangeExceeded);
            LOG_LEVEL2(oError.toString());
        }

        int iXPos = GetXpos();
        if( !(iXPos > 0 && iXPos <= GETCONF_NUM(eDisplayManagerViewWidth)) )
        {
            //TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
            //throw GIS_EXCEPTION(GISCommandStatusEnum(GISCommandStatusEnum::eInvalidXPositionOrXRangeExceeded));
            GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidXPositionOrXRangeExceeded);
            LOG_LEVEL2(oError.toString());
        }
    }
}

QString GISAddSegmentMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISAddSegmentMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

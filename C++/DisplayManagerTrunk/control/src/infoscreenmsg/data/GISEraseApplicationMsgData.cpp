#include "GISEraseApplicationMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISEraseApplicationMsgData::m_CName="EraseApplicationCommand";
const char * GISEraseApplicationMsgData::m_CCommandId="031";

GISEraseApplicationMsgData::GISEraseApplicationMsgData()
{
	LOG_LEVEL4("GISEraseApplicationMsgData()");
}

GISEraseApplicationMsgData::~GISEraseApplicationMsgData()
{
	LOG_LEVEL4("~GISEraseApplicationMsgData()");
}

void GISEraseApplicationMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	enum { eDefaultDisplayNumber = 1 };

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: DisplayManager Constrain - If improvement is done on Specs, this should be removed.
	if ( GetDisplay() != static_cast<int>(eDefaultDisplayNumber))
	{
		SetDisplay(static_cast<int>(eDefaultDisplayNumber));
		
		GISCommandStatusEnum oError(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
		LOG_LEVEL2(oError.toString() );
		//throw GIS_EXCEPTION(GISCommandStatusEnum::eInvalidDisplayNumberOrDisplayNumberDoesNotExist);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

QString GISEraseApplicationMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISEraseApplicationMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

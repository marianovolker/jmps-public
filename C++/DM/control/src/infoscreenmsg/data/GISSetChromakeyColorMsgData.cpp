#include "GISSetChromakeyColorMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISSetChromakeyColorMsgData::m_CName="SetChromakeyColorCommand";
const char * GISSetChromakeyColorMsgData::m_CCommandId="033";

GISSetChromakeyColorMsgData::GISSetChromakeyColorMsgData()
{
	LOG_LEVEL4("GISSetChromakeyColorMsgData()");
}

GISSetChromakeyColorMsgData::~GISSetChromakeyColorMsgData()
{
	LOG_LEVEL4("~GISSetChromakeyColorMsgData()");
}

void GISSetChromakeyColorMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}

QString GISSetChromakeyColorMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISSetChromakeyColorMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

#include "GISSegmentDisplayCountMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISSegmentDisplayCountMsgData::m_CName="SegmentDisplayCountCommand";
const char * GISSegmentDisplayCountMsgData::m_CCommandId="011";

GISSegmentDisplayCountMsgData::GISSegmentDisplayCountMsgData()
{
	LOG_LEVEL4("GISSegmentDisplayCountMsgData()");
}

GISSegmentDisplayCountMsgData::~GISSegmentDisplayCountMsgData()
{
	LOG_LEVEL4("~GISSegmentDisplayCountMsgData()");
}

void GISSegmentDisplayCountMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}

QString GISSegmentDisplayCountMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISSegmentDisplayCountMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

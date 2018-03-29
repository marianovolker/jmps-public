#include "GISObjectAddressMsgData.h"

#include "control/src/infoscreenmsg/utils/GISCommandStatusEnum.h"
#include "control/src/infoscreenmsg/utils/GISException.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"
#include "utils/logger/src/GLogger.h"

const char * GISObjectAddressMsgData::m_CName="ObjectAddressCommand";
const char * GISObjectAddressMsgData::m_CCommandId="018";

GISObjectAddressMsgData::GISObjectAddressMsgData()
{
	LOG_LEVEL4("GISObjectAddressMsgData()");
}

GISObjectAddressMsgData::~GISObjectAddressMsgData()
{
	LOG_LEVEL4("~GISObjectAddressMsgData()");
}

void GISObjectAddressMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	// Do nothing at this level.
}

QString GISObjectAddressMsgData::GetName()
{
	LOG_LEVEL4("GetName()");

	return QString(m_CName);
}

QString GISObjectAddressMsgData::GetCommand()
{
	LOG_LEVEL4("GetCommand()");

	return QString(m_CCommandId);
}

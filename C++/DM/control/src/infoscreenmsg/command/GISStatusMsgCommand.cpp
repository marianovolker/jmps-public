#include "GISStatusMsgCommand.h"
#include <QLatin1String>
#include "control/src/infoscreenmsg/data/GISCommandResponse.h"

#include "utils/logger/src/GLogger.h"

GISStatusMsgCommand::GISStatusMsgCommand()
{
	LOG_LEVEL4("GISStatusMsgCommand()");
}

GISStatusMsgCommand::~GISStatusMsgCommand()
{
	LOG_LEVEL4("~GISStatusMsgCommand()");
}

void GISStatusMsgCommand::DoExecute() throw (GException)
{
	LOG_LEVEL3("DoExecute()");

	GISCommandResponse * pResponse = GetResponse();

	QString oResponseData;

	// Return all this due to backward compatibility for Java Application

	oResponseData = QString("1234567890")		// version number
					+ '\0' 
					+ QString("1234567890")	// version date
					+ '\0' 
					+ QString("02")			// display type
					+ '\0' 
					+ QString("00")			// speech processor
					+ '\0' 
					+ QString("00")			// audio sound level
					+ '\0' 
					+ QString("00")			// crind beeper level
					+ '\0'; 

	pResponse->SetData(oResponseData);
}

#ifndef CLOCK_MSG_DATA_XML_H
#define CLOCK_MSG_DATA_XML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GClockMsgData.h"
#include "utils/base/src/GException.h"

class GClockMsgDataXML
{
public:
	static GClockMsgData* UnMarshall(QByteArray & strXML) throw(GException);

	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLAction;
	static const QString CXMLXPos;
	static const QString CXMLYPos;
};

#endif

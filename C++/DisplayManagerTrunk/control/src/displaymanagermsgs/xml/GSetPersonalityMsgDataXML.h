//! GSetPersonalityMsgDataXML
/*!
  GSetPersonality Marhsall / UnMarshall MsgData
*/

#ifndef GSetPersonalityMsgDataXML_H
#define GSetPersonalityMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GSetPersonalityMsgData.h"
#include "utils/base/src/GException.h"


class GSetPersonalityMsgDataXML
{
public:
	static QByteArray Marshall(GSetPersonalityMsgData* oRequest);
	static GSetPersonalityMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLPersonality;
	static const QString CXMLSynchronousRequest;
};

#endif

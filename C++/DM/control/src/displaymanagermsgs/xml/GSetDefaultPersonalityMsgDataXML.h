//! GSetDefaultPersonalityMsgDataXML
/*!
  GSetDefaultPersonality Marhsall / UnMarshall MsgData
*/

#ifndef GSetDefaultPersonalityMsgDataXML_H
#define GSetDefaultPersonalityMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GSetDefaultPersonalityMsgData.h"
#include "utils/base/src/GException.h"

class GSetDefaultPersonalityMsgDataXML
{
public:
	static QByteArray Marshall(GSetDefaultPersonalityMsgData* oRequest);
	static GSetDefaultPersonalityMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLPersonality;
	static const QString CXMLSynchronousRequest;
};

#endif

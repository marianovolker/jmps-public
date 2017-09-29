//! GKioskIdMsgDataXML
/*!
  GKioskId Marhsall / UnMarshall MsgData
*/

#ifndef GKioskIdMsgDataXML_H
#define GKioskIdMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GKioskIdMsgData.h"
#include "utils/base/src/GException.h"


class GKioskIdMsgDataXML
{
public:
	static QByteArray Marshall(GKioskIdMsgData* oRequest);
	static GKioskIdMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLSynchronousRequest;
};

#endif

//! GURLMsgDataXML
/*!
  URL Marhsall / UnMarshall MsgData
*/

#ifndef GURLMsgDataXML_H
#define GURLMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GURLMsgData.h"
#include "utils/base/src/GException.h"


class GURLMsgDataXML
{
public:
	static QByteArray Marshall(GURLMsgData* oRequest);
	static GURLMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLURL;
	static const QString CXMLSynchronousRequest;
};

#endif

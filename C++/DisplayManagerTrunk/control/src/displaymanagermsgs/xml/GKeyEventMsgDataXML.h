//! GKeyEventMsgDataXML
/*!
  GKeyEvent Marhsall / UnMarshall MsgData
*/

#ifndef GKeyEventMsgDataXML_H_
#define GKeyEventMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GKeyEventMsgData.h"
#include "utils/base/src/GException.h"


class GKeyEventMsgDataXML
{
public:
	static QByteArray Marshall(GKeyEventMsgData* oRequest);
	static GKeyEventMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLKeyCode;
	static const QString CXMLSynchronousRequest;
};

#endif

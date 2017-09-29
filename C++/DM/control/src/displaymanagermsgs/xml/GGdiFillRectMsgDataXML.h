//! GGdiFillRectMsgDataXML
/*!
  GGdiFillRect Marshall / UnMarshall MsgData
*/

#ifndef GGdiFillRectMsgDataXML_H_
#define GGdiFillRectMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiFillRectMsgData.h"
#include "utils/base/src/GException.h"


class GGdiFillRectMsgDataXML
{
public:
	static QByteArray Marshall(GGdiFillRectMsgData* oRequest);
	static GGdiFillRectMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLXLoc;
	static const QString CXMLYLoc;
	static const QString CXMLColor;
	static const QString CXMLHeight;
	static const QString CXMLWidth;
	static const QString CXMLSynchronousRequest;
};

#endif

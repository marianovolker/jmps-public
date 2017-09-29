//! GFlyTextMsgDataXML
/*!
  GFlyTextRequestMgr Marshall / UnMarshall MessageData
*/

#ifndef GFlyTextMsgDataXML_H_
#define GFlyTextMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GFlyTextMsgData.h"
#include "utils/base/src/GException.h"


class GFlyTextMsgDataXML
{
public:
	static QByteArray Marshall(GFlyTextMsgData* oRequest);
	static GFlyTextMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLFont;
	static const QString CXMLText;
	static const QString CXMLXLoc;
	static const QString CXMLYLoc;
	static const QString CXMLSynchronousRequest;
};

#endif

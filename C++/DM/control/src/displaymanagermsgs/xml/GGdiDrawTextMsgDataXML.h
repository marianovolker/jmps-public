//! GGdiDrawTextMsgDataXML
/*!
  GGdiDrawText Marshall / UnMarshall MessageData
*/

#ifndef GGdiDrawTextMsgDataXML_H_
#define GGdiDrawTextMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiDrawTextMsgData.h"
#include "utils/base/src/GException.h"

class GGdiDrawTextMsgDataXML
{
public:
	static QByteArray Marshall(GGdiDrawTextMsgData* oRequest);
	static GGdiDrawTextMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLBGColor;
	static const QString CXMLFGColor;
	static const QString CXMLText;
	static const QString CXMLXLoc;
	static const QString CXMLYLoc;
	static const QString CXMLFont;
	static const QString CXMLAttr;
	static const QString CXMLSynchronousRequest;
};

#endif

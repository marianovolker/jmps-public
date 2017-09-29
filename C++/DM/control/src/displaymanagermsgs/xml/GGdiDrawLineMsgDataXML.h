//! GGdiDrawLineMsgDataXML
/*!
  GGdiDrawLine Marshall / UnMarshall MessageData
*/

#ifndef GGdiDrawLineMsgDataXML_H_
#define GGdiDrawLineMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiDrawLineMsgData.h"
#include "utils/base/src/GException.h"


class GGdiDrawLineMsgDataXML
{
public:
	static QByteArray Marshall(GGdiDrawLineMsgData* oRequest);
	static GGdiDrawLineMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLThickness;
	static const QString CXMLColor;
	static const QString CXMLLength;
	static const QString CXMLXLoc;
	static const QString CXMLYLoc;
	static const QString CXMLSynchronousRequest;
};

#endif

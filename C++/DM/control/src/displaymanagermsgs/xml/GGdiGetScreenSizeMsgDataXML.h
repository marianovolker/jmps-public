//! GGdiGetScreenSizeMsgDataXML 
/*!
  GGdiGetScreenSize Marshall / UnMarshall MsgData
*/

#ifndef GGdiGetScreenSizeMsgDataXML_H_
#define GGdiGetScreenSizeMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiGetScreenSizeMsgData.h"
#include "utils/base/src/GException.h"


class GGdiGetScreenSizeMsgDataXML
{
public:
	static QByteArray Marshall(GGdiGetScreenSizeMsgData* oRequest);
	static GGdiGetScreenSizeMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLSynchronousRequest;
};

#endif

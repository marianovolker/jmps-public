//! GGdiClearScreenMsgDataXML
/*!
  GGdiClearScreenRequestMgr Marshall / UnMarshall MessageData
*/

#ifndef GGdiClearScreenMsgDataXML_H_
#define GGdiClearScreenMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiClearScreenMsgData.h"
#include "utils/base/src/GException.h"


class GGdiClearScreenMsgDataXML
{
public:
	static QByteArray Marshall(GGdiClearScreenMsgData* oRequest);
	static GGdiClearScreenMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLSynchronousRequest;
};

#endif

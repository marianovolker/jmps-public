//! GGdiRefreshScreenMsgDataXML
/*!
  GGdiRefreshScreen Marhsall / UnMarshall MsgData
*/

#ifndef GGdiRefreshScreenMsgDataXML_H_
#define GGdiRefreshScreenMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGdiRefreshScreenMsgData.h"
#include "utils/base/src/GException.h"


class GGdiRefreshScreenMsgDataXML
{
public:
	static QByteArray Marshall(GGdiRefreshScreenMsgData* oRequest);
	static GGdiRefreshScreenMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLSynchronousRequest;
};

#endif

//! GInfoScreenMsgDataXML
/*!
  GInfoScreen Marhsall / UnMarshall MsgData
*/

#ifndef GInfoScreenMsgDataXML_H_
#define GInfoScreenMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GInfoScreenMsgData.h"
#include "utils/base/src/GException.h"

class GInfoScreenMsgDataXML
{
public:
	static QByteArray Marshall(GInfoScreenMsgData* oRequest);
	static GInfoScreenMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLData;
	static const QString CXMLSynchronousRequest;
};

#endif

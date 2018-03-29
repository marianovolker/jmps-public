//! GResponseMsgDataXML
/*!
  GResponse Marhsall / UnMarshall MsgData
*/

#ifndef GResponseMsgDataXML_H
#define GResponseMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GResponseMsgData.h"
#include "utils/base/src/GException.h"


class GResponseMsgDataXML
{
public:
	static QByteArray Marshall(GResponseMsgData* oRequest);
	static GResponseMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXML;
	static const QString CXMLType;	
	
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLCommand;
	static const QString CXMLData;
	static const QString CXMLResponseCode;	
};

#endif

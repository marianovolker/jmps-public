//! GClearScreenMsgDataXML 
/*!
  GClearScreenRequestMgr Marshall / UnMarshall MessageData
*/

#ifndef GClearScreenMsgDataXML_H_
#define GClearScreenMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GClearScreenMsgData.h"
#include "utils/base/src/GException.h"

class GClearScreenMsgDataXML
{
public:
	static QByteArray Marshall(GClearScreenMsgData* oRequest);
	static GClearScreenMsgData* UnMarshall(QByteArray & strXML) throw(GException);
	
public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLColor;
	static const QString CXMLSynchronousRequest;
};

#endif

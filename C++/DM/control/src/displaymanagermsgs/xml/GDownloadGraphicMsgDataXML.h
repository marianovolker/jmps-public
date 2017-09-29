//! GDownloadGraphicMsgDataXML 
/*!
  GDownloadGraphicRequestMgr Marshall / UnMarshall MessageData
*/

#ifndef GDownloadGraphicMsgDataXML_H_
#define GDownloadGraphicMsgDataXML_H_

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GDownloadGraphicMsgData.h"
#include "utils/base/src/GException.h"

class GDownloadGraphicMsgDataXML
{
public:
	static QByteArray Marshall(GDownloadGraphicMsgData* oRequest);
	static GDownloadGraphicMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLScreen;
	static const QString CXMLType;
	static const QString CXMLCRC;
	static const QString CXMLDisplay;
	static const QString CXMLFilename;
	static const QString CXMLSegment;
	static const QString CXMLSynchronousRequest;
};

#endif

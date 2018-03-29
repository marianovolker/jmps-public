//! GGetPersonalitiesMsgDataXML
/*!
  GGetPersonalities Marhsall / UnMarshall MsgData
*/

#ifndef GGetPersonalitiesMsgDataXML_H
#define GGetPersonalitiesMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGetPersonalitiesMsgData.h"
#include "utils/base/src/GException.h"

class GGetPersonalitiesMsgDataXML
{
public:
	static QByteArray Marshall(GGetPersonalitiesMsgData* oRequest);
	static GGetPersonalitiesMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLPersonalitiesListResponse;
	static const QString CXMLDefaultPersonalityResponse;
	static const QString CXMLCurrentPersonalityResponse;
	static const QString CXMLSynchronousRequest;
};

#endif

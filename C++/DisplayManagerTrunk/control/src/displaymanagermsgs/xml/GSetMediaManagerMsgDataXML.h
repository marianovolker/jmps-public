//! GSetMediaManagerMsgDataXML
/*!
  GSetMediaManager Marhsall / UnMarshall MsgData
*/

#ifndef GSetMediaManagerMsgDataXML_H
#define GSetMediaManagerMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GSetMediaManagerMsgData.h"
#include "utils/base/src/GException.h"

class GSetMediaManagerMsgDataXML
{
public:
	static QByteArray Marshall(GSetMediaManagerMsgData* oRequest);
	static GSetMediaManagerMsgData* UnMarshall(QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLMediaManagerType;
	static const QString CXMLSynchronousRequest;

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GSetMediaManagerMsgDataXML( const GSetMediaManagerMsgDataXML& gSetMediaManagerMsgDataXML );
    GSetMediaManagerMsgDataXML& operator=( const GSetMediaManagerMsgDataXML& gSetMediaManagerMsgDataXML );

};

#endif

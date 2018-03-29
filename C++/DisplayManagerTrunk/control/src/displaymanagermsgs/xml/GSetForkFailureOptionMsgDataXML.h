//! GSetForkFailureOptionMsgDataXML
/*!
  GSetForkFailureOption Marhsall / UnMarshall MsgData
*/

#ifndef GSetForkFailureOptionMsgDataXML_H
#define GSetForkFailureOptionMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GSetForkFailureOptionMsgData.h"
#include "utils/base/src/GException.h"

class GSetForkFailureOptionMsgDataXML
{
public:
	static QByteArray Marshall(const GSetForkFailureOptionMsgData* oRequest);
	static GSetForkFailureOptionMsgData* UnMarshall(const QByteArray & strXML) throw(GException);

public:
	static const QString CXMLTag;
	static const QString CXMLVersion;
	static const QString CXMLForkFailureOption;
	static const QString CXMLSynchronousRequest;

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GSetForkFailureOptionMsgDataXML( const GSetForkFailureOptionMsgDataXML& gSetForkFailureOptionMsgDataXML );
    GSetForkFailureOptionMsgDataXML& operator=( const GSetForkFailureOptionMsgDataXML& gSetForkFailureOptionMsgDataXML );

};

#endif

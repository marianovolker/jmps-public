//! GGetForkFailureOptionsMsgDataXML
/*!
  GGetForkFailureOptions Marhsall / UnMarshall MsgData
 */

#ifndef GGetForkFailureOptionsMsgDataXML_H
#define GGetForkFailureOptionsMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGetForkFailureOptionsMsgData.h"
#include "utils/base/src/GException.h"

class GGetForkFailureOptionsMsgDataXML {
public:
    static QByteArray Marshall(const GGetForkFailureOptionsMsgData* oRequest);
    static GGetForkFailureOptionsMsgData* UnMarshall(const QByteArray & strXML) throw (GException);

public:
    static const QString CXMLTag;
    static const QString CXMLVersion;
    static const QString CXMLForkFailureOptionsListResponse;
    static const QString CXMLCurrentSettingResponse;
    static const QString CXMLSynchronousRequest;

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetForkFailureOptionsMsgDataXML( const GGetForkFailureOptionsMsgDataXML& gGetForkFailureOptionsMsgDataXML );
    GGetForkFailureOptionsMsgDataXML& operator=( const GGetForkFailureOptionsMsgDataXML& gGetForkFailureOptionsMsgDataXML );

};

#endif

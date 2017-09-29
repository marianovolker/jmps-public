//! GGetMediaManagersMsgDataXML
/*!
  GGetMediaManagers Marhsall / UnMarshall MsgData
 */

#ifndef GGetMediaManagersMsgDataXML_H
#define GGetMediaManagersMsgDataXML_H

#include <QString>
#include <QByteArray>

#include "control/src/displaymanagermsgs/data/GGetMediaManagersMsgData.h"
#include "utils/base/src/GException.h"

class GGetMediaManagersMsgDataXML {
public:
    static QByteArray Marshall(GGetMediaManagersMsgData* oRequest);
    static GGetMediaManagersMsgData* UnMarshall(QByteArray & strXML) throw (GException);

public:
    static const QString CXMLTag;
    static const QString CXMLVersion;
    static const QString CXMLMediaManagersListResponse;
    static const QString CXMLCurrentMediaManagerResponse;
    static const QString CXMLSynchronousRequest;

private:
    // Define a private copy constructor and assignment operator to prevent their use
    GGetMediaManagersMsgDataXML( const GGetMediaManagersMsgDataXML& gGetMediaManagersMsgDataXML );
    GGetMediaManagersMsgDataXML& operator=( const GGetMediaManagersMsgDataXML& gGetMediaManagersMsgDataXML );

};

#endif

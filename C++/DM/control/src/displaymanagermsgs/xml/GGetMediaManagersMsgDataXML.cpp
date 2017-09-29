#include "GGetMediaManagersMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GGetMediaManagersMsgDataXML::CXMLTag("get_media_managers_request");
const QString GGetMediaManagersMsgDataXML::CXMLVersion("interface_version");
const QString GGetMediaManagersMsgDataXML::CXMLMediaManagersListResponse("media_manager_list");
const QString GGetMediaManagersMsgDataXML::CXMLCurrentMediaManagerResponse("current_media_manager");
const QString GGetMediaManagersMsgDataXML::CXMLSynchronousRequest("SynchronizeMessage");

QByteArray GGetMediaManagersMsgDataXML::Marshall(GGetMediaManagersMsgData* pRequest) {
    LOG_LEVEL4("Marshall()");

    if (pRequest != NULL) {
        LOG_LEVEL4("This type of message doesn't have functionality for Marshalling.");

        return QByteArray();
    } else {
        return QByteArray();
    }
}

GGetMediaManagersMsgData* GGetMediaManagersMsgDataXML::UnMarshall(QByteArray & strXML) throw (GException) {
    LOG_LEVEL4("UnMarshall()");

    GGetMediaManagersMsgData* poRequest = new GGetMediaManagersMsgData();
    QDomDocument oDoc;

    try {
        QString sErrorMsg;
        int iErrorLine;
        int iErrorColumn;

        // Set Document content
        if (!oDoc.setContent(strXML, true, &sErrorMsg, &iErrorLine, &iErrorColumn)) {
            LOG_LEVEL1(QString("UnMarshall error: Error building dom tree[%1]").arg(QString(strXML)).simplified());
            LOG_LEVEL1(QString("[%1][%2][%3]").arg(sErrorMsg).arg(iErrorLine).arg(iErrorColumn).simplified());

            throw GEXCEPTION(QString("Error building dom tree"));
        }

        // Check root tag name
        QDomElement oRoot(oDoc.documentElement());
        if (oRoot.tagName() != CXMLTag) {
            throw GEXCEPTION(QString("Error: get_media_managers_request opening tag not detected"));
        }

        // Load Interface Version
        QDomNodeList oNodes = oRoot.elementsByTagName(CXMLVersion);
        if (oNodes.count() == 1) {
            poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
        }

        // Load SynchronizeMessage
        oNodes = oRoot.elementsByTagName(CXMLSynchronousRequest);
        if (oNodes.count() >= 1) {
            poRequest->SetSynchronousRequest(true);
        }

        // Load Namespace Prefix
        poRequest->SetNamespacePrefix(oRoot.prefix());

        // Load Namespace URI
        poRequest->SetNamespaceURI(oRoot.namespaceURI());
    } catch (GException &ex) {
        delete poRequest;
        poRequest = NULL;
        throw;
    }

    return poRequest;
}

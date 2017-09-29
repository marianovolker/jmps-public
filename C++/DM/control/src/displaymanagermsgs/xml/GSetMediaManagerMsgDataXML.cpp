#include "GSetMediaManagerMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GSetMediaManagerMsgDataXML::CXMLTag                   ("set_media_manager_type_request");
const QString GSetMediaManagerMsgDataXML::CXMLVersion               ("interface_version");
const QString GSetMediaManagerMsgDataXML::CXMLMediaManagerType           ("mediaManagerType");
const QString GSetMediaManagerMsgDataXML::CXMLSynchronousRequest    ("SynchronizeMessage");

QByteArray GSetMediaManagerMsgDataXML::Marshall(GSetMediaManagerMsgData* pRequest)
{
	LOG_LEVEL4("Marshall()");

	if( pRequest != NULL )
	{
		LOG_LEVEL4("This type of message doesn't have functionality for Marshalling.");

		return QByteArray();
	}
	else
	{
		return QByteArray();
	}
}

GSetMediaManagerMsgData* GSetMediaManagerMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GSetMediaManagerMsgData* poRequest = new GSetMediaManagerMsgData();
	QDomDocument oDoc;
	
	try
	{
		QString sErrorMsg;
		int iErrorLine;
		int iErrorColumn;

		// Set Document content
	    if( !oDoc.setContent(strXML, true, &sErrorMsg, &iErrorLine, &iErrorColumn) )
	    {
	    	LOG_LEVEL1( QString("UnMarshall error: Error building dom tree[%1]").arg(QString(strXML)).simplified() );
	    	LOG_LEVEL1( QString("[%1][%2][%3]").arg(sErrorMsg).arg(iErrorLine).arg(iErrorColumn).simplified() );

	    	throw GEXCEPTION(QString("Error building dom tree"));
	    }

	    // Check root tag name
	    QDomElement oRoot(oDoc.documentElement());
	    if (oRoot.tagName() != CXMLTag) 
    	{
	    	throw GEXCEPTION(QString("Error: set_media_manager_type_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    QDomNodeList oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Media Manager that was sent in the request
	    oNodes = oRoot.elementsByTagName(CXMLMediaManagerType);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetType(oNodes.item(0).toElement().text());
	    }

	    // Load SynchronizeMessage
	    oNodes = oRoot.elementsByTagName(CXMLSynchronousRequest);
	    if(oNodes.count() >= 1)
	    {
	    	poRequest->SetSynchronousRequest(true);
	    }

	    // Load Namespace Prefix
	    poRequest->SetNamespacePrefix(oRoot.prefix());

		// Load Namespace URI
	    poRequest->SetNamespaceURI(oRoot.namespaceURI());


	} 
	catch(GException &ex)
	{
		delete poRequest;
		poRequest = NULL;
		throw;
	}

	return poRequest;
}

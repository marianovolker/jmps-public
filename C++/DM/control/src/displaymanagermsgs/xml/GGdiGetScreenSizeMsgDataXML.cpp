#include "GGdiGetScreenSizeMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GGdiGetScreenSizeMsgDataXML::CXMLTag					("gdi_getscreensize_request");
const QString GGdiGetScreenSizeMsgDataXML::CXMLVersion				("interface_version");
const QString GGdiGetScreenSizeMsgDataXML::CXMLSynchronousRequest	("SynchronizeMessage");

QByteArray GGdiGetScreenSizeMsgDataXML::Marshall(GGdiGetScreenSizeMsgData* pRequest)
{
	LOG_LEVEL4("Marshall()");

	if( pRequest != NULL )
	{
		LOG_LEVEL4("This type of message doesn't have functionality for Marshaling.");

		return QByteArray();
	}
	else
	{
		return QByteArray();
	}
}

GGdiGetScreenSizeMsgData* GGdiGetScreenSizeMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GGdiGetScreenSizeMsgData* poRequest = new GGdiGetScreenSizeMsgData();
	QDomDocument oDoc;
	QDomNodeList oNodes;
	
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
	    	throw GEXCEPTION(QString("Error: gdi_getscreensize_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
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

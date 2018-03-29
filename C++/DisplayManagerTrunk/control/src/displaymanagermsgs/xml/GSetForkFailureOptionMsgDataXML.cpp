#include "GSetForkFailureOptionMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GSetForkFailureOptionMsgDataXML::CXMLTag                   ("set_fork_failure_option_request");
const QString GSetForkFailureOptionMsgDataXML::CXMLVersion               ("interface_version");
const QString GSetForkFailureOptionMsgDataXML::CXMLForkFailureOption           ("forkFailureOption");
const QString GSetForkFailureOptionMsgDataXML::CXMLSynchronousRequest    ("SynchronizeMessage");

QByteArray GSetForkFailureOptionMsgDataXML::Marshall(const GSetForkFailureOptionMsgData* pRequest)
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

GSetForkFailureOptionMsgData* GSetForkFailureOptionMsgDataXML::UnMarshall(const QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GSetForkFailureOptionMsgData* poRequest = new GSetForkFailureOptionMsgData();
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
	    	throw GEXCEPTION(QString("Error: set_fork_failure_option_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    QDomNodeList oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Fork Failure Option that was sent in the request
	    oNodes = oRoot.elementsByTagName(CXMLForkFailureOption);
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
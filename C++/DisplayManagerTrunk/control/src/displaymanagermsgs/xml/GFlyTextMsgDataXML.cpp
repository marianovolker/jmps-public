#include "GFlyTextMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GFlyTextMsgDataXML::CXMLTag				("flytext_request");
const QString GFlyTextMsgDataXML::CXMLVersion			("interface_version");
const QString GFlyTextMsgDataXML::CXMLFont				("font");
const QString GFlyTextMsgDataXML::CXMLText				("text");
const QString GFlyTextMsgDataXML::CXMLXLoc				("x_loc");
const QString GFlyTextMsgDataXML::CXMLYLoc				("y_loc");
const QString GFlyTextMsgDataXML::CXMLSynchronousRequest("SynchronizeMessage");

QByteArray GFlyTextMsgDataXML::Marshall(GFlyTextMsgData* pRequest)
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

GFlyTextMsgData* GFlyTextMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GFlyTextMsgData* poRequest = new GFlyTextMsgData();
	QDomDocument oDoc;
	QDomNodeList oNodes;
	bool bCastOk;
	int iLoc;
	
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
	    	throw GEXCEPTION(QString("Error: flytext_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Font
	    oNodes = oRoot.elementsByTagName(CXMLFont);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetFont(oNodes.item(0).toElement().text());
	    }	    
	    
	    // Load Text
	    oNodes = oRoot.elementsByTagName(CXMLText);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetText(oNodes.item(0).toElement().text());
	    }
	    
	    // Load XLoc
	    oNodes = oRoot.elementsByTagName(CXMLXLoc);
	    if(oNodes.count() == 1) 
	    {	    	
	    	iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);
	    	if(bCastOk) 
	    	{
	    		poRequest->SetXLoc(iLoc);
	    	}
	    	else
	    	{
	    		throw GEXCEPTION(QString("XLoc casting error."));
	    	}
	    }	    
	    
	    // Load YLoc
	    oNodes = oRoot.elementsByTagName(CXMLYLoc);
	    if(oNodes.count() == 1) 
	    {	    	
	    	iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);
	    	if(bCastOk) 
	    	{
	    		poRequest->SetYLoc(iLoc);
	    	}
	    	else
	    	{
	    		throw GEXCEPTION(QString("YLoc casting error."));
	    	}
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

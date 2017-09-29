#include "GGdiFillRectMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GGdiFillRectMsgDataXML::CXMLTag					("gdi_fillrect_request");
const QString GGdiFillRectMsgDataXML::CXMLVersion				("interface_version");
const QString GGdiFillRectMsgDataXML::CXMLXLoc					("x_loc");
const QString GGdiFillRectMsgDataXML::CXMLYLoc					("y_loc");
const QString GGdiFillRectMsgDataXML::CXMLColor					("color");
const QString GGdiFillRectMsgDataXML::CXMLHeight				("height");
const QString GGdiFillRectMsgDataXML::CXMLWidth					("width");
const QString GGdiFillRectMsgDataXML::CXMLSynchronousRequest    ("SynchronizeMessage");

QByteArray GGdiFillRectMsgDataXML::Marshall(GGdiFillRectMsgData* pRequest)
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

GGdiFillRectMsgData* GGdiFillRectMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GGdiFillRectMsgData* poRequest = new GGdiFillRectMsgData();
	QDomDocument oDoc;
	QDomNodeList oNodes;
	int iLoc;
	bool bCastOk;
	
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
	    	throw GEXCEPTION(QString("Error: gdi_fillrect_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
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
	    
	    // Load Color
	    oNodes = oRoot.elementsByTagName(CXMLColor);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetColor(oNodes.item(0).toElement().text());
	    }	    
	    
	    // Load Height
	    oNodes = oRoot.elementsByTagName(CXMLHeight);
	    if(oNodes.count() == 1) 
	    {	    	
	    	iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);
	    	if(bCastOk) 
	    	{
	    		poRequest->SetHeight(iLoc);
	    	}
	    	else
	    	{
	    		throw GEXCEPTION(QString("Height casting error."));
	    	}
	    }	    
	    
	    // Load Width
	    oNodes = oRoot.elementsByTagName(CXMLWidth);
	    if(oNodes.count() == 1) 
	    {	    	
	    	iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);
	    	if(bCastOk) 
	    	{
	    		poRequest->SetWidth(iLoc);
	    	}
	    	else
	    	{
	    		throw GEXCEPTION(QString("Width casting error."));
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

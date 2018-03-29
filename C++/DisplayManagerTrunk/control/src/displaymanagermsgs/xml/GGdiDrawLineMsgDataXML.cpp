#include "GGdiDrawLineMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GGdiDrawLineMsgDataXML::CXMLTag					("gdi_drawline_request");
const QString GGdiDrawLineMsgDataXML::CXMLVersion				("interface_version");
const QString GGdiDrawLineMsgDataXML::CXMLThickness				("thickness");
const QString GGdiDrawLineMsgDataXML::CXMLColor					("color");
const QString GGdiDrawLineMsgDataXML::CXMLLength				("length");
const QString GGdiDrawLineMsgDataXML::CXMLXLoc					("x_loc");
const QString GGdiDrawLineMsgDataXML::CXMLYLoc					("y_loc");
const QString GGdiDrawLineMsgDataXML::CXMLSynchronousRequest    ("SynchronizeMessage");

QByteArray GGdiDrawLineMsgDataXML::Marshall(GGdiDrawLineMsgData* pRequest)
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

GGdiDrawLineMsgData* GGdiDrawLineMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GGdiDrawLineMsgData* poRequest = new GGdiDrawLineMsgData();
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
	    if (oRoot.tagName() != "gdi_drawline_request") 
    	{
	    	throw GEXCEPTION(QString("Error: gdi_drawline_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName("interface_version");
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Thickness
	    oNodes = oRoot.elementsByTagName(CXMLThickness);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetThickness(oNodes.item(0).toElement().text());

	    	LOG_LEVEL4( QString("CXMLThickness=%1").arg(oNodes.item(0).toElement().text()) );
	    }
	    
	    // Load Color
	    oNodes = oRoot.elementsByTagName(CXMLColor);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetColor(oNodes.item(0).toElement().text());

	    	LOG_LEVEL4( QString("CXMLColor=%1").arg(oNodes.item(0).toElement().text()) );
	    }
	    
	    // Load Length
	    oNodes = oRoot.elementsByTagName(CXMLLength);
	    if(oNodes.count() == 1) 
	    {
			iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);

	    	LOG_LEVEL4( QString("CXMLLength=%1").arg(oNodes.item(0).toElement().text()) );

			if(bCastOk)
	    	{
	    		poRequest->SetLength(iLoc);
				//poRequest->SetLength(oNodes.item(0).toElement().text());
	    	}
	    	else
	    	{
	    		throw GEXCEPTION(QString("Length casting error."));
	    	}

	    }

	    // Load XLoc
	    oNodes = oRoot.elementsByTagName(CXMLXLoc);
	    if(oNodes.count() == 1) 
	    {	    	
	    	iLoc = oNodes.item(0).toElement().text().toInt(&bCastOk);

	    	LOG_LEVEL4( QString("CXMLXLoc=%1").arg(oNodes.item(0).toElement().text()) );

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

	    	LOG_LEVEL4( QString("CXMLYLoc=%1").arg(oNodes.item(0).toElement().text()) );

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

#include "GGdiDrawTextMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>
#include <QXmlStreamReader>

const QString GGdiDrawTextMsgDataXML::CXMLTag					("gdi_drawtext_request");
const QString GGdiDrawTextMsgDataXML::CXMLVersion				("interface_version");
const QString GGdiDrawTextMsgDataXML::CXMLBGColor				("bgcolor");
const QString GGdiDrawTextMsgDataXML::CXMLFGColor				("fgcolor");
const QString GGdiDrawTextMsgDataXML::CXMLText					("text");
const QString GGdiDrawTextMsgDataXML::CXMLXLoc					("x_loc");
const QString GGdiDrawTextMsgDataXML::CXMLYLoc					("y_loc");
const QString GGdiDrawTextMsgDataXML::CXMLFont					("font");
const QString GGdiDrawTextMsgDataXML::CXMLAttr					("attr");
const QString GGdiDrawTextMsgDataXML::CXMLSynchronousRequest    ("SynchronizeMessage");

QByteArray GGdiDrawTextMsgDataXML::Marshall(GGdiDrawTextMsgData* pRequest)
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

GGdiDrawTextMsgData* GGdiDrawTextMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GGdiDrawTextMsgData* poRequest = new GGdiDrawTextMsgData();
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
	    	throw GEXCEPTION(QString("Error: gdi_drawtext_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load BGColor
	    oNodes = oRoot.elementsByTagName(CXMLBGColor);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetBGColor(oNodes.item(0).toElement().text());
	    }
	    	    
	    // Load FGColor
	    oNodes = oRoot.elementsByTagName(CXMLFGColor);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetFGColor(oNodes.item(0).toElement().text());
	    }    
	    
	    // Load Text
	    oNodes = oRoot.elementsByTagName(CXMLText);
	    if(oNodes.count() == 1) 
	    {
	    	if( oNodes.item(0).toElement().text().isEmpty() )
	    	{
	    		QXmlStreamReader oXmlBuffer(strXML);

	    		while( !oXmlBuffer.isEndDocument() && oXmlBuffer.name().toString() != CXMLText )
	    		{
	    			oXmlBuffer.readNext();
	    		}

	    		if( oXmlBuffer.name().toString() == CXMLText )
	    		{
	    			poRequest->SetText(oXmlBuffer.readElementText());
	    		}
	    	}
	    	else
	    	{
	    		poRequest->SetText(oNodes.item(0).toElement().text());
	    	}
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
	    
	    // Load Font
	    oNodes = oRoot.elementsByTagName(CXMLFont);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetFont(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Attr
	    oNodes = oRoot.elementsByTagName(CXMLAttr);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetAttr(oNodes.item(0).toElement().text());
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

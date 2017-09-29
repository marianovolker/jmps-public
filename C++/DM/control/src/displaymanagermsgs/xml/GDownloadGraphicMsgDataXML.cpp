#include "GDownloadGraphicMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>

const QString GDownloadGraphicMsgDataXML::CXMLTag				("download_graphic_request");
const QString GDownloadGraphicMsgDataXML::CXMLVersion			("interface_version");
const QString GDownloadGraphicMsgDataXML::CXMLScreen			("screen");
const QString GDownloadGraphicMsgDataXML::CXMLType				("type");
const QString GDownloadGraphicMsgDataXML::CXMLCRC				("crc");
const QString GDownloadGraphicMsgDataXML::CXMLDisplay			("display");
const QString GDownloadGraphicMsgDataXML::CXMLFilename			("file_name");
const QString GDownloadGraphicMsgDataXML::CXMLSegment			("segment");
const QString GDownloadGraphicMsgDataXML::CXMLSynchronousRequest("SynchronizeMessage");

QByteArray GDownloadGraphicMsgDataXML::Marshall(GDownloadGraphicMsgData* pRequest)
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

GDownloadGraphicMsgData* GDownloadGraphicMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GDownloadGraphicMsgData* poRequest = new GDownloadGraphicMsgData();
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
	    	throw GEXCEPTION(QString("Error: download_graphic_request opening tag not detected"));
    	}
	    
	    // Load Interface Version
	    oNodes = oRoot.elementsByTagName(CXMLVersion);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Screen
	    oNodes = oRoot.elementsByTagName(CXMLScreen);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetScreen(oNodes.item(0).toElement().text());
	    }
	    
	    // Load Type
	    oNodes = oRoot.elementsByTagName(CXMLType);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetType(oNodes.item(0).toElement().text());
	    }

	    // Load CRC
	    oNodes = oRoot.elementsByTagName(CXMLCRC);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetCRC(oNodes.item(0).toElement().text());
	    }	    
	    	    
	    // Load Display
	    oNodes = oRoot.elementsByTagName(CXMLDisplay);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetDisplay(oNodes.item(0).toElement().text());
	    }	    
	    
	    // Load Filename
	    oNodes = oRoot.elementsByTagName(CXMLFilename);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetFilename(oNodes.item(0).toElement().text());
	    }	    
	    	    
	    // Load Segment
	    oNodes = oRoot.elementsByTagName(CXMLSegment);
	    if(oNodes.count() == 1) 
	    {
	    	poRequest->SetSegment(oNodes.item(0).toElement().text());
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

#include "GResponseMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <QFile>
#include <qdom.h>
#include <QDomProcessingInstruction>

const QString GResponseMsgDataXML::CXMLTag					("response");
const QString GResponseMsgDataXML::CXMLVersion				("interface_version");
const QString GResponseMsgDataXML::CXMLCommand				("command");
const QString GResponseMsgDataXML::CXMLData					("data");
const QString GResponseMsgDataXML::CXMLResponseCode			("response_code");

const QString GResponseMsgDataXML::CXML						("xml");
const QString GResponseMsgDataXML::CXMLType					("version=\"1.0\" encoding=\"utf-8\"");

QByteArray GResponseMsgDataXML::Marshall(GResponseMsgData* pRequest)
{
	LOG_LEVEL4("Marshall()");

    QDomDocument oDoc;
    QDomElement oRootElement, oElement;
    QDomProcessingInstruction oProcess;    
    QDomText oText;    
    QString strRespCode;
    
    oProcess = oDoc.createProcessingInstruction(CXML, CXMLType);
    oDoc.appendChild(oProcess);     
    
    {
    	QString strTagName(CXMLTag);
    	QString strNamespaceURI(pRequest->GetNamespaceURI());
    	strNamespaceURI = strNamespaceURI.trimmed();
    	if (!strNamespaceURI.isEmpty())
    	{
    		QString strNamespacePrefix(pRequest->GetNamespacePrefix());
    		strNamespacePrefix = strNamespacePrefix.trimmed();
    		if (!strNamespacePrefix.isEmpty())
    		{
    			strTagName.prepend(strNamespacePrefix+":");
    		}
    		oRootElement = oDoc.createElementNS(strNamespaceURI,strTagName);
    	}
    	else
    	{
    		oRootElement = oDoc.createElement(strTagName);
    	}
		oDoc.appendChild(oRootElement);
    }
    
    oText = oDoc.createTextNode(pRequest->GetInterfaceVersion());
    oElement = oDoc.createElement(CXMLVersion);
    oElement.appendChild(oText);
    oRootElement.appendChild(oElement);
    
    if(!pRequest->GetData().isEmpty())
    {
    	oText = oDoc.createTextNode(pRequest->GetData());
    	oElement = oDoc.createElement(CXMLData);
    	oElement.appendChild(oText);
	    oRootElement.appendChild(oElement);
    }
    
    oText = oDoc.createTextNode(pRequest->GetCommand());
    oElement = oDoc.createElement(CXMLCommand);
    oElement.appendChild(oText);
    oRootElement.appendChild(oElement);    
    
    // Convert Response Code from enum to String
    switch(pRequest->GetResponseCode())
    {
	    case GResponseMsgData::eOk:
		{
			strRespCode = GResponseMsgData::CResponseOk;
			break;
		}
	    case GResponseMsgData::eParseError:
		{
			strRespCode = GResponseMsgData::CResponseParseError;
			break;
		}	    	
	    case GResponseMsgData::eInvalidPersonality:
		{
			strRespCode = GResponseMsgData::CResponseInvalidPersonality;
			break;
		}
	    case GResponseMsgData::eUnknownError:	
		{
			strRespCode = GResponseMsgData::CResponseUnknownError;
			break;
		}
	    case GResponseMsgData::eError: 
		{
			strRespCode = GResponseMsgData::CResponseError;
			break;
		}
	    case GResponseMsgData::eInfoScreenError:
		{
			strRespCode = GResponseMsgData::CResponseInfoScreenError;
			break;
		}
	    case GResponseMsgData::eURLError:
		{
			strRespCode = GResponseMsgData::CResponseURLError;
			break;
		}
	    case GResponseMsgData::eGDIError:
		{
			strRespCode = GResponseMsgData::CResponseGDIError;
			break;
		}
	    case GResponseMsgData::eCantGetPersonalityList:
		{
			strRespCode = GResponseMsgData::CResponseCantGetPersonalityList;
			break;
		}
	    case GResponseMsgData::eInvalidMediaManagerType:
		{
			strRespCode = GResponseMsgData::CResponseInvalidMediaManagerType;
			break;
		}
	    case GResponseMsgData::eCantGetMediaManagerList:
		{
			strRespCode = GResponseMsgData::CResponseCantGetMediaManagerList;
			break;
		}
	    case GResponseMsgData::eInvalidForkFailureOption:
		{
			strRespCode = GResponseMsgData::CResponseInvalidForkFailureOption;
			break;
		}
	    case GResponseMsgData::eCantGetForkFailureOptionsList:
		{
			strRespCode = GResponseMsgData::CResponseCantGetForkFailureOptionsList;
			break;
		}
    }
	
    oText = oDoc.createTextNode(strRespCode);    
    oElement = oDoc.createElement(CXMLResponseCode);
    oElement.appendChild(oText);
    oRootElement.appendChild(oElement);

	return oDoc.toByteArray();
}

GResponseMsgData* GResponseMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	LOG_LEVEL4("UnMarshall()");

	GResponseMsgData* poRequest = new GResponseMsgData();
	QDomDocument doc;
	QDomNodeList oNodes;
	
	try
	{
		// Set Document content
		if (!doc.setContent(strXML, true))
		{
			LOG_LEVEL1(QString("UnMarshall error: Error building dom tree[%1]").arg(QString(strXML)));
			throw GEXCEPTION(QString("Error building dom tree"));
		}

		// Check root tag name
		QDomElement oRoot(doc.documentElement());
		if (oRoot.tagName() != CXMLTag)
		{
			throw GEXCEPTION(QString("Error: response opening tag not detected"));
		}
	    
		// Load Interface Version
		oNodes = oRoot.elementsByTagName(CXMLVersion);
		if(oNodes.count() == 1)
		{
			poRequest->SetInterfaceVersion(oNodes.item(0).toElement().text());
		}
	    
		// Load Command
		oNodes = oRoot.elementsByTagName(CXMLCommand);
		if(oNodes.count() == 1)
		{
			poRequest->SetCommand(oNodes.item(0).toElement().text());
		}
	    
		// Load Data
		oNodes = oRoot.elementsByTagName(CXMLData);
		if(oNodes.count() == 1)
		{
			poRequest->SetData(oNodes.item(0).toElement().text().toAscii());
		}
	    
		// Load ResponseCode
		oNodes = oRoot.elementsByTagName(CXMLResponseCode);
		if(oNodes.count() == 1)
		{
			QString strResponse( oNodes.item(0).toElement().text() );

			if(strResponse == GResponseMsgData::CResponseOk)
			{
				poRequest->SetResponseCode(GResponseMsgData::eOk);
			}
			else if(strResponse == GResponseMsgData::CResponseParseError)
			{
				poRequest->SetResponseCode(GResponseMsgData::eParseError);
			}
			else if(strResponse == GResponseMsgData::CResponseInvalidPersonality)	
			{
				poRequest->SetResponseCode(GResponseMsgData::eInvalidPersonality);
			}
			else if(strResponse == GResponseMsgData::CResponseUnknownError)	
			{
				poRequest->SetResponseCode(GResponseMsgData::eUnknownError);
			}
			else if(strResponse == GResponseMsgData::CResponseError)
			{
				poRequest->SetResponseCode(GResponseMsgData::eError);
			}
			else if(strResponse == GResponseMsgData::CResponseInfoScreenError)
			{
				poRequest->SetResponseCode(GResponseMsgData::eInfoScreenError);
			}
			else if(strResponse == GResponseMsgData::CResponseURLError)
			{
				poRequest->SetResponseCode(GResponseMsgData::eURLError);
			}
			else if(strResponse == GResponseMsgData::CResponseGDIError)
			{
				poRequest->SetResponseCode(GResponseMsgData::eGDIError);
			}
			else if(strResponse == GResponseMsgData::CResponseCantGetPersonalityList)
			{
				poRequest->SetResponseCode(GResponseMsgData::eCantGetPersonalityList);
			}
			else if(strResponse == GResponseMsgData::CResponseInvalidMediaManagerType)
			{
				poRequest->SetResponseCode(GResponseMsgData::eInvalidMediaManagerType);
			}
			else if(strResponse == GResponseMsgData::CResponseCantGetMediaManagerList)
			{
				poRequest->SetResponseCode(GResponseMsgData::eCantGetMediaManagerList);
			}
			else if(strResponse == GResponseMsgData::CResponseInvalidForkFailureOption)
			{
				poRequest->SetResponseCode(GResponseMsgData::eInvalidForkFailureOption);
			}
			else if(strResponse == GResponseMsgData::CResponseCantGetForkFailureOptionsList)
			{
				poRequest->SetResponseCode(GResponseMsgData::eCantGetForkFailureOptionsList);
			}
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

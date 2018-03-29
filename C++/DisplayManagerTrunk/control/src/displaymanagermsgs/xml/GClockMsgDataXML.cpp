#include "GClockMsgDataXML.h"

#include "utils/logger/src/GLogger.h"

#include <new>
#include <qdom.h>

const QString GClockMsgDataXML::CXMLTag ("clock_request");
const QString GClockMsgDataXML::CXMLVersion ("interface_version");
const QString GClockMsgDataXML::CXMLAction ("action");
const QString GClockMsgDataXML::CXMLXPos ("x_loc");
const QString GClockMsgDataXML::CXMLYPos ("y_loc");

GClockMsgData* GClockMsgDataXML::UnMarshall(QByteArray & strXML) throw(GException)
{
	GClockMsgData *pData = new (std::nothrow) GClockMsgData();
	if (!pData)
	{
		throw GEXCEPTION(QString("Error allocating memory."));
	}

	QDomDocument oDoc;
	QDomNodeList oNodes;

	try
	{
		QString strErrorMsg;
		int iErrorLine, iErrorColumn;

		/* Build the XML document representation from the byte array strXML */
		bool bRet = oDoc.setContent(strXML, true, &strErrorMsg, &iErrorLine, &iErrorColumn);
		if (!bRet)
		{
			LOG_LEVEL1( QString("UnMarshall error: Error building dom tree[%1]").arg(QString(strXML)).simplified() );
			LOG_LEVEL1( QString("[%1][%2][%3]").arg(strErrorMsg).arg(iErrorLine).arg(iErrorColumn).simplified() );

			throw GEXCEPTION(QString("Error building dom tree"));
		}
	
		/* Get the Root element */
		QDomElement oRoot(oDoc.documentElement());
		if (oRoot.tagName() != CXMLTag)
		{
			throw GEXCEPTION(QString("Error: clock_request opening tag not detected"));
		}
	
		oNodes = oRoot.elementsByTagName(CXMLVersion);
		if (oNodes.count() == 1)
		{
			pData->SetInterfaceVersion(oNodes.item(0).toElement().text());
		}

		oNodes = oRoot.elementsByTagName(CXMLAction);
		if (oNodes.count() == 1)
		{
			pData->SetAction(oNodes.item(0).toElement().text());
		}

		oNodes = oRoot.elementsByTagName(CXMLXPos);
		if (oNodes.count() == 1)
		{
			pData->SetXpos(oNodes.item(0).toElement().text().toInt());
		}

		oNodes = oRoot.elementsByTagName(CXMLYPos);
		if (oNodes.count() == 1)
		{
			pData->SetYpos(oNodes.item(0).toElement().text().toInt());
		}

		/* Load Namespace Prefix */
		pData->SetNamespacePrefix(oRoot.prefix());

		/* Load Namespace URI */
		pData->SetNamespaceURI(oRoot.namespaceURI());
	}
	catch(GException &ex)
	{
		delete pData;
		pData = NULL;
		throw;
	}

	return pData;
}

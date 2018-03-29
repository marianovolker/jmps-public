#include "GImageMappingsExporter.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>


GImageMappingsExporter::GImageMappingsExporter() { }


bool GImageMappingsExporter::Export(QSharedPointer<GImageMappings> oImageMappings, QString strFilename)
{

	std::auto_ptr<QDomDocument> oDoc(ExportImageMappings(oImageMappings));

    QFile oFile( strFilename );
    bool bRet( oFile.open(QIODevice::WriteOnly) );
    
    if(bRet)
    {
		QTextStream oTS( &oFile );
		oTS << oDoc->toString();		
		oFile.close();
		
		bRet = oTS.status() == QTextStream::Ok;
    }

    oDoc.reset();
	
	return bRet;	
}


std::auto_ptr<QDomDocument> GImageMappingsExporter::ExportImageMappings(QSharedPointer<GImageMappings> oImageMappings)
{

	std::auto_ptr<QDomDocument> oDoc(new QDomDocument);

    QDomElement oElementImageMappings(oDoc->createElement(GImageMappings::CXMLTag));

               		
    // Iterate through the Images and export them
	QList<GImageMap *> oImageList(oImageMappings->GetAllImageMaps());

	QList<GImageMap *>::iterator oImageIterator(oImageList.begin());

	GImageMap* oImage;

	while (oImageIterator != oImageList.end())
	{ 
		oImage = *oImageIterator;
		oDoc = ExportImage(oImage, oDoc, &oElementImageMappings);
		++oImageIterator;
	}      

	// Append generated elements into Document  
	oDoc->appendChild(oElementImageMappings);

	return oDoc;
}


std::auto_ptr<QDomDocument> GImageMappingsExporter::ExportImage(GImageMap* oImage, std::auto_ptr<QDomDocument> oDoc, QDomElement* oParent)
{	
	QDomElement oElementImageMap(oDoc->createElement(GImageMap::CXMLTag));

	oElementImageMap.setAttribute(GImageMap::CXMLSegment, 	oImage->GetSegment());
	oElementImageMap.setAttribute(GImageMap::CXMLName, 		oImage->GetName());
	oElementImageMap.setAttribute(GImageMap::CXMLFileName, 	oImage->GetFileName());
	oElementImageMap.setAttribute(GImageMap::CXMLFileFormat,oImage->GetFileFormat());	
	
	// Append generated elements into Document	
	oParent->appendChild(oElementImageMap);

	return oDoc;
}

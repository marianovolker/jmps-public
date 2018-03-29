#include "GImageMappingsImporter.h"

#include "utils/logger/src/GLogger.h"


QSharedPointer<GImageMappings> GImageMappingsImporter::Import(QString strFilename, QSharedPointer<GImageMappings> poImageMappings)
{
	LOG_LEVEL4("Import()");

	LOG_LEVEL3("Importing Image Mappings from file...");
    QFile file(strFilename);
    QXmlInputSource inputSource(&file);
    QXmlSimpleReader reader;
    GImageMappingsImporter importer(poImageMappings);
	
    reader.setContentHandler(static_cast<QXmlContentHandler *> (&importer));
    reader.setErrorHandler(&importer);
    reader.parse(inputSource);

    return importer.GetParsedImageMappings();
}


GImageMappingsImporter::GImageMappingsImporter():
m_poImageMappings(NULL)
{
	LOG_LEVEL4("GImageMappingsImporter()");

}

GImageMappingsImporter::GImageMappingsImporter(QSharedPointer<GImageMappings> poImageMappings):
m_poImageMappings(poImageMappings)
{
	LOG_LEVEL4("GImageMappingsImporter()");

}



QSharedPointer<GImageMappings> GImageMappingsImporter::GetParsedImageMappings()
{
	LOG_LEVEL4("GetParsedImageMappings()");

	return m_poImageMappings;
}

bool GImageMappingsImporter::startElement(const QString & namespaceURI,
		                                  const QString & localName,
		                                  const QString & qName,
		                                  const QXmlAttributes &attributes)
{
	LOG_LEVEL4("startElement()");

	LOG_LEVEL4( QString("namespaceURI=") + namespaceURI + ", localName=" + localName);

    if (qName == GImageMappings::CXMLTag)
    {
    	ParseStartMappings(attributes);
    }
    else if (qName == GImageMap::CXMLTag)
    {
    	ParseStartImage(attributes);
    }   
    
    return true;
}

bool GImageMappingsImporter::characters(const QString &str)
{	
	LOG_LEVEL4("characters()");

	LOG_LEVEL4(QString("str=") + str);

    return true;
}

bool GImageMappingsImporter::endElement(const QString &namespaceURI ,
		                                const QString &localName,
		                                const QString &qName)
{
	LOG_LEVEL4("endElement()");

	LOG_LEVEL4(QString("namespaceURI=") + namespaceURI + ", localName=" + localName + ", qName=" + qName);

    return true;
}

bool GImageMappingsImporter::fatalError(const QXmlParseException &exception)
{ 
	LOG_LEVEL3("fatalError()");

	throw GEXCEPTION( QString("Parsing Error: [") + exception.message() + QString("].") );
}

void GImageMappingsImporter::ParseStartMappings(const QXmlAttributes &oAttributes)
{
	LOG_LEVEL4("ParseStartMappings()");

	LOG_LEVEL4(QString("Number of attributes in the list[QXmlAttributes]: [") + QString::number(oAttributes.length()) + "]");

	m_poImageMappings->RemoveAllMappings();


}

void GImageMappingsImporter::ParseStartImage(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartImage()");

	std::auto_ptr<GImageMap> poImageMap(new GImageMap(oAttributes.value(GImageMap::CXMLName),
								oAttributes.value(GImageMap::CXMLFileName),
								oAttributes.value(GImageMap::CXMLSegment).toInt(),
								oAttributes.value(GImageMap::CXMLFileFormat)));
		
	// Append this Image Map
	if(poImageMap.get() != NULL)
	{
		m_poImageMappings->AddImageMap(poImageMap);
	}
	else
	{
		throw GEXCEPTION(QString("Parsing Error: No Container defined."));
	}
}


#ifndef GIMAGEMAPPINGSIMPORTER_H_
#define GIMAGEMAPPINGSIMPORTER_H_

#include <QXmlDefaultHandler>
#include <QXmlParseException>
#include <qxml.h>
#include <QString>
#include <QFile>
#include <QTextStream>


#include "GImageMappings.h"
#include "GImageMap.h"
#include "utils/base/src/GException.h"

class GImageMappingsImporter : public QXmlDefaultHandler
{
public:
	static QSharedPointer<GImageMappings> Import(QString strFilename, QSharedPointer<GImageMappings> oImageMappings);
	
private:
	GImageMappingsImporter();
	GImageMappingsImporter(QSharedPointer<GImageMappings> poImageMappings);

	QSharedPointer<GImageMappings> GetParsedImageMappings();
	
	// SAX-Overriden Parsing Methods	
	bool startElement(const QString & namespaceURI,
					  const QString & localName,
					  const QString & qName,
					  const QXmlAttributes & attributes);
	bool characters(const QString & str);
	bool endElement(const QString & namespaceURI,
					const QString & localName,
					const QString & qName);
	bool fatalError(const QXmlParseException & exception);
	
	// Our Parsing methods
	void ParseStartMappings(const QXmlAttributes &oAttributes);
	void ParseStartImage(const QXmlAttributes &oAttributes) throw(GException);
	
private:
	QSharedPointer<GImageMappings> m_poImageMappings;

};

#endif

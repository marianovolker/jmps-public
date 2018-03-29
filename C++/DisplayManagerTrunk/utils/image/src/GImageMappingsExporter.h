#ifndef GIMAGEMAPPINGSEXPORTER_H_
#define GIMAGEMAPPINGSEXPORTER_H_

#include "GImageMappings.h"

#include "qdom.h"
#include <memory>

class GImageMappingsExporter
{
public:	
	static bool Export(QSharedPointer<GImageMappings> oImageMappings, QString strFilename);
	
private:
	GImageMappingsExporter();
	static std::auto_ptr<QDomDocument> ExportImageMappings(QSharedPointer<GImageMappings> oImageMappings);
	static std::auto_ptr<QDomDocument> ExportImage(GImageMap* oImage, std::auto_ptr<QDomDocument> oDoc, QDomElement* oParent);
};

#endif

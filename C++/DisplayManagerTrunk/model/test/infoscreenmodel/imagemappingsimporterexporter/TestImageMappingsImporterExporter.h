#ifndef TESTIMAGEMAPPINGSIMPORTEREXPORTER_H_
#define TESTIMAGEMAPPINGSIMPORTEREXPORTER_H_

#include <QtTest/QtTest>
#include <QString>
#include <QFile>
#include <QSharedPointer>

#include "utils/image/src/GImageMappingsExporter.h"
#include "utils/image/src/GImageMappingsImporter.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/image/src/GImageMap.h"

class TestImageMappingsImporterExporter : public QObject
{
	Q_OBJECT
	
private:
	QSharedPointer<GImageMappings> m_poImageMappings;

private slots:		
	void initTestCase();	
	void cleanupTestCase();
	
	// Export ImageMappings Test
	void ExportImageMappings();
	void ImportAndReExportImageMappings();
	void CompareFiles();
};


#endif

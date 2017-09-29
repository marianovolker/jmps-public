#ifndef GISMODELEXPORTER_H_
#define GISMODELEXPORTER_H_

#include <QFile>
#include <qdom.h>
#include <QString>
#include <QTextStream>
#include "GISModel.h"
#include "GDisplay.h"
#include "GScreen.h"
#include "GSegment.h"
#include "GTextField.h"

class GISModelExporter
{
public:	
	static bool Export(GISModel* pModel, QString strFilename);
	
private:	
	GISModelExporter();
	static QDomDocument* ExportModel(GISModel* pModel);
	static void ExportDisplay(GDisplay* oDisplay, QDomDocument* oDoc, QDomElement* oParent);
	static void ExportColorPalette(GColorPalette* oColorPalette, QDomDocument* oDoc, QDomElement* oParent);	
	static void ExportScreen(GScreen* oDisplay, QDomDocument* oDoc, QDomElement* oParent);
	static void ExportSegment(GSegment* oSegment, QDomDocument* oDoc, QDomElement* oParent);
	static void ExportTextField(GTextField* oTextField, QDomDocument* oDoc, QDomElement* oParent);	
};

#endif /*GISMODELEXPORTER_H_*/

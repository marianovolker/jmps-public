#ifndef GISModelIMPORTER_H_
#define GISModelIMPORTER_H_

#include <QXmlDefaultHandler>
#include <QXmlParseException>
#include <qxml.h>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "GISModel.h"
#include "GDisplay.h"
#include "GScreen.h"
#include "GSegment.h"
#include "GTextField.h"
#include "utils/base/src/GException.h"

class GISModelImporter : public QXmlDefaultHandler
{
public:
	static GISModel* Import(QString strFilename);
	
private:
	GISModelImporter();
	GISModel* GetParsedModel();
	
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
	void ParseStartModel(const QXmlAttributes &oAttributes);
	
	void ParseStartDisplay(const QXmlAttributes &oAttributes) throw(GException);
	void ParseEndDisplay();
	
	void ParseStartColorRegister(const QXmlAttributes &oAttributes) throw(GException);	
	
	void ParseStartScreen(const QXmlAttributes &oAttributes) throw(GException);
	void ParseEndScreen();
	
	void ParseStartSegment(const QXmlAttributes &oAttributes) throw(GException);
	void ParseEndSegment();
	
	void ParseStartTextfield(const QXmlAttributes &oAttributes) throw(GException);
	void ParseEndTextfield();
	
private:
	GISModel* m_pModel;
	GDisplay* m_oDisplay;
	int m_iCurrentScreen;
	GScreen* m_oScreen;
	GSegment* m_oSegment;
	GTextField* m_oTextField;
};

#endif

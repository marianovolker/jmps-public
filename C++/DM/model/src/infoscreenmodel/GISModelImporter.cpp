#include "GISModelImporter.h"

#include "utils/logger/src/GLogger.h"

GISModel* GISModelImporter::Import(QString strFilename)
{
	LOG_LEVEL4("Import()");

    QFile file(strFilename);
    QXmlInputSource inputSource(&file);
    QXmlSimpleReader reader;
    GISModelImporter importer;
    GISModel* pModel;
    
    reader.setContentHandler((QXmlContentHandler *) &importer);
    reader.setErrorHandler(&importer);

    try
    {    
	    reader.parse(inputSource);
	    pModel = importer.GetParsedModel(); 
    }
    catch(...)
    {
    	pModel = NULL;
    }
    
    return pModel; 
}

GISModelImporter::GISModelImporter() :
	m_pModel(NULL),
	m_oDisplay(NULL),
	m_oScreen(NULL),
	m_oSegment(NULL),
	m_oTextField(NULL)
{
	LOG_LEVEL4("GISModelImporter()");

}


GISModel* GISModelImporter::GetParsedModel()
{
	LOG_LEVEL4("GetParsedModel()");

	return m_pModel;
}

bool GISModelImporter::startElement(const QString & namespaceURI,
                                  const QString & localName,
                                  const QString & qName,
                                  const QXmlAttributes &attributes)
{
	LOG_LEVEL4("startElement()");

	LOG_LEVEL4(QString("namespaceURI=") + namespaceURI + ", localName=" + localName);

    if (qName == GISModel::CXMLTag)
    {
    	ParseStartModel(attributes);
    }
    else if (qName == GDisplay::CXMLTag)
    {
    	ParseStartDisplay(attributes);
    }
    else if (qName == GColorPalette::CXMLRegisterTag)
    {
    	ParseStartColorRegister(attributes);
    }    
    else if (qName == GScreen::CXMLTag)
    {
    	ParseStartScreen(attributes);
    }
    else if (qName == GSegment::CXMLTag)
    {
    	ParseStartSegment(attributes);
    }
    else if (qName == GTextField::CXMLTag)
    {
    	ParseStartTextfield(attributes);
    }    
    
    return true;
}

bool GISModelImporter::characters(const QString &str)
{
	LOG_LEVEL4("characters()");

	LOG_LEVEL4(QString("str=") + str);

    return true;
}

bool GISModelImporter::endElement(const QString &namespaceURI ,
                                const QString &localName,
                                const QString &qName)
{
	LOG_LEVEL4("endElement()");

	LOG_LEVEL4(QString("namespaceURI=") + namespaceURI + ", localName=" + localName + ", qName=" + qName);

    if (qName == GDisplay::CXMLTag)
    {
		ParseEndDisplay();
    }
    else if (qName == GScreen::CXMLTag)
    {
    	ParseEndScreen();
    }
    else if (qName == GSegment::CXMLTag)
    {
    	ParseEndSegment();
    }
    else if (qName == GTextField::CXMLTag)
    {
    	ParseEndTextfield();
    }  
	
    return true;
}

bool GISModelImporter::fatalError(const QXmlParseException &exception)
{
	LOG_LEVEL3("fatalError()");

	throw GEXCEPTION( QString("Parsing Error: [") + exception.message() + QString("].") );
}

void GISModelImporter::ParseStartModel(const QXmlAttributes &oAttributes)
{
	LOG_LEVEL4("ParseStartModel()");

	LOG_LEVEL4(QString("Number of attributes in the list[QXmlAttributes]: [") + QString::number(oAttributes.length()) + "]");

	m_pModel = new GISModel();
}

void GISModelImporter::ParseStartDisplay(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartDisplay()");

	m_oDisplay = new GDisplay(oAttributes.value(GDisplay::CXMLNumber).toInt());

	m_oDisplay->SetColor	( oAttributes.value(GDisplay::CXMLIsColor).toInt() );	 
	m_iCurrentScreen = 	  	  oAttributes.value(GDisplay::CXMLCurrentScreen).toInt();
	
	// Append this display to the model
	if(m_pModel != NULL)
	{
		m_pModel->AddDisplay(m_oDisplay);
	}
	else
	{
		throw QXmlParseException(QString("Parsing Error: Display with no model associated"));
	}
}

void GISModelImporter::ParseEndDisplay()
{
	LOG_LEVEL4("ParseEndDisplay()");

	GScreen* oCurrentScreen = m_oDisplay->GetScreen(m_iCurrentScreen);
	m_oDisplay->SetCurrentScreen(oCurrentScreen); 
	m_oDisplay = NULL;
}

void GISModelImporter::ParseStartColorRegister(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartColorRegister()");

	GColorPalette* oPalette;
	
	// Set the Received Color Register
	if(m_oDisplay != NULL)
	{
		oPalette = m_oDisplay->GetColorPalette();
		oPalette->SetColor( oAttributes.value(GColorPalette::CXMLNumber).toInt(),
							oAttributes.value(GColorPalette::CXMLRed).toInt(),
							oAttributes.value(GColorPalette::CXMLGreen).toInt(),
							oAttributes.value(GColorPalette::CXMLBlue).toInt()  	);		
	}
	else
	{
		throw QXmlParseException(QString("Parsing Error: Color Register with no Display Associated"));
	}
}

void GISModelImporter::ParseStartScreen(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartScreen()");

	m_oScreen = new GScreen	( oAttributes.value(GScreen::CXMLNumber).toInt(), m_oDisplay );
	
	m_oScreen->SetBGColor	( oAttributes.value(GScreen::CXMLBGColor).toInt()	);
		
	// Append this screen to the latest display
	if(m_oDisplay != NULL)
	{
		m_oDisplay->AddScreen(m_oScreen);
	}
	else
	{
		throw QXmlParseException(QString("Parsing Error: Screen with no display associated"));
	}	
}

void GISModelImporter::ParseEndScreen()
{
	LOG_LEVEL4("ParseEndScreen()");

	m_oScreen = NULL;
}

void GISModelImporter::ParseStartSegment(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartSegment()");

	m_oSegment = new GSegment	( oAttributes.value(GSegment::CXMLNumber).toInt(), m_oScreen);
	
	m_oSegment->SetBGColor		( oAttributes.value(GSegment::CXMLBGColor).toInt()	);
	m_oSegment->SetVisible		( oAttributes.value(GSegment::CXMLVisible).toInt() 	);
	m_oSegment->SetWidth		( oAttributes.value(GSegment::CXMLWidth).toInt()	);
	m_oSegment->SetHeight		( oAttributes.value(GSegment::CXMLHeight).toInt()	);
	m_oSegment->SetXPos			( oAttributes.value(GSegment::CXMLXPos).toInt()		);
	m_oSegment->SetYPos			( oAttributes.value(GSegment::CXMLYPos).toInt()		);	
	
	// Append this segment to the latest screen	
	if(m_oScreen != NULL)
	{
		m_oScreen->AddSegment(m_oSegment);
	}
	else
	{
		throw QXmlParseException(QString("Parsing Error: Segment with no Screen associated"));
	}
}

void GISModelImporter::ParseEndSegment()
{
	LOG_LEVEL4("ParseEndSegment()");

	m_oSegment = NULL;
}

void GISModelImporter::ParseStartTextfield(const QXmlAttributes &oAttributes) throw(GException)
{
	LOG_LEVEL4("ParseStartTextfield()");

	m_oTextField = new GTextField	( oAttributes.value(GTextField::CXMLNumber).toInt(), m_oSegment);
	
	m_oTextField->SetText			( oAttributes.value(GTextField::CXMLText) 					);
	m_oTextField->SetFlashing		( oAttributes.value(GTextField::CXMLFlashing).toInt()		);
	m_oTextField->SetJustification	( oAttributes.value(GTextField::CXMLJustification).toInt()	);
	m_oTextField->SetScrollSpeed	( oAttributes.value(GTextField::CXMLScrollSpeed).toInt()	);
	m_oTextField->SetUnderscore		( oAttributes.value(GTextField::CXMLUnderscore).toInt() 	);	
	m_oTextField->SetVisible		( oAttributes.value(GTextField::CXMLVisible).toInt()		);
	m_oTextField->SetXpos			( oAttributes.value(GTextField::CXMLXPos).toInt()			);	
	m_oTextField->SetYpos			( oAttributes.value(GTextField::CXMLYPos).toInt()			);	
	m_oTextField->SetScroll			( oAttributes.value(GTextField::CXMLScroll).toInt()			);
	m_oTextField->SetFont			( oAttributes.value(GTextField::CXMLFont).toInt()			);	
	
	// Append this text field to the latest segment
	if(m_oSegment != NULL)
	{
		m_oSegment->AddField(m_oTextField);
	}
	else
	{
		throw QXmlParseException(QString("Parsing Error: Text Field with no Segment associated"));
	}	
}

void GISModelImporter::ParseEndTextfield()
{
	LOG_LEVEL4("ParseEndTextfield()");

	m_oTextField = NULL;
}

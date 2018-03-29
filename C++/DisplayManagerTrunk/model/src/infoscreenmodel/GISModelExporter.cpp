#include "GISModelExporter.h"

#include <QDataStream>

GISModelExporter::GISModelExporter()
{
}

bool GISModelExporter::Export(GISModel* pModel, QString strFilename)
{
    QDomDocument *oDoc = ExportModel(pModel);
    QFile oFile( strFilename );
    bool bRet( oFile.open(QIODevice::WriteOnly) );
    
    if( bRet )
    {
		QTextStream oTS( &oFile );
		oTS << oDoc->toString();
		oFile.close();
		
		bRet = oTS.status() == QTextStream::Ok;
    }

	delete oDoc;
	oDoc = NULL;
	
	return bRet;
}

QDomDocument* GISModelExporter::ExportModel(GISModel* pModel)
{
    QDomDocument *oDoc = new QDomDocument;
    QDomElement oElementModel = oDoc->createElement(GISModel::CXMLTag);
               		
    // Iterate through the Displays and export them
	QList<GDisplay *> oDisplayList(pModel->GetAllDisplays());	
	QList<GDisplay *>::iterator oDisplayIterator = oDisplayList.begin();
	GDisplay* oDisplay;
	
	while( oDisplayIterator != oDisplayList.end() )
	{
		oDisplay = (GDisplay *) *oDisplayIterator;
		ExportDisplay(oDisplay, oDoc, &oElementModel);
		++oDisplayIterator;
	}

	// Append generated elements into Document  
	oDoc->appendChild(oElementModel);
	
	return oDoc;
}

void GISModelExporter::ExportDisplay(GDisplay* oDisplay, QDomDocument* oDoc, QDomElement* oParent)
{
	QDomElement oElementDisplay = oDoc->createElement(GDisplay::CXMLTag);

	oElementDisplay.setAttribute(GDisplay::CXMLNumber, oDisplay->GetNumber());

	if( oDisplay->GetCurrentScreen() != NULL )
	{
		oElementDisplay.setAttribute(GDisplay::CXMLCurrentScreen, oDisplay->GetCurrentScreen()->GetNumber());
	}
	else
	{
		oElementDisplay.setAttribute(GDisplay::CXMLCurrentScreen, 0);
	}

	oElementDisplay.setAttribute(GDisplay::CXMLIsColor, oDisplay->IsColor());			

	// Export the Color Palette 
	ExportColorPalette(oDisplay->GetColorPalette(), oDoc, &oElementDisplay);

	// Iterate through the Screens and export them	
	QList<GScreen *> oScreenList(oDisplay->GetAllScreens());	
	QList<GScreen *>::iterator oScreenIterator = oScreenList.begin();
	GScreen* pScreen;
	
	while( oScreenIterator != oScreenList.end() )
	{
		pScreen = (GScreen *) *oScreenIterator;

		if( pScreen->IsPersistent() )
		{
			ExportScreen(pScreen, oDoc, &oElementDisplay);
		}

		++oScreenIterator;
	}
	
	// Append generated elements into Document	
	oParent->appendChild(oElementDisplay);
}

void GISModelExporter::ExportColorPalette(GColorPalette* oColorPalette, QDomDocument* oDoc, QDomElement* oParent)
{	
	QDomElement oElementColorPalette = oDoc->createElement(GColorPalette::CXMLTag);
	int i = 0;
		
	while (++i <= GColorPalette::CLastRegister)
	{
		QDomElement oElementRegister = oDoc->createElement(GColorPalette::CXMLRegisterTag);
		
		oElementRegister.setAttribute(GColorPalette::CXMLNumber, 	i);
		oElementRegister.setAttribute(GColorPalette::CXMLRed, 		oColorPalette->GetRed(i));
		oElementRegister.setAttribute(GColorPalette::CXMLGreen, 	oColorPalette->GetGreen(i));
		oElementRegister.setAttribute(GColorPalette::CXMLBlue, 		oColorPalette->GetBlue(i));
		
		oElementColorPalette.appendChild(oElementRegister);
	}

	// Append generated elements into Document	
	oParent->appendChild(oElementColorPalette);
}

void GISModelExporter::ExportScreen(GScreen* oScreen, QDomDocument* oDoc, QDomElement* oParent)
{	
	QDomElement oElementScreen = oDoc->createElement(GScreen::CXMLTag);

	oElementScreen.setAttribute(GScreen::CXMLNumber, 		oScreen->GetNumber());
	oElementScreen.setAttribute(GScreen::CXMLDisplayNumber, oScreen->GetDisplay()->GetNumber());	
	oElementScreen.setAttribute(GScreen::CXMLBGColor, 		oScreen->GetBGColor());	
		
	// Iterate through the Segments and export them	
	QList<GSegment *> pSegmentList(oScreen->GetAllSegments());
	QList<GSegment *>::iterator pSegmentIterator = pSegmentList.begin();
	GSegment* pSegment;
	
	while( pSegmentIterator != pSegmentList.end() )
	{ 
		pSegment = (GSegment *) *pSegmentIterator;

		ExportSegment(pSegment, oDoc, &oElementScreen);

		++pSegmentIterator;
	}

	// Append generated elements into Document	
	oParent->appendChild(oElementScreen);
}

void GISModelExporter::ExportSegment(GSegment* oSegment, QDomDocument* oDoc, QDomElement* oParent)
{	
	QDomElement oElementSegment= oDoc->createElement(GSegment::CXMLTag);

	oElementSegment.setAttribute(GSegment::CXMLNumber, 			oSegment->GetNumber());
	oElementSegment.setAttribute(GSegment::CXMLScreenNumber, 	oSegment->GetScreen()->GetNumber());
	oElementSegment.setAttribute(GSegment::CXMLBGColor, 		oSegment->GetBGColor());	
	oElementSegment.setAttribute(GSegment::CXMLVisible, 		oSegment->IsVisible());
	oElementSegment.setAttribute(GSegment::CXMLWidth, 			oSegment->GetWidth());
	oElementSegment.setAttribute(GSegment::CXMLHeight, 			oSegment->GetHeight());
	oElementSegment.setAttribute(GSegment::CXMLXPos, 			oSegment->GetXPos());
	oElementSegment.setAttribute(GSegment::CXMLYPos, 			oSegment->GetYPos());		
	
	// Iterate through the Segments and export them	
	QList<GTextField *> oFieldList(oSegment->GetAllFields());	
	QList<GTextField *>::iterator pFieldIterator = oFieldList.begin();
	GTextField* pField;
	
	while( pFieldIterator != oFieldList.end() )
	{
		pField = (GTextField *) *pFieldIterator;

		ExportTextField(pField, oDoc, &oElementSegment);

		++pFieldIterator;
	}

	// Append generated elements into Document	
	oParent->appendChild(oElementSegment);
}

void GISModelExporter::ExportTextField(GTextField* pTextField, QDomDocument* oDoc, QDomElement* oParent)
{	
	QDomElement oElementTextField = oDoc->createElement(GTextField::CXMLTag);
	
	oElementTextField.setAttribute(GTextField::CXMLNumber, 			pTextField->GetNumber());
	oElementTextField.setAttribute(GTextField::CXMLSegmentNumber, 	pTextField->GetSegment()->GetNumber());
	oElementTextField.setAttribute(GTextField::CXMLText, 			pTextField->GetText());
	oElementTextField.setAttribute(GTextField::CXMLFlashing, 		pTextField->IsFlashing());
	oElementTextField.setAttribute(GTextField::CXMLJustification, 	pTextField->GetJustification());
	oElementTextField.setAttribute(GTextField::CXMLScrollSpeed, 	pTextField->GetScrollSpeed());
	oElementTextField.setAttribute(GTextField::CXMLUnderscore, 		pTextField->IsUnderscore());
	oElementTextField.setAttribute(GTextField::CXMLVisible, 		pTextField->IsVisible());
	oElementTextField.setAttribute(GTextField::CXMLXPos, 			pTextField->GetXpos());
	oElementTextField.setAttribute(GTextField::CXMLYPos, 			pTextField->GetYpos());
	oElementTextField.setAttribute(GTextField::CXMLScroll, 			pTextField->GetScroll());
	oElementTextField.setAttribute(GTextField::CXMLFont, 			pTextField->GetFont());
		
	// Append generated elements into Document	
	oParent->appendChild(oElementTextField);
}

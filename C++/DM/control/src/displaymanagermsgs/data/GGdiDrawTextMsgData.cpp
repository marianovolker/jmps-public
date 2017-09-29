#include "GGdiDrawTextMsgData.h"

#include "utils/logger/src/GLogger.h"

inline QSet<QString> InitializeValidFonts (void)
{
	LOG_LEVEL4("InitializeValidFonts()");
	QSet<QString> qsetValidFont;
	qsetValidFont.insert("0");
	qsetValidFont.insert("1");
	qsetValidFont.insert("sm");
	return qsetValidFont;
}

QSet<QString> const GGdiDrawTextMsgData::_qksetValidFont(InitializeValidFonts());

GGdiDrawTextMsgData::GGdiDrawTextMsgData()
{
	LOG_LEVEL4("GGdiDrawTextMsgData()");
}

GGdiDrawTextMsgData::~GGdiDrawTextMsgData()
{
	LOG_LEVEL4("~GGdiDrawTextMsgData()");
}

QString GGdiDrawTextMsgData::GetBGColor()
{
	LOG_LEVEL4("GetBGColor()");

	return m_strBGColor;
}

void GGdiDrawTextMsgData::SetBGColor(QString strBGColor)
{
	LOG_LEVEL4("SetBGColor()");

	m_strBGColor = strBGColor;
}

QString GGdiDrawTextMsgData::GetFGColor()
{
	LOG_LEVEL4("GetFGColor()");

	return m_strFGColor;
}

void GGdiDrawTextMsgData::SetFGColor(QString strFGColor)
{
	LOG_LEVEL4("SetFGColor()");

	m_strFGColor = strFGColor;
}

QString GGdiDrawTextMsgData::GetText()
{
	LOG_LEVEL4("GetText()");

	return m_strText;
}

void GGdiDrawTextMsgData::SetText(QString strText)
{
	LOG_LEVEL4("SetText()");

	m_strText = strText;
}

int GGdiDrawTextMsgData::GetXLoc()
{
	LOG_LEVEL4("GetXLoc()");

	return m_iXLoc;
}

void GGdiDrawTextMsgData::SetXLoc(int iXLoc)
{
	LOG_LEVEL4("SetXLoc()");

	m_iXLoc = iXLoc;
}

int GGdiDrawTextMsgData::GetYLoc()
{
	LOG_LEVEL4("GetYLoc()");

	return m_iYLoc;
}

void GGdiDrawTextMsgData::SetYLoc(int iYLoc)
{
	LOG_LEVEL4("SetYLoc()");

	m_iYLoc = iYLoc;
}

QString GGdiDrawTextMsgData::GetFont()
{
	LOG_LEVEL4("GetFont()");

	return  m_strFont;
}

void GGdiDrawTextMsgData::SetFont(QString strFont)
{
	LOG_LEVEL4("SetFont()");

	m_strFont = strFont;
}

QString GGdiDrawTextMsgData::GetAttr()
{
	LOG_LEVEL4("GetAttr()");

	return m_strAttr;
}

void GGdiDrawTextMsgData::SetAttr(QString strAttr)
{
	LOG_LEVEL4("SetAttr()");

	m_strAttr = strAttr;
}

void GGdiDrawTextMsgData::Validate() throw(GException)
{
	LOG_LEVEL4("Validate()");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if ( !IsValidFont(GetFont().toLower()) )
	{
		//TODO: Commented for Backward Compatibility. Validation not present in Java Code. Only logging instead of Exception throwing.
		//throw GException("Error: GDI Font Undefined.");
		LOG_LEVEL2("Error: GDI Font Undefined.");
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

bool GGdiDrawTextMsgData::IsValidFont(QString const & qksFont)
{
	LOG_LEVEL4("IsValidFont()");
	return 	_qksetValidFont.contains(qksFont);
}

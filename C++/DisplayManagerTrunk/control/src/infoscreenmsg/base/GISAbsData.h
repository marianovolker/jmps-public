//! GISAbsData 
/*!
  Implementation of the Class GISAbsData
*/

#if !defined(GISAbsData_h)
#define GISAbsData_h

#include <QSet>
#include "utils/base/src/GAbsData.h"

class GISAbsData : public GAbsData 
{
public:
	GISAbsData();
	virtual ~GISAbsData();
	
	int GetAttr();
	void SetAttr(int iAttr);
	
	int GetColor();
	void SetColor(int iColor);
	
	int GetDisplay();
	void SetDisplay(int iCisplayNumber);
	
	int GetField();
	void SetField(int iFieldNumber);
	
	int GetFont();
	void SetFont(int iFont);
	
	int GetScreen();
	void SetScreen(int iScreenNumber);
	
	int GetSegment();
	void SetSegment(int iSegmentNumber);
	
	int GetXpos();
	void SetXpos(int iXpos);
	
	int GetYpos();
	void SetYpos(int iYpos);
	
	QByteArray GetVLD();
	void SetVLD(QByteArray strVLD);
	
	bool GetSynchronousRequest();
	void SetSynchronousRequest(bool bSynchronousRequest);

	virtual void Validate() throw(GException) = 0;
	virtual QString GetName() = 0;
	virtual QString GetCommand() = 0;	
	
protected:
	static bool IsValidFont(int iFont);

private:
	int m_iAttr;
	int m_iColor;	
	int m_iDisplay;
	int m_iField;
	int m_iFont;
	int m_iScreen;
	int m_iSegment;
	int m_iXpos;
	int m_iYpos;
	QByteArray m_strVLD;
	bool m_bSynchronousRequest;

private:
	static QSet<unsigned> const _qksetValidFont;
};
#endif

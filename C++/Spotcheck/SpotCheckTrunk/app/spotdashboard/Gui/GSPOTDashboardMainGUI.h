///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GSPOTDashboardMainGUI__
#define __GSPOTDashboardMainGUI__

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GSPOTDashboardMainGUI
///////////////////////////////////////////////////////////////////////////////
class GSPOTDashboardMainGUI : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel132511;
		wxPanel* m_panel1325111;
		wxPanel* m_panel1325112;
		wxStaticBitmap* m_bitmap41;
		wxStaticText* m_staticText5;
		wxPanel* m_panel13251;
		wxPanel* m_panel132514;
		wxStaticText* m_staticText6;
		wxPanel* m_panel132512;
		wxPanel* m_panel132513;
		wxStaticText* m_staticText412;
		wxPanel* m_panel1325131;
		wxStaticText* m_staticText11311;
		wxStaticText* m_staticText1131;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText111;
		wxStaticText* m_staticText1111;
		wxStaticText* m_staticText112;
		wxStaticText* m_staticText113;
		wxPanel* m_panel171;
		wxStaticText* m_staticText41;
		wxStaticBitmap* m_bitmap12;
		wxPanel* m_panel1711;
		wxStaticText* m_staticText411;
		wxStaticBitmap* m_bitmap1;
		wxPanel* m_panel17111;
		wxStaticText* m_staticText4111;
		wxStaticBitmap* m_bitmap11;
		wxPanel* m_panel13251211;
		wxPanel* m_panel132512111;
		wxPanel* m_panel132512112;
		wxPanel* m_panel17;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnRefresh( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_textCtrlSpotType;
		wxTextCtrl* m_textCtrlSpotSerial;
		wxPanel* m_panelCategories;
		wxPanel* m_panelDisplay;
		wxTextCtrl* m_textCtrlSerialDISPLAY;
		wxTextCtrl* m_textCtrlFirmwareDISPLAY;
		wxStaticText* m_staticTextCommDISPLAY;
		wxStaticText* m_staticTextDismountDISPLAY;
		wxStaticText* m_staticTextOtherErrorsDISPLAY;
		wxStaticText* m_staticTextTamperedDISPLAY;
		wxTextCtrl* m_textCtrlStatusDISPLAY;
		wxPanel* m_panelCR;
		wxTextCtrl* m_textCtrlSerialCR;
		wxTextCtrl* m_textCtrlFirmwareCR;
		wxStaticText* m_staticTextCommCR;
		wxStaticText* m_staticTextDismountCR;
		wxStaticText* m_staticTextOtherErrorsCR;
		wxStaticText* m_staticTextTamperedCR;
		wxTextCtrl* m_textCtrlStatusCR;
		wxPanel* m_panelKP;
		wxTextCtrl* m_textCtrlSerialKP;
		wxTextCtrl* m_textCtrlFirmwareKP;
		wxStaticText* m_staticTextCommKP;
		wxStaticText* m_staticTextDismountKP;
		wxStaticText* m_staticTextOtherErrorsKP;
		wxStaticText* m_staticTextTamperedKP;
		wxTextCtrl* m_textCtrlStatusKP;
		wxButton* m_buttonRefresh;
		wxButton* m_buttonExit;
		wxStatusBar* m_statusBar;
		
		GSPOTDashboardMainGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SPOTDashboard"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,710 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GSPOTDashboardMainGUI();
	
};

#endif //__GSPOTDashboardMainGUI__

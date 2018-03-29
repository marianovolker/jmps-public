///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GSPOTDashboardMainGUI.h"

///////////////////////////////////////////////////////////////////////////

GSPOTDashboardMainGUI::GSPOTDashboardMainGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 900,710 ), wxSize( 900,710 ) );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 5, 4, 3, 5 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel132511 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132511->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132511, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1325111 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1325111->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel1325111, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1325112 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1325112->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel1325112, 1, wxEXPAND | wxALL, 5 );
	
	m_bitmap41 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("images/gilbarco_logo.jpg"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bitmap41, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("SPOT type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	m_textCtrlSpotType = new wxTextCtrl( this, wxID_ANY, wxT("M3"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( m_textCtrlSpotType, 0, wxALL, 5 );
	
	m_panel13251 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel13251->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel13251, 1, wxEXPAND | wxALL, 5 );
	
	m_panel132514 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132514->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132514, 1, wxEXPAND | wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("SPOT serial (PPN):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL, 5 );
	
	m_textCtrlSpotSerial = new wxTextCtrl( this, wxID_ANY, wxT("0002061115731202"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( m_textCtrlSpotSerial, 0, wxALL|wxEXPAND, 5 );
	
	m_panel132512 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132512->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132512, 1, wxEXPAND | wxALL, 5 );
	
	m_panel132513 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132513->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132513, 1, wxEXPAND | wxALL, 5 );
	
	m_panelCategories = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelCategories->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxFlexGridSizer* fgSizer62;
	fgSizer62 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer62->SetFlexibleDirection( wxBOTH );
	fgSizer62->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer62->SetMinSize( wxSize( 5,-1 ) ); 
	wxFlexGridSizer* fgSizer72;
	fgSizer72 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer72->SetFlexibleDirection( wxBOTH );
	fgSizer72->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText412 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Categories"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText412->Wrap( -1 );
	m_staticText412->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText412, 0, wxALL, 5 );
	
	m_panel1325131 = new wxPanel( m_panelCategories, wxID_ANY, wxDefaultPosition, wxSize( -1,185 ), wxTAB_TRAVERSAL );
	m_panel1325131->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_panel1325131->SetMinSize( wxSize( -1,185 ) );
	
	fgSizer72->Add( m_panel1325131, 1, wxEXPAND | wxALL, 5 );
	
	m_staticText11311 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Device S/N:"), wxDefaultPosition, wxSize( -1,18 ), 0 );
	m_staticText11311->Wrap( -1 );
	m_staticText11311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText11311, 0, wxALL, 5 );
	
	m_staticText1131 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Device F/W:"), wxDefaultPosition, wxSize( -1,18 ), 0 );
	m_staticText1131->Wrap( -1 );
	m_staticText1131->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText1131, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Comm OK?"), wxDefaultPosition, wxSize( -1,18 ), 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText11, 0, wxALL, 5 );
	
	m_staticText111 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Dismount OK?"), wxDefaultPosition, wxSize( -1,18 ), 0 );
	m_staticText111->Wrap( -1 );
	m_staticText111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText111, 0, wxALL, 5 );
	
	m_staticText112 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Tampered OK?"), wxDefaultPosition, wxSize( -1,15 ), 0 );
	m_staticText112->Wrap( -1 );
	m_staticText112->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText112->Enable( false );
	m_staticText112->Hide();
	
	fgSizer72->Add( m_staticText112, 0, wxALL, 5 );
	
	m_staticText113 = new wxStaticText( m_panelCategories, wxID_ANY, wxT("Status:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText113->Wrap( -1 );
	m_staticText113->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer72->Add( m_staticText113, 0, wxALL, 5 );
	
	fgSizer62->Add( fgSizer72, 1, wxEXPAND, 5 );
	
	m_panelCategories->SetSizer( fgSizer62 );
	m_panelCategories->Layout();
	fgSizer62->Fit( m_panelCategories );
	fgSizer1->Add( m_panelCategories, 1, wxEXPAND | wxALL, 5 );
	
	m_panelDisplay = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelDisplay->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer6->SetMinSize( wxSize( 5,-1 ) ); 
	m_panel171 = new wxPanel( m_panelDisplay, wxID_ANY, wxDefaultPosition, wxSize( 5,-1 ), wxTAB_TRAVERSAL );
	m_panel171->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer6->Add( m_panel171, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText41 = new wxStaticText( m_panelDisplay, wxID_ANY, wxT("SPOT Display"), wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_staticText41->Wrap( -1 );
	m_staticText41->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer7->Add( m_staticText41, 0, wxALL, 5 );
	
	m_bitmap12 = new wxStaticBitmap( m_panelDisplay, wxID_ANY, wxBitmap( wxT("images/screen.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 230,180 ), 0 );
	fgSizer7->Add( m_bitmap12, 0, wxALL, 5 );
	
	m_textCtrlSerialDISPLAY = new wxTextCtrl( m_panelDisplay, wxID_ANY, wxT("263B971100000052"), wxDefaultPosition, wxSize( -1,20 ), wxTE_READONLY );
	fgSizer7->Add( m_textCtrlSerialDISPLAY, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlFirmwareDISPLAY = new wxTextCtrl( m_panelDisplay, wxID_ANY, wxT("DY0178"), wxDefaultPosition, wxSize( 230,20 ), wxTE_READONLY );
	fgSizer7->Add( m_textCtrlFirmwareDISPLAY, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextCommDISPLAY = new wxStaticText( m_panelDisplay, wxID_ANY, wxT("YES"), wxDefaultPosition, wxSize( -1,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextCommDISPLAY->Wrap( -1 );
	m_staticTextCommDISPLAY->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextCommDISPLAY->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_staticTextCommDISPLAY->SetBackgroundColour( wxColour( 0, 128, 0 ) );
	
	fgSizer7->Add( m_staticTextCommDISPLAY, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDismountDISPLAY = new wxStaticText( m_panelDisplay, wxID_ANY, wxT("YES"), wxDefaultPosition, wxSize( 230,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextDismountDISPLAY->Wrap( -1 );
	m_staticTextDismountDISPLAY->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextDismountDISPLAY->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_staticTextDismountDISPLAY->SetBackgroundColour( wxColour( 0, 128, 0 ) );
	
	fgSizer7->Add( m_staticTextDismountDISPLAY, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	m_staticTextTamperedDISPLAY = new wxStaticText( m_panelDisplay, wxID_ANY, wxT("YES"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticTextTamperedDISPLAY->Wrap( -1 );
	m_staticTextTamperedDISPLAY->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextTamperedDISPLAY->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_staticTextTamperedDISPLAY->SetBackgroundColour( wxColour( 0, 128, 0 ) );
	m_staticTextTamperedDISPLAY->Hide();
	
	fgSizer7->Add( m_staticTextTamperedDISPLAY, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlStatusDISPLAY = new wxTextCtrl( m_panelDisplay, wxID_ANY, wxT("Installation sensor not activated: cannot operate\nSoftware Master Key not loaded into the device\nError in encrypted communication: SMK not matching"), wxDefaultPosition, wxSize( 230,70 ), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	fgSizer7->Add( m_textCtrlStatusDISPLAY, 0, wxALL|wxEXPAND, 5 );
	
	fgSizer6->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	m_panelDisplay->SetSizer( fgSizer6 );
	m_panelDisplay->Layout();
	fgSizer6->Fit( m_panelDisplay );
	fgSizer1->Add( m_panelDisplay, 1, wxEXPAND | wxALL, 5 );
	
	m_panelCR = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelCR->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer61->SetMinSize( wxSize( 5,-1 ) ); 
	m_panel1711 = new wxPanel( m_panelCR, wxID_ANY, wxDefaultPosition, wxSize( 5,-1 ), wxTAB_TRAVERSAL );
	m_panel1711->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer61->Add( m_panel1711, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer71;
	fgSizer71 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer71->SetFlexibleDirection( wxBOTH );
	fgSizer71->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText411 = new wxStaticText( m_panelCR, wxID_ANY, wxT("Card Reader"), wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_staticText411->Wrap( -1 );
	m_staticText411->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer71->Add( m_staticText411, 0, wxALL, 5 );
	
	m_bitmap1 = new wxStaticBitmap( m_panelCR, wxID_ANY, wxBitmap( wxT("images/hcr.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 130,180 ), 0 );
	fgSizer71->Add( m_bitmap1, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 5 );
	
	m_textCtrlSerialCR = new wxTextCtrl( m_panelCR, wxID_ANY, wxT("130A502868101709"), wxDefaultPosition, wxSize( 230,20 ), wxTE_READONLY );
	fgSizer71->Add( m_textCtrlSerialCR, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlFirmwareCR = new wxTextCtrl( m_panelCR, wxID_ANY, wxT("RD012-100204_1"), wxDefaultPosition, wxSize( 230,20 ), wxTE_READONLY );
	fgSizer71->Add( m_textCtrlFirmwareCR, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextCommCR = new wxStaticText( m_panelCR, wxID_ANY, wxT("YES"), wxDefaultPosition, wxSize( -1,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextCommCR->Wrap( -1 );
	m_staticTextCommCR->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextCommCR->SetForegroundColour( wxColour( 255, 255, 255 ) );
	m_staticTextCommCR->SetBackgroundColour( wxColour( 0, 128, 0 ) );
	
	fgSizer71->Add( m_staticTextCommCR, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDismountCR = new wxStaticText( m_panelCR, wxID_ANY, wxT("NO"), wxDefaultPosition, wxSize( 230,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextDismountCR->Wrap( -1 );
	m_staticTextDismountCR->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextDismountCR->SetBackgroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer71->Add( m_staticTextDismountCR, 0, wxALL, 5 );
	
	m_staticTextTamperedCR = new wxStaticText( m_panelCR, wxID_ANY, wxT("YES"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticTextTamperedCR->Wrap( -1 );
	m_staticTextTamperedCR->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextTamperedCR->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_staticTextTamperedCR->SetBackgroundColour( wxColour( 0, 128, 0 ) );
	m_staticTextTamperedCR->Hide();
	
	fgSizer71->Add( m_staticTextTamperedCR, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlStatusCR = new wxTextCtrl( m_panelCR, wxID_ANY, wxT("SYSTEM is in error due to errors on one or more critical subsystems 4 (M5 only)\nSecurity module command failed\nGeneric error or timely frw cryptographic-check failed (SPOT M3c-2.1)"), wxDefaultPosition, wxSize( 230,70 ), wxTE_MULTILINE|wxTE_READONLY );
	fgSizer71->Add( m_textCtrlStatusCR, 0, wxALL|wxEXPAND, 5 );
	
	fgSizer61->Add( fgSizer71, 1, wxEXPAND, 5 );
	
	m_panelCR->SetSizer( fgSizer61 );
	m_panelCR->Layout();
	fgSizer61->Fit( m_panelCR );
	fgSizer1->Add( m_panelCR, 1, wxEXPAND | wxALL, 5 );
	
	m_panelKP = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelKP->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	wxFlexGridSizer* fgSizer611;
	fgSizer611 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer611->SetFlexibleDirection( wxBOTH );
	fgSizer611->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer611->SetMinSize( wxSize( 5,-1 ) ); 
	m_panel17111 = new wxPanel( m_panelKP, wxID_ANY, wxDefaultPosition, wxSize( 5,-1 ), wxTAB_TRAVERSAL );
	m_panel17111->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer611->Add( m_panel17111, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer711;
	fgSizer711 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer711->SetFlexibleDirection( wxBOTH );
	fgSizer711->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4111 = new wxStaticText( m_panelKP, wxID_ANY, wxT("Keypad"), wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_staticText4111->Wrap( -1 );
	m_staticText4111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer711->Add( m_staticText4111, 0, wxALL, 5 );
	
	m_bitmap11 = new wxStaticBitmap( m_panelKP, wxID_ANY, wxBitmap( wxT("images/keypad_py.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 200,180 ), 0 );
	fgSizer711->Add( m_bitmap11, 0, wxALL, 5 );
	
	m_textCtrlSerialKP = new wxTextCtrl( m_panelKP, wxID_ANY, wxT("26683971000000D4"), wxDefaultPosition, wxSize( 230,20 ), wxTE_READONLY );
	fgSizer711->Add( m_textCtrlSerialKP, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlFirmwareKP = new wxTextCtrl( m_panelKP, wxID_ANY, wxT("KB0172"), wxDefaultPosition, wxSize( 230,20 ), wxTE_READONLY );
	fgSizer711->Add( m_textCtrlFirmwareKP, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextCommKP = new wxStaticText( m_panelKP, wxID_ANY, wxT("NO"), wxDefaultPosition, wxSize( 230,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextCommKP->Wrap( -1 );
	m_staticTextCommKP->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextCommKP->SetBackgroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer711->Add( m_staticTextCommKP, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDismountKP = new wxStaticText( m_panelKP, wxID_ANY, wxT("NO"), wxDefaultPosition, wxSize( 230,20 ), 0|wxSIMPLE_BORDER );
	m_staticTextDismountKP->Wrap( -1 );
	m_staticTextDismountKP->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextDismountKP->SetBackgroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer711->Add( m_staticTextDismountKP, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextTamperedKP = new wxStaticText( m_panelKP, wxID_ANY, wxT("NO"), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticTextTamperedKP->Wrap( -1 );
	m_staticTextTamperedKP->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticTextTamperedKP->SetBackgroundColour( wxColour( 255, 0, 0 ) );
	m_staticTextTamperedKP->Hide();
	
	fgSizer711->Add( m_staticTextTamperedKP, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlStatusKP = new wxTextCtrl( m_panelKP, wxID_ANY, wxT("Error initializing driver\nDevice connection timeout (warning)\nDevice connection failure (fatal error) – device could be tampered"), wxDefaultPosition, wxSize( 230,70 ), wxTE_MULTILINE|wxTE_READONLY );
	fgSizer711->Add( m_textCtrlStatusKP, 0, wxALL|wxEXPAND, 5 );
	
	fgSizer611->Add( fgSizer711, 1, wxEXPAND, 5 );
	
	m_panelKP->SetSizer( fgSizer611 );
	m_panelKP->Layout();
	fgSizer611->Fit( m_panelKP );
	fgSizer1->Add( m_panelKP, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1325126 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1325126->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel1325126, 1, wxEXPAND | wxALL, 5 );
	
	m_panel13251265 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel13251265->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel13251265, 1, wxEXPAND | wxALL, 5 );
	
	m_panel13251210 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel13251210->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel13251210, 1, wxEXPAND | wxALL, 5 );
	
	m_panel132512101 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132512101->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132512101, 1, wxEXPAND | wxALL, 5 );
	
	m_panel13251211 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel13251211->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel13251211, 1, wxEXPAND | wxALL, 5 );
	
	m_panel132512111 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132512111->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132512111, 1, wxEXPAND | wxALL, 5 );
	
	m_panel132512112 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel132512112->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer1->Add( m_panel132512112, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonRefresh = new wxButton( this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonRefresh->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer1->Add( m_buttonRefresh, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_panel17 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel17->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer1->Add( m_panel17, 1, wxEXPAND | wxALL, 5 );
	
	m_buttonExit = new wxButton( this, wxID_ANY, wxT("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonExit->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer1->Add( m_buttonExit, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	fgSizer1->Add( bSizer1, 1, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	m_statusBar->SetFont( wxFont( 12, 74, 93, 92, false, wxT("Tahoma") ) );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonRefresh->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GSPOTDashboardMainGUI::OnRefresh ), NULL, this );
	m_buttonExit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GSPOTDashboardMainGUI::OnQuit ), NULL, this );
}

GSPOTDashboardMainGUI::~GSPOTDashboardMainGUI()
{
	// Disconnect Events
	m_buttonRefresh->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GSPOTDashboardMainGUI::OnRefresh ), NULL, this );
	m_buttonExit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GSPOTDashboardMainGUI::OnQuit ), NULL, this );
	
}

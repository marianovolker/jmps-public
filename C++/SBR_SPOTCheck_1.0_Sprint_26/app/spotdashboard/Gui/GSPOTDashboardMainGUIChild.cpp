/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSPOTDashboardMainGUIChild.h
 * @author Sebastian Lipchak
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */


#include "GSPOTDashboardMainGUIChild.h"
#include "wx/app.h"
#include "GWxApp.h"




GSPOTDashboardMainGUIChild::GSPOTDashboardMainGUIChild( wxWindow* parent )
:
GSPOTDashboardMainGUI( NULL,wxID_ANY, wxT("SPOTDashboard"),  wxDefaultPosition, wxSize( 900,640 ),  wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxMINIMIZE|wxTAB_TRAVERSAL )
{

}

void GSPOTDashboardMainGUIChild::OnQuit( wxCommandEvent& event )
{
Close(TRUE);
}

void GSPOTDashboardMainGUIChild::OnRefresh(wxCommandEvent& event)
{
	wxAppConsole * pwxApp =  wxApp::GetInstance();
	
	//This cannot be done due we dont compile with RTTI : wxTextCtrl *text = wxDynamicCast(win, wxTextCtrl);
	
	MyApp* pMyApp = static_cast<MyApp*>(pwxApp);
		
	
	pMyApp->Refresh();

}


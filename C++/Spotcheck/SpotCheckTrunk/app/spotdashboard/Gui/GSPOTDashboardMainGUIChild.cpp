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
#include "Common/Util/DEBUGOUT.h"



BEGIN_EVENT_TABLE(GSPOTDashboardMainGUIChild, wxFrame)
EVT_COMMAND  (GConectionThread::CONNECTION_STATUS_EVENT_ID, wxEVT_COMMAND_TEXT_UPDATED, GSPOTDashboardMainGUIChild::OnConnectionStatusUpdate)
END_EVENT_TABLE()



/*
 * @author slipchak
 * wxSTAY_ON_TOP was added to constructor due to an issue in Windows XP, the window was showing minimized or in the background.
 * It resulted that neither the Maximize or tweaking the window dimensions by code not being the solution, so the solution is to make the Frame to stay on top.
 */
GSPOTDashboardMainGUIChild::GSPOTDashboardMainGUIChild( wxWindow* parent )
:
GSPOTDashboardMainGUI( NULL,wxID_ANY, wxT("SPOTDashboard"),  wxDefaultPosition, wxSize( 900,640 ),  wxDEFAULT_FRAME_STYLE|wxMAXIMIZE|wxMINIMIZE|wxTAB_TRAVERSAL| wxSTAY_ON_TOP
)
{
	m_bClosed=false;
}

void GSPOTDashboardMainGUIChild::OnQuit( wxCommandEvent& event )
{
	Close(TRUE);
}

void GSPOTDashboardMainGUIChild::OnRefresh(wxCommandEvent& event)
{
	wxAppConsole * pwxApp =  wxApp::GetInstance();
	
	MyApp* pMyApp = static_cast<MyApp*>(pwxApp);
		
	pMyApp->Refresh();
}

void GSPOTDashboardMainGUIChild::OnConnectionStatusUpdate(wxCommandEvent& evt)
{
	wxAppConsole * pwxApp =  wxApp::GetInstance();

	MyApp* pMyApp = static_cast<MyApp*>(pwxApp);

	if( evt.GetInt() == GConectionThread::CONNECTION_STATUS_OK )
	{
		pMyApp->UpdateStatusConnection(true);
	}
	else
	{
		pMyApp->UpdateStatusConnection(false);
	}
}

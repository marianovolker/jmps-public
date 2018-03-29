/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file MyApp.cpp
 * @author Sebastian Lipchak
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 *
 *
 * KP = keypad
 * CR = card reader
 * DISPLAY = Display ( Main unit (+) security module)
 */

#include "GWxApp.h"
#include "wx/wx.h" 
#include <wx/string.h>
#include <wx/stattext.h>
#include "GSPOTDashboardMainGUIChild.h"
#include <string>
#include "GSystem.h"
#include "Common/Util/DEBUGOUT.h"



void MyApp::SetSpotType(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlSpotType->SetValue(mystring);
}
void MyApp::SetSpotSerial(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlSpotSerial->SetValue(mystring);
}


void MyApp::SetFirmwareKP(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlFirmwareKP->SetValue(mystring);

}

void MyApp::SetFirmwareCR(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlFirmwareCR->SetValue(mystring);
}

void MyApp::SetFirmwareDY(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlFirmwareDISPLAY->SetValue(mystring);

}

void MyApp::SetSerialKP(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlSerialKP->SetValue(mystring);

}

void MyApp::SetSerialCR(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlSerialCR->SetValue(mystring);

}

void MyApp::SetSerialDY(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlSerialDISPLAY->SetValue(mystring);

}

void MyApp::SetStatusKP(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlStatusKP->SetValue(mystring);

}

void MyApp::SetStatusCR(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlStatusCR->SetValue(mystring);

}

void MyApp::SetStatusDY(std::string strValue)
{
	wxString mystring(strValue.c_str(), wxConvUTF8);
	frame->m_textCtrlStatusDISPLAY->SetValue(mystring);

}

void MyApp::SetTextCommKP(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextCommKP);
}

void MyApp::SetTextCommCR(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextCommCR);
}

void MyApp::SetTextCommDY(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextCommDISPLAY);
}

void MyApp::SetTextDismountKP(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextDismountKP);
}

void MyApp::SetTextDismountCR(bool bValue)
{
		SetTextError(bValue, frame->m_staticTextDismountCR);
}

void MyApp::SetTextDismountDY(bool bValue)
{

	SetTextError(bValue, frame->m_staticTextDismountDISPLAY);
}

void MyApp::SetTextTamperedKP(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextTamperedKP);
}

void MyApp::SetTextTamperedCR(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextTamperedCR);
}

void MyApp::SetTextTamperedDY(bool bValue)
{
	SetTextError(bValue, frame->m_staticTextTamperedDISPLAY);
}

// @author slipchak
// change text label with corresponding text and colors, acording to the status reported.
// @param bValue true = YES ( no error); false = NO (error)
// @param widget the label to set green or red and put text with "YES" or "NO". must be a static text component or child of it.
void MyApp::SetTextError( bool bValue, wxStaticText* widget)
{


	if(bValue) //YES = OK ( NO ERROR)
	{
		widget->SetForegroundColour( wxColour( 255, 255, 255 ) ); //white
		widget->SetBackgroundColour( wxColour( 0, 128, 0 ) ); //green
		wxString mystring("YES", wxConvUTF8);
		widget->SetLabel(mystring);
	}
	else //NO = ERROR
	{
		widget->SetForegroundColour( wxColour( 255, 255, 255 ) );// black
		widget->SetBackgroundColour( wxColour( 255, 0, 0 ) ); //red
		wxString mystring("NO", wxConvUTF8);
	    widget->SetLabel(mystring);
	}
	widget->SetSize(230 ,20 );
}

// @author slipchak
//This method calls the Controller to re connect, get status from SPOT and re update data in GUI.
void MyApp::Refresh()
{


	//ask controller to disconnect
	//ask controller to connect
	//ask controller to get hard information and soft info
	//ask controller to get sys status info

	//gray GUI comp

	frame->m_panelDisplay->Enable( false );
	frame->m_panelKP->Enable( false );
	frame->m_panelCR->Enable( false );
	frame->m_buttonRefresh->Enable(false);

	frame->SetStatusText( _("RE - CONNECTING....."), 1 );

	if (controller->DisconnectAndConnect())
	{

		//call to update all data in all components.
		RefreshDataOnComponents();

		frame->SetStatusText(_("DATA OK"), 1);

		//////////////
		//update Timestamp in status on success case

		UpdateTimeStampOnStatusBar(true);

	}
	else
	{

		frame->SetStatusText(_("OLD DATA !!"), 1);
		//////////////
		//update Timestamp in status on success case

		UpdateTimeStampOnStatusBar(false);

	}


	frame->m_panelDisplay->Enable( true );
	frame->m_panelKP->Enable( true );
	frame->m_panelCR->Enable( true );
	frame->m_buttonRefresh->Enable(true);

	frame->Refresh(true, NULL);
	frame->Update();
}

// @author slipchak
//This method updates data in GUI components
void MyApp::RefreshDataOnComponents()
{

	GrayKPIfCommError(false);
	GrayCRIfCommError(false);
	GrayDisplayIfCommError(false);

	//update GUI with all data.
	SetSpotType(controller->UpdateSpotType());
	SetSpotSerial(controller->UpdateSpotSerial());

	SetFirmwareKP(controller->UpdateFirmwareKP());
	SetFirmwareCR(controller->UpdateFirmwareCR());
	SetFirmwareDY(controller->UpdateFirmwareDY());

	SetSerialKP(controller->UpdateSerialKP());
	SetSerialCR(controller->UpdateSerialCR());
	SetSerialDY(controller->UpdateSerialDY());


	//// KEYPAD
	//true= Ok or false=error
	bool bFlag = controller->UpdateCommKP();
	SetTextCommKP(bFlag);

	SetTextDismountKP(controller->UpdateDismountKP());
	SetTextTamperedKP(controller->UpdateTamperKP());
	SetStatusKP(controller->UpdateOtherStatusKP());
	//gray or disable components
	GrayKPIfCommError(bFlag);

	//// CARD READER

	//true= Ok or false=error
	bFlag = controller->UpdateCommCR();
	SetTextCommCR(bFlag);
	SetTextDismountCR(controller->UpdateDismountCR());

	SetTextTamperedCR(controller->UpdateTamperCR());
	SetStatusCR(controller->UpdateOtherStatusCR());

	//gray or disable components
	GrayCRIfCommError(bFlag);

	//// DISPLAY
	//true= Ok or false=error
	bFlag = controller->UpdateCommDY();
	SetTextCommDY(bFlag);
	SetTextDismountDY(controller->UpdateDismountDY());
	SetTextTamperedDY(controller->UpdateTamperDY());
	SetStatusDY(controller->UpdateOtherStatusDY());

	//gray or disable components
	GrayDisplayIfCommError(bFlag);

}

// make the wxWidget application to not use a main in wx library just use the spotdashboard main routine
// if this is not used it will conflict with the spotdashboard application main. ( it cannot be 2 mains)
IMPLEMENT_APP_NO_MAIN(MyApp)

// @author slipchak
//include Timestamp in status to have somthing like "Time stamp : ? ? ? . Copyright 2013 Gilbarco Inc."
//the format is equal to log file timestamp so the user can search for debugging or issue investigation
void MyApp::UpdateTimeStampOnStatusBar(bool bSuccess)
{
	std::string strMessage;
	std::string strTimeStamp;

	//////////////
	char mst_arycTimeStamp[30] = {};
	memset(mst_arycTimeStamp, 0, sizeof(mst_arycTimeStamp));
	GLogger::String_timestamp(mst_arycTimeStamp, sizeof(mst_arycTimeStamp));
	strTimeStamp = mst_arycTimeStamp;

	if (bSuccess)
	{
		strMessage = "Time stamp :" + strTimeStamp + ". Copyright 2013 Gilbarco Inc.";
	}
	else
	{
		strMessage = "Error :" + strTimeStamp + ". Copyright 2013 Gilbarco Inc.";
	}

	wxString mystring(strMessage.c_str(), wxConvUTF8);
	frame->SetStatusText(mystring, 0);
}


// @author slipchak
// Init method called by the main routine
bool MyApp::OnInit()
{

	wxInitAllImageHandlers();

	frame = new GSPOTDashboardMainGUIChild(NULL);

	RefreshDataOnComponents();

	//set status bar parameters to set the 2 areas in it.
	const int iaryWidth[] =
	{
	-1, 200, -1, 200
	};
	frame->m_statusBar->SetFieldsCount(2, iaryWidth);

	//////////////
	//include Timestamp in status to have somthing like "Time stamp : ? ? ? hs mm ss. Copyright 2013 Gilbarco Inc."

	UpdateTimeStampOnStatusBar(true);
	////////////////

	frame->Show(true);
	SetTopWindow(frame);

	return true;
}

// @author slipchak
// sets the single and only controller by now for minimal MVC pattern by now.
void MyApp::SetController(GSPOTDashboardController* controller)
{
	this->controller = controller;
}

// @author slipchak
// @param bValue indicates if there was an error communicating with the device
// Gray out the components on the row taking into account the bValue
void MyApp::GrayDisplayIfCommError(bool bValue)
{

	if (bValue)
	{
		frame->m_textCtrlStatusDISPLAY->SetBackgroundColour(wxColour(255, 255, 255)); //white
	}
	else
	{
		frame->m_staticTextDismountDISPLAY->SetForegroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedDISPLAY->SetForegroundColour(wxColour(191, 191, 191)); //gray light

		frame->m_staticTextDismountDISPLAY->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedDISPLAY->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_textCtrlStatusDISPLAY->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
	}
}

// @author slipchak
// @param bValue indicates if there was an error communicating with the device
// Gray out the components on the row taking into account the bValue
void MyApp::GrayCRIfCommError(bool bValue)
{

	if (bValue)
	{
		frame->m_textCtrlStatusCR->SetBackgroundColour(wxColour(255, 255, 255)); //white
	}
	else
	{

		frame->m_staticTextDismountCR->SetForegroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedCR->SetForegroundColour(wxColour(191, 191, 191)); //gray light

		frame->m_staticTextDismountCR->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedCR->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_textCtrlStatusCR->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
	}

}

// @author slipchak
// @param bValue indicates if there was an error communicating with the device
// Gray out the components on the row taking into account the bValue
void MyApp::GrayKPIfCommError(bool bValue)
{

	if (bValue)
	{
		frame->m_textCtrlStatusKP->SetBackgroundColour(wxColour(255, 255, 255)); //white
	}
	else
	{
		frame->m_staticTextDismountKP->SetForegroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedKP->SetForegroundColour(wxColour(191, 191, 191)); //gray light

		frame->m_staticTextDismountKP->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_staticTextTamperedKP->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
		frame->m_textCtrlStatusKP->SetBackgroundColour(wxColour(191, 191, 191)); //gray light
	}

}

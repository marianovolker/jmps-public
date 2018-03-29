/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file MyApp.cpp
 * @author Sebastian Lipchak
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GWXAPP_H_
#define GWXAPP_H_


#include "wx/wx.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include "GSPOTDashboardMainGUIChild.h"
#include "GSPOTDashboardController.h"
#include <string>


class MyApp: public wxApp
{
	public:
		virtual bool OnInit();
		virtual int  OnExit();
		void SetSpotType(std::string bValue);
		void SetSpotSerial(std::string strValue);
		void SetFirmwareKP(std::string strValue);
		void SetFirmwareCR(std::string strValue);
		void SetFirmwareDY(std::string strValue);
		void SetSerialKP(std::string strValue);
		void SetSerialCR(std::string strValue);
		void SetSerialDY(std::string strValue);
		void SetStatusKP(std::string strValue);
		void SetStatusCR(std::string strValue);
		void SetStatusDY(std::string strValue);
		void SetTextCommKP(bool bValue);
		void SetTextCommCR(bool bValue);
		void SetTextCommDY(bool bValue);
		void SetTextDismountKP(bool bValue);
		void SetTextDismountCR(bool bValue);
		void SetTextDismountDY(bool bValue);
		void SetTextTamperedKP(bool bValue);
		void SetTextTamperedCR(bool bValue);
		void SetTextTamperedDY(bool bValue);
		//New Other Errors semaphores
		void SetOtherErrorsKP(bool bValue);
		void SetOtherErrorsCR(bool bValue);
		void SetOtherErrorsDY(bool bValue);
		void SetController(GSPOTDashboardController* controller);
		void RefreshDataOnComponents();
		void Refresh();
		void GrayDisplayIfCommError(bool bValue);
		void GrayCRIfCommError(bool bValue);
		void GrayKPIfCommError(bool bValue);
		bool DisconnectAndConnect();
		void UpdateStatusConnection(bool bSuccess=true);


	private:
		// change text with corresponding text and colors.
		void SetTextError( bool bValue, wxStaticText* widget);
		//include Timestamp in status to have somthing like "Time stamp : ? ? ? hs mm ss. Copyright 2013 Gilbarco Inc."
		void UpdateTimeStampOnStatusBar(bool bSuccess);

		GSPOTDashboardMainGUIChild* frame;
		GSPOTDashboardController*	controller;


};


#endif /* GWXAPP_H_ */

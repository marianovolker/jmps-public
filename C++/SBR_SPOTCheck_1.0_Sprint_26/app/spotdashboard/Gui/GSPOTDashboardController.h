/*
 * GSPOTDashboardController.h
 *
 *  Created on: 26/09/2013
 *      Author: slipchak
 */


#include <string>

#ifndef GSPOTDASHBOARDCONTROLLER_H_
#define GSPOTDASHBOARDCONTROLLER_H_

class GSPOTDashboardController
{
	public:
		GSPOTDashboardController();
		virtual ~GSPOTDashboardController();

		void startWxApp(int ac, char **av);
		bool DisconnectAndConnect();

		//return true= Ok or false=error
		bool        UpdateCommKP();
		bool        UpdateDismountKP();
		bool        UpdateTamperKP();
		bool        UpdateCommCR();
		bool        UpdateDismountCR();
		bool        UpdateTamperCR();
		bool        UpdateCommDY();
		bool        UpdateDismountDY();
		bool        UpdateTamperDY();
		std::string UpdateOtherStatusCR();
		std::string UpdateOtherStatusDY();
		std::string UpdateOtherStatusKP();

		std::string UpdateSpotType();
		std::string UpdateSpotSerial();
		std::string UpdateFirmwareKP();
		std::string UpdateFirmwareCR();
		std::string UpdateFirmwareDY();
		std::string UpdateSerialKP();
		std::string UpdateSerialCR();
		std::string UpdateSerialDY();

};

#endif /* GSPOTDASHBOARDCONTROLLER_H_ */

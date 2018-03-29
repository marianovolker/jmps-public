/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSPOTDashboardController.h
 * @author Sebastian Lipchak
 * @date Sep 26 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
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

		//return true= Ok or false=error
		bool        UpdateCommKP();
		bool        UpdateDismountKP();
		bool        UpdateTamperKP();
		bool        UpdateOtherErrorsKP();
		bool        UpdateCommCR();
		bool        UpdateDismountCR();
		bool        UpdateTamperCR();
		bool        UpdateOtherErrorsCR();
		bool        UpdateCommDY();
		bool        UpdateDismountDY();
		bool        UpdateTamperDY();
		bool        UpdateOtherErrorsDY();
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

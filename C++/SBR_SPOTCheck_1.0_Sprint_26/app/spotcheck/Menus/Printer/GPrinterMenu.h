/*!
 * GPrinterMenu.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GPrinterMenu.h
 * @author Guillermo Paris
 * @date Abr 19 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GPrinterMenu_H
#define GPrinterMenu_H 1

#include "Menus/GAbsMenu.h"

class GPrinterManager;

class GPrinterMenu : public GAbsMenu
{
	public:
		             GPrinterMenu(void);
		virtual      ~GPrinterMenu(void);
		void         Initialize();
		void         NotifyPrintResult(const char* pcResult);
		void         NotifyPrinterStatus(const char* pcStatus);

		virtual bool DrawMenu();
		virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);

		static  int  GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

		static const char*  mst_pcPassed;
		static const char*  mst_pcFail;

	private:
		virtual void HandleSoftKeyEvent(UINT8 KeyCode);

		GPrinterManager *   m_pPRManager;
};

#endif // GGPrinterMenu_H


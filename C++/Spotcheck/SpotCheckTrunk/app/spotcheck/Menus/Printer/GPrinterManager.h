/*!
 * GPrinterManager.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GPrinterManager.h
 * @author Guillermo Paris
 * @date Abr 19 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include "Common/Util/GSimpleCommand.h"
#include "Common/Util/GTimer.h"

#ifndef GPRINTERMANAGER_H_
#define GPRINTERMANAGER_H_

const int PR_TOUT_INITIAL_DELAY   =   50; // 50 mSec

class GPrinterMenu;
class GPrinterService;

class GPrinterManager
{
	public:

		enum eStatus
		{
			eInitial,  //show initial window
		};

		GPrinterManager(GPrinterMenu*);
		~GPrinterManager();

		void              Initialize();
		void              Dispose();
		void              PrintHardwareReportRequest();
		void              PrintTestResultsReportRequest();

		inline eStatus    GetStatus() { return m_eStatus; }
		bool              ChangeStateTo(eStatus est);
		void              NotifyPrinterStatus(const char* pcStatus);

	private:
		inline void       SetStatus(eStatus WindowStatus) { m_eStatus = WindowStatus; }
		void              TemporizedPrintReport();

		int               m_iReportType;
		eStatus           m_eStatus;
		GPrinterMenu*     m_pPRMenu;
		GPrinterService*  m_pPRService;
		GTimer*           m_pTimer;
		GSimpleCommand<GPrinterManager> m_TOutCommand; // command to process timeout event
};


#endif /* GPRINTERMANAGER_H_ */

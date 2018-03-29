/*!
 * GPrinterManager.cpp
 *
 *  Created on: 18/04/2013
 *      Author: Guillermo Paris
 */

#include "Reports/TestInfo/GTestInfoReport.h"
#include "GPrinterManager.h"
#include "GPrinterMenu.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Services/Printer/GPrinterService.h"
#include "Reports/HardwareInfo/GHWInfoReport.h"

GPrinterManager::GPrinterManager(GPrinterMenu* pMenu)
	: m_iReportType(GReportHeader::eHardwareReport),
	  m_eStatus(eInitial),
	  m_pPRMenu(pMenu),
	  m_TOutCommand(this, &GPrinterManager::TemporizedPrintReport)
{
	m_pPRService = GPrinterService::GetService();

	// this is a ONE SHOT timer / thread
	m_pTimer = new GTimer(&m_TOutCommand, PR_TOUT_INITIAL_DELAY, true, false);
}

GPrinterManager::~GPrinterManager()
{
}


// called after construction
void GPrinterManager::Initialize()
{
	m_pPRService->AttachManager(this);
}

// called before destruction
void GPrinterManager::Dispose()
{
	m_pPRService->DetachManager();
}


void GPrinterManager::NotifyPrinterStatus(const char* pcStatus)
{
	if( pcStatus && m_pPRMenu )
		m_pPRMenu->NotifyPrinterStatus(pcStatus);
}


void GPrinterManager::PrintHardwareReportRequest()
{
	m_iReportType = GReportHeader::eHardwareReport;

	m_pTimer->Start(); // start the delayed print report
	return; // release the session listener thread (soft-key notification)
}

void GPrinterManager::PrintTestResultsReportRequest()
{
	m_iReportType = GReportHeader::eTestResultsReport;

	m_pTimer->Start(); // start the delayed print report
	return; // release the session listener thread (soft-key notification)
}

/*!
 * \brief This method runs in the timer's thread to avoid retaining the soft-key
 * event thread (SPOT answer thread owned by session's queue). Moreover, the
 * call to m_pPRService->PrintText() is blocking because it waits for a
 * sequence of several SPOT answers before returning. So, this call to PrintText()
 * can't be called from a SPOT answer thread like soft-key notification routine.
 *
 * \note For all previously exposed, this method MUST NOT BE CALLED from a SPOT
 * ANSWER queue thread.
 */
void GPrinterManager::TemporizedPrintReport()
{
	bool               blret = true;
	int                iDumpSize = 0;
	char*              pcText = NULL;
	const char*        pcMethod = "GPrinterManager::TemporizedPrintReport() - ";
	GAbsPrinterReport* pReport = NULL;

	if(m_iReportType == GReportHeader::eHardwareReport)
	{
		pReport = new GHWInfoReport();
	}
	else if(m_iReportType == GReportHeader::eTestResultsReport)
	{
		blret = GCardReaderService::GetService()->RequestStatisticsToSpot();

		if(blret)
			pReport = new GTestInfoReport();
	}
	else // not valid report type code
	{
		LOG(ePrinter, eWarning,
			"Wrong report type %d. Printing aborted.\n", m_iReportType);
		blret = false;
	}

	if(blret)
	{
		pReport->OpenReport();
		iDumpSize = pReport->TextDump();
		pReport->CloseReport();

		UINT32 u32RepoLen = pReport->GetReportLength();
		LOG(ePrinter, eDebug, "%sReport:\ndump=%d, length=%d, text=%s\n",
			pcMethod, iDumpSize, u32RepoLen, pReport->GetReportText());

		if( ! m_pPRService->PrintText(u32RepoLen, (const char*)pReport->GetReportText()) )
			blret = false;
	}

	const char* pcResult = (blret ? GPrinterMenu::mst_pcPassed : GPrinterMenu::mst_pcFail);

	m_pPRMenu->NotifyPrintResult(pcResult); // this enables the soft-keys again
	delete pReport;
}

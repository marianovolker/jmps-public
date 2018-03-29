/*
 * GFeatureConnectorMenu.h
 *
 *  Created on: Feb 9, 2015
 *      Author: Sayyed Mohammad
 */

#ifndef GFEATURECONNECTORMENU_H_
#define GFEATURECONNECTORMENU_H_

#include "Menus/GAbsMenu.h"
#include "Menus/GMenuManager.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "Menus/FeatureConnector/GFeatureConnectorStatistics.h"

using namespace GSpotDefs;

const int FC_TOUT_FEATURECONNECTOR_DELAY = 3000;

class GFeatureConnectorMenu: public GAbsMenu
{
	enum eEvents
	{
		eEventSoftKeyL1 = 0,
		eEventSoftKeyL2    ,
		eEventSoftKeyL3    ,
		eEventSoftKeyL4    ,

		eEventSoftKeyR1    ,
		eEventSoftKeyR2    ,
		eEventSoftKeyR3    ,
		eEventSoftKeyR4    ,

		eEventTimeout      ,
		eEventError        ,

		eLastEvent = (eEventError+1)
	};

	enum eFCStatus
	{
		eFCStatusInitial,
		eFCStatusMedia,
		eFCStatusCheckTest,
		eFCStatusSummary,

		eLastStatus = (eFCStatusSummary+1)
	};

public:





                    GFeatureConnectorMenu(void);
	virtual         ~GFeatureConnectorMenu(void);

	virtual bool     DrawMenu( bool blDisablePost = false );

	static  int      GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);
	void             ProcessTimeout();
	bool             DrawExternalMediaTestMenu(std::string text);
	bool             DrawInternalNonSecureTestMenu(std::string text);
	bool             DrawTestResultMenu();
	void             ClearScreen();



private:
	GTimer*          m_pTimer;
	eFCStatus        m_eFCStatus;

	GSimpleCommand<GFeatureConnectorMenu> m_TOutCommand;
	static SPOT_PP_DY_SECUREMEDIACONNECTOR_MODES m_eDisplayMode;
	virtual void     HandleSoftKeyEvent(UINT8 KeyCode);
			bool     SwitchDisplay(UINT8 u8Type);
			bool     HandleSPOTMessage(GSPOTMessage* pMsg);
			void     ProcessEventsReceived(eEvents);
			void     NextState(eEvents e, eFCStatus s);
};


#endif /* GFEATURECONNECTORMENU_H_ */

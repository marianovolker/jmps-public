/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GAbsMenu_H
#define GAbsMenu_H 1

#include <string>
#include <stdio.h>

#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"
#include "Menus/Templates/GSoftKeysWindowTemplate.h"

class GMenuManager;

class GAbsMenu
{
	public:

		enum eAuxKeyCodes
		{
			eP1 = 0x01, eP2 = 0x02, eP3 = 0x03, eP4 = 0x04, eP5 = 0x05,
			eP6 = 0x06, eP7 = 0x07, eP8 = 0x08, eP9 = 0x09, eP10 = 0x0A
		};

		enum eSoftKeyCodes
		{
			eL1 = 0x10, eL2 = 0x11, eL3 = 0x12, eL4 = 0x13,
			eR1 = 0x14, eR2 = 0x15, eR3 = 0x16, eR4 = 0x17
		};

		virtual             ~GAbsMenu();

		inline virtual bool DrawScreen();
		virtual bool        DrawMenu( bool blDisablePost = false );
		virtual bool        DrawTimer();
		virtual bool        HandleSPOTMessage(GSPOTMessage* pMsg) = 0;
		virtual void        HandleSoftKeyEvent(UINT8 KeyCode) = 0;
		virtual bool        HandleInactiveTimeEvent(UINT32 tLeft);


		const static int    CiMaxUserLabels;
		void                SetTitleLabel(const char* pcLabel);
		void                UpdateUserLabels(int nLabels, const char* arypcLabels[]);
		void                SetLabel(GSoftKeysWindowTemplate::eTAG_ID eTagId, const char * szLabel);
		void                ResetTimerLabel();
		bool                EnableSoftKeys(bool blEnableAuxKeysToo = false);

	protected:
		                    GAbsMenu();
		virtual void        PostDrawingAction();

		inline  bool        isInitialDrawing()         { return m_blInitialDrawing; }
		inline  void        SetInitialDrawing(bool bl) { m_blInitialDrawing = bl;   }

		int                                      m_iWinID;
		GSoftKeysWindowTemplate::tStringTagMap   m_MenuLabelsMap;
		GSoftKeysWindowTemplate*                 m_pWinTemplate;
		GMenuManager*                            m_pManager;


	private:

		bool                                     m_blInitialDrawing;
};

inline bool GAbsMenu::DrawScreen()
{
	return DrawMenu(true); // don't do a post-drawing action as in DrawMenu()
}

#endif

/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GDisplayMenu_H
#define GDisplayMenu_H 1

#include "Menus/GAbsMenu.h"
#include "GDisplayManager.h"

class GDisplayManager;

class GDisplayMenu : public GAbsMenu
{
	public:
		                     GDisplayMenu(void);
		        virtual      ~GDisplayMenu(void);
		                void Initialize();

		        virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);

		                void ClearScreen();
		                bool DrawMainMenu();
		                bool DrawCheckTestMenu(std::string text);
		                bool DrawTestResultMenu();
		                bool DrawErrorMenu(std::string text);
		                bool DrawTimeoutMenu(std::string text);
		                void Quit();

		inline        UINT16 GetWidth()       {return m_u16Width;}
		inline        UINT16 GetHeight()      {return m_u16Height;}
		inline GSPOTMessage* GetHideMessage() {return m_pWinTemplate->GetHideMessage();}
		inline GSPOTMessage* GetShowMessage() {return m_pWinTemplate->GetShowMessage();}

		static           int GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);


	private:
		        virtual void HandleSoftKeyEvent(UINT8 KeyCode);

		              UINT16 m_u16Width;
		              UINT16 m_u16Height;
		   GDisplayManager * m_pDYManager;
};

#endif


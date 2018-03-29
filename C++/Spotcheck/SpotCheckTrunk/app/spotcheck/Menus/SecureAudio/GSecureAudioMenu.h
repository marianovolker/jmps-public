/*
 * ©2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

#ifndef GSecureAudioMenu_H
#define GSecureAudioMenu_H 1

#include "Menus/GAbsMenu.h"
#include "GSecureAudioManager.h"

class GDisplayManager;

class GSecureAudioMenu : public GAbsMenu
{
	public:
		                     GSecureAudioMenu(void);
		        virtual      ~GSecureAudioMenu(void);
		                void Initialize();

		        virtual bool HandleSPOTMessage(GSPOTMessage* pMsg);

		                void ClearScreen();
		                bool DrawMainMenu();
		                bool DrawNonSecureCheckTestMenu(std::string text);
		//                bool DrawNonSecureTestMenu(std::string text);
		                bool DrawSecureTestMenu(std::string text);
		                bool DrawSecureCheckTestMenu(std::string text);
		                bool DrawTestResultMenu(std::string text5,std::string text6);
		                bool DrawErrorMenu(std::string text);
		                bool DrawTimeoutMenu(std::string text);
		                void Quit();
		                void PushMenu(GAbsMenu* pMenu);

		inline        UINT16 GetWidth()       {return m_u16Width;}
		inline        UINT16 GetHeight()      {return m_u16Height;}
		inline GSPOTMessage* GetHideMessage() {return m_pWinTemplate->GetHideMessage();}
		inline GSPOTMessage* GetShowMessage() {return m_pWinTemplate->GetShowMessage();}

		static           int GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);




	private:
		        virtual void HandleSoftKeyEvent(UINT8 KeyCode);

		              UINT16 m_u16Width;
		              UINT16 m_u16Height;
		   GSecureAudioManager * m_pDYManager;

};

#endif


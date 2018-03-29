#ifndef GBNAMainMenu_H
#define GBNAMainMenu_H 1

#include "Menus/GAbsMenu.h"
#include "Menus/Templates/GSoftKeysWindowTemplate.h"

class GBNAMainMenu : public GAbsMenu
{
   public:
      GBNAMainMenu();
      ~GBNAMainMenu();
      bool DrawMenu();
      bool HandleSPOTMessage(GSPOTMessage* pMsg);

      static int GetTagStrings(GSoftKeysWindowTemplate::tStringTagMap & rMap);

   private:
      void HandleSoftKeyEvent(UINT8 KeyCode);
      void PostDrawingAction();
      void GetBNAFirmwareVersion();
      void EnableBNA();
      void DisableBNA();
      void ConfirmRead();
      void HandleBNAVersion(UINT8* Data, UINT16 DataLen);
      void HandleBNAStatus(UINT8* Data, UINT16 DataLen);
      std::string GetBillName(UINT8 index);
};

#endif

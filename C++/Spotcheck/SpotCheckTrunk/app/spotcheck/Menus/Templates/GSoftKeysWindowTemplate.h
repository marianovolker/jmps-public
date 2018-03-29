#ifndef GSoftKeysWindowTemplate_h
#define GSoftKeysWindowTemplate_h 1

#include "Menus/Templates/GWindowTemplate.h"
#include <map>
#include <string>

class GSoftKeysWindowTemplate : public GWindowTemplate
{
	public:
		enum eTAG_ID
		{
			eCONTAINER,
			eSOFT_L1, eSOFT_L2, eSOFT_L3, eSOFT_L4,
			eSOFT_R1, eSOFT_R2, eSOFT_R3, eSOFT_R4,
			eSEGMENT_1, eSEGMENT_2, eSEGMENT_3, eSEGMENT_4,
			eSEGMENT_5, eSEGMENT_6, eSEGMENT_7, eSEGMENT_8, eSEGMENT_9,
			eTIMER, eTOTAL_TAGS = (eTIMER +1)
		};

		typedef std::map<eTAG_ID,std::string>            tStringTagMap;
		typedef std::map<eTAG_ID,std::string>::iterator  tStringTagMapIterator;

		static GSoftKeysWindowTemplate* GetSoleInstance();
		static void DestroySoleInstance();

		void  ClearLabels();
		void  SetLabels(tStringTagMap& NewMap, bool ClearAllLabels);
		static const char* m_pcTimerInitLabel;
		static const char* m_pcTimerLabelKey;

	private:
		      GSoftKeysWindowTemplate();
		      ~GSoftKeysWindowTemplate();
		void  InitializeTagList();
		void  BuildContainerTag();
		void  BuildTextTag(std::string Text, eTAG_ID TagId);
		void  GetTagPosition(eTAG_ID TagId, UINT16& XOrg, UINT16& YOrg);
		void  GetTagSize(eTAG_ID TagId, UINT16& Width, UINT16& Height);
		UINT8 GetTextAlignment(eTAG_ID TagId);
		void  GetFontColor(eTAG_ID TagId, GTagRGB* RGB);

		static GSoftKeysWindowTemplate* mst_pSoleInstance;

		tStringTagMap                   m_TagMap;
};

#endif

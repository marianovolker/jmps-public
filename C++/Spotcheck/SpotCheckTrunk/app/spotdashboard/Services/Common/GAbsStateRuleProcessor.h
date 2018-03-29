/**
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAbsStateRuleProcessor.h
 * @author Mariano Volker
 * @date Oct 01 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GAbsStateRuleProcessor_h
#define GAbsStateRuleProcessor_h 1

#include <map>
#include <string>
#include <stdexcept>

#include <SPOT/SPOTProtocolInterface/GSPOTDefs.h>

class GAbsStateRuleProcessor
{
	public:

		enum eCodeType
		{
			eAckType = 0, eStatusType , eStatusExType, eErrorType
		};

		typedef std::map<UINT32, std::string> MapType;

		                        GAbsStateRuleProcessor ();
		           virtual      ~GAbsStateRuleProcessor();

		           virtual void ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error) = 0;
		           virtual void ProcessServiceNotification(UINT8 u8Ack) = 0;

		           const char * GetDescription(eCodeType eType, UINT8 u8Code) const;
		           std::string  GetDescriptionList() const;

		           inline bool  IsDismounted()  const { return m_bDismounted; }
		           inline bool  IsTampered	()  const { return m_bTampered;   }
		           inline bool  IsOffline   ()  const { return m_bOffline;    }
		           inline bool  IsOtherError()  const { return m_bOtherError; }

		           inline UINT8 GetStatus	()  const { return m_u8Status;    }
		           inline UINT8 GetStatusEx ()  const { return m_u8StatusEx;  }
		           inline UINT8 GetError	()  const { return m_u8Error;     }
		           inline UINT8 GetAck	 	()  const { return m_u8Ack;       }


	protected:

		           virtual bool AddNewDescription(eCodeType eType, UINT8 u8Code);
		                   bool AddNewDescription(eCodeType eType, UINT8 u8Code, std::string sDescription);

		   inline static UINT32 MakeKey(eCodeType e, UINT8 u8Code)  { return e * 256 + u8Code; }
		   inline static UINT8  GetCodeFromkey(UINT32 u32Key)       { return UINT8(u32Key & 0xFF); }
		            inline void ClearCodes()                        { m_u8Status = m_u8StatusEx = m_u8Error = 0; }
		            inline void ClearFlags()                        { m_bTampered = m_bDismounted = m_bOffline = m_bOtherError = false; }
		            inline void ClearMap()                          { m_descriptionMap.clear(); }
		            inline void Clear()                             { ClearCodes(); ClearFlags(); }

		                   bool m_bDismounted;
		                   bool m_bTampered;
		                   bool m_bOffline;
		                   bool m_bOtherError;
		                  UINT8 m_u8Status;
		                  UINT8 m_u8StatusEx;
		                  UINT8 m_u8Error;
		                  UINT8 m_u8Ack;
		                MapType m_descriptionMap;

};

#endif

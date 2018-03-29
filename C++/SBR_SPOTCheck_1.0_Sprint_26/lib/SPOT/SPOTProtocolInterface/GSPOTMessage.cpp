#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"
#include "GSystem.h" 
#include "Common/Util/GConversion.h"
#include <string.h>
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"

using namespace GSpotDefs;

GSPOTMessage::GSPOTMessage()
  : m_u8AppId(0),
    m_u8SSK(0),
    m_u8CommandCode(0),
    m_u8SubCommandCode(0),
    m_u16DataLen(0),
    m_pData(NULL)
{
}

GSPOTMessage::GSPOTMessage(UINT8 AppId, UINT8 CommandCode, UINT8 SubCommandCode, UINT8* Data, UINT16 DataLen)
  : m_u8AppId(AppId),
    m_u8SSK(0),
    m_u8CommandCode(CommandCode),
    m_u8SubCommandCode(SubCommandCode)
{
	m_pData = NULL;
	m_u16DataLen = 0;

	SetData(Data, DataLen);
}

GSPOTMessage::GSPOTMessage(UINT8* MsgBuffer)
{
	UINT16 offset;

	// the buffer should be the entire buffer from the socket read(s)
	UINT16 msg_size = (MsgBuffer[0] * 0x100) + MsgBuffer[1];
	m_u8AppId = MsgBuffer[2];
	m_u8SSK = MsgBuffer[3];
	m_u8CommandCode = MsgBuffer[4];
	m_pData = NULL;
	m_u16DataLen = 0;
	// SPOT SERVICE CATEGORY doesn't have sub-command codes
	if ((m_u8AppId != eSPOT_SERVICE_CATEGORY) && (m_u8AppId != eSPOT_SERVICE_ANSWER))
	{
		m_u8SubCommandCode = MsgBuffer[5];
		offset = 6;
	}
	else
	{
		offset = 5;
	}

	SetData(&MsgBuffer[offset], (msg_size - offset));
}

GSPOTMessage::GSPOTMessage(GSPOTMessage* pMsg)
  : m_u8AppId(pMsg->GetAppId()),
    m_u8SSK(pMsg->GetSSK()),
    m_u8CommandCode(pMsg->GetCommandCode()),
    m_u8SubCommandCode(pMsg->GetSubCommandCode())
{
	UINT16 msg_size = pMsg->GetMessageLen();
	UINT8* msg_buffer = new UINT8[msg_size];
	UINT16 offset;

	// need to initialize these to be safe
	m_pData = NULL;
	m_u16DataLen = 0;

	pMsg->GetTransmitBuffer(msg_buffer);
	if ((m_u8AppId != eSPOT_SERVICE_CATEGORY) && (m_u8AppId != eSPOT_SERVICE_ANSWER))
	{
		offset = 6;
	}
	else
	{
		offset = 5;
	}

	SetData(&msg_buffer[offset], (msg_size - offset));

	delete[] msg_buffer;
}

GSPOTMessage::~GSPOTMessage()
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}

bool GSPOTMessage::GetData(UINT8* DataBuff, UINT16 SizeOfDataBuff)
{
	if (SizeOfDataBuff >= m_u16DataLen)
	{
		memcpy(static_cast<void*>(DataBuff), static_cast<const void*>(m_pData), m_u16DataLen);
		return true;
	}
	return false;
}

bool GSPOTMessage::SetData(UINT8 AppId, UINT8 CommandCode, UINT8 SubCommandCode, UINT8* Data, UINT16 DataLen)
{
	m_u8AppId = AppId;
	m_u8CommandCode = CommandCode;
	m_u8SubCommandCode = SubCommandCode;

	return SetData(Data, DataLen);
}

bool GSPOTMessage::SetData(UINT8* Data, UINT16 DataLen)
{
   // make sure any old data is cleaned up
   if ( m_pData )
   {
      delete[] m_pData;
      m_pData = NULL;
      m_u16DataLen = 0;
   }

   return Append(Data, DataLen);
}

bool GSPOTMessage::Append(UINT8* NewData, UINT16 NewDataLen)
{
	if ((NewDataLen == 0) || (!NewData))
	{
		return false;
	}

	// only need to copy the old data in if the pointers not null
	if (m_pData)
	{
		UINT8* tmp = m_pData;

		m_pData = new UINT8[m_u16DataLen + NewDataLen];

		memcpy(static_cast<void*>(m_pData), static_cast<const void*>(tmp), m_u16DataLen);

		delete[] tmp;
	}
	else
	{
		// in this case there was no data, so just use the new data len to be safe
		m_pData = new UINT8[NewDataLen];
		// just a safeguard in case the data length is not 0 when data ptr is null
		m_u16DataLen = 0;
	}

	// copy in the new data and update the data length member var
	memcpy(static_cast<void*>(&m_pData[m_u16DataLen]), static_cast<const void*>(NewData), NewDataLen);
	m_u16DataLen += NewDataLen;

	return true;
}

bool GSPOTMessage::PeekByte(UINT uOffset, UINT8 & u8Byte)
{
	if(m_pData == NULL || uOffset >= m_u16DataLen)
		return false; // out of boundary

	u8Byte = m_pData[uOffset];
	return true;
}

bool GSPOTMessage::PokeByte(UINT uOffset, UINT8 u8Byte)
{
	if(m_pData == NULL || uOffset >= m_u16DataLen)
		return false; // out of boundary

	m_pData[uOffset] = u8Byte;
	return true;
}

bool GSPOTMessage::AppendByte(UINT8 Byte)
{
   return Append(&Byte, 1);
}

bool GSPOTMessage::AppendWord(UINT16 u16Word)
{
   UINT16 u16Conv = GConversion::HostToBigEndian(u16Word);
   return Append(reinterpret_cast<UINT8*>(&u16Conv), sizeof(u16Conv));
}

bool GSPOTMessage::AppendDWord(UINT32 u32DWord)
{
	UINT32 u32Conv = GConversion::HostToBigEndian(u32DWord);
	return Append(reinterpret_cast<UINT8*>(&u32Conv), sizeof(u32Conv));
}

UINT16 GSPOTMessage::GetTransmitBuffer(UINT8* Dst)
{
   UINT16 offset = ((m_u8AppId != eSPOT_SERVICE_CATEGORY) && (m_u8AppId != eSPOT_SERVICE_ANSWER)) ? 6 : 5;
   UINT16 total_len = m_u16DataLen + offset; // data length + sizes of appid, ssk, command code, sub-command code

   Dst[0] = (total_len >> 8) & 0x00FF;
   Dst[1] = total_len & 0x00FF;
   Dst[2] = m_u8AppId;
   Dst[3] = m_u8SSK;
   Dst[4] = m_u8CommandCode;
   if( (m_u8AppId != eSPOT_SERVICE_CATEGORY) && (m_u8AppId != eSPOT_SERVICE_ANSWER) )
   {
      Dst[5] = m_u8SubCommandCode;
   }
   
   if (m_pData)
   {
      memcpy(static_cast<void*>(&Dst[offset]), static_cast<const void*>(m_pData), m_u16DataLen);
   }

   return total_len;
}

UINT16 GSPOTMessage::GetMessageLen()
{
   UINT8 header_len = ((m_u8AppId != eSPOT_SERVICE_CATEGORY) && (m_u8AppId != eSPOT_SERVICE_ANSWER)) ? 6 : 5;
   return (m_u16DataLen + header_len);
}

bool GSPOTMessage::AppendShort(INT16 i16)
{
  INT16 i16Conv = (INT16)GConversion::HostToBigEndian((UINT16)i16);
  return Append(reinterpret_cast<UINT8*>(&i16Conv), sizeof(i16Conv));
}

bool  GSPOTMessage::AppendInt(INT32 i32)
{
  INT32 i32Conv = (INT32)GConversion::HostToBigEndian((UINT32)i32);
  return Append(reinterpret_cast<UINT8*>(&i32Conv), sizeof(i32Conv));
}

bool  GSPOTMessage::AppendString(char *str)
{
  UINT32 u32Len = strlen(str);
  return Append(reinterpret_cast<UINT8*>(str), u32Len);
}




#ifndef GSPOTMsg_h
#define GSPOTMsg_h 1

#include <GTypesImpl.h>

class GSPOTMessage
{
  public:
    GSPOTMessage();
    GSPOTMessage(UINT8 AppId, UINT8 CommandCode, UINT8 SubCommandCode = 0, UINT8* Data = 0, UINT16 DataLen = 0);
    GSPOTMessage(UINT8* MsgBuffer);
    GSPOTMessage(GSPOTMessage* pMsg);
    ~GSPOTMessage();

    bool   SetData(UINT8 AppId, UINT8 CommandCode, UINT8 SubCommandCode = 0, UINT8* Data = 0, UINT16 DataLen = 0);
    bool   SetData(UINT8* Data, UINT16 DataLen);
    bool   Append(UINT8* NewData, UINT16 DataLen);

    bool   AppendByte(UINT8 Byte);
    bool   AppendWord(UINT16 Word);
    bool   AppendDWord(UINT32 u32DWord);
    bool   AppendShort(INT16 i16);
    bool   AppendInt(INT32 i32);
    bool   AppendString(char *str);
    bool   PeekByte(UINT offset, UINT8 & Byte);
    bool   PokeByte(UINT offset, UINT8 Byte);

    UINT16 GetTransmitBuffer(UINT8* Dst);
    UINT16 GetMessageLen();
    UINT16 GetDataLen() { return m_u16DataLen; };
    UINT8* GetData(){ return m_pData; };
    bool   GetData(UINT8* DataBuff, UINT16 SizeOfDataBuff);
    UINT8  GetAppId() { return m_u8AppId; };
    UINT8  GetSSK() { return m_u8SSK; };
    UINT8  GetCommandCode() { return m_u8CommandCode; };
    UINT8  GetSubCommandCode() { return m_u8SubCommandCode; };


  private:
    UINT8  m_u8AppId;
    UINT8  m_u8SSK;
    UINT8  m_u8CommandCode;
    UINT8  m_u8SubCommandCode;
    UINT8 *m_pData;
    UINT16 m_u16DataLen;
};
#endif

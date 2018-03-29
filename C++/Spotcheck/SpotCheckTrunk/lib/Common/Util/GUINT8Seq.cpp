#include <string.h>
#include "Common/Util/GUINT8Seq.h"
#include "Common/Util/DEBUGOUT.h"

GUINT8Seq::GUINT8Seq (void)
	:	m_pBuffer(NULL),
		m_u32Size(0),
		m_boolRelease(false)
{
}
GUINT8Seq::GUINT8Seq (UINT32 size)
	:	m_pBuffer(NULL),
		m_u32Size(0),
		m_boolRelease(true)
{
	Length(size);
}
GUINT8Seq::GUINT8Seq (UINT32 size, UINT8 * data, bool release)
	:	m_pBuffer(data),
		m_u32Size(size),
		m_boolRelease(release)
{
}
GUINT8Seq::GUINT8Seq (UINT32 maxSize, UINT32 currentSize, UINT8 * data, bool release)
	:	m_pBuffer(data),
		m_u32Size(currentSize),
		m_boolRelease(release)
{
}
GUINT8Seq::GUINT8Seq (GUINT8Seq const & aMessage)
	:	m_pBuffer(NULL),
		m_u32Size(0),
		m_boolRelease(true)
{
	Length(aMessage.Length());
    memcpy(m_pBuffer, aMessage.DataNP(), m_u32Size);
}
GUINT8Seq::~GUINT8Seq (void)
{
	if ((m_boolRelease) && (m_pBuffer != NULL))
	{
		delete[] m_pBuffer;
	}
	m_pBuffer = NULL;
	m_u32Size = 0;
}
UINT8 * GUINT8Seq::DataNP (void) const
{
    return m_pBuffer;
}
UINT8 * GUINT8Seq::GetBuffer (void) const
{
    return m_pBuffer;
}
UINT8 & GUINT8Seq::operator [] (int index) const
{
    if ((UINT32) index > (m_u32Size - 1))
    {
        LOG(eCommon, eError, "GUINT8Seq::operator [] - index=%d, size=%ul\n",index,m_u32Size);
        return m_pBuffer[0];
    }
    else
    {
        return m_pBuffer[index];
    }
}
UINT32 GUINT8Seq::Length (void) const
{
    return m_u32Size;
}
void GUINT8Seq::Length (UINT32 size)
{
    // Recreate a buffer if the size is different, or if we didn't already own the buffer.
    if (( size != m_u32Size ) || ( (size == m_u32Size) && !m_boolRelease ))
    {
        // Algorithm matches delete[] conditions in destructor.
        if ((m_boolRelease) && (m_pBuffer != NULL))
        {
            delete[] m_pBuffer;
        }

        m_pBuffer = new UINT8[size + 1];
        m_u32Size = size;
    }

    if (m_pBuffer != NULL)
    {
    	memset(m_pBuffer, 0, m_u32Size + 1);
    }
    m_boolRelease = true;
}

#ifndef GUINT8Seq_h
#define GUINT8Seq_h 1

#include <GTypes.h>

class GUINT8Seq
{
public:
	GUINT8Seq (void);
	GUINT8Seq (UINT32 size);
	GUINT8Seq (UINT32 size, UINT8 * data, bool release = false);
	GUINT8Seq (UINT32 maxSize, UINT32 currentSize, UINT8 * data, bool release = false);
	GUINT8Seq (GUINT8Seq const & aMessage);
	virtual ~GUINT8Seq (void);

	UINT8 * DataNP (void) const;
	UINT8 * GetBuffer (void) const;
	inline UINT8 & operator [] (int index) const;
	UINT32 Length (void) const;
	void Length (UINT32 size);

private:
    UINT8 *	m_pBuffer;
    UINT32	m_u32Size;
    bool	m_boolRelease;
};

#endif



/////////////////////////////////////////////////////////////////////////////
//
// Implementation for GString class.
// General-purpose class for managing strings.
// Parts of class interface borrowed from java.lang.String,
// java.lang.StringBuffer.
//
// Doesn't do reference counting.
// Doesn't have intrinsic MT support.
//
// Bill Royal
// (c) Gilbarco 1996
//
/////////////////////////////////////////////////////////////////////////////

// GString
#include "Common/Util/GString.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
bool operator==( const GString& s1, const GString& s2 )
{
	UINT8* tpc1;
	UINT8* tpc2;
	UINT32 i;


	tpc1 = s1.m_pData;
	tpc2 = s2.m_pData;
	
	if ( s1.m_ulStrLen != s2.m_ulStrLen )
	{
		return false;
	}

	i = 0;
	while ( ( i < s1.m_ulStrLen ) && ( tpc1[ i ] == tpc2[ i ] ) )
	{
		i++;
	}

	if ( i != s1.m_ulStrLen )
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool operator!=( const GString& s1, const GString& s2 )
{
	return !( s1 == s2 );	// hack
}

/////////////////////////////////////////////////////////////////////////////
// Accommodate() returns false when more space was needed but couldn't
// be allocated.
//## end module%36FFE68F02B7.additionalDeclarations


// Class GString 




GString::GString(const GString &right)
  //## begin GString::GString%copy.hasinit preserve=no
  //## end GString::GString%copy.hasinit
  //## begin GString::GString%copy.initialization preserve=yes
  //## end GString::GString%copy.initialization
{
  //## begin GString::GString%copy.body preserve=yes
	UINT32 i;
	
	m_ulStrLen = right.m_ulStrLen;
	m_ulBufLen = right.m_ulStrLen;
	m_pData = new UINT8[ m_ulStrLen + 1 ];

	if ( m_pData != NULL )
	{
		// copy all the chars, including the trailing '\0'.
		for ( i = 0; i <= m_ulStrLen; i++ )
		{
			m_pData[ i ] = right.m_pData[ i ];
		}
	}
	else
	{
		m_ulStrLen = 0;
		m_ulBufLen = 0;
	}
  //## end GString::GString%copy.body
}

//## Operation: GString%-1595840421; C++
GString::GString ()
  //## begin GString::GString%-1595840421.hasinit preserve=no
  //## end GString::GString%-1595840421.hasinit
  //## begin GString::GString%-1595840421.initialization preserve=yes
  //## end GString::GString%-1595840421.initialization
{
  //## begin GString::GString%-1595840421.body preserve=yes
	m_pData = NULL;
	m_ulStrLen = 0;
	m_ulBufLen = 0;
  //## end GString::GString%-1595840421.body
}

//## Operation: GString%2132440592; C++
GString::GString (const char* pc)
  //## begin GString::GString%2132440592.hasinit preserve=no
  //## end GString::GString%2132440592.hasinit
  //## begin GString::GString%2132440592.initialization preserve=yes
  //## end GString::GString%2132440592.initialization
{
  //## begin GString::GString%2132440592.body preserve=yes
	GStringBase( (UINT8*) pc );
  //## end GString::GString%2132440592.body
}

//## Operation: GString%-178549195; C++
GString::GString (UINT8* pc)
  //## begin GString::GString%-178549195.hasinit preserve=no
  //## end GString::GString%-178549195.hasinit
  //## begin GString::GString%-178549195.initialization preserve=yes
  //## end GString::GString%-178549195.initialization
{
  //## begin GString::GString%-178549195.body preserve=yes
	GStringBase( pc );
  //## end GString::GString%-178549195.body
}


//## Operation: ~GString%894033309; C++
GString::~GString ()
{
  //## begin GString::~GString%894033309.body preserve=yes
	if ( m_pData )
	{
		delete[] m_pData;
	}
  //## end GString::~GString%894033309.body
}



//## Other Operations (implementation)
//## Operation: GStringBase%-1153569521; C++
void GString::GStringBase (UINT8* pc)
{
  //## begin GString::GStringBase%-1153569521.body preserve=yes
	UINT32 i;

	UINT8* tpc = pc;

	m_ulStrLen = 0;
	while ( *tpc++ )
	{
		m_ulStrLen++;
	}
	m_ulBufLen = m_ulStrLen;

	m_pData = new UINT8[ m_ulStrLen + 1 ];
	if ( m_pData )
	{
		for ( i = 0; *( pc + i ); i++ )
		{
			*( m_pData + i ) = *( pc + i );
		}
		*( m_pData + i ) = '\0';
	}
	else
	{
		m_ulStrLen = 0;
		m_ulBufLen = 0;
	}
  //## end GString::GStringBase%-1153569521.body
}

//## Operation: operator =%-1761252987; C++
GString& GString::operator = (const GString& s)
{
  //## begin GString::operator =%-1761252987.body preserve=yes
	UINT32 i;
	if ( s.m_pData != m_pData )		// don't allow delete when called on self
	{
		if ( s.m_ulStrLen > m_ulBufLen )
		{
			if ( m_pData != NULL )
			{
				delete[] m_pData;
			}
			m_ulBufLen = s.m_ulBufLen;
			m_pData = new UINT8[ m_ulBufLen + 1 ];
		}
		
		if ( m_pData != NULL )
		{
			m_ulStrLen = s.m_ulStrLen;
			// copy all the chars, including the trailing '\0'.
			for ( i = 0; i < m_ulStrLen + 1; i++ )
			{
				m_pData[ i ] = s.m_pData[ i ];
			}
		}
		else
		{
			m_ulStrLen = 0;
			m_ulBufLen = 0;
		}
	}
	return *this;
  //## end GString::operator =%-1761252987.body
}

//## Operation: operator =%-164714759; C++
GString& GString::operator = (UINT8* pc)
{
  //## begin GString::operator =%-164714759.body preserve=yes
	UINT32 i;
	if ( pc != m_pData )	// don't allow delete when called on self
	{
		
		// Get the length of the character array.
		// We have to assume that it ends at the first '\0'.
		for ( i = 0; pc[ i ] != '\0'; i++ )
			;

		if ( m_ulStrLen < i )
		{
			if ( m_pData != NULL )
			{
				delete[] m_pData;
			}
			m_ulBufLen = i;
			m_pData = new UINT8[ m_ulBufLen + 1 ];
		}

		if ( m_pData != NULL )
		{
			m_ulStrLen = i;
			// copy all the chars, including the trailing '\0'.
			for ( i = 0; i < m_ulStrLen + 1; i++ )
			{
				m_pData[ i ] = pc[ i ];
			}
		}
		else
		{
			m_ulStrLen = 0;
			m_ulBufLen = 0;
		}
	}
	return *this;
  //## end GString::operator =%-164714759.body
}

//## Operation: Accommodate%1371690148; C++
bool GString::Accommodate (UINT32 size)
{
  //## begin GString::Accommodate%1371690148.body preserve=yes
	UINT8* pNewBuf;
	UINT32 i;

	if ( size <= m_ulBufLen )
		return true;

	pNewBuf = new UINT8[ size + 1 ];
	if ( pNewBuf == NULL )
		return false;

	if ( m_pData )
	{
		for ( i = 0; i <= m_ulStrLen; i++ )
			pNewBuf[ i ] = m_pData[ i ];
		delete[] m_pData;
	}

	m_pData = pNewBuf;

	return true;
  //## end GString::Accommodate%1371690148.body
}

//## Operation: Append%-719218967; C++
GString& GString::Append (const GString& s)
{
  //## begin GString::Append%-719218967.body preserve=yes
	return AppendBase( s );
  //## end GString::Append%-719218967.body
}

//## Operation: Append%-158911557; C++
GString& GString::Append (const UINT8* pc)
{
  //## begin GString::Append%-158911557.body preserve=yes
	// Hack.
	GString tString(const_cast<UINT8*>(pc));
	return AppendBase( tString );
  //## end GString::Append%-158911557.body
}

//## Operation: BeginsWith%1040507481; C++
bool GString::BeginsWith (const GString& s) const
{
  //## begin GString::BeginsWith%1040507481.body preserve=yes
	return BeginsWithBase( s );
  //## end GString::BeginsWith%1040507481.body
}

//## Operation: BeginsWith%-1654532229; C++
bool GString::BeginsWith (UINT8* pc) const
{
  //## begin GString::BeginsWith%-1654532229.body preserve=yes
	// Hack again.
	GString tString( pc );
	return BeginsWithBase( tString );
  //## end GString::BeginsWith%-1654532229.body
}

//## Operation: GetCharAt%-300194292; C++
UINT8 GString::GetCharAt (UINT32 index) const
{
  //## begin GString::GetCharAt%-300194292.body preserve=yes
	if ( m_pData == NULL )
	{
		return (UINT8) -1;
	}

	if ( index < m_ulStrLen )
	{
		return m_pData[ index ];
	}
	else
	{
		return (UINT8) -1;
	}
  //## end GString::GetCharAt%-300194292.body
}

//## Operation: EndsWith%632705578; C++
bool GString::EndsWith (const GString& s) const
{
  //## begin GString::EndsWith%632705578.body preserve=yes
	return EndsWithBase( s );
  //## end GString::EndsWith%632705578.body
}

//## Operation: EndsWith%1238827395; C++
bool GString::EndsWith (UINT8* pc) const
{
  //## begin GString::EndsWith%1238827395.body preserve=yes
	// YAH.
	GString tString( pc );
	return EndsWithBase( tString );
  //## end GString::EndsWith%1238827395.body
}

//## Operation: IndexOf%-210531823; C++
UINT32 GString::IndexOf (const GString& s) const
{
  //## begin GString::IndexOf%-210531823.body preserve=yes
	return IndexOfBase( s );
  //## end GString::IndexOf%-210531823.body
}

//## Operation: IndexOf%58922758; C++
UINT32 GString::IndexOf (UINT8* pc) const
{
  //## begin GString::IndexOf%58922758.body preserve=yes
	// YAH
	GString tString( pc );
	return IndexOfBase( pc );
  //## end GString::IndexOf%58922758.body
}

//## Operation: Insert%649731657; C++
GString& GString::Insert (const GString& s, UINT32 index)
{
  //## begin GString::Insert%649731657.body preserve=yes
	return InsertBase( s, index );
  //## end GString::Insert%649731657.body
}

//## Operation: Insert%-675481100; C++
GString& GString::Insert (UINT8* pc, UINT32 index)
{
  //## begin GString::Insert%-675481100.body preserve=yes
	GString tString( pc );
	return InsertBase( tString, index );
  //## end GString::Insert%-675481100.body
}

//## Operation: Reset%-294769644; C++
void GString::Reset ()
{
  //## begin GString::Reset%-294769644.body preserve=yes
	if ( m_pData != NULL )
	{
		delete[] m_pData;
	}
	m_pData = NULL;
	m_ulStrLen = 0;
	m_ulBufLen = 0;
  //## end GString::Reset%-294769644.body
}

//## Operation: SetLength%1299215270; C++
UINT32 GString::SetLength (UINT32 newLength)
{
  //## begin GString::SetLength%1299215270.body preserve=yes
	if ( newLength <= m_ulBufLen )
	{
		m_ulStrLen = newLength;
		m_pData[ newLength ] = '\0';
	}
	else
	{
		m_ulStrLen = m_ulBufLen;
		m_pData[ m_ulStrLen ] = '\0';
	}

	return m_ulStrLen;		
  //## end GString::SetLength%1299215270.body
}

//## Operation: SetCharAt%-413482095; C++
UINT8 GString::SetCharAt (UINT32 index, UINT8 ch)
{
  //## begin GString::SetCharAt%-413482095.body preserve=yes
	static UINT8 cErr = (UINT8) -1;

	if ( m_pData == NULL )
	{
		return cErr;
	}

	if ( index >= m_ulStrLen )
	{
		return cErr;
	}

	m_pData[ index ] = ch;
	return ch;
  //## end GString::SetCharAt%-413482095.body
}

//## Operation: Substr%1725058201; C++
GString GString::Substr (UINT32 start, UINT32 length) const
{
  //## begin GString::Substr%1725058201.body preserve=yes
	GString retVal;
	UINT32 iDest;
	UINT32 iTemp;

	retVal.m_pData = new UINT8[ length + 1 ];
	if ( retVal.m_pData != NULL )
	{
		// Otherwise, copy chars from this string to the return string.
		iDest = start;
		iTemp = 0;
		while ( ( iTemp < length ) && ( iDest < m_ulStrLen ) )
		{
			retVal.m_pData[ iTemp ] = m_pData[ iDest ];
			iDest++;
			iTemp++;
		}
		retVal.m_pData[ iTemp ] = '\0';
		retVal.m_ulStrLen = iTemp;
		retVal.m_ulBufLen = length;
	}

	return retVal;
  //## end GString::Substr%1725058201.body
}

//## Operation: ToLower%439560369; C++
GString& GString::ToLower ()
{
  //## begin GString::ToLower%439560369.body preserve=yes
	UINT32 i;

	for ( i = 0; i < m_ulStrLen; i++ )
	{
		if ( ( m_pData[ i ] <= 'Z' ) && ( m_pData[ i ] >= 'A' ) )
		{
			m_pData[ i ] += ( 'a' - 'A' );
		}
	}
	return *this;
  //## end GString::ToLower%439560369.body
}

//## Operation: ToUpper%1113028979; C++
GString& GString::ToUpper ()
{
  //## begin GString::ToUpper%1113028979.body preserve=yes
	UINT32 i;

	for ( i = 0; i < m_ulStrLen; i++ )
	{
		if ( ( m_pData[ i ] >= 'a' ) && ( m_pData[ i ] <= 'z' ) )
		{
			m_pData[ i ] -= ( 'a' - 'A' );
		}
	}
	return *this;
  //## end GString::ToUpper%1113028979.body
}

//## Operation: MemCopy%1366567150
GString& GString::MemCopy (UINT8* pbyData, UINT16 length)
{
  //## begin GString::MemCopy%1366567150.body preserve=yes
	UINT16 i;

	// [DBM - 1/30/04] This code was originally written (by CLE!) to copy 
	// length+1 bytes.  So, the current monochrome and possibly 
	// configuration interface code depend on this behavior :<
	length++;

	if (length > m_ulBufLen)
	{
		m_ulBufLen = length;

		delete[] m_pData;
		m_pData = new UINT8[ m_ulBufLen + 1 ];
	}

	if ( m_pData )
	{
		for ( i = 0; i < length; i++ )
		{
			*( m_pData + i ) = *( pbyData + i );
		}
		*( m_pData + i ) = '\0';

		m_ulStrLen = length;
	}
	else
	{
		m_ulStrLen = 0;
		m_ulBufLen = 0;
	}

	return *this;
  //## end GString::MemCopy%1366567150.body
}

//## Operation: AppendBase%-1456783971; C++
GString& GString::AppendBase (const GString& s)
{
  //## begin GString::AppendBase%-1456783971.body preserve=yes
	UINT32 len;
	UINT32 i = 0, j = 0;
	UINT8 *pNewBuf;

	// This function currently doesn't work if a string is appended
	// to itself.  Don't do anything.
	if ( m_pData == s.m_pData )
	{
		return *this;
	}

	// Find the combined length of the new string.
	len = m_ulStrLen + s.m_ulStrLen;

	if ( len > m_ulBufLen )
	{
		// Get a new character buffer that's big enough, and copy in the
		// original string before we start appending _anything_.
		pNewBuf = new UINT8[ len + 1 ];
		if ( pNewBuf == NULL )
		{
			// best thing to do would be to throw an exception.
			return *this;
		}

		// Make sure there's something in the original string
		// before we start copying.
		if ( m_pData != NULL )
		{
			// If we got some memory for the new string, copy the
			// original in first, then delete the old.
			for ( i = 0; i < m_ulStrLen; i++ )
				pNewBuf[ i ] = m_pData[ i ];
			delete[] m_pData;
		}
		m_pData = pNewBuf;
		m_ulBufLen = len;
	}
	else
	{
		// Adjust i so that the next loop works out correctly.
		i = m_ulStrLen;
	}

	if ( m_pData )
	{
		// Now append the new string data.
		for ( j = 0; j < s.m_ulStrLen; j++ )
		{
			m_pData[ i + j ] = s.m_pData[ j ];
		}

		m_ulStrLen = len;
		m_pData[ m_ulStrLen ] = '\0';		// Make sure to terminate properly.
	}

	return *this;
  //## end GString::AppendBase%-1456783971.body
}

//## Operation: InsertBase%-983740696; C++
GString& GString::InsertBase (const GString& s, UINT32 index)
{
  //## begin GString::InsertBase%-983740696.body preserve=yes
	UINT32 len;
	UINT32 i, j;
	UINT8 *pNewBuf;

	// This function currently doesn't work if a string is inserted
	// into itself.  Don't do anything if this is the case.
	// Also do nothing if the insertion string is empty.
	if ( ( m_pData == s.m_pData ) || ( s.m_ulStrLen == 0 ) )
	{
		return *this;
	}

	// Find the combined length of the new string.
	len = m_ulStrLen + s.m_ulStrLen;

	if ( len > m_ulBufLen )
	{
		// Get a new character buffer that's big enough, and copy in the
		// original string before we start appending _anything_.
		pNewBuf = new UINT8[ len + 1 ];
		if ( pNewBuf == NULL )
		{
			// best thing to do would be to throw an exception.
			return *this;
		}
		m_ulBufLen = len;

		// Make sure there's something in the original string
		// before we start copying.
		if ( m_pData != NULL )
		{
			// Copy the data before the index.  If index is
			// greater than the length, only copy the length.
			if ( index > m_ulStrLen )
				index = m_ulStrLen;

			for ( i = 0; i < index; i++ )
				pNewBuf[ i ] = m_pData[ i ];
		}
	}
	else
	{
		pNewBuf = m_pData;
	}

	// Shift any data after the insertion index to make room
	// for the new string data.
	if ( index < m_ulStrLen )
	{
		for ( i = m_ulStrLen - 1; i >= index; i-- )
			pNewBuf[ i + s.m_ulStrLen ] = m_pData[ i ];
	}

	if ( m_pData )
	{
		// Now append the new string data.
		for ( j = 0; j < s.m_ulStrLen; j++ )
		{
			pNewBuf[ index + j ] = s.m_pData[ j ];
		}
		pNewBuf[ len ] = '\0';		// Make sure to terminate properly.

		// Get rid of the old string only if we made a new buffer.
		if ( m_pData != pNewBuf )
		{
			delete[] m_pData;
		}
		m_ulStrLen = len;
	}

	m_pData = pNewBuf;
	return *this;
  //## end GString::InsertBase%-983740696.body
}

//## Operation: BeginsWithBase%-553227041; C++
bool GString::BeginsWithBase (const GString& pat) const
{
  //## begin GString::BeginsWithBase%-553227041.body preserve=yes
	UINT32 i;

	if ( pat.m_ulStrLen == 0 )
		return true;

	if ( m_ulStrLen == 0 )
		return false;

	for ( i = 0; i < pat.m_ulStrLen; i++ )
	{
		if ( ( i >= m_ulStrLen ) || ( m_pData[ i ] != pat.m_pData[ i ] ) )
		{
			return false;
		}
	}

	return true;
  //## end GString::BeginsWithBase%-553227041.body
}

//## Operation: EndsWithBase%2079829877; C++
bool GString::EndsWithBase (const GString& pat) const
{
  //## begin GString::EndsWithBase%2079829877.body preserve=yes
	INT32 iThis = m_ulStrLen;		// must be signed to detect < 0
	INT32 iPat;

	if ( pat.m_ulStrLen == 0 )		// always valid if pattern string is null
		return true;

	if ( m_ulStrLen == 0 )			// can't be valid at this pt. if this string is null
		return false;

	// Iterate through the pattern string in reverse order.
	// If we get to the beginning of the pattern before falling out,
	// return a success condition.
	for ( iPat = pat.m_ulStrLen; iPat >= 0; iPat-- )
	{
		// If we made it back to the start of the string, or
		// if there was a difference in the strings, then return the
		// failure condition.
		if ( ( iThis < 0 ) || ( m_pData[ iThis ] != pat.m_pData[ iPat ] ) )
		{
			return false;
		}
		iThis--;
	}
	return true;
  //## end GString::EndsWithBase%2079829877.body
}

//## Operation: IndexOfBase%343323557; C++
UINT32 GString::IndexOfBase (const GString& pat) const
{
  //## begin GString::IndexOfBase%343323557.body preserve=yes
	UINT32 iDest = 0;
	UINT32 iTDest;
	UINT32 iPat;
	bool bFound = false;

	while ( !bFound && ( iDest < m_ulStrLen ) )
	{
		if ( m_pData[ iDest ] == pat.m_pData[ 0 ] )
		{
			// Found a first-character match, so enter a new 'state'.
			iTDest = iDest + 1;
			iPat = 1;
			while ( ( iTDest < m_ulStrLen ) && ( iPat < pat.m_ulStrLen ) &&
				( m_pData[ iTDest ] == pat.m_pData[ iPat ] ) )
			{
				iTDest++;
				iPat++;
			}

			// If the reason we bombed out is because we got to the end of the
			// pattern, then we matched.
			if ( iPat == pat.m_ulStrLen )
			{
				bFound = true;
			}
		}

		iDest++;
				
	}
	return ( bFound ) ? iDest - 1 : (UINT32) -1;
  //## end GString::IndexOfBase%343323557.body
}

// Additional Declarations
  //## begin GString%36FFE68F0202.declarations preserve=yes
const char* GString::GetString() const
{
	return (const char*)m_pData;
}

  //## end GString%36FFE68F0202.declarations

//## begin module%36FFE68F02B7.epilog preserve=yes
//## end module%36FFE68F02B7.epilog

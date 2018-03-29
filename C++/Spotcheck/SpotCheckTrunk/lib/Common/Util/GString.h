//## Module: GString%36FFE68F019E; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: src\Common\Util\GString.h

#ifndef GString_h
#define GString_h 1

//## begin module%36FFE68F019E.includes preserve=yes

#include "GTypes.h"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// GString -- general purpose string class
//
// Class to manage textual string representations.
//
// GString auto-increases its internal buffer to store whatever string
// data it holds.  It does not automatically shrink to fit, however.
// It does not do reference counting.
//
// TBD:  add a clone() member
//
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//## end module%36FFE68F019E.includes

//## begin module%36FFE68F019E.additionalDeclarations preserve=yes
//## end module%36FFE68F019E.additionalDeclarations


//## begin GString%36FFE68F0202.preface preserve=yes
//## end GString%36FFE68F0202.preface

//## Class: GString%36FFE68F0202
//	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//	GString -- general purpose string class
//
//	Class to manage textual string representations.
//
//	GString auto-increases its internal buffer to store whatever
//	data it holds.  It does not automatically shrink to fit, however.
//	It does not do reference counting.
//
//	TBD:  add a clone() member
//
//	=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36FFE68F0272; { -> }
//## Uses: <unnamed>%36FFE68F0280; { -> }

class GString 
{
  //## begin GString%36FFE68F0202.initialDeclarations preserve=yes
public:
	friend bool operator==( const GString& s1, const GString& s2 );
	friend bool operator!=( const GString& s1, const GString& s2 );
  //## end GString%36FFE68F0202.initialDeclarations

  public:
    //## Constructors (generated)
      GString(const GString &right);

    //## Constructors (specified)
      //## Operation: GString%-1595840421; C++
      //	///////////////////////////////////////////////////////////////////////////
      //
      //	Implementation for GString class.
      //	General-purpose class for managing strings.
      //	Parts of class interface borrowed from java.lang.String,
      //	java.lang.StringBuffer.
      //
      //	Doesn't do reference counting.
      //	Doesn't have intrinsic MT support.
      //
      //	Bill Royal
      //	(c) Gilbarco 1996
      //
      //	///////////////////////////////////////////////////////////////////////////
      //	///////////////////////////////////////////////////////////////////////////
      GString ();

      //## Operation: GString%2132440592; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString (const char* pc);

      //## Operation: GString%-178549195; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString (UINT8* pc);

    //## Destructor (specified)
      //## Operation: ~GString%894033309; C++
      //	///////////////////////////////////////////////////////////////////////////
      ~GString ();


    //## Other Operations (specified)
      //## Operation: operator =%-1761252987; C++
      //	Needed these operator overloads mainly for template use.
      //	///////////////////////////////////////////////////////////////////////////
      GString& operator = (const GString& s);

      //## Operation: operator =%-164714759; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString& operator = (UINT8* pc);

      //## Operation: operator UINT8*%488989251; C++
      operator UINT8* () const;

      //## Operation: Accommodate%1371690148; C++
      //	///////////////////////////////////////////////////////////////////////////
      //	Accommodate() returns false when more space was needed but couldn't
      //	be allocated.
      bool Accommodate (UINT32 size);

      //## Operation: Append%-719218967; C++
      //	forces buffer to a particular size
      //	///////////////////////////////////////////////////////////////////////////
      GString& Append (const GString& s);

      //## Operation: Append%-158911557; C++
      //	error if string appended to self
      //	///////////////////////////////////////////////////////////////////////////
      GString& Append (const UINT8* pc);

      //## Operation: BeginsWith%1040507481; C++
      //	///////////////////////////////////////////////////////////////////////////
      bool BeginsWith (const GString& s) const;

      //## Operation: BeginsWith%-1654532229; C++
      bool BeginsWith (UINT8* pc) const;

      //## Operation: GetCharAt%-300194292; C++
      //	///////////////////////////////////////////////////////////////////////////
      UINT8 GetCharAt (UINT32 index) const;

      //## Operation: GetLength%380037776; C++
      UINT32 GetLength () const;

      //## Operation: EndsWith%632705578; C++
      //	///////////////////////////////////////////////////////////////////////////
      bool EndsWith (const GString& s) const;

      //## Operation: EndsWith%1238827395; C++
      bool EndsWith (UINT8* pc) const;

      //## Operation: IndexOf%-210531823; C++
      //	///////////////////////////////////////////////////////////////////////////
      UINT32 IndexOf (const GString& s) const;

      //## Operation: IndexOf%58922758; C++
      UINT32 IndexOf (UINT8* pc) const;

      //## Operation: Insert%649731657; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString& Insert (const GString& s, UINT32 index);

      //## Operation: Insert%-675481100; C++
      //	error if inserted into self
      GString& Insert (UINT8* pc, UINT32 index);

      //## Operation: Reset%-294769644; C++
      //	///////////////////////////////////////////////////////////////////////////
      void Reset ();

      //## Operation: SetLength%1299215270; C++
      //	///////////////////////////////////////////////////////////////////////////
      UINT32 SetLength (UINT32 newLength);

      //## Operation: SetCharAt%-413482095; C++
      //	not the best impl.
      //	///////////////////////////////////////////////////////////////////////////
      UINT8 SetCharAt (UINT32 index, UINT8 ch);

      //## Operation: Substr%1725058201; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString Substr (UINT32 start, UINT32 length) const;

      //## Operation: ToLower%439560369; C++
      //	///////////////////////////////////////////////////////////////////////////
      GString& ToLower ();

      //## Operation: ToUpper%1113028979; C++
      GString& ToUpper ();

      //## Operation: MemCopy%1366567150
      //	Warning This function contains no out of bounds checking.  It will copy the
      //	exact length specified no matter if it passes the length of the allocated
      //	array passed to it.
      GString& MemCopy (UINT8* pbyData, UINT16 length);

    // Additional Public Declarations
      //## begin GString%36FFE68F0202.public preserve=yes
	  const char* GetString() const;
      //## end GString%36FFE68F0202.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pData%36FFE68F0254
      //## begin GString::m_pData%36FFE68F0254.attr preserve=no  protected: UINT8* {VA} 
      UINT8* m_pData;
      //## end GString::m_pData%36FFE68F0254.attr

      //## Attribute: m_ulStrLen%36FFE68F028A
      //## begin GString::m_ulStrLen%36FFE68F028A.attr preserve=no  protected: UINT32 {VA} 
      UINT32 m_ulStrLen;
      //## end GString::m_ulStrLen%36FFE68F028A.attr

      //## Attribute: m_ulBufLen%36FFE68F02B3
      //	length of string, not counting '\0' at end
      //## begin GString::m_ulBufLen%36FFE68F02B3.attr preserve=no  protected: UINT32 {VA} 
      UINT32 m_ulBufLen;
      //## end GString::m_ulBufLen%36FFE68F02B3.attr

    // Additional Protected Declarations
      //## begin GString%36FFE68F0202.protected preserve=yes
      //## end GString%36FFE68F0202.protected

  private:

    //## Other Operations (specified)
      //## Operation: GStringBase%-1153569521; C++
      //	///////////////////////////////////////////////////////////////////////////
      void GStringBase (UINT8* pc);

      //## Operation: AppendBase%-1456783971; C++
      //	Common implementation of overloaded methods.
      //	///////////////////////////////////////////////////////////////////////////
      GString& AppendBase (const GString& s);

      //## Operation: InsertBase%-983740696; C++
      //	insertBase() is the common functionality for the two insert() functions.
      GString& InsertBase (const GString& s, UINT32 index);

      //## Operation: BeginsWithBase%-553227041; C++
      //	Implementation of both beginsWith() methods.
      bool BeginsWithBase (const GString& pat) const;

      //## Operation: EndsWithBase%2079829877; C++
      bool EndsWithBase (const GString& pat) const;

      //## Operation: IndexOfBase%343323557; C++
      UINT32 IndexOfBase (const GString& pat) const;

    // Additional Private Declarations
      //## begin GString%36FFE68F0202.private preserve=yes
      //## end GString%36FFE68F0202.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin GString%36FFE68F0202.implementation preserve=yes
      //## end GString%36FFE68F0202.implementation

};

//## begin GString%36FFE68F0202.postscript preserve=yes
//## end GString%36FFE68F0202.postscript

// Class GString 


//## Other Operations (inline)
//## Operation: operator UINT8*%488989251; C++
inline GString::operator UINT8* () const
{
  //## begin GString::operator UINT8*%488989251.body preserve=yes
 return m_pData; 
  //## end GString::operator UINT8*%488989251.body
}

//## Operation: GetLength%380037776; C++
inline UINT32 GString::GetLength () const
{
  //## begin GString::GetLength%380037776.body preserve=yes
 return m_ulStrLen; 
  //## end GString::GetLength%380037776.body
}

//## begin module%36FFE68F019E.epilog preserve=yes
//## end module%36FFE68F019E.epilog


#endif

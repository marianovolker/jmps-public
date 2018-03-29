//## begin module%3677C2C302C1.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3677C2C302C1.cm

//## begin module%3677C2C302C1.cp preserve=no
//## end module%3677C2C302C1.cp

//## Module: ReferenceList%3677C2C302C1; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: z:\ngd_pump\source\Common\Util\ReferenceList.h

#ifndef ReferenceList_h
#define ReferenceList_h 1

//## begin module%3677C2C302C1.additionalIncludes preserve=no
//## end module%3677C2C302C1.additionalIncludes

//## begin module%3677C2C302C1.includes preserve=yes
//## end module%3677C2C302C1.includes

// GTypes
#include "GTypes.h"
//## begin module%3677C2C302C1.declarations preserve=no
//## end module%3677C2C302C1.declarations

//## begin module%3677C2C302C1.additionalDeclarations preserve=yes
//## end module%3677C2C302C1.additionalDeclarations


//## begin ReferenceList%367787E10045.preface preserve=yes
typedef void Iterator;
//## end ReferenceList%367787E10045.preface

//## Class: ReferenceList%367787E10045; Parameterized Class
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class ReferenceType>
class ReferenceList 
{
  //## begin ReferenceList%367787E10045.initialDeclarations preserve=yes
	struct list_node 
		{
		list_node* next;
		list_node* prev;
		ReferenceType* data;
	    };
  //## end ReferenceList%367787E10045.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ReferenceList%913799086;  C++
      //	*********************************************************
      //	***********************//
      //	Parameterized Class ReferenceList
      ReferenceList ();

    //## Destructor (specified)
      //## Operation: ~ReferenceList%913799087;  C++
      //	*********************************************************
      //	***********************//
      ~ReferenceList ();


    //## Other Operations (specified)
      //## Operation: PushBack%913799088;  C++
      //	*********************************************************
      //	***********************//
      void PushBack (ReferenceType* pElem);

      //## Operation: PopFront%913799089;  C++
      //	*********************************************************
      //	***********************//
      ReferenceType* PopFront ();

      //## Operation: PopBack%913799090;  C++
      //	*********************************************************
      //	***********************//
      ReferenceType* PopBack ();

      //## Operation: GetFirst%913799091;  C++
      //	*********************************************************
      //	***********************//
      Iterator* GetFirst ();

      //## Operation: GetNext%913799092;  C++
      //	*********************************************************
      //	***********************//
      Iterator* GetNext (Iterator* prev_node);

      //## Operation: GetLast%913799093;  C++
      Iterator* GetLast ();

      //## Operation: Size%913799094;  C++
      UINT32 Size ();

      //## Operation: IsEmpty%913799095;  C++
      //	*********************************************************
      //	***********************//
      bool IsEmpty ();

      //## Operation: MakeEmpty%913799096;  C++
      //	*********************************************************
      //	***********************//
      void MakeEmpty (int bDeleteObjectsReferedTo = true);

      //## Operation: EraseThis%913799097;  C++
      //	*********************************************************
      //	***********************//
      //	Additional Public Declarations
      void EraseThis (Iterator* this_node);

    // Additional Public Declarations
      //## begin ReferenceList%367787E10045.public preserve=yes
      //## end ReferenceList%367787E10045.public

  protected:
    // Additional Protected Declarations
      //## begin ReferenceList%367787E10045.protected preserve=yes
      //## end ReferenceList%367787E10045.protected

  private:
    //## Constructors (generated)
      ReferenceList(const ReferenceList< ReferenceType > &right);

    //## Assignment Operation (generated)
      const ReferenceList< ReferenceType > & operator=(const ReferenceList< ReferenceType > &right);

    // Data Members for Class Attributes

      //## Attribute: m_pLNode_f%367787E10054
      //	First element;
      //## begin ReferenceList::m_pLNode_f%367787E10054.attr preserve=no  private: list_node* {UA} 
      list_node* m_pLNode_f;
      //## end ReferenceList::m_pLNode_f%367787E10054.attr

      //## Attribute: m_pLNode_l%367787E10055
      //	Last element;
      //## begin ReferenceList::m_pLNode_l%367787E10055.attr preserve=no  private: list_node* {UA} 
      list_node* m_pLNode_l;
      //## end ReferenceList::m_pLNode_l%367787E10055.attr

      //## Attribute: m_pActual%367787E10056
      //	Used to search throght
      //## begin ReferenceList::m_pActual%367787E10056.attr preserve=no  private: list_node* {UA} 
      list_node* m_pActual;
      //## end ReferenceList::m_pActual%367787E10056.attr

      //## Attribute: m_pUnused%367787E10057
      //	Unused nodes
      //## begin ReferenceList::m_pUnused%367787E10057.attr preserve=no  private: list_node* {UA} 
      list_node* m_pUnused;
      //## end ReferenceList::m_pUnused%367787E10057.attr

      //## Attribute: m_size%367787E10058
      //## begin ReferenceList::m_size%367787E10058.attr preserve=no  private: int {UA} 
      int m_size;
      //## end ReferenceList::m_size%367787E10058.attr

    // Additional Private Declarations
      //## begin ReferenceList%367787E10045.private preserve=yes
      //## end ReferenceList%367787E10045.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ReferenceList%367787E10045.implementation preserve=yes
      //## end ReferenceList%367787E10045.implementation

};

//## begin ReferenceList%367787E10045.postscript preserve=yes
//## end ReferenceList%367787E10045.postscript

// Parameterized Class ReferenceList 

//## Operation: ReferenceList%913799086;  C++
//## Semantics:
//	========================================================================
//	Name: ReferenceList
//
//	Purpose: *********************************************************
//	***********************//
//	Parameterized Class ReferenceList
//
//	Returns: ReferenceList
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline ReferenceList<ReferenceType>::ReferenceList ()
  //## begin ReferenceList::ReferenceList%913799086.hasinit preserve=no
  //## end ReferenceList::ReferenceList%913799086.hasinit
  //## begin ReferenceList::ReferenceList%913799086.initialization preserve=yes
  //## end ReferenceList::ReferenceList%913799086.initialization
{
  //## begin ReferenceList::ReferenceList%913799086.body preserve=yes
	m_size=0;
	m_pLNode_f=0;
	m_pLNode_l=0;
	m_pActual=0;
	m_pUnused=0;
  //## end ReferenceList::ReferenceList%913799086.body
}


//## Operation: ~ReferenceList%913799087;  C++
//## Semantics:
//	========================================================================
//	Name: ~ReferenceList
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns:
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline ReferenceList<ReferenceType>::~ReferenceList ()
{
  //## begin ReferenceList::~ReferenceList%913799087.body preserve=yes
	list_node * temp_node=m_pLNode_f;

	while (temp_node) 
		{
		m_pLNode_f=temp_node->prev;
		m_pActual=m_pLNode_f;
		ReferenceType* pTmpData=temp_node->data;
		
		delete temp_node;

		m_size--;
		temp_node=m_pLNode_f;
		}

	while (m_pUnused) 
		{
		temp_node=m_pUnused;
		m_pUnused=m_pUnused->prev;
		delete temp_node;
		}
  //## end ReferenceList::~ReferenceList%913799087.body
}



//## Other Operations (inline)
//## Operation: PushBack%913799088;  C++
//## Semantics:
//	========================================================================
//	Name: PushBack
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: void
//
//	Inputs: pElem
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline void ReferenceList<ReferenceType>::PushBack (ReferenceType* pElem)
{
  //## begin ReferenceList::PushBack%913799088.body preserve=yes
	list_node * temp_node;
	if (m_pUnused) 
		{
		temp_node=m_pUnused;
		m_pUnused=m_pUnused->prev;
		}
	else 
	{
		temp_node = new list_node ();

	};
	temp_node->data=pElem;
	temp_node->next=0;
	temp_node->prev=0;
	m_size++;


	if (m_pLNode_l)
				{
		   		 temp_node->next=m_pLNode_l;
		   		 temp_node->next->prev=temp_node;	
				 m_pLNode_l=temp_node;
				}
			else
				{
		   		 m_pLNode_f=temp_node;
				 m_pActual=m_pLNode_f;
				 m_pLNode_l=temp_node;
				};
  //## end ReferenceList::PushBack%913799088.body
}

//## Operation: PopFront%913799089;  C++
//## Semantics:
//	========================================================================
//	Name: PopFront
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: ReferenceType*
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline ReferenceType* ReferenceList<ReferenceType>::PopFront ()
{
  //## begin ReferenceList::PopFront%913799089.body preserve=yes
	list_node * temp_node=m_pLNode_f;
	ReferenceType* pTmpData;

	if (m_pLNode_f) 
		{
		m_pLNode_f=temp_node->prev;
		m_pActual=m_pLNode_f;
		pTmpData=temp_node->data;
		if (temp_node->prev)
			temp_node->prev->next=0;
		else m_pLNode_l=0;


		temp_node->prev=m_pUnused;
		temp_node->next=0;
		temp_node->data=0;
	    m_pUnused = temp_node;

		m_size--;
		return pTmpData;
		}
	else 
		{
		return 0;
		}
  //## end ReferenceList::PopFront%913799089.body
}

//## Operation: PopBack%913799090;  C++
//## Semantics:
//	========================================================================
//	Name: PopBack
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: ReferenceType*
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline ReferenceType* ReferenceList<ReferenceType>::PopBack ()
{
  //## begin ReferenceList::PopBack%913799090.body preserve=yes
	list_node * temp_node=m_pLNode_l;
	ReferenceType* pTmpData;

	if (temp_node) 
		{
		m_pLNode_l=temp_node->next;
		pTmpData=temp_node->data;
		if (temp_node->next)
			temp_node->next->prev=0;
		else
			{ 
			m_pLNode_f=0;
			m_pActual=m_pLNode_f;
			}

		temp_node->prev=m_pUnused;
		temp_node->next=0;
		temp_node->data=0;
		m_pUnused = temp_node;

		m_size--;
		return pTmpData;
		}
	else
		{
		return 0;
		}
  //## end ReferenceList::PopBack%913799090.body
}

//## Operation: GetFirst%913799091;  C++
//## Semantics:
//	========================================================================
//	Name: GetFirst
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: Iterator*
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline Iterator* ReferenceList<ReferenceType>::GetFirst ()
{
  //## begin ReferenceList::GetFirst%913799091.body preserve=yes
	return (Iterator*) m_pLNode_f;
  //## end ReferenceList::GetFirst%913799091.body
}

//## Operation: GetNext%913799092;  C++
//## Semantics:
//	========================================================================
//	Name: GetNext
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: Iterator*
//
//	Inputs: prev_node
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline Iterator* ReferenceList<ReferenceType>::GetNext (Iterator* prev_node)
{
  //## begin ReferenceList::GetNext%913799092.body preserve=yes
	list_node* tmp_node =(list_node*) prev_node;

	tmp_node=tmp_node->prev;

	return (Iterator*) tmp_node;
  //## end ReferenceList::GetNext%913799092.body
}

//## Operation: GetLast%913799093;  C++
//## Semantics:
//	========================================================================
//	Name: GetLast
//
//	Purpose:
//
//	Returns: Iterator*
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline Iterator* ReferenceList<ReferenceType>::GetLast ()
{
  //## begin ReferenceList::GetLast%913799093.body preserve=yes
	return (Iterator*) m_pLNode_l;
  //## end ReferenceList::GetLast%913799093.body
}

//## Operation: Size%913799094;  C++
//## Semantics:
//	========================================================================
//	Name: Size
//
//	Purpose:
//
//	Returns: UINT32
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline UINT32 ReferenceList<ReferenceType>::Size ()
{
  //## begin ReferenceList::Size%913799094.body preserve=yes
	  return m_size;
  //## end ReferenceList::Size%913799094.body
}

//## Operation: IsEmpty%913799095;  C++
//## Semantics:
//	========================================================================
//	Name: IsEmpty
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: bool
//
//	Inputs:
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline bool ReferenceList<ReferenceType>::IsEmpty ()
{
  //## begin ReferenceList::IsEmpty%913799095.body preserve=yes
	if (m_size==0) return true;
		else return false;
  //## end ReferenceList::IsEmpty%913799095.body
}

//## Operation: MakeEmpty%913799096;  C++
//## Semantics:
//	========================================================================
//	Name: MakeEmpty
//
//	Purpose: *********************************************************
//	***********************//
//
//	Returns: void
//
//	Inputs: bDeleteObjectsReferedTo
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline void ReferenceList<ReferenceType>::MakeEmpty (int bDeleteObjectsReferedTo)
{
  //## begin ReferenceList::MakeEmpty%913799096.body preserve=yes
	list_node * temp_node=m_pLNode_f;

	while (temp_node) 
		{
		m_pLNode_f=temp_node->prev;
		m_pActual=m_pLNode_f;
		ReferenceType* pTmpData=temp_node->data;

		temp_node->prev=m_pUnused;
		temp_node->next=0;
		temp_node->data=0;
		if (m_pUnused==0) m_pUnused = temp_node;

		m_size--;
		temp_node=m_pLNode_f;
		}
  //## end ReferenceList::MakeEmpty%913799096.body
}

//## Operation: EraseThis%913799097;  C++
//## Semantics:
//	========================================================================
//	Name: EraseThis
//
//	Purpose: *********************************************************
//	***********************//
//	Additional Public Declarations
//
//	Returns: void
//
//	Inputs: this_node
//
//	Inputs/Outputs:
//
//	Outputs:
//
//	Pre Conditions:
//
//	Post Conditions:
//
//	Notes:
//
//	========================================================================
template <class ReferenceType>
inline void ReferenceList<ReferenceType>::EraseThis (Iterator* this_node)
{
  //## begin ReferenceList::EraseThis%913799097.body preserve=yes
	list_node * temp_node=(list_node *) this_node;

	if (temp_node) 
		{
		if (temp_node->next)
			temp_node->next->prev=temp_node->prev;
		else m_pLNode_f=temp_node->prev;
		
		if (temp_node->prev)
			temp_node->prev->next=temp_node->next;
		else m_pLNode_l=temp_node->next;

		delete temp_node;
		};
  //## end ReferenceList::EraseThis%913799097.body
}

//## begin module%3677C2C302C1.epilog preserve=yes
//## end module%3677C2C302C1.epilog


#endif

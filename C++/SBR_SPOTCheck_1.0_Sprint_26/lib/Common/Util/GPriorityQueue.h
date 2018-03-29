//## begin module%357E88C002A4.cm preserve=yes

//=============================================================================
// 
// File: GPriorityQueue.h
// 
// Description: Contains the GPriorityQueue class definition.
// 
// Created: 03/10/98
// 
// Author: Jon Towne
// 
//=============================================================================

//## end module%357E88C002A4.cm

//## begin module%357E88C002A4.cp preserve=no
//## end module%357E88C002A4.cp

//## Module: GPriorityQueue%357E88C002A4; Package specification
//## Subsystem: Common::Util%359176090340
//## Source file: src\Common\Util\GPriorityQueue.h

#ifndef GPriorityQueue_H
#define GPriorityQueue_H 1

// GTypes
#include "GTypes.h"
// GMutex
#include "GMutex.h"

#include "Common/MessageAdmin/MessagePriority.h"

#include <vector>
#include <queue>
using std::queue;
using std::priority_queue;

using namespace std;


//## Class: GPriorityQueue%357E88C00350; Parameterized Class
//	-----------------------------------------------------------------------------
//	This class implements a object queue.
//## Category: CommonLP::UtilLP%35901B900282; Global
//## Subsystem: Common::Util%359176090340
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%357E88C00377;UINT32 { -> }

template <class QueuedObject, class Compare>
class GPriorityQueue 
{
  //## begin GPriorityQueue%357E88C00350.initialDeclarations preserve=yes

	// Type definition of an STL priority queue which holds pointers to 
	// the type of object and the compare function which this template is 
	// instantiated with, 
	// 

	typedef priority_queue <QueuedObject*, 
							vector<QueuedObject*>, 
							Compare> PriorityQueue;

  public:
    //## Constructors (specified)
      //## Operation: GPriorityQueue%-1957344983; C++
      //	Default constructor.
      GPriorityQueue ();

    //## Destructor (specified)
      //## Operation: ~GPriorityQueue%1944010959; C++
      //	Desctructor
      ~GPriorityQueue ();


    //## Other Operations (specified)
      //## Operation: PopObject%-1487809694; C++
      //	Returns the next object pointer from the queue and
      //	removes that object pointer from the queue.
      QueuedObject* PopObject ();

      //## Operation: PushObject%1638039072; C++
      //	Adds a pointer to a object to the queue.
      void PushObject (QueuedObject* pObject);

      //## Operation: IsEmpty%-670105907; C++
      //	Returns TRUE if there are no objects in the queue.
      bool IsEmpty ();

      //## Operation: Size%-1262152690; C++
      //	Returns the number of objects in the queue.
      UINT32 Size ();

      //## Operation: Front%794813864; C++
      //	Returns a peek at the front value without modification to the queue
      QueuedObject* Front ();

      //## Operation: MakeEmpty%-492003940; C++
      //	Deletes all objects from the Queue
      void MakeEmpty (bool bDeleteObjectsReferedTo = true);

    // Additional Public Declarations
      //## begin GPriorityQueue%357E88C00350.public preserve=yes
      //## end GPriorityQueue%357E88C00350.public

  protected:
    // Additional Protected Declarations
      //## begin GPriorityQueue%357E88C00350.protected preserve=yes
      //## end GPriorityQueue%357E88C00350.protected

  private:
    //## Constructors (generated)
      GPriorityQueue(const GPriorityQueue< QueuedObject,Compare > &right);

    //## Assignment Operation (generated)
      const GPriorityQueue< QueuedObject,Compare > & operator=(const GPriorityQueue< QueuedObject,Compare > &right);

    // Data Members for Class Attributes

      //## Attribute: m_queue%357E88C0037D
      //	The actual queue. Current implementation uses an STL priority queue.
      //## begin GPriorityQueue::m_queue%357E88C0037D.attr preserve=no  private: PriorityQueue {VA} 
      PriorityQueue m_queue;
      //## end GPriorityQueue::m_queue%357E88C0037D.attr

      //## Attribute: m_mutex%357E88C00381
      //	Mutex used to provide thread-safe access to the queue.
      //## begin GPriorityQueue::m_mutex%357E88C00381.attr preserve=no
      GMutex m_mutex;
      //## end GPriorityQueue::m_mutex%357E88C00381.attr

    // Additional Private Declarations
      //## begin GPriorityQueue%357E88C00350.private preserve=yes
      //## end GPriorityQueue%357E88C00350.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin GPriorityQueue%357E88C00350.implementation preserve=yes
      //## end GPriorityQueue%357E88C00350.implementation

};

//## begin GPriorityQueue%357E88C00350.postscript preserve=yes
//## end GPriorityQueue%357E88C00350.postscript

// Parameterized Class GPriorityQueue 

template <class QueuedObject, class Compare>
inline GPriorityQueue<QueuedObject,Compare>::GPriorityQueue ()
  //## begin GPriorityQueue::GPriorityQueue%-1957344983.hasinit preserve=no
  //## end GPriorityQueue::GPriorityQueue%-1957344983.hasinit
  //## begin GPriorityQueue::GPriorityQueue%-1957344983.initialization preserve=yes
  //## end GPriorityQueue::GPriorityQueue%-1957344983.initialization
{
  //## begin GPriorityQueue::GPriorityQueue%-1957344983.body preserve=yes

//======================================================================== 
// 
// Name: GPriorityQueue
// 
// Purpose: Default constructor.
//
//=======================================================================

  //## end GPriorityQueue::GPriorityQueue%-1957344983.body
}


template <class QueuedObject, class Compare>
inline GPriorityQueue<QueuedObject,Compare>::~GPriorityQueue ()
{
  //## begin GPriorityQueue::~GPriorityQueue%1944010959.body preserve=yes
		
//======================================================================== 
// 
// Name: ~GPriorityQueue
// 
// Purpose: Destructor
//
//=======================================================================

  //## end GPriorityQueue::~GPriorityQueue%1944010959.body
}



//## Other Operations (inline)
template <class QueuedObject, class Compare>
inline QueuedObject* GPriorityQueue<QueuedObject,Compare>::PopObject ()
{
  //## begin GPriorityQueue::PopObject%-1487809694.body preserve=yes

//======================================================================== 
// 
// Name: PopObject
// 
// Purpose: Returns the next object pointer from the queue and
// removes that object pointer from the queue.
//
// Returns: The next object pointer from the queue.
//
//=======================================================================

	QueuedObject* pObject = (QueuedObject*)NULL;
	
	// Lock the queue.
	//
	m_mutex.Lock();

	// If the queue is not empty, get the pointer that 
	// is next in the queue.
	//
	if (!m_queue.empty())
	{
		pObject = m_queue.top();
	}
	// Remove the object pointer from the queue.	
	//
	m_queue.pop();
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return the retrieved object pointer.
	//
	return pObject;

  //## end GPriorityQueue::PopObject%-1487809694.body
}

template <class QueuedObject, class Compare>
inline void GPriorityQueue<QueuedObject,Compare>::PushObject (QueuedObject* pObject)
{
  //## begin GPriorityQueue::PushObject%1638039072.body preserve=yes

//======================================================================== 
// 
// Name: PushObject
// 
// Purpose: Adds a pointer to a object to the queue.
//
//=======================================================================

	// Lock the queue.
	//
	m_mutex.Lock();

	// Add the object to the queue.
	//
	m_queue.push (pObject);
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

  //## end GPriorityQueue::PushObject%1638039072.body
}

template <class QueuedObject, class Compare>
inline bool GPriorityQueue<QueuedObject,Compare>::IsEmpty ()
{
  //## begin GPriorityQueue::IsEmpty%-670105907.body preserve=yes

//======================================================================== 
// 
// Name: IsEmpty
// 
// Purpose: Returns TRUE if there are no objects in the queue.
// 
// Returns: TRUE if there are no objects in the queue..
//
//=======================================================================

	// Lock the queue.
	//
	m_mutex.Lock();

	// Find out whether the queue is empty or not.
	//
	bool bVal = m_queue.empty();
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return whether the queue is empty or not.
	//
	return bVal;

  //## end GPriorityQueue::IsEmpty%-670105907.body
}

template <class QueuedObject, class Compare>
inline UINT32 GPriorityQueue<QueuedObject,Compare>::Size ()
{
  //## begin GPriorityQueue::Size%-1262152690.body preserve=yes

//======================================================================== 
// 
// Name: Size
// 
// Purpose: Returns the number of objects in the Queue
//
//=======================================================================

	// Lock the queue.
	//
	m_mutex.Lock();

	// Retrieve the number of objects in the Queue.
	//
	UINT32 unSize = m_queue.size();
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return the number of objects in the Queue.
	//
	return unSize;

  //## end GPriorityQueue::Size%-1262152690.body
}

template <class QueuedObject, class Compare>
inline QueuedObject* GPriorityQueue<QueuedObject,Compare>::Front ()
{
  //## begin GPriorityQueue::Front%794813864.body preserve=yes

//======================================================================== 
// 
// Name: Front
// 
// Purpose: Returns a pointer to the Queue's first object
//
//=======================================================================

	// Lock the queue.
	//
	m_mutex.Lock();

	QueuedObject* pObject = (QueuedObject*)NULL;

	// If the queue is not empty, get the pointer that 
	// is next in the queue.
	//
	if (!m_queue.empty())
	{
		pObject = m_queue.top();
	}
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return the retrieved object pointer.
	//
	return pObject;

  //## end GPriorityQueue::Front%794813864.body
}

template <class QueuedObject, class Compare>
inline void GPriorityQueue<QueuedObject,Compare>::MakeEmpty (bool bDeleteObjectsReferedTo)
{
  //## begin GPriorityQueue::MakeEmpty%-492003940.body preserve=yes

//======================================================================== 
// 
// Name: MakeEmpty
// 
// Purpose: Removes all elements from the queue
//
//=======================================================================

	QueuedObject* pObject = (QueuedObject*)NULL;

	//Lock the Queue
	//
	m_mutex.Lock();
	
	// Delete all queue members.
	//
	while(!m_queue.empty())
	{
		// Get the next pointer in the queue.
		//
		pObject = (QueuedObject*)NULL;
	
		// If the queue is not empty, get the pointer that 
		// is next in the queue.
		//
		pObject = Front();

		// Remove the object pointer from the queue.	
		//
		m_queue.pop();
	
		// If we're supposed to delete the actual object refered to
		// by the pointers in the queue and a valid pointer has been
		// retrieved from the queue.
		//
		if (bDeleteObjectsReferedTo && 
			(pObject != NULL))
		{
			// Delete the actual object.
			//
			delete pObject;
		}
	}

	// UnLock the Queue
	//
	m_mutex.Unlock();

  //## end GPriorityQueue::MakeEmpty%-492003940.body
}

//## begin module%357E88C002A4.epilog preserve=yes
//## end module%357E88C002A4.epilog


#endif

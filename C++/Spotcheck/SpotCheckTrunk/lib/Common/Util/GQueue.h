//=============================================================================
// 
// File: GQueue.h
// 
// Description: Contains the GQueue class definition.
// 
// Created: 03/10/98
// 
// Author: Jon Towne and Rob Warren
// 
//=============================================================================

#ifndef GQueue_H
#define GQueue_H 1


// GTypes
#include "GTypes.h"
// GMutex
#include "GMutex.h"

#include <queue>
using std::queue;


template <class QueuedObject>
class GQueue 
{
  public:
	// Type definition of an STL queue which holds pointers to the type 
	// of object which this template is instantiated with.
	//
	typedef queue<QueuedObject*> Queue;

  public:
      //	Default constructor.
      GQueue ();

      //	Desctructor
      ~GQueue ();


      //	Returns the next object pointer from the queue and
      //	removes that object pointer from the queue.
      QueuedObject* PopObject ();

      //	Adds a pointer to a object to the object queue.
      void PushObject (QueuedObject* pObject);

      //	Returns TRUE if there are no objects in the queue.
      bool IsEmpty ();

      //	Returns the number of elements in the Queue
      UINT32 Size ();

      //	Returns a peek at the front value without modification to the queue.
      QueuedObject* Front ();

      //	Deletes all objects from the Queue
      void MakeEmpty (bool bDeleteObjectsReferedTo = true);

  protected:
    // Data Members for Class Attributes

      //	The actual queue. Current implementation uses an STL queue.
      Queue m_queue;

      //	Mutex used to provide thread-safe access to the queue.
      GMutex m_mutex;

  private:
      GQueue(const GQueue< QueuedObject > &right);

      const GQueue< QueuedObject > & operator=(const GQueue< QueuedObject > &right);

};


// Parameterized Class GQueue 

template <class QueuedObject>
inline GQueue<QueuedObject>::GQueue ()
{
}


template <class QueuedObject>
inline GQueue<QueuedObject>::~GQueue ()
{
}



template <class QueuedObject>
inline QueuedObject* GQueue<QueuedObject>::PopObject ()
{
//======================================================================== 
// 
// Name: PopObject
// 
// Purpose: Returns the next object pointer from the queue and
//			removes that object pointer from the queue.
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
		pObject = m_queue.front();
	
		// Remove the object pointer from the queue.	
		//
		m_queue.pop();
	}
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return the retrieved object pointer.
	//
	return pObject;
}

template <class QueuedObject>
inline void GQueue<QueuedObject>::PushObject (QueuedObject* pObject)
{
//======================================================================== 
// 
// Name: PushObject
// 
// Purpose: Adds a pointer to a new object in th Queue
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
}

template <class QueuedObject>
inline bool GQueue<QueuedObject>::IsEmpty ()
{
//======================================================================== 
// 
// Name: IsEmpty
// 
// Purpose: Returns TRUE when the queue contains no objects
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
}

template <class QueuedObject>
inline UINT32 GQueue<QueuedObject>::Size ()
{
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

}

template <class QueuedObject>
inline QueuedObject* GQueue<QueuedObject>::Front ()
{
//======================================================================== 
// 
// Name: Front
// 
// Purpose: Returns a pointer to the Queue's first object
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
		pObject = m_queue.front();
	}
	
	// Unlock the queue.
	//
	m_mutex.Unlock();

	// Return the retrieved object pointer.
	//
	return pObject;
}

template <class QueuedObject>
inline void GQueue<QueuedObject>::MakeEmpty (bool bDeleteObjectsReferedTo)
{
//======================================================================== 
// 
// Name: MakeEmpty
// 
// Purpose: Removes all elements from the queue
//
//=======================================================================

	QueuedObject* pObject = (QueuedObject*)NULL;

	//Lock the Queue
	m_mutex.Lock();
	
	// Delete all queue members
	while(!m_queue.empty())
	{
		// Get the next pointer in the queue.
		//
		pObject = (QueuedObject*)NULL;
	
		// If the queue is not empty, get the pointer that 
		// is next in the queue.
		//
		pObject = m_queue.front();

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
	m_mutex.Unlock();
}

#endif

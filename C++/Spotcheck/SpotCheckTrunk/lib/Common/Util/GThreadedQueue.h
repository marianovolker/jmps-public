//=============================================================================
// 
// File: GThreadedQueue.h
// 
// Description: Contains the GThreadedQueue class definition.
// 
// Created: 05/20/98
// 
// Author: Jon Towne
// 
//=============================================================================

//-----------------------------------------------------------------------------
// This class implements a thread which owns a queue. The queue can be either
// a GPriorityQueue or a GQueue (non-priority queue). This class is a template
// which takes as it's parameters the type of the object the queue is to hold
// and the type of queue to use.
//
// Notes:
//
// This thread will wait() until CheckQueue() is called. When CheckQueue() is
// called, it will wake up and check the queue to see if there is anything in 
// it. If there is anything in the queue, ProcessObject() will be called, passing
// it the pointer from the queue.
//-----------------------------------------------------------------------------


#ifndef GThreadedQueue_h
#define GThreadedQueue_h 1

#include "Common/Util/DEBUGOUT.h"

// GTimer
#include "Common/Util/GTimer.h"
// GTypes
#include "GTypes.h"
// GTask
#include "GTask.h"
// GSemaphore
#include "GSemaphore.h"
// GSimpleCommand
#include "Common/Util/GSimpleCommand.h"

class QueueClass;

template <class QueuedObject, class QueueClass>

class GThreadedQueue : public GTask
{
    typedef GSimpleCommand < GThreadedQueue > GQueueTimeoutCommand;

public:

    typedef enum { eIdle, eBusy } eThreadedQueueStates;

      //	Constructor which sets the ID and optionally allows specification of
      //	whether or not the thread should start running right away. If bStart
      //	is passed in as FALSE, the thread will not start running until the
      //	GTask::Start() method is called.
      GThreadedQueue (UINT32 nID, bool bStart = true, UINT32 u32TimeToWait = 0)
		:
		m_bRunning		(false),			// The thread is not initially running.
		m_pMutex		(new GMutex()),
		m_pQueue		(new QueueClass),	
		m_bOneAtATime	(false),
		m_EventCountSemaphore (0),
        m_pQueueTimer(NULL),
	    m_bTimeoutOccurred(false),
    	m_uiID (nID)
      {
         // only create timer if one is requested
        if (u32TimeToWait)
        {
            CreateThreadedQueueTimer( u32TimeToWait );
        }

		// If requested to do so, start the thread.
		if (bStart)
		{
    	  	// Note that the thread is running.
    	  	m_bRunning = true;

			// Call the base class Start() method.
			Start(eJoinable);
		}
      }


      //	Pushes a pointer to an object into the queue. The queue stores pointers
      //	to objects and does not take life-cycle responsibility for the objects
      //	themselves. In other words, it does not delete the objects themselves.
      //	The only exception to this is the method MakeEmpty().
      void Push (QueuedObject* pObject)
      {
		// If the queue exists,
		if (m_pQueue)
		{
			    m_pQueue->PushObject(pObject);
			    m_EventCountSemaphore.Post();
		}
      }

      //	Clears all the pointers from the queue and optionally deletes the objects
      //	which the stored pointers refered to.
      void MakeEmpty (bool bDeleteObjectsReferedTo = true)
      {
		// If the queue exists,
		if (m_pQueue != (QueueClass*)NULL)
		{
			// Call MakeEmpty () on the queue.
			m_pQueue->MakeEmpty (bDeleteObjectsReferedTo);

			// Temporary test to see if semaphore count affects
			// printer diagnostics
			while( 0 != m_EventCountSemaphore.Wait(0) )
			{
			}
		}
      }

      //	Returns TRUE if the queue is empty.
      bool IsEmpty ()
      {
		// If the queue exists,
		if (m_pQueue != (QueueClass*)NULL)
		{
			// Return whether the queue is empty or not.
			return m_pQueue->IsEmpty();
		}
		// Return empty status, if the queue is not there.
		return true;
      }

      //	If this method is passes a value of true, each time the thread wakes up and
      //	checks it's queue, because of CheckQueue() being called, it will process only
      //	one object before going back to waiting. The default behavior for this class
      //	is to process all the objects in the queue, each time it is woken up. Calling
      //	this method with FALSE for the parameter will reset the class back to this
      //	default behaviour.
      void SetOneAtATime (bool bOneAtATime)
      {
		// Set the member variable.
		m_bOneAtATime = bOneAtATime;
      }

      //	This method specifies how long to wait before calling ProcessObject().
      //	Value is in milliseconds.
      void SetQueueTimeOut (UINT32 u32Time)
      {
          if( m_pQueueTimer )
          {
              delete m_pQueueTimer;
              m_pQueueTimer = NULL;
          }

          if (0 != u32Time)
          {
              CreateThreadedQueueTimer( u32Time );
          }
      }

protected:
      //	Destructor.
      virtual ~GThreadedQueue ()
      {
    	// Shutdown and wait for task exit.
    	m_bRunning = false;
    	m_EventCountSemaphore.Post();
    	Join();

		// If the queue exists,
		if (m_pQueue != (QueueClass*)NULL)
		{
			// Delete the queue.
			delete m_pQueue;
			m_pQueue = (QueueClass*)NULL;
		}

		if( m_pQueueTimer )
        {
			delete m_pQueueTimer;
            m_pQueueTimer = NULL;
        }
      }

      //	This method creates the timer that generates "idle" events.
      void CreateThreadedQueueTimer (UINT32 u32Time)
      {
        m_TimeoutCommand.SetCommand( this, &GThreadedQueue::CreateTimeoutMessage );
        m_pQueueTimer = new GTimer ( &m_TimeoutCommand,
							         u32Time,       // wait time
							         true,		    // One shot timer
							         true		    // Start immediately
							        );
      }

      //	Processes any messages found in the queue.
      //	This method has to be implemented in any derived classes.
      virtual void ProcessObject (QueuedObject* pObject)
      {
      }

      //	This virtual method is called when a timeout occurs while waiting for data
      //	to be put on the queue.  The default implementation is empty; subclasses
      //	that make use of the timeout functionality should implement this method.
      virtual void ProcessTimeoutEvent ()
      {
      }

      //	Virtual method executed by the object's thread ONCE after thread execution
      //	begins, PRIOR to start of queued object processing (invocations of Process
      //	Object).  The default implementation is empty.   The intention is for
      //	subclasses to override this "do once" method to perform any initialization
      //	tasks that need to be driven by the object's thread.
      //
      //	**********************************************************************
      //	It is STRONGLY SUGGESTED that subclasses who override this method also
      //	OVERRIDE THE DEFAULT GThreadedQueue BEHAVIOR OF AUTOMATICALLY STARTING
      //	THREAD EXECUTION.  This is done by specifying false for GThreadedQueue's "b
      //	Start" constructor parameter.
      //
      //	This is needed to prevent the possibility of the subclass's Initialize()
      //	method executing BEFORE THE SUBCLASS CONSTRUCTOR COMPLETES.  Otherwise the
      //	subclass's Initialize() method could reference UNINITIALIZED attributes.
      //	Having the default GThreadedQueue behavior be to not auto start would be
      //	better, but there is a lot of existing code that depends on the current
      //	implementation.
      //	**********************************************************************
      virtual void ThreadedQueueInit ()
      {
      }

      //	Internal method which tells the thread what to do when it runs.
      void Run ()
      {
    	  // Make sure there is an associated queue.
    	  if (m_pQueue == (QueueClass*)NULL)
    	  {
    		  return;
    	  }

    	  // Execute the virtual "do once" initialization method.  If not implemented
    	  // by the subclass, it does nothing.
    	  ThreadedQueueInit();

    	  // Pointer used to retrieve objects from the queue.
    	  QueuedObject* pObject = (QueuedObject*)NULL;


    	  // Run until told to stop.
    	  while (m_bRunning)
    	  {
    		  // Lock the mutex.
    		  m_pMutex->Lock();

    		  if (m_pQueueTimer)
    		  {
    			  m_pQueueTimer -> Stop();
    		  }
			  
              if (m_bTimeoutOccurred)
              {
                  m_bTimeoutOccurred = false;
                  ProcessTimeoutEvent();
              }

    		  // If we are not supposed to get items from the queue
    		  // one at a time.
    		  if (m_bOneAtATime == false)
    		  {
    			  // Process all the objects in the queue, removing
    			  // the pointers from the queue as they are processed.
    			  while (!m_pQueue->IsEmpty())
    			  {
    				  // Get the next pointer from the queue.
    				  pObject = m_pQueue->PopObject ();

    				  // If an object was found in the queue,
    				  if (pObject)
    				  {
    					  // Process the object. This method has to be implemented
    					  // in classes derived from this class to be meaningfull.
   						  ProcessObject ( pObject );
    				  }
    			  }
    		  }
    		  else
    		  {
    			  // Process a single object from the queue, removing
    			  // the pointer from the queue as the object is processed.
    			  if (!m_pQueue->IsEmpty())
    			  {
    				  // Get the next pointer from the queue.
    				  pObject = m_pQueue->PopObject ();

    				  // If an object was found in the queue,
    				  if (pObject)
    				  {
						  // Process the object. This method has to be implemented
						  // in classes derived from this class to be meaningfull.
    					  ProcessObject ( pObject );
    				  }
    			  }
    		  }

    		  if (m_pQueueTimer)
    		  {
    			  m_pQueueTimer -> Start();
    		  }

    		  // Unlock the mutex.
    		  m_pMutex->Unlock();

    		  // Wait for a signal to wake up and check again.
    		  // These three calls are all required and must be performed
    		  // in this order to work correctly.
    		  m_EventCountSemaphore.Wait();
    	  }
      }

      //	Represents whether or not the thread is running.
      bool m_bRunning;

      //	Mutex for protecting this classes internal data.
      GMutex* m_pMutex;

      //	The associated queue.
      QueueClass* m_pQueue;

      //	If this variable is TRUE, each time the thread wakes up and checks it's
      //	queue, it will process only one object before going back to waiting.
      //	If this variable is FALSE, it will process all the objects in the queue,
      //	each time it is woken up.
      bool m_bOneAtATime;

  private:
      GThreadedQueue(const GThreadedQueue< QueuedObject,QueueClass > &right);


      //	This method pushes a predefined value TIMER_EVENT on the queue which
      //	indicates the idle timer has expired.
      void CreateTimeoutMessage ()
      {
          m_pMutex->Lock();
          m_bTimeoutOccurred = true;
          m_pMutex->Unlock();
		  m_EventCountSemaphore.Post();
      }

      //	This attribute keeps track of the number of items in the queue.  The queues
      //	thread blocks on this semaphore to know when it has something in the queue
      //	to process.
      GSemaphore m_EventCountSemaphore;

      //	This attribute holds the command to execute if using a timeout.
      GQueueTimeoutCommand m_TimeoutCommand;

      //	Optional timer for generating "idle" events.
      GTimer* m_pQueueTimer;

	  bool m_bTimeoutOccurred;

      UINT32 m_uiID;
};

#endif

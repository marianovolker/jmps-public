#include "SPOT/SPOTProtocolInterface/GAbsSPOTSessionListener.h"

GAbsSPOTSessionListener::GAbsSPOTSessionListener ()
  :  m_bRunning(false),			// The thread is not initially running.
     m_pMutex(new GMutex()),
     m_EventCountSemaphore (0),
     m_pSessionListenerTimer(NULL),
     m_bTimeoutOccurred(false)
{
  // only create timer if one is requested
  CreateSessionListenerTimer( 50 );

  // Note that the thread is running.
  m_bRunning = true;

  // Call the base class Start() method.
  Start(eJoinable);
}

//	Destructor.
GAbsSPOTSessionListener::~GAbsSPOTSessionListener ()
{
  // Shutdown and wait for task exit.
  m_bRunning = false;
  m_EventCountSemaphore.Post();
  Join();

  if( m_pSessionListenerTimer )
  {
    delete m_pSessionListenerTimer;
    m_pSessionListenerTimer = NULL;
  }

  // clean up the queue
  MakeEmpty();
  delete m_pMutex;
}

//	Pushes a pointer to an object into the queue. The queue stores pointers
//	to objects and does not take life-cycle responsibility for the objects
//	themselves. In other words, it does not delete the objects themselves.
//	The only exception to this is the method MakeEmpty().
void GAbsSPOTSessionListener::Push (GSPOTMessage* pMsg)
{
   GSPOTMessage* pMsgCopy = new GSPOTMessage(pMsg);
   m_pQueue.push(pMsgCopy);
   m_EventCountSemaphore.Post();
   // pMsgCopy will be deleted later when "popped" off the queue
}

//	Clears all the pointers from the queue and optionally deletes the objects
//	which the stored pointers refered to.
void GAbsSPOTSessionListener::MakeEmpty ()
{
   // pop all the elements off the queue until it's empty
   while (!m_pQueue.empty())
   {
      GSPOTMessage* ptr = m_pQueue.front();
      m_pQueue.pop();
      delete ptr;
   }
}

//	This method creates the timer that generates "idle" events.
void GAbsSPOTSessionListener::CreateSessionListenerTimer (UINT32 u32Time)
{
   m_ListenerTimeoutCommand.SetCommand( this, &GAbsSPOTSessionListener::CreateTimeoutMessage );
   m_pSessionListenerTimer = new GTimer (  &m_ListenerTimeoutCommand,
                                           u32Time,       // wait time
                                           true,	  // One shot timer
                                           true           // Start immediately
                                        );
}

//	Internal method which tells the thread what to do when it runs.
void GAbsSPOTSessionListener::Run ()
{
   // Run until told to stop.
   while (m_bRunning)
   {
      // Lock the mutex.
      m_pMutex->Lock();

      if (m_pSessionListenerTimer)
      {
         m_pSessionListenerTimer -> Stop();
      }
			  
      if (m_bTimeoutOccurred)
      {
         m_bTimeoutOccurred = false;
         ProcessTimeoutEvent();
      }

      // Process all the objects in the queue, removing
      // the messages from the queue as they are processed.
      while (!m_pQueue.empty())
      {
         // Get the next pointer from the queue.
         GSPOTMessage* pMsg = m_pQueue.front ();

         // If an object was found in the queue,
         if (pMsg)
         {
            // Process the object. This method has to be implemented
            // in classes derived from this class to be meaningfull.
            ProcessMessage(pMsg);
            // actually remove the object from the queue and delete
            m_pQueue.pop();

            delete pMsg;
         }
      }

      if (m_pSessionListenerTimer)
      {
         m_pSessionListenerTimer -> Start();
      }

      // Unlock the mutex.
      m_pMutex->Unlock();

      // Wait for a signal to wake up and check again.
      // These three calls are all required and must be performed
      // in this order to work correctly.
      m_EventCountSemaphore.Wait();
   }
}

//	This method pushes a predefined value TIMER_EVENT on the queue which
//	indicates the idle timer has expired.
void GAbsSPOTSessionListener::CreateTimeoutMessage ()
{
   m_pMutex->Lock();
   m_bTimeoutOccurred = true;
   m_pMutex->Unlock();
   m_EventCountSemaphore.Post();
}


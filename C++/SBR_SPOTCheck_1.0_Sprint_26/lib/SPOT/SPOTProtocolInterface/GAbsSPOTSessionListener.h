#ifndef GAbsSPOTSessionListener_h
#define GAbsSPOTSessionListener_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SPOTProtocolInterface/GSPOTMessage.h"

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

#include <queue>

class GAbsSPOTSessionListener : public GTask
{
  public:
    typedef GSimpleCommand < GAbsSPOTSessionListener > GListenerTimeoutCommand;

    //	Constructor which sets the ID and optionally allows specification of
    //	whether or not the thread should start running right away. If bStart
    //	is passed in as FALSE, the thread will not start running until the
    //	GTask::Start() method is called.
    GAbsSPOTSessionListener ();

    //	Pushes a pointer to an object into the queue. The queue stores pointers
    //	to objects and does not take life-cycle responsibility for the objects
    //	themselves. In other words, it does not delete the objects themselves.
    //	The only exception to this is the method MakeEmpty().
    void Push (GSPOTMessage* pMsg);

    //	Clears all the pointers from the queue and optionally deletes the objects
    //	which the stored pointers refered to.
    void MakeEmpty ();

    //	Returns TRUE if the queue is empty.
    bool IsEmpty () { return m_pQueue.empty(); };

    /*!
     * \brief [Receive error notification from transport layer]
     *
     * This methods receives a notification from the transport layer
     * that indicates a recoverable error in communication to/from SPOT.
     */
    virtual void NotifyError(GSpotDefs::eSPOTTransportLayerError eSessionError) = 0;


  protected:
    //	Destructor.
    virtual ~GAbsSPOTSessionListener ();

    //	This method creates the timer that generates "idle" events.
    void CreateSessionListenerTimer (UINT32 u32Time);

    //	Processes any messages found in the queue.
    //	This method has to be implemented in any derived classes.
    virtual void ProcessMessage (GSPOTMessage* pMessage) = 0;

    //	This virtual method is called when a timeout occurs while waiting for data
    //	to be put on the queue.  The default implementation is empty; subclasses
    //	that make use of the timeout functionality should implement this method.
    virtual void ProcessTimeoutEvent () = 0;

    //	Internal method which tells the thread what to do when it runs.
    void Run ();

    //	Represents whether or not the thread is running.
    bool m_bRunning;

    //	Mutex for protecting this classes internal data.
    GMutex* m_pMutex;

    //	The associated queue.
    std::queue<GSPOTMessage*> m_pQueue;
    
    //	This attribute keeps track of the number of items in the queue.  The queues
    //	thread blocks on this semaphore to know when it has something in the queue
    //	to process.
    GSemaphore m_EventCountSemaphore;

  private:
    GAbsSPOTSessionListener(const GAbsSPOTSessionListener &right);

    //	This method pushes a predefined value TIMER_EVENT on the queue which
    //	indicates the idle timer has expired.
    void CreateTimeoutMessage ();

    //	This attribute holds the command to execute if using a timeout.
    GListenerTimeoutCommand m_ListenerTimeoutCommand;

    //	Optional timer for generating "idle" events.
    GTimer* m_pSessionListenerTimer;

    bool m_bTimeoutOccurred;
};

#endif


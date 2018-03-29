// File: GTimer.h

#ifndef GTIMER_H
#define GTIMER_H 1


// GSystem
#include <GSystem.h>
// GTask
#include "GTask.h"
// GCondition
#include "GCondition.h"
// GTypes
#include "GTypes.h"
// GAbsCommand
#include "Common/Util/GAbsCommand.h"


//	GTimer is a software timer that can be used to drive execution of a generic
//	"command" after a specified interval has passed.
//
//	The design of GTimer centers around a "command" and a thread.  The timer is
//	separated from it's specific application via the well known "Command
//	Pattern".  The timer is supplied with an interval and a command which serves
//	as an abstraction for an action/request.  When (if) the timer's interval
//	expires, the timer's thread drives execution of the command.  The
//	implementation of GTimer uses the GTask to avoid
//	dependency on any specific operating system.
//
//	GTimer provides methods for typical timer manipulation such as:  start, stop,
//	reset, and setting the interval.  One GTimer trait worth mentioning is that
//	an instance is either "One Shot" or "Repeating".  A "One Shot" GTimer will
//	automatically disable itself after firing once.  A "Repeating" timer will
//	continue to run after each expiration until it is explicitly stopped.
//
//	The following is an example of typical GTimer usage.  This shows how a class
//	may create a timer attribute that invokes one of the class's methods on the
//	specific instance.  When viewing in Rose, WIDEN THE DOCUMENATION WINDOW to
//	make this readable.
//
//	Command and timer attributes from class definition.  Note that the command
//	can be automatic or dynamically allocated.  All instances of GTimer MUST BE
//	DYNAMICALLY ALLOCATED!
//
//	class GXyzProtocol {
//	  ...
//	// ACK timeout command attribute.
//	GSimpleCommand<GXyzProtocol> m_ACKTimeoutCmd;
//
//	// ACK timeout timer.
//	GTimer* m_pACKTimeoutTimer;
//	  ...
//	};
//
//	Intialize the command and timer in the class's constructor:
//
//	GXyzProtocol::GXyzProtocol()
//	{
//	     m_ACKTimeoutCmd.SetCommand(this,  // class instance
//	          &GXyzProtocol::ACKTimeout);  // class method
//
//	     m_pACKTimeoutTimer = new GTimer(&m_ACKTimeoutCmd,  // command
//	          1000,    // interval in millisec
//	          true,    // "one shot" timer
//	          false);  // Don't auto start
//	}
//
//	Here the timer is started after transmitting a message.  The idea is that if
//	the timer expires it signals a timeout with regard to a response from the
//	message:
//
//	GXyzProtocol::SendMessage(UINT8* pu8Msg)
//	{
//	     // Do whatever to transmit the message...
//
//	     m_pACKTimeoutTimer->Start();
//	}
//
//	Here the timer is stopped when a response is recieved.
//
//	GXyzProtocol::ProcessACK()
//	{
//	     ...
//	     m_pACKTimeoutTimer->Stop();
//	}
//
//	This shows what action might be taken if no response is received within the
//	specified time interval:
//
//	GXyzProtocol::ACKTimeout()
//	{
//	     // Do whatever to resend the message...
//
//	    // Restart the timer -- it was setup as a "one shot".
//	     m_pACKTimeoutTimer->Start()
//	}

class GTimer : public GTask
{
  public:

    class GConditionalMutexLock 
    {
      public:
          //	Constructor.  Conditionally locks the specified mutex based on the boolean
          //	argument.
          //
          //	Arguments:
          //
          //	Mutex:  If bLockMutex is true, this mutex will be locked at construction and
          //	unlocked at destruction.
          //
          //	bLockMutex:  Determines whether the specified mutex is locked/unlocked.
          GConditionalMutexLock (GMutex& Mutex, bool bLockMutex);

          //	Desstructor.  Conditionally unlocks the specified mutex based on the m_bLock
          //	Mutex.
          ~GConditionalMutexLock ();

      private:
          GConditionalMutexLock(const GConditionalMutexLock &right);

          const GConditionalMutexLock & operator=(const GConditionalMutexLock &right);

        // Data Members for Class Attributes

          //	Associated mutex.  This mutex will be conditionally locked at construction and
          //	unlocked at destruction based on m_bLockMutex.
          GMutex& m_Mutex;

          //	This flag determines whether the associated mutex is locked at construction
          //	and unlocked at destruction.
          bool m_bLockMutex;

    };

      //	Default constructor.
      GTimer ();

      //	Constructor which allows the user to set the associated command,
      //	seconds and nanoseconds to delay, whether the timer kicks off only
      //	once before stopping or repeats until turned off, and whether or not
      //	to start the timer right away.
      GTimer (const GAbsCommand* pCommand, UINT32 u32MillisecondInterval, bool bOneShot, bool bAutoStart);

      ~GTimer ();

      //	Sets the associated command.
      void SetCommand (const GAbsCommand* pCommand);

      //	Gets the associated command.
      GAbsCommand* GetCommand ();

      //	Sets the interval between calls to the associated command.
      void SetInterval (UINT32 u32Milliseconds);

      //	Returns the current setting for the interval between calls
      //	to the associated command.
      UINT32 GetInterval ();

      //	Sets whether the timer kicks off only once before stopping or
      //	repeats until turned off
      void SetOneShot (bool bOneShot);

      //	Returns a value indicating whether the timer kicks off only once
      //	before stopping or repeats until turned off.
      bool GetOneShot ();

      //	Starts the timer.
      bool Start ();

      //	Stops the timer.
      bool Stop ();

      //	Returns true if the timer is running.
      bool IsRunning ();

      //	Resets the timer.
      void Reset ();

      //	Blocks the calling thread for the timer interval.
      void Block ();

      //	Removes the associated command. Also stops the timer, if it was running.
      void RemoveEvent ();


  private:
      GTimer(const GTimer &right);
      const GTimer & operator=(const GTimer &right);


      //	Encapsulates functionality common to the various explicit constructors.
      void CommonConstructor ();

      //	Override the GTask base class method.  The timer's thread will drive
      //	execution of this method after the timer is created.  This method contains
      //	the algorithm for timer operation.
      void Run ();

      //	Internal helper method used to determine whether the calling thread is the
      //	timer's thread.
      //
      //	Returns:
      //
      //	Boolean value which indicates whether the calling thread is the timer's
      //	thread (true) or not (false).
      bool IsCallerTimersThread ();

    // Data Members for Class Attributes

      //	This mutex is used in conjunction with m_ConditionVar to synchronize the
      //	timer's internal thread with client thread(s).  The mutex is locked prior to
      //	any operations on m_ConditionVar (signal or wait).
      //
      //	All public methods lock this mutex prior to executing regardless of whether
      //	they manipulate the condition variable.  This in itslelf prevents multiple
      //	client threads from operating on the timer instance simultaneously.
      //
      //	In addition, the timer's internal thread keeps the mutex locked except when
      //	stopped or counting down to expiration (blocked on wait() or timedwait() on
      //	the condition variable respectively).  This prevents any manipulation of the
      //	timer by client threads while the timer thread is "running" (updating
      //	attributes, executing its associated command, etc.).
      //
      //	This mutex is used to initialize m_ConditionVar.  it is *IMPERATIVE* THAT
      //	THIS ATTRIBUTE BE BEFORE m_ConditionVar IN THE CLASS DEFINITION!!
      GMutex m_Mutex;

      //	This condition variable is used in conjunction with m_Mutex to control
      //	operation of the timer's internal thread.  The mutex  is locked prior to any
      //	operation on the condition variable.
      //
      //	When the timer is in a "stopped" state, its thread is blocked in a wait()
      //	operation on this condition variable.  When the timer is "running" (counting
      //	down to expiration),  it's thread is blocked in a timedwait() operation on
      //	the condition variable.  Client threads signal the condition variable to
      //	start and stop the timer by invoking the appropriate methods (Start, Stop,
      //	etc.).
      GCondition m_ConditionVar;

      //	TRUE if the timer is running.
      volatile bool m_bTimerRunning;

      //	TRUE if the Stop() command is being executed. This avoid the ever running thread
      //   when the interval is set to 0ms
      volatile bool m_bStopInProgress;

      //	If true the timer will disable itself after firing once.  Otherwise the
      //	timer will be "repeating" and  will continue to run after each expiration
      //	until it is explicitly stopped.
      bool m_bOneShot;

      //	The associated command.  When (if) the timer fires, this command is executed.
      GAbsCommand* m_pCommand;

      //	Timer interval in milliseconds.
      UINT32 m_u32MillisecondInterval;

      //	This is a flag indicating that the thread is to shut down.
      //
      //	It's purpose is to let the thread know that it is being destroyed, causing
      //	it to return from its Run() method.
      volatile bool m_bShuttingdown;

      //	This flag indicates whether the timer's thread has begun execution (entered
      //	method run_undetached).  This information is used to block the thread
      //	constructing the timer object until the timer's thread has actually started.
      volatile bool m_bThreadStarted;

      GCondition * m_pInitConditionVar;
      static GMutex mst_InitMutex;
};


// Class GTimer::GConditionalMutexLock 

inline GTimer::GConditionalMutexLock::GConditionalMutexLock (GMutex& Mutex, bool bLockMutex)
      : m_Mutex(Mutex), m_bLockMutex(bLockMutex)
{
	if (m_bLockMutex)
	{
		m_Mutex.Lock();
	}
}


inline GTimer::GConditionalMutexLock::~GConditionalMutexLock ()
{
	if (m_bLockMutex)
	{
		m_Mutex.Unlock();
	}
}


// Class GTimer 

inline bool GTimer::IsCallerTimersThread ()
{
	GTaskId caller_id = GSystem::GetTaskId();

	return (caller_id == GetId());
}


#endif

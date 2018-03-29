// GTaskImpl.h
//
// Linux implementation of the GTask class.


#ifndef GTaskImpl_h
#define GTaskImpl_h

//#include <GWindows.h>
#include <GTaskTypes.h>
#include <GMutex.h>
#include <GCondition.h>
#include <pthread.h>


// GTaskImpl

class GTaskImpl
{
protected:
	// Constructor
	GTaskImpl(eTaskPriority Priority=ePriorityNormal);

	// Start() - Creates the task and calls the Run method.
	void Start(eTaskRunMode RunMode);

	// Join() - Wait for task to exit.
	void Join(void);

	// Detach() - Cancel task's joinable status.
	void Detach(void);

	// GetId() - Return the tasks id.
	GTaskId GetId(void);

protected:
	// Destructor
	virtual ~GTaskImpl(void);

	// Task Id.
	GTaskId m_Id;
	pthread_t m_gImplementationId;

	// Task priority.
	eTaskPriority m_Priority;

	// Task run mode.
	eTaskRunMode m_RunMode;

private:
	// Init() - Task initialization.
	void Init(void);

	// Run() - The task execution function. Must be provided
	//         by the application task classes.
	virtual void Run() = 0;

	// Exit() - Task exit method. Returns run mode at exit.
	eTaskRunMode Exit(void);

	// Task exclusion mutex.
	GMutex m_Mutex;

	// Task init, join and exit conditions.
	GCondition m_InitCondition;
	GCondition m_JoinCondition;
	GCondition m_ExitCondition;

private:
	// Task starter function.
	static void * TaskMain(void *);

private:
	// Prohibit GTaskImpl copy.
	GTaskImpl(const GTaskImpl &);
	GTaskImpl& operator=(const GTaskImpl &);
};


#endif

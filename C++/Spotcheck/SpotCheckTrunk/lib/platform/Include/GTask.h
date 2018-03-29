// GTask.h


#ifndef GTask_h
#define GTask_h


#include <GTaskTypes.h>
#include <GTaskImpl.h>


// GTask
//
// Standard application task/thread interface.
//
// Construction:
//    All objects that derive from GTask must be created dynamically.
//    GTask is a abstract class where the derived class must implement the task Run() method.
//    A separate thread of execution is started for the Run() method by calling the Start() method.
//
// Destruction: 
//    Tasks running in detached mode will have their objects deleted automatically upon return from
//    the Run() method. Tasks running in joinable mode will not be automatically deleted. These
//    tasks will exit and wait for the task owner to join the exited task. A joinable task object
//    should not be deleted until the Join() method returns indicating the Run() has exited.
//
// Application:
//    The destructors of all classes that derive from GTask, and implement the Run() method, should
//    cause its Run() method to exit and call the GTask Join() method to wait for exit confirmation
//    before allowing the destruction to continue. With this method, task derived objects can be
//    allocated and deleted as usual at the application level without additional task synchronization.

class GTask : private GTaskImpl
{
public:
	// Constructor
	GTask(eTaskPriority Priority=ePriorityNormal) : GTaskImpl(Priority)
	{
	}

	// Start() - Creates the task and calls the Run method.
	//   eRunMode - Task run mode, default=eDetached
	void Start(eTaskRunMode RunMode)
	{
		GTaskImpl::Start(RunMode);
	}

	// Join() - Wait for task to exit.
	void Join(void)
	{
		GTaskImpl::Join();
	}

	// Detach() - Cancel task's joinable status.
	void Detach(void)
	{
		GTaskImpl::Detach();
	}

	// GetId() - Return the tasks id.
	GTaskId GetId(void)
	{
		return GTaskImpl::GetId();
	}

protected:
	// Destructor - By being protected, only dynamically allocated
	// instances of the class can be created. This prevents issues
	// related to object destruction while the task may still be running.
	virtual ~GTask(void)
	{
	}

private:
	// Run() - The task execution function. Must be provided
	//         by the application task classes.
	virtual void Run() = 0;

	// Prohibit GTask copy.
	GTask(const GTask &right);
	GTask& operator=(const GTask &right);
};


#endif

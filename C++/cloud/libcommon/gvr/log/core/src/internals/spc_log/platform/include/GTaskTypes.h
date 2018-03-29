// GTaskTypes.h


#ifndef GTaskTypes_h
#define GTaskTypes_h


// Define task priority levels.

typedef enum
{
	ePriorityLow,
	ePriorityNormal,
	ePriorityHigh
} eTaskPriority;


// Define task run modes.

typedef enum
{
	eJoinable,
	eDetached
} eTaskRunMode;

typedef signed long int GTaskId;
extern GTaskId const eInvalidTaskId;

#endif

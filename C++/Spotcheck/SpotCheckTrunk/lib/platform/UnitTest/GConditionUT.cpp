#include "GConditionUT.h"
#include "GMutex.h"
#include "GCondition.h"
#include "GTask.h"


class ConditionTask
	:	public GTask
{
public:
	ConditionTask(GConditionUT *pConditionUT);
private:
	virtual ~ConditionTask(void);
	virtual void Run();

	GConditionUT * m_pConditionUT;

	ConditionTask(const ConditionTask &);
	ConditionTask & operator=(const ConditionTask &);
};

ConditionTask::ConditionTask(GConditionUT *pConditionUT)
             : m_pConditionUT(pConditionUT)
{
}

ConditionTask::~ConditionTask(void)
{
}

void ConditionTask::Run()
{
	if (m_pConditionUT)
	{
		GMutex * pMutex(m_pConditionUT->m_pMutex);
		GCondition * pCondition(m_pConditionUT->m_pCondition);

		if (pMutex && pCondition)
		{
			{
				GMutexLock oScopedLock (*pMutex);
				++(m_pConditionUT->m_ulCounter);
				pCondition->Signal();
			}
		}
	}
}

GConditionUT::GConditionUT()
	:	m_ulCounter(0),
	 	m_bDone(false),
	 	m_pMutex(NULL),
	 	m_pCondition(NULL)
{
}

GConditionUT::~GConditionUT()
{

}

void GConditionUT::SetUp()
{
	m_ulCounter = 0;
	m_bDone = false;
	m_pMutex = new GMutex();
	if (m_pMutex)
	{
		m_pCondition = new GCondition(m_pMutex);
	}
}

void GConditionUT::TearDown()
{
	delete m_pCondition;
	m_pCondition = NULL;
	delete m_pMutex;
	m_pMutex = NULL;
	m_bDone = false;
	m_ulCounter = 0;
}

TEST_F(GConditionUT, TestCondition)
{
	static int const eTasksToTest(10);
	if (m_pMutex && m_pCondition)
	{
		unsigned long int uliTaskCount(0);
		{
			GMutexLock oScopedLock (*m_pMutex);

			for( int iI=0; iI < eTasksToTest; ++iI )
			{
				ConditionTask* pTask = new ConditionTask(this);
				if (pTask)
				{
					pTask->Start(eDetached);
					if (pTask->GetId() != eInvalidTaskId)
					{
						++uliTaskCount;
					}
				}
			}

			EXPECT_EQ(eTasksToTest,uliTaskCount);

			while (m_ulCounter < uliTaskCount)
			{
				m_pCondition->Wait(10);
			}
		}
	}
}

#include "GTask.h"
#include "GSystem.h"

#include "GSystemUT.h"


#include <string>
#include <vector>
#include <algorithm>
using namespace std;


class SystemTask
	:	public GTask
{
public:
	SystemTask(GSystemUT *pSystemUT);
private:
	virtual ~SystemTask(void);
	virtual void Run();

	GSystemUT * m_pSystemUT;

	SystemTask(const SystemTask &);
	SystemTask & operator=(const SystemTask &);
};

SystemTask::SystemTask(GSystemUT *pSystemUT)
                : m_pSystemUT(pSystemUT)
{
}

SystemTask::~SystemTask(void)
{
}

void SystemTask::Run()
{
	if (m_pSystemUT)
	{
		EXPECT_EQ(this->GetId(),GSystem::GetTaskId());

		GSystem::Sleep(1000);

		GMutex * pgMutex(&(m_pSystemUT->m_gMutex));
		GCondition * pgCondition(&(m_pSystemUT->m_gCondition));
		if (pgMutex && pgCondition)
		{
			{
				GMutexLock gScopedLock(*pgMutex);
				(m_pSystemUT->m_bInit) = true;
				pgCondition->Signal();
			}
			{
				bool bGO(false);
				for (unsigned int iI = 0; bGO == false; ++iI)
				{
					if (iI % 10000)
					{
						GSystem::Yield();
						GMutexLock gScopedLock(*pgMutex);
						if ((m_pSystemUT->m_bDone) == true)
						{
							bGO = true;
						}
					}
				}
			}
			{
				GMutexLock gScopedLock(*pgMutex);
				(m_pSystemUT->m_bFinish) = true;
				pgCondition->Signal();
			}
		}
	}
}

GSystemUT::GSystemUT()
	:	m_bInit(false),
		m_bDone(false),
		m_bFinish(false),
	 	m_gMutex(),
	 	m_gCondition(&m_gMutex)
{
}

GSystemUT::~GSystemUT()
{
}

void GSystemUT::SetUp()
{
	m_bInit = false;
	m_bDone = false;
	m_bFinish = false;
}

void GSystemUT::TearDown()
{
	m_bInit = false;
	m_bDone = false;
	m_bFinish = false;
}

TEST_F(GSystemUT, TestSystemTask)
{
	SystemTask* pTask = new SystemTask(this);
	if (pTask)
	{
		pTask->Start(eDetached);
		EXPECT_NE(pTask->GetId(),eInvalidTaskId);

		{
			GMutexLock gScopedLock(m_gMutex);
			while (m_bInit == false)
			{
				m_gCondition.Wait(-1);
			}
		}
		GSystem::Yield();
		{
			GMutexLock gScopedLock(m_gMutex);
			m_bDone = true;
		}
		{
			GMutexLock gScopedLock(m_gMutex);
			while (m_bFinish == false)
			{
				m_gCondition.Wait(-1);
			}
		}
	}
}

TEST_F(GSystemUT, TestGetTickCount)
{
	unsigned long count1 = GSystem::GetTickCount();
	GSystem::Sleep(100);
	unsigned long count2 = GSystem::GetTickCount();
	ASSERT_NE(count1, count2);
}

TEST_F(GSystemUT, ShouldHandlePathSlashes)
{
	const static char* TestFileHelperPath = "HelpFiles/test.xml";
	const std::string RenamedFile = std::string(TestFileHelperPath) + std::string(".new");

	//Verify file exist works
	EXPECT_TRUE(GSystem::FileExist(TestFileHelperPath));

	//Verify CopyFile handles path correctly
	//Create temp file for rename
	EXPECT_TRUE(GSystem::CopyFile(TestFileHelperPath, RenamedFile));

	//Cleanup
	EXPECT_TRUE(GSystem::RemoveFile(RenamedFile.c_str()));

	//Create temp file for rename
	std::string TempFileForRename = std::string(TestFileHelperPath) +  std::string(".ForRename");
	EXPECT_TRUE(GSystem::CopyFile(TestFileHelperPath, TempFileForRename));


	//Verify that rename works with the slashes in the path
	EXPECT_TRUE(GSystem::RenameFile(TempFileForRename.c_str(), RenamedFile.c_str()));

	//Cleanup
	EXPECT_TRUE(GSystem::RemoveFile(RenamedFile.c_str()));
}

TEST_F(GSystemUT, TestByteOrderIsLE)
{
	// Expect unit test platform to be little endian.
	EXPECT_TRUE(GSystem::HostIsLittleEndian());
	EXPECT_FALSE(GSystem::HostIsBigEndian());
}

TEST_F(GSystemUT, GetDirShouldWorkWithOrWithoutSlashAtTheEnd)
{
	std::list < std::pair <std::string, bool> > entryList ;

	const int preStoredNumberOfHelpFilesInDir = 4;

	entryList = GSystem::GetDirEntryList("HelpFiles/DirHelper/");
	EXPECT_EQ(preStoredNumberOfHelpFilesInDir, entryList.size());

	entryList = GSystem::GetDirEntryList("HelpFiles/DirHelper");
	EXPECT_EQ(preStoredNumberOfHelpFilesInDir, entryList.size());

	entryList = GSystem::GetDirEntryList("./HelpFiles/DirHelper");
	EXPECT_EQ(preStoredNumberOfHelpFilesInDir, entryList.size());

	vector<string> dirFiles;
	for(std::list < std::pair <std::string, bool> >::iterator iter = entryList.begin(); iter != entryList.end(); iter++)
	{
		dirFiles.push_back((*iter).first);
	}

	EXPECT_NE(dirFiles.end(), std::find(dirFiles.begin(), dirFiles.end(), "."));
	EXPECT_NE(dirFiles.end(), std::find(dirFiles.begin(), dirFiles.end(), ".."));
	EXPECT_NE(dirFiles.end(), std::find(dirFiles.begin(), dirFiles.end(), "dir_helper_1.xml"));
	EXPECT_NE(dirFiles.end(), std::find(dirFiles.begin(), dirFiles.end(), "dir_helper_2.xml"));
}
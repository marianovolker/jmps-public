/*
 * GSelfDeleteUT.cpp
 *
 *  Created on: March 13 2014
 *  Author: Sayyed Mohammad
 */
#include <cstring>

#include "GSelfDeleteUT.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

#include "Common/Logger/ConfigLabels.h"
#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"

using namespace GSpotDefs;
//using namespace std;

int DeleteFolder(const char *dirname)
{

	DIR *dir;
	struct dirent *entry;
	char path[1024];

	if (path == NULL)
	{
		fprintf(stderr,"DeleteFolder path NULL\n");
		return -1;
	}
	dir = opendir(dirname);
	if (dir == NULL)
	{
		fprintf(stderr,"DeleteFolder dir NULL\n");
		return -1;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
		{
			snprintf(path, (size_t) 1024, "%s/%s", dirname, entry->d_name);
			fprintf(stderr,"Folder entry : %s\n",entry->d_name);
			if (entry->d_type == DT_DIR)
			{
				fprintf(stderr,"DeleteFolder recursive dir NULL\n");
				DeleteFolder(path);

			}
			else
			{
				unlink(path);
				fprintf(stderr,"DeleteFolder unlink :%s\n",path);
			}
		}

	}
	closedir(dir);

	return rmdir(dirname);
}




bool SelfDelete(int iRomfsNumber, const char* pcRomfsDir, const char* pcLogFilePath, const char* pcSpotCheckfsPath)
{
	char pcNewRootPath[512]={'\0'};

	getcwd(pcNewRootPath, 508);
	sprintf(pcNewRootPath, "%s%s",pcNewRootPath,"/TestFiles");
	std::cout <<pcNewRootPath<< std::endl ;
	//std::cout<< chroot(pcNewRootPath)<<std::endl;

	char pcSysmanKey[20]        = {"\0"};
	char pcSysmanConfig[1024]   = {'\0'};
	char pcTmpSysman[1024]      = {'\0'};
	char pcVersionFilePath[1024]= {'\0'};
	char pcRomfsFilePath[1024]  = {'\0'};
	char pcRomfsSysmanPath[1024]= {'\0'};
	char pcSPCIniPath[1024]     = {'\0'};
	char pcLogFile[1024]        = {'\0'};

	sprintf(pcSysmanConfig,"%s%s%s", pcNewRootPath, pcConfigPathM3,"sysman.ini");
	sprintf(pcTmpSysman,"%s%s%s",pcNewRootPath, pcTmpPathM3,"sysman.ini");
	sprintf(pcSysmanKey, pcSysmanKeyholder,iRomfsNumber);
	sprintf(pcVersionFilePath, "%s%sversion-ROMfs%d",pcNewRootPath, pcConfigPathM3,iRomfsNumber);

	sprintf(pcRomfsFilePath, "%s%sROMfs%d",pcNewRootPath, pcRomfsDir,iRomfsNumber);

	sprintf(pcRomfsSysmanPath, "%s%ssysman-ROMfs%d.ini",pcNewRootPath, pcConfigPathM3,iRomfsNumber);
	sprintf(pcSPCIniPath, "%s%s%s",pcNewRootPath, pcConfigPathM3,INI_SPOTCHECK_CONFIG_DEFAULT_FILE);
	sprintf(pcLogFile,"%s%s",pcNewRootPath,pcLogFilePath);


	int iret = remove(pcVersionFilePath);
	fprintf(stderr,"Removing file |%s| answer: %d\n",pcVersionFilePath,iret);

	iret = remove(pcRomfsFilePath);
	fprintf(stderr,"Removing file |%s| answer: %d\n",pcRomfsFilePath,iret);

	iret = remove(pcRomfsSysmanPath);
	fprintf(stderr,"Removing file |%s| answer: %d\n",pcRomfsSysmanPath,iret);

	//Remove  SPOTCheck.ini
	iret = remove(pcSPCIniPath);
	fprintf(stderr,"Removing file |%s| answer: %d\n",pcSPCIniPath,iret);

	//For deleting the log file
	iret =remove(pcLogFile);
	fprintf(stderr,"Removing file |%s| answer: %d\n",pcLogFile,iret);

	//Deleting spotcheckfs
	iret =DeleteFolder(pcSpotCheckfsPath);
	fprintf(stderr,"Removing Folder |%s| answer: %d\n",pcSpotCheckfsPath,iret);


	return true;

}


GSelfDeleteUT::GSelfDeleteUT()
{
}

GSelfDeleteUT::~GSelfDeleteUT()
{
}

void GSelfDeleteUT::SetUp()
{
}

void GSelfDeleteUT::TearDown()
{
}

TEST_F(GSelfDeleteUT, TestStates)
{

	EXPECT_TRUE(SelfDelete(6,pcRomfsDirM3, "/jffs2/logs/SPOTCheck.log", "/jffs2/storage/spotcheckfs"));

}


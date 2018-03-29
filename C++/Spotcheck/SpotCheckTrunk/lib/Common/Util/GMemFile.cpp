//	=============================================================================
//
//	File: GMemFile.cpp
//
//	Description: Dynamic memory implementation with file based backup.
//
//	Created: 1/2/2007
//
//	Author: Chris Fleming
//
//	=============================================================================


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Common/Util/GMemFile.h"
#include "Common/Util/GMD5.h"
#include "Common/Util/GSimpleCommand.h"

#include "Common/Util/DEBUGOUT.h"

#include "Common/ConfigFile/GConfigSys.h"


// GMemFile static declaration/initialization.
GMutex GMemFile::m_Mutex;

// GMemFile Class Implementation

GMemFile::GMemFile(const char *Name,int Size)
{
	// Save name and size.
	m_Size = Size;

	m_FilePath = GConfigSys::GetSoleInstance()->GetVarDir();
	m_FilePath += "mem/";
	m_FilePath += Name;

   // Dynamically allocate memory.
	m_pMem = malloc(Size);
	if (!m_pMem)
	{
		LOG(eCommon, eError, "GMemFile::GMemFile(): Memory allocation failed. Name=%s, Size=%d\n", Name, Size);
	}

	// Create timer process to periodically save simulated NVRAM.
	GSimpleCommand<GMemFile>* pCommand;
	pCommand = new GSimpleCommand<GMemFile>(this, &GMemFile::Dump);
	if (pCommand != NULL)
	{
		// Set standard 1 minute timer to flush data to disk.
		m_pTimer = new GTimer (pCommand, MEMFILE_AUTOSAVE_TIME, false, true);
	}
	else
	{
		m_pTimer = NULL;
	}

   // Attempt to load previous contents.
	if (!Load())
	{
		// Restore any backup copy available.
		Restore();

		// Attempt to load restored copy.
		Load();
	}
}


GMemFile::~GMemFile()
{
	// Delete timer.
	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}

	// Last chance save.
	Dump();

	// Delete memory.
	if (m_pMem)
	{
		free(m_pMem);
		m_pMem = NULL;
	}
}


void *GMemFile::GetMemPtr()
{
	return m_pMem;
}


void GMemFile::Save()
{
	// Reset autosave timer.
	if (m_pTimer)
	{
		m_pTimer->Reset();
	}

	// Force dump.
	Dump();

	return;
}


bool GMemFile::Load()
{
	void*  pMem;
	GMD5   MD5;
	UINT8* pMD5Sum;
	FILE*  pFp;
	bool   bRetVal = false;

	// Wait for mutex to reduce memory requirements.
	m_Mutex.Lock();

	// Create temp copy of memory area.
	pMem = malloc(m_Size);
	if (pMem != NULL)
	{
		// Allocate MD5Sum buffer.
		pMD5Sum = (UINT8 *)malloc(MD5.GetHashLength());
		if (pMD5Sum != NULL)
		{
			// Open memory file if present.
			pFp = fopen(m_FilePath.c_str(), "rb");
			if (pFp != NULL)
			{
				// Read checksum and data.
				fread(pMD5Sum, MD5.GetHashLength(), 1, pFp);
				fread(pMem, m_Size, 1, pFp);

				fclose(pFp);

				// Verify data integrity.
				if (memcmp(MD5.GetHashValue((UINT8 *)pMem, m_Size),pMD5Sum,MD5.GetHashLength()) == 0)
				{
					// Copy good data to memory area.
					memcpy(m_pMem, pMem, m_Size);

					// Set good return value.
					bRetVal = true;
				}
			}

			free(pMD5Sum);
		}
		free(pMem);
	}

	// Release mutex.
	m_Mutex.Unlock();

	return bRetVal;
}


void GMemFile::Dump()
{
	GMD5   MD5;
	UINT8* pMD5Sum;
	FILE*  pFp;
	void*  pMem;

	// Lock mutex to reduce running memory requirements.
	m_Mutex.Lock();

	// Create temp copy of memory area.
	pMem = malloc(m_Size);
	if (pMem != NULL)
	{
		// Copy current memory image and calc checksum.
		memcpy(pMem, m_pMem, m_Size);
		pMD5Sum = MD5.GetHashValue((UINT8 *)pMem, m_Size);

		// Backup current file.
		Backup();

		// Remove current copy.
		GSystem::RemoveFile(m_FilePath.c_str());

		// Save new memory image.
		pFp = fopen(m_FilePath.c_str(), "wb");
		if (pFp != NULL)
		{
			// Write checksum and data.
			fwrite(pMD5Sum, MD5.GetHashLength(), 1, pFp);
			fwrite(pMem, m_Size, 1, pFp);

			fclose(pFp);
		}
		else
		{
			// Restore previous image.
			Restore();
		}
		free(pMem);
	}

	m_Mutex.Unlock();

	return;
}


void GMemFile::Backup()
{
	// Derive backup name.
	std::string BackupFile = m_FilePath + ".bak";

	// Remove any previous backup file.
	if(!GSystem::RemoveFile(BackupFile.c_str()))
	{
		LOG(eCommon, eNotice, "GMemFile::Backup(): Cannot remove backed up memory file: %s\n", BackupFile.c_str());
	}

	// Create new backup of current file.
	if(!GSystem::CopyFile(m_FilePath, BackupFile))
	{
		LOG(eCommon, eError, "GMemFile::Backup(): Cannot BACKUP the memory file - memory will not be saved!!!, original memory file: %s, target backup file: %s\n", m_FilePath.c_str(), BackupFile.c_str());
	}

	return;
}


void GMemFile::Restore()
{
	// Derive backup name.
	std::string BackupFile = m_FilePath + ".bak";

	// Remove any current file.
	if(!GSystem::RemoveFile(m_FilePath.c_str()))
	{
		LOG(eCommon, eNotice, "GMemFile::Restore(): Cannot remove memory file: %s\n", m_FilePath.c_str());
	}

	// Restore file from backup copy.
	if(!GSystem::CopyFile(BackupFile, m_FilePath))
	{
		LOG(eCommon, eError, "GMemFile::Restore():  Cannot restore the memory file - original memory file: %s, backed up file: %s\n", m_FilePath.c_str(), BackupFile.c_str());
	}

	return;
}


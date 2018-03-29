#include <algorithm>
#include "GSystem.h"
#include "GFile.h"
#include "Common/Util/DEBUGOUT.h"
#include "GConfigFile.h"
#include "GConfigUtils.h"
#include "GXMLWriter.h"

using namespace GConfigTypes;


GConfigFile::GConfigFile (std::string FileName, bool bDefaultsFile)
	:	m_bHasChanges(false),
		m_bIsDefaults(bDefaultsFile),
		m_FileName(FileName),
		m_ConfigItems(),
		m_SetSaveMutex(),
		m_FileTime()
{
}

GConfigFile::~GConfigFile (void)
{
}

eConfigStatusType GConfigFile::Load (void)
{
	GMutexLock Lock(m_SetSaveMutex);
	return _Load();
}

eConfigStatusType GConfigFile::Save (void)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	GMutexLock Lock(m_SetSaveMutex);
	if(m_bHasChanges)
	{
		// Since we're about to save a copy of the file, make a backup first.
		_Backup();

		GXMLWriter oXMLWriter(m_FileName);
		{

			std::list<GConfigItem>::iterator iter(m_ConfigItems.begin());
			for (; iter != m_ConfigItems.end(); ++iter)
			{
				oXMLWriter.AddConfigItem((*iter));
			}

			ConfigStatus = oXMLWriter.Save();
			if (ConfigStatus != eConfigOk)
			{
				LOG(eConfig, eError, "GConfigFile::Save() - Failed to write XML file\n");
				GSystem::RemoveFile(m_FileName.c_str());
			}
		}
	}
	return ConfigStatus;
}

eConfigStatusType GConfigFile::SetValue (std::string XMLPath, std::string Value)
{
	GMutexLock Lock(m_SetSaveMutex);
	eConfigStatusType ConfigStatus(eConfigOk);

	if(m_bIsDefaults == false)
	{
		_ReloadFileIfNewer();

		{
			GConfigItem* LocalConfigItem(NULL);
			ConfigStatus = _GetConfigItem(XMLPath,LocalConfigItem);
			if (eConfigOk == ConfigStatus)
			{
				LocalConfigItem->SetValue(Value);
				m_bHasChanges = true;
			}
			else if (eConfigItemDoesNotExist == ConfigStatus)
			{
				std::list<std::string> Parts;
				GConfigUtils::StringSplit(XMLPath, '/', Parts);

				std::string Path;

				UINT32 u32Size = Parts.size();
				for (unsigned int Part=0;Part<u32Size-1;Part++)
				{
					Path += "/" + Parts.front();
					Parts.pop_front();
				}

				std::map<std::string, std::string> Attribs;
				GConfigItem NewConfigItem(Parts.front(), Value, Path, Attribs);

				m_ConfigItems.push_back(NewConfigItem);

				m_bHasChanges = true;

				ConfigStatus = eConfigOk;
			}
		}
	}
	else
	{
		ConfigStatus = eConfigCanNotChangeNonDefault;
		LOG(eConfig, eNotice, "Value is default, cannot be changed\n");
	}
	return(ConfigStatus);
}

eConfigStatusType GConfigFile::GetValue (std::string XMLPath, std::string& Value)
{
	GMutexLock Lock(m_SetSaveMutex);
	GConfigItem* LocalConfigItem(NULL);
	eConfigStatusType ConfigStatus(_GetConfigItem(XMLPath,LocalConfigItem));
	if (eConfigOk == ConfigStatus)
	{
		Value = LocalConfigItem->GetValue();
	}
	return ConfigStatus;
}

bool GConfigFile::XMLPathExists (std::string XMLPath)
{
	GMutexLock Lock(m_SetSaveMutex);
	GConfigItem* LocalConfigItem(NULL);
	return (_GetConfigItem(XMLPath,LocalConfigItem) == eConfigOk);
}

std::list<std::string> GConfigFile::GetIndex (void)
{
	GMutexLock Lock(m_SetSaveMutex);
	std::list<std::string> ReturnData;

	std::list<GConfigItem>::iterator Itr = m_ConfigItems.begin();

	for(;Itr != m_ConfigItems.end();Itr++)
	{
		ReturnData.push_back((*Itr).GetXMLPath() + "/" + (*Itr).GetName());
	}

	return ReturnData;
}

void GConfigFile::DebugDump (void)
{
	GMutexLock Lock(m_SetSaveMutex);
	std::list<GConfigItem>::iterator Itr = m_ConfigItems.begin();

	LOG(eConfig, eError, "GConfigFile::DebugDump() - \nDebug Dump\n");

	for(;Itr != m_ConfigItems.end();Itr++)
	{
		LOG(eConfig, eError, "GConfigFile::DebugDump() - %s/%s ==== %s\n",(*Itr).GetXMLPath().c_str(), (*Itr).GetName().c_str(), (*Itr).GetValue().c_str());
	}
}

eConfigStatusType GConfigFile::_Load (void)
{
	m_ConfigItems.clear();
	GXMLParser oXMLParser(m_FileName, m_ConfigItems);
	// Try the main file
	eConfigStatusType ConfigStatus(oXMLParser.Load());
	{
		if (ConfigStatus != eConfigOk)
		{
			// An error opening/parsing the file has occured, restore from the back up
			LOG(eConfig, eError, "GConfigFile::Load() - Handling an error reading from main copy of %s\n", m_FileName.c_str());

			m_ConfigItems.clear();
			// Delete the corrupt file
			GSystem::RemoveFile(m_FileName.c_str());

			// Restore the backup copy
			_Restore();

			ConfigStatus = oXMLParser.Load();
			if (ConfigStatus != eConfigOk)
			{
				// At this point the primary and back copies of the file are a bust, throw the exception
				LOG(eConfig, eError, "GConfigFile::Load() - Handling an error reading from backup copy of %s\n", m_FileName.c_str());

				// Delete the corrupt file so it's not read again
				GSystem::RemoveFile(m_FileName.c_str());

				LOG(eBios, eError,"GConfigFile::Load() - Neither file worked, bailing out\n");
				ConfigStatus = eConfigError;
			}
		}

		// Save the file's last modified time
		m_FileTime = GSystem::FileTime(m_FileName.c_str());
	}
	return ConfigStatus;
}

eConfigStatusType GConfigFile::_GetConfigItem (std::string XMLPath, GConfigItem * & pConfigItem)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	pConfigItem = NULL;
	std::list<GConfigItem>::iterator iter(m_ConfigItems.begin());
	for(; (iter != m_ConfigItems.end()) && (pConfigItem == NULL); ++iter)
	{
		std::string CIPath = iter->GetXMLPath() + "/" + iter->GetName();
		if(CIPath == XMLPath)
		{
			pConfigItem = &(*iter);
		}
	}
	if (pConfigItem == NULL)
	{
		ConfigStatus = eConfigItemDoesNotExist;
	}
	return ConfigStatus;
}

eConfigStatusType GConfigFile::_ReloadFileIfNewer (void)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	if (GSystem::FileTime(m_FileName.c_str()) != m_FileTime)
	{
		ConfigStatus = _Load();
	}
	return ConfigStatus;
}

void GConfigFile::_Backup (void)
{
	std::string FileName = m_FileName;

	// Create the backup filename
	GConfigUtils::IRReplace(FileName, ".xml", ".bak");

	// Move the current file to the backup file
	GSystem::RemoveFile(FileName.c_str());
	GSystem::RenameFile(m_FileName.c_str(),FileName.c_str());

	// If it's not there, there is something wrong
	if(!GSystem::FileExist(FileName.c_str()))
	{
		LOG(eConfig, eError, "GConfigFile::Backup() - Error creating backup file\n");
	}
}

void GConfigFile::_Restore (void)
{
	std::string FileName = m_FileName;

	// Create the backup filename
	GConfigUtils::IRReplace(FileName, ".xml", ".bak");

	if(!GSystem::FileExist(FileName.c_str()))
	{
		LOG(eBios, eError, "GConfigFile::Restore() - Error: backup file doesn't exist\n");
		return;
	}

	{
		GFile oSrc(FileName), oDst(m_FileName);
		bool bContinue(false);
		if (oSrc.Open(eOpenReadOnly))
		{
			if (oDst.Open(eOpenWriteNew))
			{
				static int const CBufferLength(1024);
				unsigned char ucBuffer[CBufferLength];
				int iBytesRead(0);
				bContinue = oSrc.Read(ucBuffer,CBufferLength,iBytesRead);
				while ((iBytesRead > 0) && bContinue)
				{
					int iBytesOffset(0);
					while ((iBytesRead > 0) && bContinue)
					{
						int iBytesWrote(0);
						bContinue = oDst.Write(ucBuffer+iBytesOffset,iBytesRead,iBytesWrote);
						if (bContinue == true)
						{
							iBytesRead -= iBytesWrote;
							iBytesOffset += iBytesWrote;
						}
					}
					bContinue = oSrc.Read(ucBuffer,CBufferLength,iBytesRead);
				}
				oDst.Close();
			}
			oSrc.Close();
		}
		if (bContinue == false)
		{
			LOG(eConfig, eError, "GConfigFile::Restore() - Error: cannot copy from backup file (%s) to dest file (%s)\n",FileName.c_str(),m_FileName.c_str());
		}
	}
}

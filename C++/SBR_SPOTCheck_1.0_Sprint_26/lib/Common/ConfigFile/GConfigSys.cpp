#include <algorithm>
#include "Common/Util/DEBUGOUT.h"
#include "GConfigSys.h"
#include "GConfigUtils.h"
#include "GSystem.h"
#include "GFile.h"

using namespace GConfigTypes;


const std::string GConfigSys::CDefaultWorkingDir("./");

GConfigSys* GConfigSys::mst_pSoleInstance = NULL;
GMutex      GConfigSys::mst_SoleInstanceMutex;

std::string GConfigSys::mst_RunConfigFilePath = "./RunConfig.xml";


GConfigSys * GConfigSys::GetSoleInstance (void)
{
	// Insure that only one thread executes this code at a time.  This object
	// locks the mutex at construction and unlocks on destruction.
	GMutexLock Lock(mst_SoleInstanceMutex);
	if(mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GConfigSys;
	}
	return mst_pSoleInstance;
}



GConfigSys * GConfigSys::GetSoleInstance (const std::string& RunConfigFilePath)
{
	// Insure that only one thread executes this code at a time.  This object
	// locks the mutex at construction and unlocks on destruction.
	GMutexLock Lock(mst_SoleInstanceMutex);
	mst_RunConfigFilePath = RunConfigFilePath;
	if(mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GConfigSys;
	}
	return mst_pSoleInstance;
}



void GConfigSys::RemoveSoleInstance (void)
{
	GMutexLock Lock(mst_SoleInstanceMutex);
	delete mst_pSoleInstance;
	mst_pSoleInstance = NULL;
	mst_RunConfigFilePath = "./RunComfig.xml";
}



GConfigSys::GConfigSys (void)
	: m_OptDir(CDefaultWorkingDir),
	  m_EtcDir(CDefaultWorkingDir),
	  m_VarDir(CDefaultWorkingDir)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	if(!GSystem::FileExist(mst_RunConfigFilePath.c_str()))
	{
		LOG(eConfig, eError,
			"GConfigSys::GConfigSys() - %s DOES NOT EXIST\n", 
			mst_RunConfigFilePath.c_str());
		return;
	}

	GConfigFile ConfigSysFiles(mst_RunConfigFilePath, false);
	ConfigStatus = ConfigSysFiles.Load();
	if (ConfigStatus != eConfigOk)
	{
		LOG(eConfig, eError,
			"GConfigSys::GConfigSys() - %s could not be loaded. (err=%d)\n", 
			mst_RunConfigFilePath.c_str(), ConfigStatus);
		return;
	}

	// Load run-time system directory configuration.
	ConfigStatus = ConfigSysFiles.GetValue("/RunConfig/OptDir", m_OptDir);
	if (ConfigStatus != eConfigOk)
	{
		m_OptDir=CDefaultWorkingDir;
	}
	LOG(eConfig, eInfo, "GConfigSys::GConfigSys() - OptDir = %s\n", m_OptDir.c_str());

	ConfigStatus = ConfigSysFiles.GetValue("/RunConfig/EtcDir", m_EtcDir);
	if (ConfigStatus != eConfigOk)
	{
		m_EtcDir=CDefaultWorkingDir;
	}
	LOG(eConfig, eInfo, "GConfigSys::GConfigSys() - EtcDir = %s\n", m_EtcDir.c_str());

	ConfigStatus = ConfigSysFiles.GetValue("/RunConfig/VarDir", m_VarDir);
	if (ConfigStatus != eConfigOk)
	{
		m_VarDir=CDefaultWorkingDir;
	}
	LOG(eConfig, eInfo, "GConfigSys::GConfigSys() - VarDir = %s\n", m_VarDir.c_str());

	// Load available default configuration files.
	std::string DefaultsPath = m_EtcDir + "config/";
	std::list<std::string> DefaultFiles;
	ConfigStatus = ReadPath(DefaultsPath, DefaultFiles);
	if (eConfigOk == ConfigStatus)
	{
		ConfigStatus = LoadDefaults(DefaultFiles);
		if (ConfigStatus != eConfigOk)
		{
			LOG(eConfig, eError,
				"GConfigSys::GConfigSys() - Failed to load defaults at [%s]\n",
				DefaultsPath.c_str());
		}
	}

	// Load available override configuration files 
	std::string OverridesPath = m_VarDir + "config/";
	std::list<std::string> OverrideFiles;
	ConfigStatus = ReadPath(OverridesPath, OverrideFiles);
	if (eConfigOk == ConfigStatus)
	{
		ConfigStatus = LoadOverrides(OverrideFiles);
		if (ConfigStatus != eConfigOk)
		{
			LOG(eConfig, eError,
				"GConfigSys::GConfigSys() - Failed to load overrides at [%s]\n",
				OverridesPath.c_str());
		}
	}
}



GConfigSys::~GConfigSys (void)
{
	// Clean up allocated GConfigFile file instances
	while(m_DefaultFileList.size() > 0)
	{
		// Get the object at the back
		GConfigFile * pToDelete = m_DefaultFileList.back();
		// Delete the object
		delete pToDelete;
		// Now remove it from the vector
		m_DefaultFileList.pop_back();
	}

	// Clean up allocated GConfigFile file instances
	while(m_OverrideFileList.size() > 0)
	{
		// Get the object at the back
		GConfigFile * pToDelete = m_OverrideFileList.back();
		// Delete the object
		delete pToDelete;
		// Now remove it from the vector
		m_OverrideFileList.pop_back();
	}
}


std::string GConfigSys::GetOptDir(void)
{
	return m_OptDir;
}


std::string GConfigSys::GetEtcDir(void)
{
	return m_EtcDir;
}


std::string GConfigSys::GetVarDir(void)
{
	return m_VarDir;
}


eConfigStatusType GConfigSys::GetValue (std::string XMLPath, INT32& s32Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		long int liTempValue(strtol(StrValue.c_str(),NULL,10));
		s32Value = static_cast<INT32>(liTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, INT16& s16Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		long int liTempValue(strtol(StrValue.c_str(),NULL,10));
		s16Value = static_cast<INT16>(liTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, INT8& s8Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		long int liTempValue(strtol(StrValue.c_str(),NULL,10));
		s8Value = static_cast<INT8>(liTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, UINT32& u32Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		unsigned long int uliTempValue(strtoul(StrValue.c_str(),NULL,10));
		u32Value = static_cast<UINT32>(uliTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, UINT16& u16Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		unsigned long int uliTempValue(strtoul(StrValue.c_str(),NULL,10));
		u16Value = static_cast<UINT16>(uliTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, UINT8& u8Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		unsigned long int uliTempValue(strtoul(StrValue.c_str(),NULL,10));
		u8Value = static_cast<UINT8>(uliTempValue);
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, double& Value, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		Value = atof(StrValue.c_str());
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, bool&  bValue, bool bOverrideOnly)
{
	std::string StrValue;
	eConfigStatusType ConfigStatus(GetValue(XMLPath, StrValue, bOverrideOnly));
	
	if (eConfigOk == ConfigStatus)
	{
		std::transform(StrValue.begin(), StrValue.end(), StrValue.begin(), GConfigUtils::char2upper);

		if(StrValue == "TRUE" || StrValue == "YES" || StrValue == "T" || StrValue == "Y" || StrValue == "1")
		{
			bValue = true;
		}
		else if(StrValue == "FALSE" || StrValue == "NO" || StrValue == "F" || StrValue == "N" || StrValue == "0")
		{
			bValue = false;
		}
		else
		{
			ConfigStatus = eConfigConversionError;
			LOG(eConfig, eError,
				"GConfigSys::GetValue() - Error converting (%s) to boolean\n",
				StrValue.c_str());
		}
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::GetValue (std::string XMLPath, std::string&  Value, bool bOverrideOnly)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	bool bFound = false;

	if(m_OverrideFileList.size() > 0)
	{
		std::vector<GConfigFile*>::iterator iter(m_OverrideFileList.begin());
		for(; iter != m_OverrideFileList.end(); ++iter)
		{
			if((*iter)->XMLPathExists(XMLPath))
			{
				ConfigStatus = (*iter)->GetValue(XMLPath, Value);
				bFound = true;
				break;
			}
		}
	}

	if(!bOverrideOnly && !bFound && m_DefaultFileList.size() > 0)
	{
		std::vector<GConfigFile*>::iterator iter(m_DefaultFileList.begin());
		for(; iter != m_DefaultFileList.end(); ++iter)
		{
			if((*iter)->XMLPathExists(XMLPath))
			{
				ConfigStatus = (*iter)->GetValue(XMLPath,Value);
				bFound = true;
				break;
			}
		}
	}

	if(!bFound)
	{
		ConfigStatus = eConfigItemDoesNotExist;
		LOG(eConfig, eError,
			"GConfigSys::GetValue() - XMLPath [%s] does not exist\n",
			XMLPath.c_str());
	}

	return ConfigStatus;
}



eConfigStatusType GConfigSys::SetValue (const std::string& XMLPath, INT32 s32Value)
{
	char acBuffer[256];
	sprintf(acBuffer,"%d",s32Value);
	return SetValue(XMLPath, std::string(acBuffer));
}



eConfigStatusType GConfigSys::SetValue (const std::string& XMLPath, UINT32 u32Value)
{
	char acBuffer[256];
	snprintf(acBuffer, sizeof(acBuffer), "%u",u32Value);
	return SetValue(XMLPath, std::string(acBuffer));
}



eConfigStatusType GConfigSys::SetValue (const std::string& XMLPath, double Value)
{
	char acBuffer[256];
	sprintf(acBuffer,"%.10f",Value);
	return SetValue(XMLPath, std::string(acBuffer));
}



eConfigStatusType GConfigSys::SetValue (const std::string& XMLPath, bool bValue)
{
   std::string StrValue;
   if(bValue)
   {
      StrValue = "true";
   }
   else
   {
      StrValue = "false";
   }
   return SetValue(XMLPath, StrValue);
}



eConfigStatusType GConfigSys::SetValue (const std::string& XMLPath, std::string Value)
{
	eConfigStatusType ConfigStatus(eConfigOk);

	LOG(eConfig, eTrace,
		"GConfigSys::SetValue() - Key: %s Value: %s\n", 
		XMLPath.c_str(), Value.c_str());

	if(PathInDefaults(XMLPath))
	{
		bool bFound = false;
		std::vector<GConfigFile*>::iterator Itr;

		if(m_OverrideFileList.size() > 0)
		{
			for(Itr=m_OverrideFileList.begin();Itr != m_OverrideFileList.end();Itr++)
			{
				if((*Itr)->XMLPathExists(XMLPath))
				{
					ConfigStatus = (*Itr)->SetValue(XMLPath, Value);
					bFound = true;
					break;
				}
			}
		}

		if(!bFound)
		{
			if(m_OverrideFileList.size() > 0)
			{
				Itr=m_OverrideFileList.begin();

				ConfigStatus = (*Itr)->SetValue(XMLPath, Value);
			}
			else
			{
				GConfigFile* NewFile = CreateNewOverridesFile();
				if (NewFile != NULL)
				{
					ConfigStatus = NewFile->SetValue(XMLPath, Value);
				}
				else
				{
					ConfigStatus = eConfigError;
					LOG(eConfig, eError,
						"GConfigSys::SetValue() - Out of memory\n");
				}
			}
		}

		if (eConfigOk == ConfigStatus)
		{
			ConfigStatus = SaveChanges();
		}
	}
	else
	{
		ConfigStatus = eConfigCanNotChangeNonDefault;
		LOG(eConfig, eError,
			"GConfigSys::SetValue() - XMLPath [%s] does not have a default value\n",
			XMLPath.c_str());
	}

	return ConfigStatus;
}



GConfigFile * GConfigSys::CreateNewOverridesFile (void)
{
   std::string NewFileName = m_VarDir + "/config/Overrides.xml";

   GSystem::RemoveFile(NewFileName.c_str());
   GFile oFile(NewFileName);
   if (oFile.Open(eOpenWriteNew) == true)
   {
	   char const cBuffer [] = "<config>\n</config>\n";
	   int iBytesWritten(0);
	   oFile.Write(cBuffer,sizeof(cBuffer),iBytesWritten);
	   oFile.Close();
   }

   GConfigFile* NewFile = new GConfigFile(NewFileName, false);
   if (NewFile != NULL)
   {
	   m_OverrideFileList.push_back(NewFile);
   }
   return NewFile;
}



eConfigStatusType GConfigSys::SaveChanges (void)
{
	//Only shows if one of the files had issues while saving. It will try to save all it can.
	eConfigStatusType ConfigStatus(eConfigOk);
	if(m_OverrideFileList.size() > 0)
	{
		std::vector<GConfigFile*>::iterator iter(m_OverrideFileList.begin());
		for(; (iter != m_OverrideFileList.end()); ++iter)
		{
			eConfigStatusType ConfigStatusAux((*iter)->Save());
			if (eConfigOk == ConfigStatus)
			{
				ConfigStatus = ConfigStatusAux;
			}
		}
	}
	return ConfigStatus;
}



eConfigStatusType GConfigSys::ReadPath (std::string Path, std::list<std::string> & FileList)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	std::list < std::pair <std::string, bool> > lEntryList(GSystem::GetDirEntryList(Path));

	if (lEntryList.empty() == false)
	{
		std::list < std::pair <std::string, bool> >::iterator iter(lEntryList.begin());
		std::list < std::pair <std::string, bool> >::iterator eiter(lEntryList.end());
		for (;iter != eiter; ++iter) 
		{
			if ((*iter).second == false)
			{
				std::string sFileName = (*iter).first;
				std::transform(sFileName.begin(), sFileName.end(), sFileName.begin(), GConfigUtils::char2upper);

				std::string::size_type uDotPos = sFileName.rfind(".");

				if(uDotPos != std::string::npos)
				{
					if(sFileName.substr(uDotPos+1) == "XML")
					{
						FileList.push_back(Path + "/" + (*iter).first);
					}
				}
			}
		}
	}
	else
	{
		ConfigStatus = eConfigError;
		LOG(eConfig, eError,
			"GConfigSys::ReadPath() - Error reading directory [%s]\n",
			Path.c_str());
	}
	return ConfigStatus;
}



eConfigStatusType GConfigSys::LoadDefaults (std::list<std::string> FileList)
{
	return LoadFiles(FileList,true);
}



eConfigStatusType GConfigSys::LoadOverrides (std::list<std::string> FileList)
{
	return LoadFiles(FileList,false);
}



eConfigStatusType GConfigSys::LoadFiles (std::list<std::string> FileList, bool bDefaults)
{
	eConfigStatusType ConfigStatus(eConfigOk);
	std::list<std::string>::iterator iter(FileList.begin());
	for(; (iter != FileList.end()) && (eConfigOk == ConfigStatus); ++iter)
	{
		GConfigFile* NewFile(new GConfigFile((*iter),bDefaults));
		if (NewFile != NULL)
		{
			ConfigStatus = NewFile->Load();
			if (eConfigOk == ConfigStatus)
			{
				if(bDefaults)
				{
					m_DefaultFileList.push_back(NewFile);
				}
				else
				{
					m_OverrideFileList.push_back(NewFile);
				}
			}
		}
		else
		{
			ConfigStatus = eConfigError;
			LOG(eConfig, eError,
				"GConfigSys::LoadFiles() - Out of memory\n");
		}
	}
	return ConfigStatus;
}



bool GConfigSys::PathInDefaults (std::string XMLPath)
{
   return PathExists(XMLPath, true);
}



bool GConfigSys::PathInOverrides (std::string XMLPath)
{
   return PathExists(XMLPath, false);
}



bool GConfigSys::PathExists (std::string XMLPath, bool bDefaults)
{
   if(bDefaults)
   {
      if(m_DefaultFileList.size() > 0)
      {
         for(std::vector<GConfigFile*>::iterator Itr=m_DefaultFileList.begin();Itr != m_DefaultFileList.end();Itr++)
         {
            if((*Itr)->XMLPathExists(XMLPath))
            {
               return true;
            }
         }
      }
   }
   else
   {
      if(m_OverrideFileList.size() > 0)
      {
         for(std::vector<GConfigFile*>::iterator Itr=m_OverrideFileList.begin();Itr != m_OverrideFileList.end();Itr++)
         {
            if((*Itr)->XMLPathExists(XMLPath))
            {
               return true;
            }
         }
      }
   }
   return false;
}


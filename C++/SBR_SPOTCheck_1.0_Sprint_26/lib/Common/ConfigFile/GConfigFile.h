#ifndef GConfigFile_h
#define GConfigFile_h

#include <list>
#include <string>
#include "GTypes.h"
#include "GTask.h"
#include "GConfigItem.h"
#include "GXMLParser.h"
#include "GXMLWriter.h"
#include "GSystem.h"

/*!
 * \class [GConfigFile]
 *
 * \brief [Class representing a single XML File]
 *
 * [A class representing a single XML File, the Config File Manager may have one or more
 *  config files for each default values and override values]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */

class GConfigFile
{
public:
   GConfigFile (std::string FileName, bool bDefaultsFile);
   ~GConfigFile (void);

	/*!
	 * \brief [Load contents from File]
	 * \return [If the XMLPath is invalid a eConfigItemDoesNotExist eConfigStatusType will be return]
	 */
	GConfigTypes::eConfigStatusType Load (void);

	/*!
	 * \brief [Saves contents to File]
	 */
	GConfigTypes::eConfigStatusType Save (void);

	/*!
	 * \brief [Changes specified value]
	 *
	 * \param[in] [XMLPath] [The path to the value to set the value for]
	 * \param[in] [Value] [Actual value to be set]
	 * \return [If the XMLPath is invalid a eConfigItemDoesNotExist eConfigStatusType will be return]
	 */
	GConfigTypes::eConfigStatusType SetValue (std::string XMLPath, std::string  Value);
   
	/*!
	 * \brief [Retrieves specified value]
	 *
	 * \param[in] [XMLPath] [The path to the value to set the value for]
	 * \param[out] [Value] [Variable to pass the value back]
	 * \return [If the XMLPath is invalid a eConfigItemDoesNotExist eConfigStatusType will be return]
	 */
	GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, std::string& Value);

	/*!
	 * \brief [Checks the XMLPath to see if it is valid/exists]
	 *
	 * \param[in] [XMLPath] [Path to check]
	 * \return [Boolean: True - Exists, False - Does not Exist]
	 */
	bool XMLPathExists (std::string XMLPath);

	/*!
	 * \brief [Retrieves the config item index]
	 *
	 * [Returns a list of all XML paths in this file]
	 *
	 * \return [a list of XMLPaths contained in this file]
	 */
	std::list<std::string> GetIndex (void);

	/*!
	 * \brief [Deletes the files associated with this object]
	 *
	 * [Deletes the primary and backup copy of the file]
	 *
	 */
   void DebugDump (void);

private:
	// Prevent copy constructor and assignment usage.
	GConfigFile(const GConfigFile &right);
	const GConfigFile& operator=(const GConfigFile &right);

	GConfigTypes::eConfigStatusType _GetConfigItem (std::string XMLPath, GConfigItem * & pConfigItem);

	GConfigTypes::eConfigStatusType _Load (void);
	GConfigTypes::eConfigStatusType _ReloadFileIfNewer (void);
	void _Backup (void);
	void _Restore (void);

	bool m_bHasChanges;
	bool m_bIsDefaults;
	std::string            m_FileName;
	std::list<GConfigItem> m_ConfigItems;
	GMutex	m_SetSaveMutex;
	GSystem::TimeStamp m_FileTime;
};

#endif

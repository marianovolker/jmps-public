// GConfigSys.h

#ifndef GConfigSys_h
#define GConfigSys_h

#include <string>
#include <vector>
#include "GTypes.h"
#include "GTask.h"
#include "GConfigFile.h"
#include "GConfigTypes.h"
#include "GConfigConstants.h"

/*!
 * \class [GConfigSys]
 *
 * \brief [Configuration File Manager]
 *
 * [User interface class to the configuration file manager]
 *
 * \author [Shawn Barber]
 * \date 9Jul09
 */


class GConfigSys
{
public:
   static const std::string CDefaultWorkingDir;

   static GConfigSys * GetSoleInstance (void);
   static GConfigSys * GetSoleInstance (const std::string& RunConfigFilePath);
   static void RemoveSoleInstance (void);

   /*!
    * \brief [Retrieves the configured run-time system directories]
    */
   std::string GetOptDir(void);
   std::string GetEtcDir(void);
   std::string GetVarDir(void);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [s32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, INT32& s32Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [s32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, INT16& s16Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [s32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, INT8& s8Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [u32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, UINT32& u32Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [u32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, UINT16& u16Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [u32Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *        also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, UINT8& u8Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved,
    *			also if the value for the XMLPath is of a different type then a eConfigConversionError will be given]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, double& Value, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, bool&  bValue, bool bOverrideOnly = false);

   /*!
    * \brief [Retrieves a value from the configuration file system]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [Variable the value will be passed back in]
    * \return [If the XMLPath does not exist a eConfigItemDoesNotExist return value will be retrieved]
    */
   GConfigTypes::eConfigStatusType GetValue (std::string XMLPath, std::string&  Value, bool bOverrideOnly = false);

   /*!
    * \brief [Changes a value]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [s32Value] [New value]
    * \return [If the XMLPath does not exist in the default values file, a eConfigCanNotChangeNonDefault
    * 			return value will be retrieved because all values must have a default value]
    */
   GConfigTypes::eConfigStatusType SetValue (const std::string& XMLPath, INT32 s32Value);

   /*!
    * \brief [Changes a value]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [u32Value] [New value]
    * \return [If the XMLPath does not exist in the default values file, a eConfigCanNotChangeNonDefault
    * 			return value will be retrieved because all values must have a default value]
    */
   GConfigTypes::eConfigStatusType SetValue (const std::string& XMLPath, UINT32  u32Value);

   /*!
    * \brief [Changes a value]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [New value]
    * \return [If the XMLPath does not exist in the default values file, a eConfigCanNotChangeNonDefault
    * 			return value will be retrieved because all values must have a default value]
    */
   GConfigTypes::eConfigStatusType SetValue (const std::string& XMLPath, double Value);

   /*!
    * \brief [Changes a value]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [New value]
    * \return [If the XMLPath does not exist in the default values file, a eConfigCanNotChangeNonDefault
    * 			return value will be retrieved because all values must have a default value]
    */
   GConfigTypes::eConfigStatusType SetValue (const std::string& XMLPath, bool bValue);

   /*!
    * \brief [Changes a value]
    *
    * \param[in] [XMLPath] [Path to the value being requested]
    * \param[out] [Value] [New value]
    * \return [If the XMLPath does not exist in the default values file, a eConfigCanNotChangeNonDefault
    * 			return value will be retrieved because all values must have a default value]
    */
   GConfigTypes::eConfigStatusType SetValue (const std::string& XMLPath, std::string Value);

   /*!
    * \brief [Saves any changes made to override values to their respective files]
    */
   GConfigTypes::eConfigStatusType SaveChanges (void);

protected:
   static std::string mst_RunConfigFilePath;

private:
   GConfigSys (void);
   ~GConfigSys (void);

   GConfigFile * CreateNewOverridesFile (void);

   // Prevent copy constructor and assignment usage.
   GConfigSys (GConfigSys const & rhs);
   GConfigSys const & operator= (GConfigSys const & rhs);

   GConfigTypes::eConfigStatusType ReadPath(std::string Path, std::list<std::string>& FileList);
   GConfigTypes::eConfigStatusType LoadDefaults(std::list<std::string> FileList);
   GConfigTypes::eConfigStatusType LoadOverrides(std::list<std::string> FileList);
   GConfigTypes::eConfigStatusType LoadFiles(std::list<std::string> FileList, bool bDefaults);
   bool PathInDefaults(std::string XMLPath);
   bool PathInOverrides(std::string XMLPath);
   bool PathExists(std::string XMLPath, bool bDefaults);

   // Configured run-time System directories.
   std::string                  m_OptDir;
   std::string                  m_EtcDir;
   std::string                  m_VarDir;

   std::vector<GConfigFile*>	m_DefaultFileList;
   std::vector<GConfigFile*>	m_OverrideFileList;

   static GConfigSys *			mst_pSoleInstance;
   static GMutex				mst_SoleInstanceMutex;

};

#endif

//! GLogConfig 
/*!
  Log Configuration Object
*/

#ifndef GLOGCONFIG_H
#define GLOGCONFIG_H

#include <QString>

class GLogConfig
{
public:
	typedef enum
	{
		eLevel1 = 1,
		eLevel2 = 2,
		eLevel3 = 3,
		eLevel4 = 4

	} eLevel;
	
	typedef enum
	{
		eConsole,
		eFile,
		eBoth

	} eWriteTo;	
	
public:
	/*! 
	 * 	Constructor parameters
	 *	
	 *  eLevel: 		Logging level
	 * 
	 *  pszDirectory: 	Directory where to store the logs
	 * 
	 *  pszFileName: 	Log filename
	 * 
	 */	
	GLogConfig(GLogConfig::eWriteTo eWriteTo, GLogConfig::eLevel eLvl, QString pszFileName);
	virtual ~GLogConfig();

	bool GetWriteToConsole();
	bool GetWriteToFile();
	GLogConfig::eLevel GetLevel();
	QString GetDirectory();
	QString GetFileName();
	QString GetFullFileName();

private:
	bool m_bWriteToConsole;
	bool m_bWriteToFile;
	GLogConfig::eLevel m_eLevel;	
	QString m_strDirectory;
	QString m_strFileName;	
};

#endif

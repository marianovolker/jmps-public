#ifndef GConfigReader_H
#define	GConfigReader_H

#include <QString>
#include <QMap>
#include <QList>

#include "GConfigReaderException.h"

/**
 * GConfigReader.
 * Configuration Properties File Reader class.
 * This class is used to read a configuration file type key = value.
 * This key/value can be of a whole string, int or booleans. After reading
 * the configuration file, it is loaded into a collection of key-values.
 * This class also provides the functionality of setting certain values and
 * store that to a new configuration file.
 * It is not implemented to have sections functionality.
 * This class uses a Qmap class to implement map functionality like in STL.
*/

class GConfigReader 
{
public:

	/**
	 * Constructor.
	 * Calls to Load().
	 *
	 * @param strFileName as the filename to be load.
	 * @see Load().
	 * 
	 */
	GConfigReader (const QString& strFileName="" ) throw (GConfigReaderException);

	//! Destructor
	virtual ~GConfigReader();

	/**
	 * Clear the map.
	 */
	void Reset();

	/**
	 * Reads the configuration file and loads its key/values in the map.
	 * This member is called by constructor.
	 *
	 * @param strFileName as the filename to be load.
	 */
	void Load(const QString& strFileName="") throw (GConfigReaderException);

	/**
	 * Save the map to the configuration file.
	 *
	 * @param strOutputFileName as the new filename.
	 */
	void Save(const QString& strOutputFileName = "") throw (GConfigReaderException);

	/**
	 * Sets a value located in the internal map.
	 *
	 * @param strKey, a string that represent the key to find for into the internal map.
	 * @param strNewValue, is the new value for that key.
	 * @return true or false.
	 */
	bool Set(const QString& strKey, const QString& strNewValue);

	/**
	 * Gets an int value from the map.
	 *
	 * @param strKey a string representing string key.
	 * @param iDefaultValue an int.
	 * @return the value as numeric.
	 */
	int GetInt(const QString& strKey, int iDefaultValue = 0) const;

	/**
	 * Gets a bool value from the map.
	 *
	 * @param strKey a string representing string key.
	 * @param bDefaultValue a bool, this value is returned when the key was not found.
	 * @return the value as bool.
	 */
	bool GetBool(const QString& strKey, bool bDefaultValue = false) const;

	/**
	 * Gets a bool value from the map.
	 *
	 * @param strKey a string representing string key.
	 * @param szDefaultValue a string, this value is returned when the key was not found.
	 * @return the value as string.
	 */
	const QString GetString(const QString& strKey, const char* szDefaultValue = "") const;

	/**
	 * Print the list of values loaded from the configuration file in the log file.
	 *
	 */   
	void Show() const;

	/**
	 * GetFileName
	 *
	 * @return filename as QString.
	 */
	QString GetFileName() const;

protected:

	/**
	 * Key Value Map
	 */
	QMap<QString, QString> m_QMapKeyValue;

	/**
	 * List to store the content of the original file.
	 */
	QList<QString> m_lstOriginalFile;

	/**
	 * File Name
	 *
	 */
	QString	m_strFileName;

	/**
	 * Pattern reserved for installation purpose.
	 * The lines that contain this pattern does not read as [key=value] pair.
	 */
	static const QString CISTALLATION_PATTERN;
};

#endif

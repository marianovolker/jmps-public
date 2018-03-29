#ifndef _GConfigEnvironment_H
#define	_GConfigEnvironment_H

#include <QString>
#include <QMap>

#include "utils/base/src/GException.h"

/**
 * GConfigEnvironment.
 * This class is used to ...
 *
 */

class GConfigEnvironment
{
public:

	/**
	 * Constructor.
	 * Calls to Load().
	 *
	 * 
	 */
	GConfigEnvironment ();

	//! Destructor
	virtual ~GConfigEnvironment();

	/**
	 * Reads the process environment and load its key/values in an internal list.
	 * This member is called by constructor.
	 *
	 */
	void Load() throw (GException);

	/**
	 * Gets an int value from the internal list.
	 *
	 * @param property an string representing de string property.
	 * @param defaultValue an int.
	 * @return an int representing the property value.
	 */
	int GetInt(const QString& property, int defaultValue = 0) const;

	/**
	 * Gets a bool value from the internal list.
	 *
	 * @param property an string representing de string property.
	 * @param defaultValue a bool, this value is returned when the property is not found.
	 * @return a bool representing the property value.
	 */
	bool GetBool(const QString& property, bool defaultValue = false) const;

	/**
	 * Gets a string value from the internal list.
	 *
	 * @param property an string representing de string property.
	 * @param defaultValue a string, this value is returned when the property is not found.
	 * @return a string representing de property value.
	 */
	const QString GetString(const QString& property, const char* defaultValue = "") const;

protected:

	/**
	 * Key Value Map
	 *
	 *
	 */
	QMap<QString, QString> m_QMapKeyValue;

};

#endif

package com.gilbarco.globaltools.flexpay.targetunit.utilities;

import java.util.Observer;

/** @class IObservable IObservable.java "com.gilbarco.globaltools.flexpay.targetunit.utilities"
 * @brief This is an Observable interface to be implemented by the user for composition with an inner "java.util.Observable" field.
 *
 * @since 1.0.0
 */
public interface IObservable
{
	/** @brief Adds an observer to the set of observers for this object, provided that it is not the same as some observer already in the set.
	 *
	 * @param o [In] Observer to be notified upon updates.
	 * 
	 * @since 1.0.0
	 */ 
	void addObserver (Observer o);

	/** @brief Returns the number of observers of this Observable object.
	 *
	 * @return Number of observers of this Observable object.
	 * 
	 * @since 1.0.0
	 */
	int countObservers ();

	/** @brief Deletes an observer from the set of observers of this object.
	 *
	 * @param o [In] Observer to be notified upon updates.
	 * 
	 * @since 1.0.0
	 */
	void deleteObserver (Observer o);

	/** @brief Clears the observer list so that this object no longer has any observers.
	 *
	 * @since 1.0.0
	 */
	void deleteObservers ();

	/** @brief Tests if this object has changed.
	 *
	 * @return True if the object has been marked as changed. False otherwise.
	 *
	 * @since 1.0.0
	 */
	boolean hasChanged ();

	/** @brief If this object has changed, as indicated by the hasChanged method, then notify all of its observers and then call the clearChanged method to indicate that this object has no longer changed.
	 *
	 * @since 1.0.0
	 */
	void notifyObservers ();

	/** @brief If this object has changed, as indicated by the hasChanged method, then notify all of its observers and then call the clearChanged method to indicate that this object has no longer changed.
	 *
	 * @param arg [In] Object to be sent along with notification.
	 * 
	 * @since 1.0.0
	 */
	void notifyObservers (Object arg);
}

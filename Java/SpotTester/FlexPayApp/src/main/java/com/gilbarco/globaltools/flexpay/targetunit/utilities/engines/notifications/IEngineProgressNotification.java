/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceCollectionEngine.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications;

/** @class IEngineProgressNotification IEngineProgressNotification.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification interface.
 * 
 * @since 1.0.0
 */
public interface IEngineProgressNotification {

	/** @brief Engine progress caption update getter.
	 *
	 * @return the new caption updated when the engine made progress.
	 *
	 * @see #setCaptionUpdate(String)
	 * 
	 * @since 1.0.0
	 */	
	public String getCaptionUpdate ();

	/** @brief Engine progress caption update setter.
	 *
	 * @param sCaptionUpdate is the new caption to be updated when the engine make progress.
	 *
	 * @see #getCaptionUpdate()
	 * 
	 * @since 1.0.0
	 */	
	public void setCaptionUpdate (String sCaptionUpdate);
}

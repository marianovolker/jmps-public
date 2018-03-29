/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationConnectionNotGranted.java
 * @author pcantarini
 * @date Friday, October 11, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationConnectionNotGranted EngineProgressNotificationConnectionNotGranted.java "com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for open connection.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationConnectionNotGranted extends EngineProgressNotification
{
	/** @brief Indicates if current not granted connection is still active */
	private boolean bActive;

	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationConnectionNotGranted(String sCaptionUpdate)
	{
		super(sCaptionUpdate);
		this.bActive = false;
	}

	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * @param bActive indicates if the not granted connection is still active
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationConnectionNotGranted(String sCaptionUpdate, boolean bActive)
	{
		super(sCaptionUpdate);
		this.bActive = bActive;
	}

	/** @brief Indicates if current granted connection is authenticated
	 * 
	 * @return True if current not granted connection is still active. False, otherwise.
	 * 
	 * @since 1.0.0
	 */
	public boolean isActive ()
	{
		return bActive;
	}
}

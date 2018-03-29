/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationConnectionGranted.java
 * @author pcantarini
 * @date Friday, October 11, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationConnectionGranted EngineProgressNotificationConnectionGranted.java "com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for open connection.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationConnectionGranted extends EngineProgressNotification
{
	/** @brief Indicates if current granted connection is authenticated */
	private boolean bAuthenticated;

	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationConnectionGranted(String sCaptionUpdate)
	{
		super(sCaptionUpdate);
		this.bAuthenticated = false;
	}

	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * @param bAuthenticated indicates if the granted connection is authenticated
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationConnectionGranted(String sCaptionUpdate, boolean bAuthenticated)
	{
		super(sCaptionUpdate);
		this.bAuthenticated = bAuthenticated;
	}

	/** @brief Indicates if current granted connection is authenticated
	 * 
	 * @return True if current granted connection is authenticated. False, otherwise.
	 * 
	 * @since 1.0.0
	 */
	public boolean isAuthenticated ()
	{
		return bAuthenticated;
	}
}

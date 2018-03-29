/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationConnectionAborted.java
 * @author pcantarini
 * @date Friday, October 11, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationConnectionAborted EngineProgressNotificationConnectionAborted.java "com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for open connection.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationConnectionAborted extends EngineProgressNotification
{
	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationConnectionAborted(String sCaptionUpdate)
	{
		super(sCaptionUpdate);
	}
}

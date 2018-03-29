/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationStatusInitiated.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationStatusInitiated EngineProgressNotificationStatusInitiated.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for operation initialization done.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationStatusInitiated extends EngineProgressNotification
{
	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationStatusInitiated(String sCaptionUpdate)
	{
		super(sCaptionUpdate);
	}
}

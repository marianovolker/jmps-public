/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationStatusSucceeded.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationStatusSucceeded EngineProgressNotificationStatusSucceeded.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for success operation.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationStatusSucceeded extends EngineProgressNotification
{
	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationStatusSucceeded(String sCaptionUpdate)
	{
		super(sCaptionUpdate);
	}
}

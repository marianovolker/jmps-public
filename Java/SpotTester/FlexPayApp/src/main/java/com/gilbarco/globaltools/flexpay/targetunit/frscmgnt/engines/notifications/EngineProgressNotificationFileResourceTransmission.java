/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotificationFileResourceTransmission.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications;

import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmissionStorage;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;

/** @class EngineProgressNotificationFileResourceTransmission EngineProgressNotificationFileResourceTransmission.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications"
 *
 * @brief This class provides an engine progress notification for FileResurce transmission operation.
 * 
 * @see EngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotificationFileResourceTransmission extends EngineProgressNotification
{
	/** @brief The transmitted FileResource being notified */
	private IFileResourceTransmissionStorage fileResource;

	/** @brief Constructor
	 * 
	 * @param sCaptionUpdate is the Caption to be updated with this notification
	 * @param fileResource is the FileResource which was transmitted and being sent along with the notification.
	 * 
	 * @since 1.0.0
	 */
	public EngineProgressNotificationFileResourceTransmission(String sCaptionUpdate, IFileResourceTransmissionStorage fileResource)
	{
		super(sCaptionUpdate);
		this.fileResource = fileResource;
	}
	
	/** @brief Transmitted FileResource getter.
	 *
	 * @return the transmitted FileResource being notified.
	 *
	 * @see #setFileResourceTransmitted(String)
	 * 
	 * @since 1.0.0
	 */	
	public IFileResourceTransmissionStorage getFileResourceTransmitted ()
	{
		return this.fileResource;
	}

	/** @brief Transmitted FileResource setter.
	 *
	 * @param fileResource the transmitted FileResource being notified.
	 *
	 * @see #getFileResourceTransmitted(String)
	 * 
	 * @since 1.0.0
	 */	
	public void setFileResourceTransmitted (IFileResourceTransmissionStorage fileResource)
	{
		this.fileResource = fileResource;
	}
}
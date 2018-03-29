/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file EngineProgressNotification.java
 * @author pcantarini
 * @date Tuesday, September 10, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications;


/** @class EngineProgressNotification EngineProgressNotification.java "com.gilbarco.globaltools.flexpay.engines.notifications"
 *
 * @brief This class provides a caption update engine progress notification.
 *
 * @see IEngineProgressNotification
 * 
 * @since 1.0.0
 */
public class EngineProgressNotification implements IEngineProgressNotification {

	/** Engine progress caption update */
	private String m_sCaptionUpdate;
	
	/** @brief Constructor.
	 *
	 * @param sCaptionUpdate is the new caption to be updated when the engine make progress.
	 * 
	 * @since 1.0.0
	 */	
	public EngineProgressNotification(String sCaptionUpdate) {
		m_sCaptionUpdate = sCaptionUpdate;
	}

	/** @brief Engine progress caption update getter.
	 *
	 * @return the new caption updated when the engine made progress.
	 *
	 * @see #setCaptionUpdate(String)
	 * @see #EngineProgressNotification(String)
	 * 
	 * @since 1.0.0
	 */	
	@Override
	public String getCaptionUpdate() {
		return m_sCaptionUpdate;
	}

	/** @brief Engine progress caption update setter.
	 *
	 * @param sCaptionUpdate is the new caption to be updated when the engine make progress.
	 *
	 * @see #getCaptionUpdate()
	 * 
	 * @since 1.0.0
	 */	
	@Override
	public void setCaptionUpdate(String sCaptionUpdate) {
		m_sCaptionUpdate = sCaptionUpdate;
	}

}

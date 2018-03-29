/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISwingPublished.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities.swing;

import java.util.List;

/** @class ISwingPublished ISwingPublished.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.utilities"
 *
 * @brief This class provides a swing published (receiver) notification interface to process the notification in the "Swing Event Dispatcher Thread".
 * @since 1.0.0
 */
public interface ISwingPublished<T>
{
	/** @brief Accumulated notifications process entry point.
	 *
	 * @param list of accumulated notifications since the last execution to be processed in the "Swing Event Dispatcher Thread".
	 *
	 * @since 1.0.0
	 */	
	public void process (List<T> chunks);
}

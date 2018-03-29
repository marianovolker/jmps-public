/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IWaitCondition.java
 * @author pcantarini
 * @date Tuesday, October 15, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities;

/** @class IWaitCondition IWaitCondition.java "com.gilbarco.globaltools.flexpay.targetunit.utilities"
 * @brief This is a WaintCondition wrapper to be implemented in order to evaluate condition.
 *
 * @since 1.0.0
 */
public interface IWaitCondition
{
	public boolean expression ();
}


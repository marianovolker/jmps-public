/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file WaitCondition.java
 * @author pcantarini
 * @date Tuesday, October 15, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities;

import java.util.Calendar;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/** @class WaitCondition WaitCondition.java "com.gilbarco.globaltools.flexpay.targetunit.utilities"
 * @brief This is a WaintCondition wrapper to be implemented in order to evaluate condition.
 *
 * @since 1.0.0
 */
public abstract class WaitCondition implements IWaitCondition
{
	/** @brief Logger instance for this class type */
	static final transient Log logger = LogFactory.getLog( WaitCondition.class );

	/** @brief Waits at most timeout time for the condition expression to be satisfied.
	 * 
	 * @param monitor [In] Is the lock monitor to be used for wait and synchronization.
	 * @param waitCondition [In] Is the predicate for the Wait Condition Expression to be evaluated. 
	 * @param timeout [In] Is the amount of time to wait for the condition expression to be successful. If equals 0, waits indefinitely.
	 * 
	 * @return True if condition expression was satisfied within timeout. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	static public boolean waitConditionExpression (Object monitor, IWaitCondition waitCondition, long timeout)
	{
		boolean bResult = false;
		try
		{
			boolean bTimeout = false;
			long lTimeInterval = timeout;
			long lLastTimestampUpdate = Calendar.getInstance().getTimeInMillis();
			while ((waitCondition.expression() == false) && (bTimeout == false))
			{
				monitor.wait(lTimeInterval);
				
				if ((waitCondition.expression() == false) && (timeout > 0))
				{
					lTimeInterval = (Calendar.getInstance().getTimeInMillis() - lLastTimestampUpdate);
					if (lTimeInterval >= timeout)
					{
						bTimeout = true;
					}
					else
					{
						lTimeInterval = timeout - lTimeInterval;
					}
				}
			}
			bResult = waitCondition.expression();
		}
		catch (InterruptedException e)
		{
			logger.error("Error. Internal error while waiting condition: " + e.getMessage());
			logger.debug( e );
		}
		return bResult;
	}
	
	/** @brief Waits for the condition expression to be satisfied.
	 * 
	 * @param monitor [In] Is the lock monitor to be used for wait and synchronization.
	 * @param waitCondition [In] Is the predicate for the Wait Condition Expression to be evaluated. 
	 * 
	 * @return True if condition expression was satisfied within timeout. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	static public boolean waitConditionExpression (Object monitor, IWaitCondition waitCondition)
	{
		return waitConditionExpression(monitor,waitCondition,0);
	}

	/** @brief Wakes the monitor waiting on condition.
	 * 
	 * @param monitor [In] Is the lock monitor to be used for wait and synchronization.
	 * 
	 * @since 1.0.0
	 */ 
	static public void wakeCondition (Object monitor)
	{
		monitor.notifyAll();
	}
}

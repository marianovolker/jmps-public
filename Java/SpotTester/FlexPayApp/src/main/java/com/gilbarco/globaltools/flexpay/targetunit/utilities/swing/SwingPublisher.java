/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SwingPublisher.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.utilities.swing;

import java.util.Arrays;
import java.util.List;
import java.util.Vector;

import javax.swing.SwingUtilities;

/** @class SwingPublisher SwingPublisher.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.utilities"
*
* @brief This class provides a swing publisher (trigger-receiver) notification interface to publish and process notifications in the "Swing Event Dispatcher Thread".
* @since 1.0.0
*/
public class SwingPublisher<T> implements Runnable, ISwingPublished<T>
{
	/** @brief List of published objects pending for processing */
	private List<T> list;

	/** @brief Default Constructor
	 * 
	 * @since 1.0.0
	 */
	public SwingPublisher ()
	{
		list = new Vector<T>(); 
	}

	/** @brief Check if this publisher still have published objects pending for processing. This is a private not synchronized implementation to avoid recursion deadlock. 
	 * 
	 * @return True if there are published objects pending for processing. False otherwise.
	 * 
	 * @since 1.0.0
	 */
	private boolean isPendingForProcessPrivate ()
	{
		return (list.size() > 0);
	}

	/** @brief Check if this publisher still have published objects pending for processing.
	 * 
	 * @return True if there are published objects pending for processing. False otherwise.
	 * 
	 * @since 1.0.0
	 */
	public boolean isPendingForProcess ()
	{
		boolean bResult = false;
		synchronized (list)
		{
			bResult = isPendingForProcessPrivate();
		}
		return bResult;
	}

	/** @brief Accumulated notifications process entry point.
	 *
	 * @param list of accumulated notifications since the last execution to be processed in the "Swing Event Dispatcher Thread".
	 *
	 * @since 1.0.0
	 */	
	@Override
	public void process (List<T> chunks)
	{
	}

	/** @brief Enqueues an array of objects to be published to a specific handler which will process the obejcts in the "Swing Event Dispatcher Thread".
	 *
	 * @param published is the publication handler which will process the published objects in the "Swing Event Dispatcher Thread".
	 * @param chunks is the Array of objects which have to be published.
	 *
	 * @since 1.0.0
	 */	
	@SafeVarargs
	public static <V> void publish (ISwingPublished<V> published, V... chunks)
	{
		if (chunks.length > 0)
		{
			SwingPublisher<V> publisher = new SwingPublisher<V>()
			{
				private ISwingPublished<V> published;
				
				public SwingPublisher<V> initialize(ISwingPublished<V> published)
				{
					this.published = published;
					return this;
				}

				public void process (List<V> chunks)
				{
					this.published.process(chunks);
				}

			}.initialize(published);

			publisher.publish(chunks);
		}
	}

	/** @brief Enqueues an array of objects to be published and processed in the "Swing Event Dispatcher Thread".
	 *
	 * @param chunks is the Array of objects which have to be published.
	 *
	 * @since 1.0.0
	 */	
	@SafeVarargs
	public final void publish (T... chunks)
	{
		if (chunks.length > 0)
		{
			synchronized (list)
			{
				boolean bPendingToProcess = isPendingForProcessPrivate();
				list.addAll(Arrays.asList(chunks));
				if (bPendingToProcess == false)
				{
					SwingUtilities.invokeLater(this);
				}
			}
		}
	}

	/** @brief Background task which is run in the "Swing Event Dispatcher Thread" in order to call and run the process(chunks) user implementation.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public final void run ()
	{

		List<T> listAux = null;
		synchronized (list)
		{
			if (list.size() > 0)
			{
				listAux = new Vector<T>();
				listAux.addAll(list);
				list.clear();
			}
		}
		if (listAux != null)
		{
			process(listAux);
		}
	}
}
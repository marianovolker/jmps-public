/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileResourceCollectionEngineSwingAdapter.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapters;

import java.util.Collection;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.swing.SwingUtilities;
import javax.swing.SwingWorker;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmissionWriter;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.FileResourceCollectionEngine;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationFileResourceTransmission;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusFailed;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusInitiated;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusSucceeded;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.IEngineProgressNotification;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.swing.SwingPublisher;

/** @class FileResourceCollectionEngineSwingAdapter FileResourceCollectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
 *
 * @brief This class provides a asynchronous swing adapter for the FileResource collection engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
 * 
 * @since 1.0.0
 */
public class FileResourceCollectionEngineSwingAdapter
{
	/** @brief Logger instance for this class type */
	static final transient Log logger = LogFactory.getLog( FileResourceCollectionEngineSwingAdapter.class );

	/** @class FileResourceCollectionEngineSwingAdapter.Monitor FileResourceCollectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing helper task for the FileResource collection engine adapter to allow the proceedAtStart user implementation be run at the GUI "Swing Event Dispatcher Thread" and synchronize the adapter internally.
	 * 
	 * @see Runnable
	 * 
	 * @since 1.0.0
	 */
	private class Monitor implements Runnable
	{
		/** @brief Monitor to synchronize access to the internal fields */ 
		private Object monitor;
		/** @brief Flag to determine the last call to proceedAtStart outcome */ 
		private boolean bLastResult;
		/** @brief Flag to determine the if this monitor is waiting for proceedAtStart outcome */ 
		private boolean bWaiting;
		
		/** @brief Default Constructor.
		 * 
		 * @since 1.0.0
		 */
		Monitor ()
		{
			monitor = new Object();
			bLastResult = false;
			bWaiting = false;
		}

		/** @brief Executes this user code in the "calling thread" context before running the FileResource collection. If user code fails and reports the failure at the return, the FileResource collection is aborted.
		 *  
		 * @return True if user code executed successfully. False otherwise. 
		 * 
		 * @since 1.0.0
		 */
		public boolean proceedAtStart ()
		{
			boolean bResult = false;
			synchronized (monitor) 
			{
				if (bWaiting == false)
				{
					bLastResult = false;
					bWaiting = true;
					SwingUtilities.invokeLater(this);
					try
					{
						while (bWaiting)
						{
							monitor.wait();
						}
					}
					catch (InterruptedException e)
					{
						logger.error("Error. Monitor received an interrupted exception. " + e.getMessage(), e);
					}
					
					bResult = bLastResult;
				}
				else
				{
					logger.error("Error. Monitor already waiting.");
				}
			}
			return bResult;
		}

		/** @brief Background task which is run in the "Swing Event Dispatcher Thread" in order to call and run the proceedAtStart() user implementation.
		 * 
		 * @since 1.0.0
		 */
		@Override
		public void run ()
		{
			synchronized (monitor)
			{
				if (bWaiting)
				{
					bLastResult = FileResourceCollectionEngineSwingAdapter.this.proceedAtStart();
					bWaiting = false;
					monitor.notifyAll();
				}
			}
		}
	}

	/** @class FileResourceCollectionEngineSwingAdapter.FileResourceCollectionEngineSwingAdapterObserver FileResourceCollectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing adapter observer for the FileResource collection engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
	 * 
	 * @see Observer
	 * 
	 * @since 1.0.0
	 */
	private class FileResourceCollectionEngineSwingAdapterObserver implements Observer
	{
		/** @brief SwingPublisher helper to let the user executes code chunks in the "Swing Event Dispatcher Thread". */ 
		private SwingPublisher<IEngineProgressNotification> publisher;
		
		/** @brief Default constructor.
		 * 
		 * @since 1.0.0
		 */
		public FileResourceCollectionEngineSwingAdapterObserver ()
		{
			this.publisher = new SwingPublisher<IEngineProgressNotification>()
			{
				@Override
				public void process (List<IEngineProgressNotification> chunks)
				{
					FileResourceCollectionEngineSwingAdapterObserver.this.process(chunks);
				}
			};
		}

		/** @brief Process notifications from observables in this observer. This call is made in the foreign thread context.
		 * 
		 * @param observable which is triggering this update.
		 * @param chunk is the object being sent along with the observable notification.
		 * 
		 * @since 1.0.0
		 */
		@Override
		public void update (Observable observable, Object chunk)
		{
			if (chunk instanceof IEngineProgressNotification)
			{
				this.publisher.publish((IEngineProgressNotification)chunk); //Used this custom publisher instead of the SwingWorker publish method because the SwingWorker thread is blocked by the engine leaving the call for the SwingWorker.publish() method as a blocking point.
			}
		}

		/** @brief Accumulated notifications process entry point.
		 *
		 * @param list of accumulated notifications since the last execution to be processed in the "Swing Event Dispatcher Thread".
		 *
		 * @since 1.0.0
		 */	
		protected void process (List<IEngineProgressNotification> chunks)
		{
			for (IEngineProgressNotification chunk : chunks)
			{
				if (chunk instanceof EngineProgressNotificationFileResourceTransmission)
				{
					notifiedProgressFileResourceTransmission((EngineProgressNotificationFileResourceTransmission)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationStatusInitiated)
				{
					notifiedProgressInitiate((EngineProgressNotificationStatusInitiated)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationStatusSucceeded)
				{
					notifiedProgressSuccess((EngineProgressNotificationStatusSucceeded)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationStatusFailed)
				{
					notifiedProgressFail((EngineProgressNotificationStatusFailed)chunk);
				}
				else if (chunk instanceof EngineProgressNotification)
				{
					notifiedProgressAdvance((EngineProgressNotification)chunk);
				}
				else
				{
					notifiedProgressUnknown(chunk);
				}
			}
		}
	}


	/** @class FileResourceCollectionEngineSwingAdapter.FileResourceCollectionEngineSwingAdapterTask FileResourceCollectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing adapter task for the FileResource collection engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
	 * 
	 * @see SwingWorker
	 * @see Boolean
	 * @see IEngineProgressNotification
	 * 
	 * @since 1.0.0
	 */
	private class FileResourceCollectionEngineSwingAdapterTask extends SwingWorker<Boolean,IEngineProgressNotification>
	{
		/** @brief Accumulated notifications process entry point.
		 *
		 * @param list of accumulated notifications since the last execution to be processed in the "Swing Event Dispatcher Thread".
		 *
		 * @since 1.0.0
		 */	
		@Override
		protected void process (List<IEngineProgressNotification> chunks)
		{
			observer.process(chunks);
		}
		
		/** @brief Entry point for the task code being run in a dedicated thread.
		 *
		 * @return True if the task overall outcome was successful. False otherwise. 
		 *
		 * @since 1.0.0
		 */	
		@Override
		protected Boolean doInBackground () throws Exception
		{
			boolean bResult = false;
			Collection<Observer> observers = new Vector<Observer>();
			observers.add(observer);

			String sCaptionFileResource = null;
			String sCaptionSucessful = null;
			String sCaptionFail = null;
			Collection<IFileResourceTransmissionWriter> fileResourceVectorAux = null;

			boolean bProcessing = false;
			
			synchronized (monitor)
			{
				if (bInProgress)
				{
    				sCaptionFileResource = captionFileResource;
    				sCaptionSucessful = captionSucessful;
    				sCaptionFail = captionFail;
    				fileResourceVectorAux = fileResourceVector;
    				
    				bProcessing = true;
    
    				captionFileResource = null;
    				captionSucessful = null;
    				captionFail = null;
    				fileResourceVector = null;
				}
			}

			if (bProcessing)
			{
				if (monitor.proceedAtStart())
				{

					bResult = engine.collectFileResources(fileResourceVectorAux,observers,sCaptionFileResource,sCaptionSucessful,sCaptionFail);
				}
				else
				{
					logger.warn("Error. proceedAtStart() returned false. Collection aborted.");
					engine.abortCollection("Could not initiate collection",observers,sCaptionFileResource,sCaptionFail);
				}
			}
			return bResult;
		}
					
		/** @brief Entry point for the task code being run in the calling thread at the end of the execution at the dedicated thread.
		 *
		 * @since 1.0.0
		 */	
		@Override
		protected void done ()
		{
			boolean bResult = false;
			try
			{
				bResult = get();
				synchronized (monitor)
				{
					captionFileResource = null;
					captionSucessful = null;
					captionFail = null;
					bInProgress = false; 
					fileResourceVector = null;
				}
				proceedAtEnd();
			}
			catch (Exception e)
			{
				logger.error("Error. Unknown exception thrown while finishing swing adapter task: " + e.getMessage(), e);
			}
		}
	}

	/** @brief Caption to be published for the FileResource element */
	private String captionFileResource;
	/** @brief Caption published if collection was successful */
	private String captionSucessful;
	/** @brief Caption published if collection had failed */ 
	private String captionFail;
	
	/** @brief Flag to determine if the engine is currently processing */ 
	private boolean bInProgress;
	/** @brief Monitor to synchronize access to the internal fields */ 
	private Monitor monitor;
	/** @brief FileResurce collection engine being wrapped */ 
	private FileResourceCollectionEngine engine;
	/** @brief Connection engine adapter observer */ 
	private FileResourceCollectionEngineSwingAdapterObserver observer;
	/** @brief List of FileResources writers which can keep transmission progress information */ 
	private Collection<IFileResourceTransmissionWriter> fileResourceVector;

	/** @brief Default constructor.
	 * 
	 * @since 1.0.0
	 */
	public FileResourceCollectionEngineSwingAdapter () throws EdtServiceException
	{
		this.captionFileResource = null;
		this.captionSucessful = null;
		this.captionFail = null;
		this.bInProgress = false;
		this.monitor = new Monitor();
		this.engine = new FileResourceCollectionEngine();
		this.observer = new FileResourceCollectionEngineSwingAdapterObserver();
		this.fileResourceVector = null;
	}

	/** @brief Executes this user code in the "calling thread" context before running the FileResource collection. If user code fails and reports the failure at the return, the FileResource collection is aborted.
	 *  
	 * @return True if user code executed successfully. False otherwise. 
	 * 
	 * @since 1.0.0
	 */
	public boolean proceedAtStart ()
	{
		return true;
	}

	/** @brief Executes this user code in the "calling thread" context after running the FileResource collection.
	 * 
	 * @since 1.0.0
	 */
	public void proceedAtEnd ()
	{
	}
	
	/** @brief Entry point for an unknown progress notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressUnknown (IEngineProgressNotification notification)
	{
	}

	/** @brief Entry point for a file transmission outcome notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressFileResourceTransmission (EngineProgressNotificationFileResourceTransmission notification)
	{
	}
	
	/** @brief Entry point for an advance in progress notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressAdvance (EngineProgressNotification notification)
	{
	}

	/** @brief Entry point for initialization completion progress notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressInitiate (EngineProgressNotificationStatusInitiated notification)
	{
	}

	/** @brief Entry point for the completion progress notification with successful outcome from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressSuccess (EngineProgressNotificationStatusSucceeded notification)
	{
	}

	/** @brief Entry point for the completion progress notification with failure outcome from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressFail (EngineProgressNotificationStatusFailed notification)
	{
	}
	
	/** @brief Starts the engine collection of resources. This is an asynchronous call.
	 * 
	 * @param List of file resources writers to be used by this engine.
	 * @param Caption published for the FileResource type name.
	 * @param Caption published if collection was successful.
	 * @param Caption published if collection had failed.
	 * 
	 * @return True if the task started without issues. False otherwise. The progress is being notified asynchronously.
	 * 
	 * @since 1.0.0
	 */	
	public boolean startCollection (Collection<IFileResourceTransmissionWriter> fileResourceVector, String sCaptionFileResource, String sCaptionSucessful, String sCaptionFail)
	{
		boolean bResult = false;
		synchronized (monitor)
		{
			if (this.bInProgress == false)
			{
				FileResourceCollectionEngineSwingAdapterTask task = new FileResourceCollectionEngineSwingAdapterTask(); //SwingWorkers constrain: They can only be run once.

				this.captionFileResource = sCaptionFileResource;
				this.captionSucessful = sCaptionSucessful;
				this.captionFail = sCaptionFail;
				this.fileResourceVector = fileResourceVector;
				bInProgress = true;
				task.execute();
				bResult = true;
			}
			else
			{
				logger.warn("Error. Collection already in progress.");
			}
		}
		return bResult;
	}

	/** @brief Check if engine is currently working.
	 * 
	 * @return True if engine is currently working. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean isInProgress ()
	{
		boolean bResult = false;
		synchronized (monitor)
		{
			bResult = this.bInProgress;
		}
		return bResult;
	}
	
	/** @brief Check if engine last process outcome was successful.
	 * 
	 * @return True if engine last process outcome was successful. False otherwise.
	 * 
	 * @since 1.0.0
	 */	
	public boolean wasSuccessful ()
	{
		return this.engine.wasSuccessful();
	}
}

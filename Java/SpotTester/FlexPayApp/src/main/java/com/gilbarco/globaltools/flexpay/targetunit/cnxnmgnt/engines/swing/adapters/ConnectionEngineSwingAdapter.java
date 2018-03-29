/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ConnectionEngineSwingAdapter.java
 * @author pcantarini
 * @date Wednesday, September 18, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters;

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
import com.gilbarco.globaltools.edt.services.io.DataCommunicationLayer;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.IAuthenticationBlock;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.ConnectionEngine;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionAborted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionClosed;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionLost;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionNotGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionOpen;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.IEngineProgressNotification;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.swing.SwingPublisher;

/** @class ConnectionEngineSwingAdapter ConnectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapters"
 *
 * @brief This class provides a asynchronous swing adapter for the Challenge Authentication engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
 * 
 * @since 1.0.0
 */
public class ConnectionEngineSwingAdapter
{
	/** @brief Logger instance for this class type */
	static final transient Log logger = LogFactory.getLog(ConnectionEngineSwingAdapter.class);
	

	/** @brief Singleton default instance for backward compatibility */ 
	private static ConnectionEngineSwingAdapter	instance = null;

	/** @brief Get default singleton instance for the Connection Engine.
	 * 
	 * @return ConnectionEngineSwingAdapter singleton instance.
	 *
	 * @throws NullPointerException If no instance was set to the singleton.
	 * 
	 * @since 1.0.0
	 */ 
	public synchronized static ConnectionEngineSwingAdapter getInstance () throws NullPointerException
	{
		if (instance == null)
		{
			throw new NullPointerException();
		}
		return instance;
	}

	/** @brief Set default singleton instance for the Connection Engine.
	 * 
	 * @param newInstance [In] ConnectionEngineSwingAdapter singleton instance.
	 *
	 * @since 1.0.0
	 */ 
	public synchronized static void setInstance (ConnectionEngineSwingAdapter newInstance)
	{
		instance = newInstance;
	}
	
	/** @class ConnectionEngineSwingAdapter.IEngineCallWrapper ConnectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters"
	 * @brief This is an Engine Call Wrapper to be implemented by the user.
	 *
	 * @since 1.0.0
	 */
	private interface IEngineCallWrapper
	{
		/** @brief Trigger an engine call with provided implementation.
		 *
		 * @param observers [In] List of engine progress observers to be notified upon updates.
		 * 
		 * @return True if call is successful. False otherwise.
		 * 
		 * @since 1.0.0
		 */ 
		boolean triggerEngineCall (Collection<Observer> observers);
	}

	/** @class ConnectionEngineSwingAdapter.Monitor ConnectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing helper task for the Challenge Authentication engine adapter to allow the proceedAtStart user implementation be run at the GUI "Swing Event Dispatcher Thread" and synchronize the adapter internally.
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

		/** @brief Executes this user code in the "calling thread" context before running the FileResource collection. If user code fails and reports the failure at the return, the authentication is aborted.
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
					bLastResult = ConnectionEngineSwingAdapter.this.proceedAtStart();
					bWaiting = false;
					monitor.notifyAll();
				}
			}
		}
	}

	/** @class ConnectionEngineSwingAdapter.ConnectionEngineSwingAdapterObserver ConnectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing adapter observer for the Challenge Authentication engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
	 * 
	 * @see Observer
	 * 
	 * @since 1.0.0
	 */
	private class ConnectionEngineSwingAdapterObserver implements Observer
	{
		/** @brief SwingPublisher helper to let the user executes code chunks in the "Swing Event Dispatcher Thread". */ 
		private SwingPublisher<IEngineProgressNotification> publisher;
		
		/** @brief Default constructor.
		 * 
		 * @since 1.0.0
		 */
		public ConnectionEngineSwingAdapterObserver ()
		{
			this.publisher = new SwingPublisher<IEngineProgressNotification>()
			{
				@Override
				public void process (List<IEngineProgressNotification> chunks)
				{
					ConnectionEngineSwingAdapterObserver.this.process(chunks);
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
				if (chunk instanceof EngineProgressNotificationConnectionOpen)
				{
					notifiedProgressConnectionOpen((EngineProgressNotificationConnectionOpen)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationConnectionGranted)
				{
					notifiedProgressConnectionGranted((EngineProgressNotificationConnectionGranted)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationConnectionNotGranted)
				{
					notifiedProgressConnectionNotGranted((EngineProgressNotificationConnectionNotGranted)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationConnectionClosed)
				{
					notifiedProgressConnectionClosed((EngineProgressNotificationConnectionClosed)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationConnectionLost)
				{
					notifiedProgressConnectionLost((EngineProgressNotificationConnectionLost)chunk);
				}
				else if (chunk instanceof EngineProgressNotificationConnectionAborted)
				{
					notifiedProgressConnectionAborted((EngineProgressNotificationConnectionAborted)chunk);
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
	
	
	/** @class ConnectionEngineSwingAdapter.ConnectionEngineSwingAdapterTask ConnectionEngineSwingAdapter.java "com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapter"
	 *
	 * @brief This class provides a asynchronous swing adapter task for the Challenge Authentication engine to allow notifications to GUI be executed in the "Swing Event Dispatcher Thread".
	 * 
	 * @see SwingWorker
	 * @see Boolean
	 * @see IEngineProgressNotification
	 * 
	 * @since 1.0.0
	 */
	private class ConnectionEngineSwingAdapterTask extends SwingWorker<Boolean,IEngineProgressNotification>
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
			
			IEngineCallWrapper engineCallWrapperAux = null;
			
			boolean bProcessing = false;
			
			synchronized (monitor)
			{
				if (bInProgress)
				{
    				engineCallWrapperAux = engineCallWrapper;
    				
    				bProcessing = true;
    
    				engineCallWrapper = null;
				}
			}

			if (bProcessing)
			{
				if (monitor.proceedAtStart())
				{
					if (engineCallWrapperAux != null)
					{
						bResult = engineCallWrapperAux.triggerEngineCall(observers);
					}
				}
				else
				{
					logger.warn("Error. proceedAtStart() returned false. Collection aborted.");
					engine.abortConnection(observers);
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
					engineCallWrapper = null;

					bInProgress = false; 
				}
				proceedAtEnd();
			}
			catch (Exception e)
			{
				logger.error("Error. Unknown exception thrown while finishing swing adapter task: " + e.getMessage(), e);
			}
		}
	}

	/** @brief Current Connect Engine Call Wrapper to be executed */ 
	private IEngineCallWrapper engineCallWrapper;
	
	/** @brief Flag to determine if the engine is currently processing */ 
	private boolean bInProgress;
	/** @brief Monitor to synchronize access to the internal fields */ 
	private Monitor monitor;
	/** @brief FileResurce collection engine being wrapped */ 
	private ConnectionEngine engine;
	/** @brief Connection engine adapter observer */ 
	private ConnectionEngineSwingAdapterObserver observer;

	/** @brief Starts the engine call. This is an asynchronous call.
	 * 
	 * @param wrapper [In] the Engine Call Wrapper to be executed.
	 * 
	 * @return True if the task started without issues. False otherwise. The progress is being notified asynchronously.
	 * 
	 * @since 1.0.0
	 */	
	public boolean startEngineCall (IEngineCallWrapper wrapper)
	{
		boolean bResult = false;
		synchronized (monitor)
		{
			if (this.bInProgress == false)
			{
				ConnectionEngineSwingAdapterTask task = new ConnectionEngineSwingAdapterTask(); //SwingWorkers constrain: They can only be run once.

				this.engineCallWrapper = wrapper;
				this.bInProgress = true;
				task.execute();
				bResult = true;
			}
			else
			{
				logger.warn("Error. Task already in progress.");
			}
		}
		return bResult;
	}

	/** @brief Default constructor.
	 * 
	 * @since 1.0.0
	 */
	public ConnectionEngineSwingAdapter () throws EdtServiceException
	{
		this.engineCallWrapper = null;

		this.bInProgress = false;
		this.monitor = new Monitor();
		this.engine = new ConnectionEngine();
		this.observer = new ConnectionEngineSwingAdapterObserver();
	}

	/** @brief Executes this user code in the "calling thread" context before running the engine. If user code fails and reports the failure at the return, the engine is aborted.
	 *  
	 * @return True if user code executed successfully. False otherwise. 
	 * 
	 * @since 1.0.0
	 */
	public boolean proceedAtStart ()
	{
		return true;
	}

	/** @brief Executes this user code in the "calling thread" context after running the engine.
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

	/** @brief Entry point for an advance in progress notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressAdvance (EngineProgressNotification notification)
	{
	}

	/** @brief Entry point for a connection open notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionOpen (EngineProgressNotificationConnectionOpen notification)
	{
	}

	/** @brief Entry point for a connection granted notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionGranted (EngineProgressNotificationConnectionGranted notification)
	{
	}

	/** @brief Entry point for a connection not granted notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionNotGranted (EngineProgressNotificationConnectionNotGranted notification)
	{
	}

	/** @brief Entry point for a connection closed notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionClosed (EngineProgressNotificationConnectionClosed notification)
	{
	}

	/** @brief Entry point for a connection lost notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionLost (EngineProgressNotificationConnectionLost notification)
	{
	}

	/** @brief Entry point for a connection aborted notification from the engine. This code executes at the "calling thread" context.
	 * 
	 * @param notification is the Progress Notification report.
	 * 
	 * @since 1.0.0
	 */
	public void notifiedProgressConnectionAborted (EngineProgressNotificationConnectionAborted notification)
	{
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
	
	/** @brief Get uri address to which the connection engine was connected to.
	 * 
	 * @return Uri address to which the connection engine was connected to.
	 * 
	 * @since 1.0.0
	 */ 
	public String getUri ()
	{
		return engine.getUri();
	}

	/** @brief Get port number to which the connection engine was connected to.
	 * 
	 * @return Port number to which the connection engine was connected to.
	 * 
	 * @since 1.0.0
	 */ 
	public int getPort ()
	{
		return engine.getPort();
	}

	/** @brief Get client Id number used for this application.
	 * 
	 * @return Client Id number used for this application.
	 * 
	 * @since 1.0.0
	 */ 
	public int getClientId ()
	{
		return engine.getClientId();
	}

	/** @brief Get PPN from the connected Target Unit.
	 * 
	 * @return PPN from the connected Target Unit.
	 * 
	 * @since 1.0.0
	 */ 
	public String getPPN ()
	{
		return engine.getPPN();
	}

	/** @brief Get connection status.
	 * 
	 * @return True if connected. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isConnected ()
	{
		return engine.isConnected();
	}

	/** @brief Get login status.
	 * 
	 * @return True if logged. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isLogged ()
	{
		return engine.isLogged();
	}

	/** @brief Get authentication status.
	 * 
	 * @return True if authenticated. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean isAuthenticated ()
	{
		return engine.isAuthenticated();
	}

	/** @brief Set the authentication implementation done by the user.
	 * 
	 * @param newAuthenticationBlock [In] Authentication implementation to be used.
	 * 
	 * @since 1.0.0
	 */ 
	public void setAuthenticationBlock (IAuthenticationBlock newAuthenticationBlock)
	{
		engine.setAuthenticationBlock(newAuthenticationBlock);
	}
	
	/** @brief Get the current authentication implementation being used by this engine.
	 * 
	 * @return Current authentication implementation being used by this engine.
	 * 
	 * @since 1.0.0
	 */ 
	public IAuthenticationBlock getAuthenticationBlock ()
	{
		return engine.getAuthenticationBlock();
	}

	/** @brief Connects and Logs-in to target unit with provided uri address, port and client id.
	 *
	 * @param uri      [In] Uri address to be connected to.
	 * @param port     [In] Port number to be connected to.
	 * @param clientId [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean connectAndLogin (String uri, int port, int clientId)
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Uri address to which the socket connects  */ 
			private String sUri;
			/** @brief Port number to which the socket connects */ 
			private int iPort;
			/** @brief Client Id used by the current connection */ 
			private byte byClientId;

			/** @brief Anonymous initializer
			 * 
             * @param uri                 [In] Uri address to be connected to.
             * @param port                [In] Port number to be connected to.
             * @param clientId            [In] Client Id number to be used for this application.
			 * 
			 * @return Current anonymous instance.
			 *
			 * @since 1.0.0
			 */
			IEngineCallWrapper initialize (String uri, int port, int clientId)
			{
				this.sUri = uri;
				this.iPort = port;
				this.byClientId = ((byte)(0xFF & clientId));
				return this;
			}
			
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.connectAndLogin(observers, sUri, iPort, byClientId);
            }
		}.initialize(uri, port, clientId));
	}

	/** @brief Connects and Logs-in to target unit. 
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean connectAndLogin ()
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.connectAndLogin();
            }
		});
	}

	/** @brief Reconnects and Logs-in to target unit with provided uri address, port and client id.
	 *
	 * @param uri      [In] Uri address to be connected to.
	 * @param port     [In] Port number to be connected to.
	 * @param clientId [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean reconnectAndLogin (String uri, int port, int clientId)
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Uri address to which the socket connects  */ 
			private String sUri;
			/** @brief Port number to which the socket connects */ 
			private int iPort;
			/** @brief Client Id used by the current connection */ 
			private byte byClientId;

			/** @brief Anonymous initializer
			 * 
             * @param uri                 [In] Uri address to be connected to.
             * @param port                [In] Port number to be connected to.
             * @param clientId            [In] Client Id number to be used for this application.
			 * 
			 * @return Current anonymous instance.
			 *
			 * @since 1.0.0
			 */
			IEngineCallWrapper initialize (String uri, int port, int clientId)
			{
				this.sUri = uri;
				this.iPort = port;
				this.byClientId = ((byte)(0xFF & clientId));
				return this;
			}
			
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.reconnectAndLogin(observers, sUri, iPort, byClientId);
            }
		}.initialize(uri, port, clientId));
	}
	
	/** @brief Reconnects and Logs-in to target unit. 
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean reconnectAndLogin ()
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.reconnectAndLogin();
            }
		});
	}
	
	/** @brief Updates connection to target unit if any parameter defers from the ones given or not connected. Uses provided uri address, port, client id and authentication requirement flag.
	 *
	 * @param uri                 [In] Uri address to be connected to.
	 * @param port                [In] Port number to be connected to.
	 * @param clientId            [In] Client Id number to be used for this application.
	 * @param needsAuthentication [In] Flag which indicates if authentication should be present.
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean updateConnectionAndLogin (String uri, int port, int clientId, boolean needsAuthentication)
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Uri address to which the socket connects  */ 
			private String sUri;
			/** @brief Port number to which the socket connects */ 
			private int iPort;
			/** @brief Client Id used by the current connection */ 
			private byte byClientId;
			/** @brief Flag which indicates if authentication should be done */ 
			private boolean bNeedsAuthentication;

			/** @brief Anonymous initializer
			 * 
             * @param uri                 [In] Uri address to be connected to.
             * @param port                [In] Port number to be connected to.
             * @param clientId            [In] Client Id number to be used for this application.
             * @param needsAuthentication [In] Flag which indicates if authentication should be present.
             * 
			 * @return Current anonymous instance.
			 *
			 * @since 1.0.0
			 */
			IEngineCallWrapper initialize (String uri, int port, int clientId, boolean needsAuthentication)
			{
				this.sUri = uri;
				this.iPort = port;
				this.byClientId = ((byte)(0xFF & clientId));
				this.bNeedsAuthentication = needsAuthentication;
				return this;
			}
			
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.updateConnectionAndLogin(observers, sUri, iPort, byClientId, bNeedsAuthentication);
            }
		}.initialize(uri, port, clientId, needsAuthentication));
	}

	/** @brief Updates connection to target unit if any parameter defers from the ones given or not connected. Uses provided uri address, port and client id.
	 *
	 * @param uri      [In] Uri address to be connected to.
	 * @param port     [In] Port number to be connected to.
	 * @param clientId [In] Client Id number to be used for this application.
	 * 
	 * @return True if connection call was successfully enqueued at this call. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean updateConnectionAndLogin (String uri, int port, int clientId)
	{
		return startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Uri address to which the socket connects  */ 
			private String sUri;
			/** @brief Port number to which the socket connects */ 
			private int iPort;
			/** @brief Client Id used by the current connection */ 
			private byte byClientId;

			/** @brief Anonymous initializer
			 * 
             * @param uri                 [In] Uri address to be connected to.
             * @param port                [In] Port number to be connected to.
             * @param clientId            [In] Client Id number to be used for this application.
             * 
			 * @return Current anonymous instance.
			 *
			 * @since 1.0.0
			 */
			IEngineCallWrapper initialize (String uri, int port, int clientId)
			{
				this.sUri = uri;
				this.iPort = port;
				this.byClientId = ((byte)(0xFF & clientId));
				return this;
			}
			
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
	            return engine.updateConnectionAndLogin(observers, sUri, iPort, byClientId);
            }
		}.initialize(uri, port, clientId));
	}

	/** @brief Abort current connection.
	 * 
	 * @since 1.0.0
	 */	
	public void abortConnection ()
	{
		startEngineCall(new IEngineCallWrapper()
		{
			/** @brief Trigger and attempt to connects and log-in to target unit with provided implementation.
			 *
			 * @param observers [In] List of engine progress observers to be notified upon updates.
			 * 
			 * @return True if connection and login is granted. False otherwise.
			 * 
			 * @since 1.0.0
			 */ 
			@Override
            public boolean triggerEngineCall (Collection<Observer> observers)
            {
				engine.abortConnection(observers);
	            return false;
            }
		});
	}
	
	/** @brief Commands the authentication implementation to force the target unit to forget current authentication.
	 *
	 * @since 1.0.0
	 */ 
	public void forgetAuthentication ()
	{
		engine.forgetAuthentication();
	}

	/** @brief Asynchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully queued for sending (no done yet, waiting to be sent later). False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean asyncSend (byte [] data)
	{
		return engine.asyncSend(data);
	}

	/** @brief Synchronously sends data to connected target unit.
	 *
	 * @param data [In] Buffer holding the data that needs to be sent.
	 * 
	 * @return True if successfully sent. False otherwise.
	 * 
	 * @since 1.0.0
	 */ 
	public boolean send (byte [] data)
	{
		return engine.send(data);
	}

	/** @brief Closes connection to target unit.
	 *
	 * bDeleteObservers [In] Indicate if observers should be removed upon close.
	 *
	 * @since 1.0.0
	 */ 
	public void close (boolean bDeleteObservers)
	{
		engine.close(bDeleteObservers);
	}

	/** @brief Closes connection to target unit.
	 *
	 * @since 1.0.0
	 */ 
	public void close ()
	{
		engine.close();
	}

	/** @brief Get a DataCommunicationLayer wrapper around this connection engine.
	 *
	 * @return DataCommunicationLayer wrapper around this connection engine.
	 *
	 * @since 1.0.0
	 */ 
	public DataCommunicationLayer getDataCommunicationLayerWrapper ()
	{
		return engine.getDataCommunicationLayerWrapper();
	}
}

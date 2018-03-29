/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file FlexPayMaintenanceController.java
 * @author mgrieco
 * @date 27 Ago 2012
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.maintenance.gui;

import java.awt.Desktop;
import java.awt.HeadlessException;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.SwingWorker;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.edt.util.EdtFileUtil;
import com.gilbarco.globaltools.edt.util.EdtSignData;
import com.gilbarco.globaltools.edt.util.EdtUtil;
import com.gilbarco.globaltools.flexpay.engines.ComparisonEngine;
import com.gilbarco.globaltools.flexpay.engines.ComparisonEngine.SoftwareComparisonStatus;
import com.gilbarco.globaltools.flexpay.engines.LogCompressionEngine;
import com.gilbarco.globaltools.flexpay.engines.PackageUploadEngine;
import com.gilbarco.globaltools.flexpay.engines.ReleaseBundleCopyEngine;
import com.gilbarco.globaltools.flexpay.engines.ResourceUploadEngine;
import com.gilbarco.globaltools.flexpay.engines.SecureMenuEngine;
import com.gilbarco.globaltools.flexpay.engines.UploadNotification;
import com.gilbarco.globaltools.flexpay.engines.strategies.ICompareStrategy;
import com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy;
import com.gilbarco.globaltools.flexpay.engines.strategies.CompareM3M5Strategy;
import com.gilbarco.globaltools.flexpay.engines.strategies.M3M5ROMfsPackageUploadStrategy;
import com.gilbarco.globaltools.flexpay.engines.strategies.CompareVanguardStrategy;
import com.gilbarco.globaltools.flexpay.engines.strategies.VanguardROMfsPackageUploadStrategy;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies.IAuthenticationStrategy;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionClosed;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionLost;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionNotGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionOpen;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.FileResourceLocation;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.FileResourceTransmissionWriterFile;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.IFileResourceTransmissionWriter;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.FileResourceLogSecure;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.FileResourceLogStandard;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.FileResourceTransmissionStorageFileLog;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.api.log.IFileResourceTransmissionStorageFileLog;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationFileResourceTransmission;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusFailed;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusInitiated;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.notifications.EngineProgressNotificationStatusSucceeded;
import com.gilbarco.globaltools.flexpay.targetunit.frscmgnt.engines.swing.adapters.FileResourceCollectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.EngineProgressNotification;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.engines.notifications.IEngineProgressNotification;

/**
 * The Class FlexPayMaintenanceController.
 */
public class FlexPayMaintenanceController extends EdtAppController<FlexPayMaintenanceView, FlexPayMaintenanceModel> implements Observer
{
	/** The Constant logger. */
	private static final transient Log						logger					= LogFactory.getLog( FlexPayMaintenanceController.class );

	/** @class FlexPayMaintenanceController.ConnectionEngineWorker FlexPayMaintenanceController.java "com.gilbarco.globaltools.flexpay.maintenance.gui"
	 * @brief This interface provides the entry points for a connection worker that should be implemented for the specific task.
	 * 
	 * @since 1.0.0
	 */
	private interface ConnectionEngineWorker
	{
		/** @brief Entry point for a connection granted notification from the work dispatcher. This code executes at the "calling thread" context.
		 * 
		 * @param bAuthenticated informs if the granted connection is authenticated.
		 * 
		 * @since 1.0.0
		 */
		public void invokeOnConnectionGrated (boolean bAuthenticated);

		/** @brief Entry point for a connection not granted notification from the work dispatcher. This code executes at the "calling thread" context.
		 * 
		 * @param bActive informs if the not granted connection is still active.
		 * 
		 * @since 1.0.0
		 */
		public void invokeOnConnectionNotGrated (boolean bActive);
	}
	
	/** @class FlexPayMaintenanceController.ConnectionEngineWorkDispatcher FlexPayMaintenanceController.java "com.gilbarco.globaltools.flexpay.maintenance.gui"
	 * @brief This class provides the dispatcher for connection workers which are implemented for specific tasks.
	 * 
	 * @since 1.0.0
	 */
	private class ConnectionEngineWorkDispatcher
	{
		/** @brief Instance synchronization monitor */ 
		private Object monitor;
		
		/** @brief Internal implementation of the ConnectionEngineSwingAdapter which will be used as a singleton instance */ 
		private ConnectionEngineSwingAdapter connectionWrapper;
		
		/** @brief Ordered list of workers which should be called upon connection dispatch calls */ 
		private List<ConnectionEngineWorker> workers;
		
		/** @brief Default Constructor.
		 * 
		 * @since 1.0.0
		 */
		public ConnectionEngineWorkDispatcher ()
        {
			this.monitor = new Object();
			this.connectionWrapper = null;
			this.workers = new Vector<ConnectionEngineWorker>();
        }

		/** @brief Setup the ConnectionEngineSwingAdapter singleton instance as the the connection work dispatcher implementation.
		 *
		 * @trows EdtServiceException if an error is found while creating the instance of the ConnectionEngineSwingAdapter.
		 * 
		 * @since 1.0.0
		 */
		public boolean setupConnectionEngine () throws EdtServiceException
		{
			boolean bResult = false;
			synchronized (monitor)
			{
				if (connectionWrapper == null)
				{
					connectionWrapper = new ConnectionEngineSwingAdapter()
	    			{
	    				/** @brief Entry point for a connection granted notification from the engine. This code executes at the "calling thread" context.
	    				 * 
	    				 * @param notification is the Progress Notification report.
	    				 * 
	    				 * @since 1.0.0
	    				 */
	    				public void notifiedProgressConnectionGranted (EngineProgressNotificationConnectionGranted notification)
	    				{
	    					logger.info("Connection granted.");
	    					if (workers.isEmpty() == false)
	    					{
	    						ConnectionEngineWorker worker = workers.remove(0);
	    						worker.invokeOnConnectionGrated(notification.isAuthenticated());
	    					}
	    				}
	    
	    				/** @brief Entry point for a connection not granted notification from the engine. This code executes at the "calling thread" context.
	    				 * 
	    				 * @param notification is the Progress Notification report.
	    				 * 
	    				 * @since 1.0.0
	    				 */
	    				public void notifiedProgressConnectionNotGranted (EngineProgressNotificationConnectionNotGranted notification)
	    				{
	    					logger.info("Connection not granted.");
	    					if (workers.isEmpty() == false)
	    					{
	    						ConnectionEngineWorker worker = workers.remove(0);
	    						worker.invokeOnConnectionNotGrated(notification.isActive());
	    					}
	    				}

	    				/** @brief Entry point for a connection open notification from the engine. This code executes at the "calling thread" context.
	    				 * 
	    				 * @param notification is the Progress Notification report.
	    				 * 
	    				 * @since 1.0.0
	    				 */
	    				public void notifiedProgressConnectionOpen (EngineProgressNotificationConnectionOpen notification)
	    				{
	    					getView().startCountDownClock();
	    				}

	    				/** @brief Entry point for a connection closed notification from the engine. This code executes at the "calling thread" context.
	    				 * 
	    				 * @param notification is the Progress Notification report.
	    				 * 
	    				 * @since 1.0.0
	    				 */
	    				public void notifiedProgressConnectionClosed (EngineProgressNotificationConnectionClosed notification)
	    				{
	    					getView().stopCountDownClock();
	    				}

	    				/** @brief Entry point for a connection lost notification from the engine. This code executes at the "calling thread" context.
	    				 * 
	    				 * @param notification is the Progress Notification report.
	    				 * 
	    				 * @since 1.0.0
	    				 */
	    				public void notifiedProgressConnectionLost (EngineProgressNotificationConnectionLost notification)
	    				{
	    					getView().stopCountDownClock();
	    				}
	    			};
				}

				if (connectionWrapper != null)
				{
					workers.clear();
					ConnectionEngineSwingAdapter.setInstance(connectionWrapper);
					bResult = true;
				}
			}
			return bResult;
		}
		
		/** @brief Dispatch a new worker for the connection and verify if it is granted according to the worker requirements.
		 *
		 * @param worker [In] The worker which will be dispatched and executed.
         * @param uri                 [In] Uri address to be connected to.
         * @param port                [In] Port number to be connected to.
         * @param clientId            [In] Client Id number to be used for this application.
         * @param needsAuthentication [In] Flag which indicates if authentication should be present.
		 * 
		 * @return True if connection call was successfully dispatched at this call. False otherwise.
		 * 
		 * @since 1.0.0
		 */
		public boolean dispatch (ConnectionEngineWorker worker, String uri, int port, int clientId, boolean needsAuthentication)
		{
			boolean bResult = false;
			if (worker != null)
			{
				synchronized (monitor)
				{
					workers.add(worker);
					bResult = ConnectionEngineSwingAdapter.getInstance().updateConnectionAndLogin(uri,port,clientId,needsAuthentication);
				}
			}
			return bResult;
		}

		/** @brief Dispatch a new worker for the connection and verify if it is granted according to the worker requirements.
		 *
		 * @param worker [In] The worker which will be dispatched and executed.
         * @param uri                 [In] Uri address to be connected to.
         * @param port                [In] Port number to be connected to.
         * @param clientId            [In] Client Id number to be used for this application.
		 * 
		 * @return True if connection call was successfully dispatched at this call. False otherwise.
		 * 
		 * @since 1.0.0
		 */
		public boolean dispatch (ConnectionEngineWorker worker, String uri, int port, int clientId)
		{
			return dispatch(worker,uri,port,clientId,false);
		}
	}
	
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The maintenance controller. */
	private static volatile FlexPayMaintenanceController	maintenanceController	= null;

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	/** The model. */
	protected FlexPayMaintenanceModel							model;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	/** The boolean value for initialized. */
	volatile private boolean								blInitialized			= false;

	/** The comparison result. */
	private SoftwareComparisonStatus						comparisonResult		= SoftwareComparisonStatus.NOTHING_TO_COMPARE;

	/** The resource file engine. */
	private ResourceUploadEngine							resourceFileEngine;

	/** The romfs package upgrading engine. */
	private PackageUploadEngine								romfsPackageUpgradingEngine;

	/** The romfs package comparison engine. */
	private ComparisonEngine								comparisonEngine;

	/** The release bundle copy engine. */
	private ReleaseBundleCopyEngine							releaseBundleCopyEngine;
	
	/** The log compression Engine. */
	private LogCompressionEngine							logCompressionEngine;
	
	/** The secure menu Engine. */
	private SecureMenuEngine								secureMenuEngine;

	/** The comparison worker. */
	private StoreReleaseTask								storeReleaseWorker;

	/** The comparison worker. */
	private ComparisionTask									comparisionWorker;

	/** The secure menu worker. */
	private SecureMenuTask									secureMenuWorker;
	
	/** The installation worker. */
	private InstallationTask								installationWorker;
	
	/** The collection worker. */
	private FileResourceCollectionTask                      collectionWorker;

	/** The log compression Engine. */
	private LogCompressionTask								logCompressionWorker;

	/** The select software version dialog. */
	private SelectSoftwareVersionDialog						selectSoftwareVersionDialog;

	/** The select usb options dialog. */
	private SelectUSBOptionsDialog							selectUSBOptionsDialog;

	/** The select directory dialog. */
	private SelectDirectoryDialog							selectDirectoryDialog;

	/** The current selected version. */
	private String											versionSelected;

  /** The current selected usb drive. */
	private String											usbSelected;

	/** The compare uploadStrategy. */
	private ICompareStrategy								compareStrategy;

	/** The uploadStrategy. */
	private IROMfsPackageUploadStrategy						uploadStrategy;

	/** @brief The connection work dispatcher. */
	private ConnectionEngineWorkDispatcher					connectionWorkDispatcher;
	
	/** The boolean value about the secure maneu activation. */
	private boolean											secureMenuActive;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	/**
	 * Instantiates a new flex pay maintenance controller.
	 * 
	 * @param view
	 *            the view
	 */
	private FlexPayMaintenanceController(FlexPayMaintenanceView view)
	{
		super( view );
		logger.debug( "FlexPayMaintenanceController constructed." );

		initializeKeys( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS.
	// ******************************************************************
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppController#initController()
	 */
	/**
	 * Inits the controller.
	 */
	@Override
	public void initController()
	{
		logger.debug( "FlexPayMaintenanceController - initController()" );

		if (blInitialized)
		{
			return; // initialize work is done only once
		}

		this.model = new FlexPayMaintenanceModel( );
		this.setAppModel( model );
		FlexPayContext.setAppLocale( this.getAppLocale( ) );
		try
		{
			connectionWorkDispatcher = new ConnectionEngineWorkDispatcher();
			connectionWorkDispatcher.setupConnectionEngine();
			
			compareStrategy = null;
			uploadStrategy = null;
			if (FlexPayMaintenanceModel.isVanguardDevice( ))
			{
				uploadStrategy = new VanguardROMfsPackageUploadStrategy( );
			}
			else
			{
				uploadStrategy = new M3M5ROMfsPackageUploadStrategy( );
			}

			romfsPackageUpgradingEngine = new PackageUploadEngine( uploadStrategy );
			romfsPackageUpgradingEngine.addObserver( this );

			resourceFileEngine = new ResourceUploadEngine( );
			resourceFileEngine.addObserver( this );
			
			comparisonEngine = new ComparisonEngine( );
			comparisonEngine.addObserver( this );

			releaseBundleCopyEngine = new ReleaseBundleCopyEngine();			
			logCompressionEngine = new LogCompressionEngine();
			
			secureMenuEngine = new SecureMenuEngine();
			secureMenuEngine.addObserver( this );
			setSecureMenuActive(false);
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
		blInitialized = true;
	}

	/**
	 * return true when the secure menu is activated.
	 */
	private boolean isSecureMenuActive()
	{
		return secureMenuActive;
	}

	/**
	 * sets the secure menu as activated according to the active parameter.
	 */
	private void setSecureMenuActive(boolean active)
	{
		secureMenuActive = active;		
	}

	
	public void setAuthenticationStrategy (IAuthenticationStrategy authenticationStrategy)
	{
		if (authenticationStrategy != null)
		{
			ConnectionEngineSwingAdapter.getInstance().setAuthenticationBlock(authenticationStrategy.getConnectionAuthenticationBlock());
		}
		else
		{
			ConnectionEngineSwingAdapter.getInstance().setAuthenticationBlock(null);
		}
	}
	
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppController#updateMenuItemsState(javax.swing.JMenu)
	 */
	/**
	 * Update menu items state.
	 *
	 * @param menu the menu
	 */
	@Override
	public void updateMenuItemsState(JMenu menu)
	{
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppController#addMenuItems(javax.swing.JMenu)
	 */
	/**
	 * Adds the menu items.
	 *
	 * @param menu the menu
	 */
	@Override
	public void addMenuItems(JMenu menu)
	{

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.gilbarco.globaltools.edt.gui.app.EdtAppController#onEdtAppModelChanged(com.gilbarco.globaltools.edt.gui.app.EdtAppModel)
	 */
	/**
	 * On edt app model changed.
	 *
	 * @param model the model
	 */
	@Override
	protected void onEdtAppModelChanged(FlexPayMaintenanceModel model)
	{
		this.updateButtons( model.isLogged( ), model.isStarted( ) );
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.util.Observer#update(java.util.Observable, java.lang.Object)
	 */
	/**
	 * Update.
	 *
	 * @param o the o
	 * @param arg the arg
	 */
	@Override
	public void update(Observable o, Object arg)
	{
		if (o instanceof ResourceUploadEngine || o instanceof PackageUploadEngine)
		{
			uploadEnginesUpdates( o, arg );
			return;
		}
	}

	/**
	 * Upload engines updates.
	 *
	 * @param o the o
	 * @param arg the arg
	 */
	private void uploadEnginesUpdates(Observable o, Object arg)
	{
		String sAction;

		if (o instanceof PackageUploadEngine)
		{
			sAction = "Uploading software";
		}
		else
		{
			sAction = "Deleting and Uploading resources";
		}

		if (arg instanceof UploadNotification)
		{
			// data for GUI update is here
			UploadNotification u = (UploadNotification) arg;
			String str = String.format( "%s - file blocks %d of %d ", sAction, u.getUploaded( ), u.getTotal( ));
			getView( ).incrementProgress( u.getUploaded( ), u.getTotal( ) + 1, 1, str, false );
		}
	}

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	/**
	 * Gets or create controller.
	 * 
	 * @param view
	 *            the view
	 * @return the or create controller
	 */
	public static FlexPayMaintenanceController getOrCreateController(FlexPayMaintenanceView view)
	{
		if (maintenanceController != null) // preventive question previous to lock the class
		{
			maintenanceController.setView( view );
			if (maintenanceController.connectionWorkDispatcher != null)
			{
				try
	            {
					maintenanceController.connectionWorkDispatcher.setupConnectionEngine();
	            }
	            catch (EdtServiceException e)
	            {
		            logger.error("Error. Exception thrown while updateing connection engine." + e.getMessage(),e);
	            }
			}
			return maintenanceController;
		}

		return createController( view );
	}

	/**
	 * Creates the controller.
	 * 
	 * @param view
	 *            the view
	 * @return the flex pay maintenance controller
	 */
	public static synchronized FlexPayMaintenanceController createController(FlexPayMaintenanceView view)
	{
		if (maintenanceController == null) // race condition winner enters here
		{
			maintenanceController = new FlexPayMaintenanceController( view );
		}

		return maintenanceController;
	}

	/**
	 * Gets the version folder.
	 * 
	 * @return the version folder
	 */
	public String getVersionFolder()
	{
		return model.getVersionFolder( );
	}

	/**
	 * Gets the resources folder.
	 * 
	 * @return the resources folder
	 */
	public static String getResourcesFolder()
	{
		return FlexPayMaintenanceModel.getResourcesFolder( );
	}


	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************
	/**
	 * Gets the comparison result.
	 * 
	 * @return the comparison result
	 */
	public SoftwareComparisonStatus getComparisonResult()
	{
		return comparisonResult;
	}

	/**
	 * Gets the model.
	 * 
	 * @return the model
	 */
	public FlexPayMaintenanceModel getModel()
	{
		this.updateModel( );
		return this.model;
	}

	/**
	 * Disconnect to upm.
	 */
	private void disconnectToUPM()
	{
		logger.error( "Closing Connection." );
		try
		{
			ConnectionEngineSwingAdapter.getInstance().close();
		}
		catch (Exception e)
		{
			logger.error("Error. Exception thrown while closing connection at target unit: '" + getView().getIpInserted() + "'.\nException: '" + e.getMessage() + "'.");
		}
		logger.info( "SPOT Connection socket closed." );
	}

	/** @brief Called when the view wants to inform the controller the user requested to close the maintenance session  
	 * 
	 * @since 1.0.0
	 */
	public void closedMaintenanceNotification()
	{
		disconnectToUPM();
		ConnectionEngineSwingAdapter.setInstance(null);
	}
	
	/**
	 * Start install.
	 * 
	 * @return true, if successful
	 */
	public boolean startInstall()
	{
		view.incrementProgress( 1, getMaxFilesToInstall( ) + 1, 1, "starting installation...", false );
		logger.info( "Connecting - " + getView().getIpInserted( ) + ":" + model.getTerminalPort( ) );							

		if (ConnectionEngineSwingAdapter.getInstance().isConnected()) 
		{
			if( !upgradeResources( ))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		
		if (!upgradeROMfsPackages( ))
		{
			rebootOPT( );
			disconnectToUPM( );
			return false;
		}
		else
		{
			activatePackagesAndRebootOPT( );
		}
		return true;
	}

	/**
	 * @return the amount of files to be installed.
	 */
	private int getMaxFilesToInstall()
	{
		File currentResourceFolder = new File( getResourcesFolder() + getLastSelectedVersion( ) );
		int rescount =  (!currentResourceFolder.exists( ) || !currentResourceFolder.isDirectory( )) ? 0 : currentResourceFolder.list( ).length;		
		
		File currentFolder = new File( getVersionFolder( ) + getLastSelectedVersion( ) );
		int verscount = (!currentFolder.exists( ) || !currentFolder.isDirectory( )) ? 0 : currentFolder.list( ).length;
		
		return rescount + verscount;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * Update model.
	 */
	protected void updateModel()
	{
	}

	/**
	 * Update buttons.
	 * 
	 * @param logged
	 *            the logged
	 * @param started
	 *            the started
	 */
	protected void updateButtons(boolean logged, boolean started)
	{
	}

	/**
	 * Show load screen.
	 * 
	 * @param action
	 *            the action
	 * @param obj
	 *            the obj
	 * @return the file
	 */
	protected File showLoadScreen(Method action, Object obj)
	{
		try
		{
			JFileChooser fc = new JFileChooser( );
			fc.setFileSelectionMode( JFileChooser.FILES_AND_DIRECTORIES );
			int returnValue = fc.showOpenDialog( this.getView( ) );
			if (returnValue == JFileChooser.APPROVE_OPTION)
			{
				File file = fc.getSelectedFile( );
				if (file.isFile( ))
				{
					if (action != null)
					{
						try
						{
							action.invoke( obj, file );
						}
						catch (Exception e1)
						{
							logger.error( e1.getMessage( ) );
						}
					}
					return file;
				}
				else
				{
					showMessageDialog( getLocaleText( "FileCheck" ), "" );
				}
			}
		}

		catch (HeadlessException e)
		{
			logger.error( e.getMessage( ) );
		}
		return new File( "" );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	/**
	 * Initialize keys.
	 */
	private void initializeKeys()
	{
		EdtSignData.getInstance( ).setPrivateKeyFilename( model.getFlexpayPrivateKeyFilename( ) );
		EdtSignData.getInstance( ).setCertificateFilename( model.getSasCertificateFilename( ) );
	}

	/**
	 * Sets the view.
	 * 
	 * @param view
	 *            the new view
	 */
	protected void setView(FlexPayMaintenanceView view)
	{
		this.view = view;
	}

	/**
	 * Collect resources.
	 * 
	 * @param path
	 *            the path
	 */
	private void collectResources(String path)
	{
		File currentFolder = new File( path );
		if (!currentFolder.exists( ) || !currentFolder.isDirectory( ))
		{
			logger.error( "Resources to upload folder: " + path + "  doesn't exist.\n" );
			return;
		}

		File[] fileVector = currentFolder.listFiles( );

		for (File file : fileVector)
		{
			if (file.isDirectory( ))
			{
				try
				{
					collectResources( file.getCanonicalPath( ) );
				}
				catch (IOException e)
				{
					logger.error( e.getMessage( ) );					
				}
			}
			else
			{
				if (!file.exists( )) return;

				resourceFileEngine.addFileToUpload( file );
			}
		}
	}

	/**
	 * Collects Software Information.
	 * Gets the local files information.
	 * 
	 * @param path the local path to collect the file information
	 * @param blUpgrading true if it is called to upload software.
	 */
	private void collectSoftwareInformation(String path, boolean blUpgrading)
	{
		logger.debug( "collectSoftwareInformation - start" );
		File currentFolder = new File( path );
		if (!currentFolder.exists( ) || !currentFolder.isDirectory( ))
		{
			String sAction = (blUpgrading ? "Package Upgrade" : "Comparison");
			logger.error( "Version folder: " + path + "  doesn't exist. " + sAction + " finished.\n" );
			return;
		}

		File[] fileVector = currentFolder.listFiles( );
		for (File file : fileVector)
		{
			if (!file.isDirectory( ))
			{
				if (!file.exists( ))
				{
					logger.error( "This file (" + file.getAbsolutePath( ) + ") has an error on it!." );
					continue;
				}
				ROMfsCertificate certificate = null;
				certificate = (blUpgrading) ? uploadStrategy.newPackageCertificate( file ) : compareStrategy.newPackageCertificate( file );
					
				if (!certificate.loadCertificate( !blUpgrading ))
				{
					String sErr = "This file (" + file.getAbsolutePath( ) + ") has an invalid certificate on it!.";
					logger.error( sErr );					
					continue;
				}
				else
				{
					String sInfo = new String( "" );
					sInfo += "File " + file.getName( ) + " scanned OK.\nCertificate: ";
					sInfo += "[" + EdtConvert.bytesToHexString( certificate.getCertificateData( ), 1 ) + "]";
					sInfo += "\nSignature: ";
					sInfo += "[" + EdtConvert.bytesToHexString( certificate.getSignatureData( ), 1 ) + "]";
					logger.debug( sInfo );
				}

				if (blUpgrading)
				{
					romfsPackageUpgradingEngine.addFileToUpload( certificate );
				}
				else
				{
					String appName = new String( certificate.getAppName( ) );
					String appVersion = new String( certificate.getAppVersion( ) );
					comparisonEngine.addLocalAppToVerify( appName, appVersion );
				}
			}
		}
		logger.debug( "collectSoftwareInformation - end" );		
	}

	/**
	 * Upgrade ro mfs packages.
	 * 
	 * @return true, if successful
	 */
	private boolean upgradeROMfsPackages()
	{
		String sPkgFolder = getLastSelectedVersion( );
		String path = getVersionFolder( ) + sPkgFolder;

		romfsPackageUpgradingEngine.clearVectorFilesToUpload( );
		collectSoftwareInformation( path, true );

		try
		{
			romfsPackageUpgradingEngine.upgradePackages( );

			if (!romfsPackageUpgradingEngine.hasErrors( ))
			{
				return true;
			}

			return false;
		}
		catch (InterruptedException e)
		{
			logger.error( e.getMessage( ) );
		}

		return false;
	}

	/**
	 * Upgrade resources.
	 * 
	 * @return true, if successful
	 */
	private boolean upgradeResources()
	{
		String sPkgFolder = getLastSelectedVersion( );
		String path = getResourcesFolder( ) + sPkgFolder;

		resourceFileEngine.clearVectorFilesToUpload( );
		collectResources( path );

		try
		{
			resourceFileEngine.upgradeResources( );

			if (!resourceFileEngine.hasErrors( ))
			{
				return true;
			}

			return false;
		}
		catch (InterruptedException e)
		{
			logger.error( e.getMessage( ) );
		}

		return false;
	}


	/**
	 * Activate packages and reboot opt.
	 */
	private void activatePackagesAndRebootOPT()
	{
		boolean blActivationReceived = false;

		romfsPackageUpgradingEngine.activatePackagesAndRebootOPT( );

		try
		{
			blActivationReceived = romfsPackageUpgradingEngine.waitActivationReply( ); // wait for activation reply
		}
		catch (InterruptedException e)
		{
			logger.error( "Interrupted waiting activation reply" + e.getMessage( ) );
		}

		String sFailed = " Installation failed.";

		if (!blActivationReceived)
		{
			logger.error( "Package Activation reply was not received." + sFailed );			
		}
		else
		{
			int iAckCode = (int) romfsPackageUpgradingEngine.getLastAckCode( );

			if (iAckCode < 0)
			{
				iAckCode += 256;
			}

			String sMsg = "Package Activation ";
			String sErr = null;

			if (iAckCode == 0)
			{
				sErr = "succeeded. Installation succeeded !";
				logger.info( sMsg + sErr );
			}
			else
			{
				sErr = "failed, error code: " + String.format( "0x%02X%s", iAckCode, sFailed );
				logger.error( sMsg + sErr );
			}
		}

		// package activation does not reboot the unit in Vanguard OPT units,
		// In this case we always need to reboot the unit no matter weather the
		// installation finished OK or with errors
		if (FlexPayMaintenanceModel.isVanguardDevice( ))
		{
			rebootOPT( );
		}
	}

	/**
	 * Reboot opt.
	 */
	private void rebootOPT()
	{
		romfsPackageUpgradingEngine.rebootOPT( );
		logger.info( "OPT unit is restarting right now." );		
	}

	/**
	 * Show message dialog.
	 *
	 * @param message the message
	 * @param title the title of of dialog
	 */
	private void showMessageDialog(String message, String title)
	{
		showMessageDialog(message, title, JOptionPane.INFORMATION_MESSAGE);
	}
	
	/**
	 * Show message dialog.
	 *
	 * @param message the message
	 * @param title the title of the dialog
	 * @param option the type of dialog
	 */
	private void showMessageDialog(String message, String title, int option)
	{
		JOptionPane pane = new JOptionPane( message, option );
		JDialog dialog = pane.createDialog( title );
		dialog.setAlwaysOnTop( true );
		dialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());

		dialog.setVisible( true );
	}

	/**
	 * Show error dialog.
	 * 
	 * @param message
	 *            the message
	 */
	private void showErrorDialog(String message)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		JDialog dialog = pane.createDialog( "Error" );
		dialog.setAlwaysOnTop( true );
		dialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());

		dialog.setVisible( true );
	}


	/**
	 * Gets the last selected version.
	 * 
	 * @return the last selected version
	 */
	public String getLastSelectedVersion()
	{
		return this.versionSelected;
	}

	/**
	 * Select software version.
	 *
	 * @param action the action
	 * @param tooltip the tooltip
	 * @return true, if successful
	 */
	protected boolean selectSoftwareVersion(String action, String tooltip)
	{
		boolean result = false;
		if (this.selectSoftwareVersionDialog == null) // dialog never instanced
		{
			selectSoftwareVersionDialog = new SelectSoftwareVersionDialog( model.getVersionFolder( ) );
			selectSoftwareVersionDialog.setLocationRelativeTo( null );
			selectSoftwareVersionDialog.setAlwaysOnTop( true );
			selectSoftwareVersionDialog.setModal( true );			
			selectSoftwareVersionDialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());
		}

		selectSoftwareVersionDialog.setTitle( "Select Software Version - " + action );
		selectSoftwareVersionDialog.setVersionsToolTip( tooltip );
		selectSoftwareVersionDialog.buildVersionList( getModel( ).getVersionFolder( ) );
		selectSoftwareVersionDialog.clearVersionSelected( );
		selectSoftwareVersionDialog.enableRequest( );

		if(selectSoftwareVersionDialog.getComboBoxVersions( ).getItemCount( )>0)
		{
			// Finally here the control is transfered to the dialog
			selectSoftwareVersionDialog.setVisible( true );
			result = selectSoftwareVersionDialog.versionSelected( );
			versionSelected = selectSoftwareVersionDialog.versionItemSelected( );
		}
		else
		{
			versionSelected = "";
			showMessageDialog( "Not Versions Available on configured folder " + getModel( ).getVersionFolder( ), "Error", JOptionPane.WARNING_MESSAGE );
		}
		selectSoftwareVersionDialog.dispose( );
		return result;
	}

	/**
	 * Choose logs directory.
	 * 
	 * @param bSecure [In] Flag which indicates if the logs to be retrieved are secure. 
	 * 
	 * @return true, if successful
	 */
	protected boolean chooseLogsDirectory(boolean bSecure)
	{
		String destinationLogsFolder;
		if (bSecure)
		{
			destinationLogsFolder = model.getSecureLogsFolder( );
		}
		else
		{
			destinationLogsFolder = model.getStandardLogsFolder();
		}

		if (this.selectDirectoryDialog == null) // dialog never instanced
		{
			selectDirectoryDialog = new SelectDirectoryDialog( );
			selectDirectoryDialog.setLocationRelativeTo( null );
			selectDirectoryDialog.setAlwaysOnTop( true );
			selectDirectoryDialog.setModal( true );
			selectDirectoryDialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());
		}
		selectDirectoryDialog.setTitle( "Choose Directory" );
		selectDirectoryDialog.clearDirectorySelected( );
		selectDirectoryDialog.setDefaultDirectory( destinationLogsFolder );
		selectDirectoryDialog.enableRequest( );

		// Finally here the control is transfered to the dialog
		selectDirectoryDialog.setVisible( true );
		boolean result = selectDirectoryDialog.directorySelected( );
		if(result)
		{
			if (bSecure)
				model.setSecureLogsFolder( selectDirectoryDialog.getSelectedDirectory().toString( ) );
			else
				model.setStandardLogsFolder( selectDirectoryDialog.getSelectedDirectory().toString( ) );
		}
		selectDirectoryDialog.dispose( );
		return result;
	}

	/**
	 * Select storage usb options.
	 * 
	 * @return true, if successful
	 */
	protected boolean selectStorageUSBOptions()
	{
		if (this.selectUSBOptionsDialog == null) // dialog never instanced
		{
			selectUSBOptionsDialog = new SelectUSBOptionsDialog( );
			selectUSBOptionsDialog.setLocationRelativeTo( null );
			selectUSBOptionsDialog.setAlwaysOnTop( true );
			selectUSBOptionsDialog.setModal( true );			
			selectUSBOptionsDialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());
		}

		selectUSBOptionsDialog.setTitle( "Select USB Storage Options" );
		selectUSBOptionsDialog.buildVersionList( getVersionFolder( ) );
		selectUSBOptionsDialog.clearVersionSelected( );
		selectUSBOptionsDialog.loadDrivesComboBoxData( );
		selectUSBOptionsDialog.enableRequest( );

		// Finally here the control is transfered to the dialog
		selectUSBOptionsDialog.setVisible( true );
		boolean result = selectUSBOptionsDialog.versionSelected( );
		if (result)
		{
			versionSelected = selectUSBOptionsDialog.getVersionItemSelected( );
			usbSelected = selectUSBOptionsDialog.getUSBSelected( );
		}
		else
		{
			versionSelected = "";
			usbSelected = "";
		}

		selectUSBOptionsDialog.dispose( );
		return result;
	}

	/**
	 * Software upload.
	 */
	public void softwareUpload()
	{
		if (selectSoftwareVersion( "Software Upload", "Software package to upload to the Payment Terminal" ))
		{
			if (versionSelected.length( ) > 0)
			{
				String message = new String( "Install " + versionSelected + "?" );
				String tittle = new String( "Select an option" );
				int opcion = showConfirmDialog( message, tittle );
				if (opcion == JOptionPane.YES_OPTION)
				{
					logger.info( "Install - START" );
					view.showInitAction( "Software Upload", "uploading software" );
					view.showProgressBar( "Installing Software - Please Wait", "initializing..." );
					view.incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()
					
					connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
					{
						@Override
		                public void invokeOnConnectionGrated (boolean bAuthenticated)
		                {
							if (bAuthenticated)
							{
								installationWorker = new InstallationTask( );
								installationWorker.execute( );
							}
							else
							{
								getView().showClean(); //Still connected but authentication was canceled.
							}
		                }

						@Override
		                public void invokeOnConnectionNotGrated (boolean bActive)
		                {
							if (bActive)
							{
								getView().showError("Installing Software - Not valid authentication - Logs Generated");
							}
							else
							{
								getView().showError("Installing Software - Connection lost - Logs Generated");
							}
		                }
					},getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId(),true);
				}
			}
			else
			{
				showErrorPane( "The IP address is not valid or the version selected is not valid", "Invalid selection" );
			}
		}
	}

	/**
	 * Software compare.
	 * Executes the swing worker task to compare the local and unit software
	 */
	public void softwareCompare()
	{
		if (selectSoftwareVersion( "Software Comparison", "Software version on this computer to compare against the version on the Payment Terminal" ))
		{
			view.showInitAction( "Software Comparison", "comparing" );

			if (versionSelected.length( ) > 0)
			{
				logger.info( "Comparison - START" );
				view.showProgressBar( "Comparing Software - Please Wait", "initializing..." );
				getView().incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()
				
				connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
				{
					@Override
	                public void invokeOnConnectionGrated (boolean bAuthenticated)
	                {
						comparisionWorker = new ComparisionTask( );
						comparisionWorker.execute( );
	                }

					@Override
	                public void invokeOnConnectionNotGrated (boolean bActive)
	                {
						getView().showError("Comparing Software - Connection lost - Logs Generated");
	                }
				},getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId());
			}
			else
			{
				showErrorPane( "the version selected is not valid", "Invalid selection" );
			}
		}
	}
	
	

	/** @brief Entry point to start the Standard Logs collection. This method is the one that should be called by the view to start this process.
	 *  
     * @since 1.0.0
	 */
	public void startStandardLogsDownload()
	{
		if (chooseLogsDirectory(false))
		{
			getView().showInitAction("Collecting Standard Logs","initializing...");
			getView().showProgressBar("Collecting Standard Logs - Please Wait","initializing...");
			getView().incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()

			String sLogsDownloadPath = model.getStandardLogsFolder( );
			String sCurrentIp = view.getIpInserted();
			Path sessionDirectory = null;
			
			try
			{
				sessionDirectory = Paths.get(sLogsDownloadPath,sCurrentIp,EdtUtil.getDateTimeStamp("yyyy-MM-dd'T'HHmmss")); // The path is built with the download_path/target_unit_ip/timestamp/
			}
			catch (RuntimeException e)
			{
				sessionDirectory = Paths.get(sLogsDownloadPath,sCurrentIp);
			}

			logger.info("Obtaining valid connection for " + getView().getIpInserted() + ":" + model.getTerminalPort());
			connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
			{
				String sessionDirectory;
				
				public ConnectionEngineWorker initialize (String sessionDirectory)
				{
					this.sessionDirectory = sessionDirectory;
					return this;
				}
				@Override
                public void invokeOnConnectionGrated (boolean bAuthenticated)
                {
					if (collectionWorker == null)
					{
						collectionWorker = new FileResourceCollectionTask();
					}

					if (collectionWorker.startCollectionForStandardLogFiles(sessionDirectory) == false)
					{
						showErrorPane( "The selected directory is not valid", "Invalid selection" );
					}
                }

				@Override
                public void invokeOnConnectionNotGrated (boolean bActive)
                {
					getView().showError("Collecting Standard Logs - Connection lost - Logs Generated");
                }
			}.initialize(sessionDirectory.toString()),getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId());
		}
	}

	/**
	 * Start pci logs download.
	 */
	public void startPCILogsDownload()
	{
/*
		showMessageDialog( "Not implemented yet.", "Warning" );
		String message = "Once the PCI logs are downloaded, the Payment Terminal will reboot.\n";
		message += "This will invalidate your challenge password.\n";
		message += "Are you sure you want to do this now?";
		showConfirmDialog( message, "Warning" );
*/
		if (chooseLogsDirectory(true))
		{
			getView().showInitAction("Collecting Secure Logs","initializing...");
			getView().showProgressBar("Collecting Secure Logs - Please Wait","initializing...");
			getView().incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()

			String sLogsDownloadPath = model.getSecureLogsFolder();
			String sCurrentIp = view.getIpInserted();
			Path sessionDirectory = null;
			
			try
			{
				sessionDirectory = Paths.get(sLogsDownloadPath,sCurrentIp,EdtUtil.getDateTimeStamp("yyyy-MM-dd'T'HHmmss")); // The path is built with the download_path/target_unit_ip/timestamp/
			}
			catch (RuntimeException e)
			{
				sessionDirectory = Paths.get(sLogsDownloadPath,sCurrentIp);
			}
			
			connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
			{
				String sessionDirectory;
				
				public ConnectionEngineWorker initialize (String sessionDirectory)
				{
					this.sessionDirectory = sessionDirectory;
					return this;
				}
				@Override
                public void invokeOnConnectionGrated (boolean bAuthenticated)
                {
					if (bAuthenticated)
					{
    					if (collectionWorker == null)
    					{
    						collectionWorker = new FileResourceCollectionTask();
    					}
    
    					if (collectionWorker.startCollectionForPCILogFiles(sessionDirectory) == false)
    					{
    						showErrorPane( "The selected directory is not valid", "Invalid selection" );
    					}
					}
					else
					{
						getView().showClean(); //Still connected but authentication was canceled.
					}
                }

				@Override
                public void invokeOnConnectionNotGrated (boolean bActive)
                {
					if (bActive)
					{
						getView().showError("Collecting Secure Logs - Not valid authentication - Logs Generated");
					}
					else
					{
						getView().showError("Collecting Secure Logs - Connection lost - Logs Generated");
					}
                }
			}.initialize(sessionDirectory.toString()),getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId(),true);
		}
	}

	/**
	 * Usb storage version.
	 */
	public void usbStorageVersion()
	{
		if (selectStorageUSBOptions( ))
		{
			String strSourceVersions = model.getVersionFolder( ) + versionSelected;

			if (!ReleaseBundleCopyEngine.isAVanguardReleaseBundle( strSourceVersions ))
			{
				showMessageDialog( "Software USB Storage is available only for Vanguard Releases. ", "Alert" );
				return;
			}

			logger.info( "USB Storage - START" );
			view.showInitAction( "USB Storage", "coping release software" );
			view.showProgressBar( "Copying Release Software - Please Wait", "copying" );
			view.incrementProgress( 1, EdtFileUtil.getFilesCount( strSourceVersions ) + 1, 1, "copying release files", false );

			storeReleaseWorker = new StoreReleaseTask();
			storeReleaseWorker.setStoreReleaseParameters(strSourceVersions, usbSelected + FlexPayMaintenanceModel.getUsbFirmwarePath( ), usbSelected + FlexPayMaintenanceModel.getUsbApplicationPath( ) );
			storeReleaseWorker.execute( );
		}
	}
	
	
	/**
	 * Start log compression engine.
	 *
	 * @param filesToCompress the files to compress
	 * @param bSecureLogs [In] flag which indicates if the logs to be compressed are secure or not
	 * 
	 */
	protected void startLogCompression( Vector<IFileResourceTransmissionStorageFileLog>	filesToCompress, boolean bSecureLogs)
	{
		logger.info( "Log Compression - START" );
		view.showInitAction( "USB Storage", "coping release software" );
		view.showProgressBar( "Copying Release Software - Please Wait", "copying" );
		view.incrementProgress( 1, filesToCompress.size( ) + 1, 1, "copying release files", false );

		logCompressionWorker = new LogCompressionTask();
		logCompressionWorker.setFilesToCompress(filesToCompress,bSecureLogs);
		logCompressionWorker.execute( );
	}

	
	
	public void activateOrExitSecuremenu()
	{
		if ( isSecureMenuActive() )
		{
			int reply = JOptionPane.showConfirmDialog( view, "Are you sure you want to exit the Secure Menu?", "Confirm", JOptionPane.OK_CANCEL_OPTION );
			if (reply == JOptionPane.OK_OPTION)
			{
				view.cleanResults();
				deactivateSecureMenu( );
			}
		}
		else
		{
			view.cleanResults( );
			activateSecureMenu( );
		}
	}
	
	/**
	 * Activate secure menu.
	 */
	public void activateSecureMenu()
	{
		logger.info( "Secure menu activation - START" );

		view.showInitAction( "Secure Menu Request", "activating secure menu " );
		view.showProgressBar( "Secure Menu Activation - Please Wait", "activating secure menu" );
		view.incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()

		connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
		{
			@Override
            public void invokeOnConnectionGrated (boolean bAuthenticated)
            {
				if (bAuthenticated)
				{
    				secureMenuWorker = new SecureMenuTask( true );
    				secureMenuWorker.execute( );
    			}
    			else
    			{
					getView().showClean(); //Still connected but authentication was canceled.
    			}
            }

			@Override
            public void invokeOnConnectionNotGrated (boolean bActive)
            {
				if (bActive)
				{
					getView().showError("Secure menu activation failure - Not valid authentication - Logs Generated");
				}
				else
				{
					getView().showError("Secure menu activation failure - Connection lost - Logs Generated");
				}
            }
		},getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId(), true);
	}

	/**
	 * Deactivate secure menu.
	 */
	public void deactivateSecureMenu()
	{
		logger.info( "Exit Secure Menu - START" );

		view.showInitAction( "Exit Secure Menu Request", "deactivating secure menu " );
		view.showProgressBar( "Exit Secure Menu - Please Wait", "deactivating secure menu" );
		view.incrementProgress(0,1,0,"initializing...",false); // max == 1 and min == 0 is a trick for this incrementProgress() call in order to not disable the string progress in the added progress bar with call showProgressBar()

		connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
		{
			@Override
            public void invokeOnConnectionGrated (boolean bAuthenticated)
            {
				if (bAuthenticated)
				{
    				secureMenuWorker = new SecureMenuTask( false );
    				secureMenuWorker.execute();
    			}
    			else
    			{
					getView().showClean(); //Still connected but authentication was canceled.
    			}
            }

			@Override
            public void invokeOnConnectionNotGrated (boolean bActive)
            {
				if (bActive)
				{
					getView().showError("Exit Secure menu activation failure - Not valid authentication - Logs Generated");
				}
				else
				{
					getView().showError("Exit Secure menu activation failure - Connection lost - Logs Generated");
				}
            }
		},getView().getIpInserted(),model.getTerminalPort(),model.getSpotClientId(), true);
	}

	/**
	 * Show confirm dialog.
	 * 
	 * @param message the message
	 * @param title the title
	 * @return the int
	 */
	private int showConfirmDialog(String message, String title)
	{
		Object[] options = {"Ok","Cancel"};
		int indResult = JOptionPane.showOptionDialog( view, message, title, JOptionPane.PLAIN_MESSAGE, JOptionPane.QUESTION_MESSAGE,null,
			options,options[1] );

		if (indResult == 0) //Ok
		{
			return JOptionPane.YES_OPTION;
		}
		else //Cancel
		{
			return JOptionPane.NO_OPTION;
		}
	}

	/**
	 * Show error pane.
	 * @param message the message
	 * @param title the title
	 */
	public void showErrorPane(String message, String title)
	{
		JOptionPane pane = new JOptionPane( message, JOptionPane.ERROR_MESSAGE );
		JDialog dialog = pane.createDialog( title );
		dialog.setAlwaysOnTop( true );
		dialog.setVisible( true );

		dialog.setIconImages(SwingUtilities.getWindowAncestor(getView()).getIconImages());
	}

	/**
	 * Show log.
	 */
	public void showLog()
	{
		if (Desktop.isDesktopSupported( ))
		{
			try
			{
				Runtime runtime = Runtime.getRuntime( );
				runtime.exec( "notepad.exe " + model.getApplicationLogFilename( ) );
			}
			catch (IOException e)
			{
				showErrorPane( "Error opening log file", "Error" );
			}
		}
		else
		{
			showErrorPane( "Error opening log file - Desktop not supported", "Error" );
		}
	}
	
	
	/**
	 * Opens the destination logs folder.
	 * @param standard 
	 */
	public void showLogsFolder(String logfolder)
	{
		if (Desktop.isDesktopSupported( ))
		{
			try
			{
				Desktop.getDesktop( ).open( new File( logfolder ) );
			}
			catch (IOException e)
			{
				showErrorPane( "Error opening logs folder", "Error" );
			}
		}
		else
		{
			showErrorPane( "Error opening logs folder - Desktop not supported", "Error" );
		}
	};	

	// ******************************************************************
	// INNER CLASSES
	// ******************************************************************
	/**
	 * The Class InstallationTask.
	 */
	class InstallationTask extends SwingWorker<Boolean, Void>
	{

		/*
		 * (non-Javadoc)
		 * 
		 * @see javax.swing.SwingWorker#doInBackground()
		 */
		@Override
		public Boolean doInBackground()
		{
			return Boolean.valueOf( startInstall( ) );
		}

		/*
		 * (non-Javadoc)
		 * 
		 * @see javax.swing.SwingWorker#done()
		 */
		@Override
		public void done()
		{
			boolean bResult = false;

			try
			{
				bResult = get( );
			}
			catch (InterruptedException ignore)
			{
			}
			catch (java.util.concurrent.ExecutionException e)
			{
			}

			if (bResult)
			{
				logger.info( "Software package successfully uploaded to Payment Terminal" );
				view.showOK( "Software package successfully uploaded to Payment Terminal" );
			}
			else
			{
				logger.info( "Software Upload Failed" );
				view.showError( "Software Upload Failed - Log Generated" );
			}
			logger.info( "Install - END" );			
		}
	};

	
	/**
	 * The Class ComparisionTask.
	 * Swing worker that compares the local and unit software.
	 */
	class ComparisionTask extends SwingWorker<Boolean, Void>
	{
		/*
		 * (non-Javadoc)
		 * 
		 * @see javax.swing.SwingWorker#doInBackground()
		 */
		@Override
		public Boolean doInBackground()
		{
			return Boolean.valueOf( startComparison( ) );
		}

		/*
		 * (non-Javadoc)
		 * 
		 * @see javax.swing.SwingWorker#done()
		 */
		@Override
		public void done()
		{
			boolean bResult = false;

			try
			{
				bResult = get( );
			}
			catch (InterruptedException | ExecutionException e)
			{
			}

			if ((getComparisonResult( ) == SoftwareComparisonStatus.NOTHING_TO_COMPARE) || !bResult)
			{
				getView( ).showError( "Compare Failed - Log Generated" );
			}
			else if (getComparisonResult( ) == SoftwareComparisonStatus.VERSIONS_DO_NOT_MATCH)
			{
				getView( ).showWarning( "Compare Completed Successfully - Versions Do Not Match - See Log for Details" );
			}
			else
			{
				getView( ).showOK( "Software Comparison Successful - Versions Match" );
			}
		}

		
		/**
		 * Start the comparison process getting the local and unit software information.
		 * 
		 * @return true, if successful
		 */
		public boolean startComparison()
		{
			try
			{
				String verspath = getVersionFolder( ) + getLastSelectedVersion( );

				view.incrementProgress( 1, (new File( verspath )).list( ).length + 1, 1, "starting Comparision...", false );
				
				if (ConnectionEngineSwingAdapter.getInstance().isConnected())
				{
					compareStrategy = (FlexPayMaintenanceModel.isVanguardDevice( )) ? new CompareVanguardStrategy() : new CompareM3M5Strategy();
					comparisonEngine.setStrategy(compareStrategy);
					comparisonEngine.clearVectorAppsToVerify( );

					view.incrementProgress( 1, "collecting local software versions." );
					collectSoftwareInformation( verspath, false ); //get the local files information.

					view.incrementProgress( 1, "comparing software versions." );
					comparisonResult = comparisonEngine.doComparision( ); //get the spot information and do the comparison
					return true;
				}				
			}
			catch (InterruptedException e)
			{
				logger.error("Error. Exception thrown while connected to target unit: '" + getView().getIpInserted() + "'.\nException: '" + e.getMessage() + "'.");
			}
			return false;
		}

	};
	

	/**
	 * The Store Release Bundle Task.
	 */
	class StoreReleaseTask extends SwingWorker<Boolean, Void> implements Observer
	{		
		/** The str source versions. */
		private String	strSourceVersions;
		
		/** The dest firmware str. */
		private String destFirmwareStr;
		
		/** The dest application str. */
		private String destApplicationStr;

		
		/**
		 * Instantiates a new store release task.
		 */
		public StoreReleaseTask()
		{
			releaseBundleCopyEngine.addObserver(this);
		}

		/**
		 * Instantiates a new store release task.
		 *
		 * @param versions the versions
		 * @param destfirmware the destfirmware
		 * @param destapp the destapp
		 */
		public void setStoreReleaseParameters(String versions, String destfirmware, String destapp)
		{
			strSourceVersions = versions;
			destFirmwareStr= destfirmware;
			destApplicationStr = destapp;			
		}

		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#doInBackground()
		 */
		@Override
		public Boolean doInBackground()
		{		
			if ( releaseBundleCopyEngine.storeReleaseBundle( strSourceVersions, destFirmwareStr, destApplicationStr) )
			{
				return true;
			}
			else
			{
				showErrorDialog( "Error creating/storing release bundle in the USB" );
				return false;
			}
		}

		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#done()
		 */
		@Override
		public void done()
		{
			try
			{
				if (get( ))
				{
					view.showOK( "Software package successfully transferred to USB flash drive" );
				}
				else
				{
					view.showError( "Software USB Storage Failed - Log Generated" );
				}
			}
			catch (InterruptedException | ExecutionException e)
			{
			}

			logger.info( "USB Storage - END" );
		}
		
		/* (non-Javadoc)
		 * @see java.util.Observer#update(java.util.Observable, java.lang.Object)
		 */
		@Override
		public void update(Observable o, Object arg)
		{	
			if (o instanceof ReleaseBundleCopyEngine)
			{
				releaseBundleCopyEngineUpdates( (String)arg );
			}			
		}

		/**
		 * Release bundle copy engine updates.
		 *
		 * @param file the file that is treated
		 */
		private void releaseBundleCopyEngineUpdates(String file)
		{
			view.incrementProgress( 1, "copying release files - "+(String)file);
			publish();
		}
	};
	
	/**
	 * The Class Compression Task Swing Worker to execute the compression.
	 */
	class LogCompressionTask extends SwingWorker<Boolean, Void> implements Observer
	{
		
		/** The files to compress. */
		private Vector<IFileResourceTransmissionStorageFileLog>	filesToCompress;

		/** Flag that indicates if the files to compress are secure logs. */
		private boolean bSecureLogs;

		/**
		 * Instantiates a new log compression task.
		 */
		public LogCompressionTask()
		{
			logCompressionEngine.addObserver(this);
		}


		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#doInBackground()
		 */
		@Override
		public Boolean doInBackground()
		{
			view.incrementProgress( 1, filesToCompress.size( ) + 1, 1, "compressing log files", false );
			return Boolean.valueOf( logCompressionEngine.compress( filesToCompress, getCurrenPPN( ),(this.bSecureLogs)?("secure_log"):("standard_log") ) );
		}

		/**
		 * Sets the files to compress.
		 *
		 * @param files the new files to compress
		 * @param bSecureLogs [In] flag which indicates if the logs to be compressed are secure or not
		 */
		public void setFilesToCompress(Vector<IFileResourceTransmissionStorageFileLog> files, boolean bSecureLogs)
		{
			this.filesToCompress = files;
			this.bSecureLogs = bSecureLogs;
		}

		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#done()
		 */
		@Override
		public void done()
		{
			try
			{
				String logType = (this.bSecureLogs)?("secure"):("standard");
				if (get( ))
				{
					view.showOK( "Collected " + logType + " logs successfully" );
					view.showLogsFolderButton((this.bSecureLogs)?(model.getSecureLogsFolder()):(model.getStandardLogsFolder( )));
				}
				else
				{
					view.showError( "Collected " + logType + " logs failure - Log Generated" );
				}
				logger.info( "Log Compression - END" );
			}
			catch (InterruptedException | ExecutionException e)
			{
				logger.warn( e.getMessage( ) );
			}
		}

		/* (non-Javadoc)
		 * @see java.util.Observer#update(java.util.Observable, java.lang.Object)
		 */
		@Override
		public void update(Observable o, Object arg)
		{	
			if (o instanceof LogCompressionEngine)
			{
				LogCompressionEngineUpdates( (String)arg );
			}			
		}


		/**
		 * Log compression engine updates the view progress bar.
		 *
		 * @param arg the arg
		 */
		private void LogCompressionEngineUpdates(String arg)
		{
			view.incrementProgress( 1, "compressing log files - "+arg);
			publish();
		}
		
	};
	
	
	/**
	 * The Class Compression Task Swing Worker to execute the compression.
	 */
	class SecureMenuTask extends SwingWorker<Boolean, Void> 
	{
		private boolean	activate;


		/**
		 * @param activate true for activate, false for exit.
		 */
		public SecureMenuTask(boolean activate)
		{
			this.activate = activate;
		}


		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#doInBackground()
		 */
		@Override
		public Boolean doInBackground()
		{
			if (activate)
			{
				view.incrementProgress( 1, 3, 1, "starting secure menu activation...", false );
				view.incrementProgress( 1, "activating secure menu" );
				return secureMenuEngine.activate( );
			}
			else
			{
				view.incrementProgress( 1, 3, 1, "starting secure menu deactivation...", false );
				view.incrementProgress( 1, "deactivating secure menu" );
				return secureMenuEngine.deactivate( ); 
			}
		}


		/* (non-Javadoc)
		 * @see javax.swing.SwingWorker#done()
		 */
		@Override
		public void done()
		{
			try
			{
				if (activate)
				{
					if (get( ))
					{
						logger.info( "Secure Menu Activated" );
						setSecureMenuActive(true);
						view.showSecureMenuResultOk( );
					}
					else
					{
						logger.info( "Secure Menu Activation Failure" );
						view.showError( "Secure menu activation failure - Log Generated" );
					}
					logger.info( "Secure Menu Activation - END" );
				}
				else
				{
					setSecureMenuActive(false);
					if (get( ))
					{
						logger.info( "Exited Secure Menu Successfully" );
						view.showExitSecureMenuResultOk( );
					}
					else
					{
						logger.info( "Exit Secure Menu Failure" );
						view.showExitSecureMenuResultError( );
						disconnectToUPM(); //the authentication is not available 
					}
					logger.info( "Exit Secure Menu - END" );
				}
			}
			catch (InterruptedException | ExecutionException e)
			{
				logger.warn( e.getMessage( ) );
			}
		}
	};

	
	/**
	 * The Class FileResourceCollectionTask.
	 *
	 * @class FlexPayMaintenanceController.FileResourceCollectionTask FlexPayMaintenanceController.java "com.gilbarco.globaltools.flexpay.maintenance.gui"
	 * @brief This class process the internal file resource collection.
	 * @see FlexPayMaintenanceController
	 * @see FileResourceCollectionEngineSwingAdapter
	 * @since 1.0.0
	 */
	class FileResourceCollectionTask
	{
		
		/** The task. @brief FileResource Collection Swing Adapter Task */
		private FileResourceCollectionEngineSwingAdapter task;
	
		/** The file resource transmitted. @brief The transmitted FileResource transmitted */
		private Vector<IFileResourceTransmissionStorageFileLog>	fileResourceTransmitted;

		/**
		 * Instantiates a new file resource collection task.
		 *
		 * @brief Default Constructor
		 * @since 1.0.0
		 */
		public FileResourceCollectionTask ()
		{
			try
			{
				/** @class FlexPayMaintenanceController.FileResourceCollectionTask.Anonymous<FileResourceCollectionEngineSwingAdapter> FlexPayMaintenanceController.java "com.gilbarco.globaltools.flexpay.maintenance.gui"
				 * @brief This class is the swing adapter specialization for our purposes.
				 *
				 * @see FlexPayMaintenanceController
				 * @see FileResourceCollectionEngineSwingAdapter
				 * 
			     * @since 1.0.0
				 */
				this.task = new FileResourceCollectionEngineSwingAdapter()
				{
					/** @brief Executes this user code in the "Swing Event Dispatcher Thread" context before running the FileResource collection. If user code fails and reports the failure at the return, the FileResource collection is aborted.
					 *  
					 * @return True if user code executed successfully. False otherwise. 
					 * 
				     * @since 1.0.0
					 */
					@Override
					public boolean proceedAtStart ()
					{
						boolean bResult = true;
						fileResourceTransmitted = new Vector<IFileResourceTransmissionStorageFileLog>();
						return bResult;
					}

					/** @brief Executes this user code in the "Swing Event Dispatcher Thread" context after running the FileResource collection.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void proceedAtEnd ()
					{
					}

					/** @brief Entry point for an unknown progress notification from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressUnknown (IEngineProgressNotification notification)
					{
						getView().incrementProgress(0,notification.getCaptionUpdate());
					}

					/** @brief Entry point for a file transmission outcome notification from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressFileResourceTransmission (EngineProgressNotificationFileResourceTransmission notification)
					{
						getView().incrementProgress(1,notification.getCaptionUpdate());	
						fileResourceTransmitted.add((IFileResourceTransmissionStorageFileLog) notification.getFileResourceTransmitted());
					}

					/** @brief Entry point for an advance in progress notification from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressAdvance (EngineProgressNotification notification)
					{
						getView().incrementProgress(0,notification.getCaptionUpdate());
					}

					/** @brief Entry point for initialization completion progress notification from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressInitiate (EngineProgressNotificationStatusInitiated notification)
					{
						getView().incrementProgress(0,notification.getCaptionUpdate());
					}

					/** @brief Entry point for the completion progress notification with successful outcome from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressSuccess (EngineProgressNotificationStatusSucceeded notification)
					{
						boolean bSecureLogs = false;
						
						//Check if at least one of the logs is secure, if so treat them all as secure.
						Iterator<IFileResourceTransmissionStorageFileLog> iter = fileResourceTransmitted.iterator();
						if (iter != null)
						{
    						while (iter.hasNext() && (bSecureLogs == false))
    						{
    							IFileResourceTransmissionStorageFileLog fileResource = iter.next();
    							bSecureLogs = fileResource.isSensitive();
    						}
						}

						//Start a SwingWorker task which will run the untar/ungzip of each FileResource and then at the end zip them all in a single file.
						startLogCompression(fileResourceTransmitted,bSecureLogs); 
					}

					/** @brief Entry point for the completion progress notification with failure outcome from the engine. This code executes at the "Swing Event Dispatcher Thread" context.
					 * 
					 * @param notification is the Progress Notification report.
					 * 
				     * @since 1.0.0
					 */
					@Override
					public void notifiedProgressFail (EngineProgressNotificationStatusFailed notification)
					{
						getView().showError(notification.getCaptionUpdate());
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
					@Override
					public boolean startCollection (Collection<IFileResourceTransmissionWriter> fileResourceVector, String sCaptionFileResource, String sCaptionSucessful, String sCaptionFail)
					{
						getView().incrementProgress(0,fileResourceVector.size(),0,"Initializing...",false);
						return super.startCollection(fileResourceVector,sCaptionFileResource,sCaptionSucessful,sCaptionFail);
					}
				};
			}
			catch (EdtServiceException e)
			{
				logger.error("Error. Exception thrown while creating the FileResourceCollectionEngineSwingAdapter instance. EdtServiceException: '" + e.getMessage() + "'.",e);
			}
		}
		
		/**
		 * Start collection for pci log files.
		 *
		 * @param path is the destination location to store the collected logs.
		 * @return True if the task started without issues. False otherwise. The progress is being notified asynchronously.
		 * @brief Starts the PCI logs collection process asynchronously.
		 * @since 1.0.0
		 */
		public boolean startCollectionForPCILogFiles(String path)
		{
			boolean bResult = false;
			Path sessionDirectory = Paths.get(path);

			try
			{
				sessionDirectory = Files.createDirectories(sessionDirectory);

				Vector<IFileResourceTransmissionWriter> fileResourceVector = new Vector<IFileResourceTransmissionWriter>();
				FileResourceLocation fileResourceLocation = new FileResourceLocation(path);

				fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogSecure.PCI_HIST,fileResourceLocation)));
				fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogSecure.PCI,fileResourceLocation)));

				bResult = this.task.startCollection(fileResourceVector,"Secure Logs","Collected secure logs successfully","Collected secure logs failure");
			}
			catch (IOException e)
			{
				logger.error("Error. Directory " + sessionDirectory.toString() + " could not be created. " + e.getMessage(),e);
			}

			return bResult;
		}

		/**
		 * Start collection for standard log files.
		 *
		 * @param path is the destination location to store the collected logs.
		 * @return True if the task started without issues. False otherwise. The progress is being notified asynchronously.
		 * @brief Starts the Standard logs collection process asynchronously.
		 * @since 1.0.0
		 */
		public boolean startCollectionForStandardLogFiles(String path)
		{
			boolean bResult = false;
			Path sessionDirectory = Paths.get(path);

			try
			{
				sessionDirectory = Files.createDirectories(sessionDirectory);

				Vector<IFileResourceTransmissionWriter> fileResourceVector = new Vector<IFileResourceTransmissionWriter>();
				FileResourceLocation fileResourceLocation = new FileResourceLocation(path);
				if (FlexPayMaintenanceModel.isVanguardDevice( ))
				{
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.PINPAD,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.PINPAD_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SYSMANSERV,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SYSMANSERV_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPTJOURNAL,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPTJOURNAL_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.DISPCLIB,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.DISPCLIB_HIST,fileResourceLocation)));
				}
				else
				{
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.ABI,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.ABI_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.DISPCLIB,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.DISPCLIB_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.EMV,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.EMV_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.GVROPTEMV,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.GVROPTEMV_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.JOURNAL,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.JOURNAL_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPT,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPT_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPTJOURNAL,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.OPTJOURNAL_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.PINPAD,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.PINPAD_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SETUP,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SETUP_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SYSMANSERV,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.SYSMANSERV_HIST,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.CVXADAPTER,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.CVXADAPTER_HIST,fileResourceLocation)));
	
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.AS2805,fileResourceLocation)));
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.AS2805_HIST,fileResourceLocation)));
	
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.LINUXKERNEL,fileResourceLocation)));
	
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.EXTAPP_EXT0,fileResourceLocation)));//only M3 returns with data
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.EXTAPP_EXT1,fileResourceLocation)));//only M5 returns with data
					fileResourceVector.add(new FileResourceTransmissionWriterFile(new FileResourceTransmissionStorageFileLog(FileResourceLogStandard.EXTAPP_EXT2,fileResourceLocation)));//only M5 returns with data
				}
				bResult = this.task.startCollection(fileResourceVector,"Standard Logs","Collected standard logs successfully","Collected standard logs failure");
			}
			catch (IOException e)
			{
				logger.error("Error. Directory " + sessionDirectory.toString() + " could not be created. " + e.getMessage(),e);
			}

			return bResult;
		}
	}

	/**
	 * Gets the title app name.
	 *
	 * @return the title app name
	 */
	public String getTitleAppName()
	{
		return model.getTitleAppName( );
	}

	/**
	 * @return the ppn value for the last active connection
	 */
	private String getCurrenPPN()
	{
		String ppn = "";
		try
		{
			ppn = ConnectionEngineSwingAdapter.getInstance().getPPN();
		}
		catch (Exception e)
		{
			logger.error("Error getting the current ppn fron Connection Engine - " + e.getMessage(), e);
		}
		return ppn;
	}

}

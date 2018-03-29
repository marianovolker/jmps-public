/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;
import com.gilbarco.globaltools.flexpay.engines.ReleaseBundleCopyEngine;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.notifications.EngineProgressNotificationConnectionNotGranted;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.token.engines.SASTokenManagementEngine;
import com.gilbarco.globaltools.flexpay.token.engines.UPMTokenManagementEngine;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg.SasErrorCodeType;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenResponse;

public class TokenManagerController extends EdtAppController<TokenManagerView, TokenManagerModel> implements Observer
{
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
		 * @since 1.0.0
		 */
		public void invokeOnConnectionNotGrated ();
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
	    						worker.invokeOnConnectionNotGrated();
	    					}
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
	// INNER TYPES.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log				logger						= LogFactory.getLog( TokenManagerController.class );
	private static volatile TokenManagerController	controller;
	
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected TokenManagerModel						tokenManagerModel;
	protected SASTokenManagementEngine				sasTokenManagementEngine;
	protected UPMTokenManagementEngine				upmTokenManagementEngine;
	protected ReleaseBundleCopyEngine				releaseBundleCopyEngine;
	
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private JMenu									tokenMenu;
	private JMenu									certificateMenu;
	private JMenu									advanceMenu;

	private JMenuItem								requestTokenMenuItem;
	private JMenuItem								presentTokenMenuItem;
	private JMenuItem								storeTokenMenuItem;
	private JMenuItem								removeTokenMenuItem;

	private JMenuItem								advanceMenuItem;

	private JMenuItem								generateCertificateMenuItem;
	private JMenuItem								publishCertificateMenuItem;

	private boolean									controllerInit	= false;

	/** @brief The connection work dispatcher. */
	private ConnectionEngineWorkDispatcher			connectionWorkDispatcher;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private TokenManagerController(TokenManagerView view)
	{
		super( view );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************
	@Override
	public void update(Observable o, Object arg)
	{
	}

	@Override
	public void initController()
	{
		
		if (controllerInit)
		{
			return;
		}

		this.tokenManagerModel = new TokenManagerModel( );
		this.setAppModel( tokenManagerModel );

		try
		{
			connectionWorkDispatcher = new ConnectionEngineWorkDispatcher();
			connectionWorkDispatcher.setupConnectionEngine();

			sasTokenManagementEngine = SASTokenManagementEngine.getInstance( );
			sasTokenManagementEngine.addObserver( this );

			upmTokenManagementEngine = UPMTokenManagementEngine.getInstance( );
			upmTokenManagementEngine.addObserver( this );
			
			releaseBundleCopyEngine = ReleaseBundleCopyEngine.getInstance();
			releaseBundleCopyEngine.addObserver( this );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}

		controllerInit = true;
		FlexPayContext.setAppLocale( this.getAppLocale( ) );
	}

	@Override
	public void addMenuItems(JMenu menu)
	{
		tokenMenu = new JMenu( FlexPayContext.getLocaleText( "Token" ) );
		tokenMenu.add( this.getRequestTokenMenuItem( ) );
		tokenMenu.add( this.getPresentTokenMenuItem( ) );
		tokenMenu.add( this.getStoreTokenMenuItem( ) );
		tokenMenu.add( this.getRemoveTokenMenuItem( ) );

		certificateMenu = new JMenu( FlexPayContext.getLocaleText( "Certificate" ) );
		certificateMenu.add( this.getGenerateCertificateMenuItem( ) );
		certificateMenu.add( this.getPublishCertificateMenuItem( ) );

		advanceMenu = new JMenu( FlexPayContext.getLocaleText( "Advanced" ) );
		advanceMenu.add( this.getAdvanceMenuItem( ) );

		menu.add( tokenMenu );
		menu.add( certificateMenu );
		menu.add( advanceMenu );
	}

	@Override
	public void updateMenuItemsState(JMenu menu)
	{
	}

	@Override
	protected void onEdtAppModelChanged(TokenManagerModel model)
	{
		this.updateButtons( model.isLogged( ), model.isCertificatePublished( ) );
	}

	public void closedAuthenticationManagerNotification()
	{
		ConnectionEngineSwingAdapter.setInstance(null);
	}

	// ******************************************************************
	// PRIVATE METHODS
	// ******************************************************************
	private JMenuItem getRequestTokenMenuItem()
	{
		requestTokenMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "Request" ) );
		requestTokenMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.requestToken( );
			}
		} );

		return requestTokenMenuItem;
	}

	private JMenuItem getPresentTokenMenuItem()
	{
		presentTokenMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "Present" ) );
		presentTokenMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.setStatus( FlexPayContext.getLocaleText( "Connecting" ) + "..." );
				view.disableActions( );
				view.presentToken( );
				view.cleanStatus( );
				view.enableActions( );
			}
		} );

		return presentTokenMenuItem;
	}

	private JMenuItem getAdvanceMenuItem()
	{
		advanceMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "Preferences" ) );
		advanceMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.advancedOptionsDialog( );
			}
		} );

		return advanceMenuItem;
	}

	private JMenuItem getPublishCertificateMenuItem()
	{
		publishCertificateMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "PublishCertificate" ) );
		publishCertificateMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.setStatus( FlexPayContext.getLocaleText( "Connecting" ) + "..." );
				view.disableActions( );
				publishCertificate( );
				view.cleanStatus( );
				view.enableActions( );
			}
		} );

		return publishCertificateMenuItem;
	}

	private JMenuItem getStoreTokenMenuItem()
	{
		storeTokenMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "StoreToken" ) );
		storeTokenMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.storeTokensBundlesToUSB( );
			}
		} );

		return storeTokenMenuItem;
	}

	private JMenuItem getRemoveTokenMenuItem()
	{
		removeTokenMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "Remove" ) );
		removeTokenMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.deleteTargetUnit( true, view.getSelectedRow( ) );
			}
		} );

		return removeTokenMenuItem;
	}

	private JMenuItem getGenerateCertificateMenuItem()
	{
		generateCertificateMenuItem = new JMenuItem( FlexPayContext.getLocaleText( "Generate" ) );
		generateCertificateMenuItem.addActionListener( new ActionListener( )
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				view.generateCertificateDialog( );
			}
		} );

		return generateCertificateMenuItem;
	}

	private void setView(TokenManagerView view)
	{
		this.view = view;
	}

	private void publishCertificate()
	{
		try
		{
			sasTokenManagementEngine.doPublishCertificate( );

			String strInfo = "";
			if (!sasTokenManagementEngine.hasErrors( ))
			{
				strInfo = "Success at PublishCertificate Request action, validation code [" + sasTokenManagementEngine.getValidationCode( ) + "].";

				view.showPublishCertificateOk( sasTokenManagementEngine.getValidationCode( ) );

				logger.info( strInfo );
			}
			else
			{
				if( sasTokenManagementEngine.getResponseCode( ) == SasErrorCodeType.UNKNOWN_ERROR )
				{
					strInfo = "Error sending PublishCertificate Request - " + sasTokenManagementEngine.getErrorDescription( );
				}
				else
				{
					strInfo = "Error sending PublishCertificate Request - " + sasTokenManagementEngine.getResponseCode( ).getDescription( );
				}
				
				logger.error( strInfo );
				view.showSasErrorInfo(strInfo);
			}
		}
		catch (InterruptedException e)
		{
				logger.error( e.getMessage( ) );
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public void cleanUp()
	{
		view.clearTable( );
	}

	public boolean presentToken(byte[] xmlToken, String sIPAddress)
	{
		connectionWorkDispatcher.dispatch(new ConnectionEngineWorker()
		{
			private byte[] xmlToken;
			private String sIPAddress;
			
			public ConnectionEngineWorker initialize (byte[] xmlToken, String sIPAddress)
			{
				this.xmlToken = xmlToken;
				this.sIPAddress = sIPAddress;
				return this;
			}
			
			@Override
            public void invokeOnConnectionGrated (boolean bAuthenticated)
            {
				boolean rc = true;
				try
                {
	               logger.info( "Connect - " + sIPAddress + ":" + FlexPayAppConfig.getTerminalPort( ) );
    				upmTokenManagementEngine.setXmlToken( xmlToken );
    				upmTokenManagementEngine.doPresentToken( );
    
    				if (upmTokenManagementEngine.hasErrors( ))
    				{
    					logger.error( "Error in PresentToken action [" + upmTokenManagementEngine.getResponseCode( ).getAckDescription( ) + "]" );
    					rc = false;
    				}
    
    				logger.info( "Close Connection." );
    				ConnectionEngineSwingAdapter.getInstance().close();
                }
				catch (InterruptedException e)
				{
					logger.error("Error. Exception thrown while connected to target unit: '" + sIPAddress + "'.\nException: '" + e.getMessage() + "'.");
				}
    				
				if (rc)
				{
					getView().showMessageDialog( "The token has been presented sucessfully" );
				}
				else
				{
					getView().showErrorDialog( "Couldn't present the token pointed. The response received was [" + controller.getUPMTokenManagementEngine( ).getResponseCode( ).getAckDescription( ) + "]." );
				}
            }

			@Override
            public void invokeOnConnectionNotGrated ()
            {
   				logger.error( "Error not connected to ip: " + sIPAddress );
				getView().showErrorDialog( "Couldn't present the token pointed. The response received was [" + controller.getUPMTokenManagementEngine( ).getResponseCode( ).getAckDescription( ) + "]." );
            }
		}.initialize(xmlToken,sIPAddress),sIPAddress,TokenManagerModel.getTerminalPort(),tokenManagerModel.getSpotClientId());

		return false;
	}

	public void requestToken(String tokenCode)
	{
		try
		{
			logger.info( "Requesting token [" + tokenCode + "]." );

			sasTokenManagementEngine.setTokenCode( tokenCode );
			sasTokenManagementEngine.doTokenRequest( );

			String strInfo = "";
			if (!sasTokenManagementEngine.hasErrors( ))
			{
				strInfo = "Success at Request Token Action.";

				SasTokenResponse message = sasTokenManagementEngine.getTokenResponse( );

				view.addTokenRequested( message.getTokenCode	( ).getValue( ), message.getStartTime	( ).getValue( ),
										message.getEndTime		( ).getValue( ), message.getTargetUnit			( )			   , message.getMaxDuration	( ).getValue( ),
										message.getTimes		( ).getValue( ), message.getRemoteMaintenance	( ).getValue( ), message.getPermissions	( ).getValue( ),
										message.getTechnicianID	( ).getValue( ), message.getTechnicianDevice	( ).getValue( ), message.getOperatorID	( ).getValue( ),
										message.getVersion		( ).getValue( ), message.toString( ).getBytes	( ) );

				logger.debug( "Token [" + message.toString( ) + "]." );

				logger.info( strInfo );
			}
			else
			{
				if( sasTokenManagementEngine.getResponseCode( ) == SasErrorCodeType.UNKNOWN_ERROR )
				{
					strInfo = "Error sending Token Request - " + sasTokenManagementEngine.getErrorDescription( );
				}
				else
				{
					strInfo = "Error sending Token Message - " + sasTokenManagementEngine.getResponseCode( ).getDescription( );
				}
				
				logger.error( strInfo );
				view.showSasErrorInfo(strInfo);
			}
		}
		catch (InterruptedException e)
		{
			e.printStackTrace( );
		}
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void updateButtons(boolean logged, boolean started)
	{
		view.enableActions( );
	}

	protected void updateMenus(int tokensSelected)
	{
		boolean bTokenSelected = tokensSelected == 1;

		requestTokenMenuItem.setEnabled( true );
		presentTokenMenuItem.setEnabled( bTokenSelected );
		storeTokenMenuItem.setEnabled( bTokenSelected );
		removeTokenMenuItem.setEnabled( bTokenSelected );

		generateCertificateMenuItem.setEnabled( true );
		publishCertificateMenuItem.setEnabled( true );

		advanceMenuItem.setEnabled( true );

		this.view.getRootApp( ).getJMenuBar( ).revalidate( );
	}

	protected void disableMenus()
	{
		presentTokenMenuItem.setEnabled( false );
		requestTokenMenuItem.setEnabled( false );
		storeTokenMenuItem.setEnabled( false );
		removeTokenMenuItem.setEnabled( false );

		generateCertificateMenuItem.setEnabled( false );
		publishCertificateMenuItem.setEnabled( false );

		advanceMenuItem.setEnabled( false );

		this.view.getRootApp( ).getJMenuBar( ).revalidate( );
	}

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	public static TokenManagerController getInstance(TokenManagerView view)
	{
		if (controller == null)
		{
			controller = new TokenManagerController( view );
		}
		else
		{
			controller.setView( view );
			if (controller.connectionWorkDispatcher != null)
			{
				try
	            {
					controller.connectionWorkDispatcher.setupConnectionEngine();
	            }
	            catch (EdtServiceException e)
	            {
		            logger.error("Error. Exception thrown while updateing connection engine." + e.getMessage(),e);
	            }
			}
		}
		return controller;
	}

	public static String getCertifDefaultFolder()
	{
		return TokenManagerModel.getCertifDefaultFolder( );
	}

	public static String getDefaultCommonName()
	{
		return TokenManagerModel.getDefaultCommonName( );
	}

	public static String getDefaultOrganization()
	{
		return TokenManagerModel.getDefaultOrganization( );
	}

	public static long getDefaultExpirationDays()
	{
		return TokenManagerModel.getDefaultExpirationDays( );
	}

	public static String getDefaultMail()
	{
		return TokenManagerModel.getDefaultMail( );
	}

	public static String getUSBdrive()
	{
		return TokenManagerModel.getUSBdrive( );
	}

	public static String getDefaultCountry()
	{
		return TokenManagerModel.getDefaultCountry( );
	}

	public static String getDefaultState()
	{
		return TokenManagerModel.getDefaultState( );
	}

	public static String getDefaultLocality()
	{
		return TokenManagerModel.getDefaultLocality( );
	}

	public UPMTokenManagementEngine getUPMTokenManagementEngine()
	{
		return upmTokenManagementEngine;
	}
	
	public SASTokenManagementEngine getSASTokenManagementEngine()
	{
		return sasTokenManagementEngine;
	}	

	public static void setUSBdrive(String strUSBdrive)
	{
		TokenManagerModel.setUSBdrive( strUSBdrive );
	}

	public static String getVersionsFolder()
	{
		return TokenManagerModel.getVersionsFolder( );
	}

	public static String getUsbApplicationsPath()
	{
		return TokenManagerModel.getUsbApplicationPath( );
	}

	public static String getUsbFirmwaresPath()
	{
		return TokenManagerModel.getUsbFirmwarePath( );
	}

	public static String getUsbSecureLogPath()
	{
		return TokenManagerModel.getUsbSecureLogPath( );
	}

	public static String getUsbStandardLogPath()
	{
		return TokenManagerModel.getUsbStandardLogPath( );
	}

	public String getCurrentTerminalIP()
	{
		return TokenManagerModel.getCurrentTerminalIP();
	}


	public boolean storeReleaseBundle(String strSourceVersions, String destFirmwareStr, String destApplicationStr)
	{
		return false;
	}
	
}

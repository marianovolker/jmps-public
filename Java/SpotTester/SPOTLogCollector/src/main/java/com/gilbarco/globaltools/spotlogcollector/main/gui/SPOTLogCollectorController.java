
package com.gilbarco.globaltools.spotlogcollector.main.gui;

import java.awt.Color;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfigPropertyException;
import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.spotlogcollector.model.SPOTLogCollectorContext;
import com.gilbarco.globaltools.spotlogcollector.state.*;
import com.gilbarco.globaltools.spotlogcollector.widgets.JSPOTLogCollectorDialog;

public class SPOTLogCollectorController extends EdtAppController<SPOTLogCollectorView, SPOTLogCollectorModel>
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger	= LogFactory.getLog( SPOTLogCollectorController.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected SPOTLogCollectorModel			model;
	protected SPOTLogCollectorStateManager	stateManager;
	protected SPOTLogCollectorMessageBroker	messageBroker;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SPOTLogCollectorController(SPOTLogCollectorView view)
	{
		super( view );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Override
	public void initController()
	{
		try
		{
			model = new SPOTLogCollectorModel( );
			setAppModel( model );
			setOptionPaneProperties( );
			SPOTLogCollectorContext.setAppLocale( this.getAppLocale( ) );

			stateManager = new SPOTLogCollectorStateManager( );
			messageBroker = new SPOTLogCollectorMessageBroker( this.model , new ProgressObserver() );

		}
		catch (EdtServiceException e)
		{
			logger.error( "initController - " + e.getMessage( ) );
		}
	}

	/**
	 * @return the view application model
	 * @throws EdtAppConfigPropertyException
	 */
	public SPOTLogCollectorModel getModel()
	{
		updateModel( );
		return model;
	}

	/**
	 * Add defaults for message dialogs
	 */
	private void setOptionPaneProperties()
	{
		// locale options
		UIManager.put( "OptionPane.okButtonText", "Ok" );
		UIManager.put( "OptionPane.yesButtonText", "Yes" );
		UIManager.put( "OptionPane.noButtonText", "No" );
		UIManager.put( "OptionPane.cancelButtonText", "Cancel" );
		// style options
		UIManager.put( "OptionPane.background", Color.white );
		UIManager.put( "Panel.background", Color.white );
		UIManager.put( "OptionPane.messageFont", JSPOTLogCollectorDialog.getDefaultFont( ) );
		UIManager.put( "OptionPane.buttonFont", JSPOTLogCollectorDialog.getDefaultFont( ) );
	}

	@Override
	public void updateMenuItemsState(JMenu menu)
	{
	}

	@Override
	public void addMenuItems(JMenu menu)
	{
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	@Override
	protected void onEdtAppModelChanged(SPOTLogCollectorModel model)
	{
		updateButtons( model.isLogged( ), model.isStarted( ) );
	}

	protected void updateModel()
	{
	}

	protected void updateButtons(boolean logged, boolean started)
	{
	}

	/**
	 * @param errorCode
	 *            call this method in all cases of quit application
	 */
	protected void exitApplication(int errorCode)
	{
		int opcion = view.showConfirmDialog( "Exit SPOTLogCollector? " );
		if (opcion == JOptionPane.YES_OPTION)
		{
			logger.info( view.getAppName( ) + " Exit." );
			getModel( ).saveProperties( );
			copySPOTLogCollectorNotes( );
			copySPOTLogCollectorLog( );
			System.exit( errorCode );
		}
	}

	private synchronized void copySPOTLogCollectorLog()
	{
		if( !model.collectionStarted( ) )
		{
			return;
		}

		Path sourceFilePath = Paths.get(model.getSPOTLogCollectorOriginalLogName()); 
		Path destFilePath = Paths.get(model.getLogsDownloadPath(), model.getSPOTLogCollectorLogName()); 

		try
		{
			Files.deleteIfExists(destFilePath);
			Files.copy(sourceFilePath,destFilePath,StandardCopyOption.REPLACE_EXISTING,StandardCopyOption.COPY_ATTRIBUTES);
		}
		catch (IOException e)
		{
			logger.error("Cannot create the application copy log: " + e.getMessage());
		}
	}

	private synchronized void copySPOTLogCollectorNotes()
	{
		if( !model.collectionStarted( ) )
		{
			return;
		}

		Path sourceFilePath = Paths.get(model.getSPOTLogCollectorNotesFileName()); 
		Path destFilePath = Paths.get(model.getLogsDownloadPath(), model.getSPOTLogCollectorNotesFileName()); 

		try
		{
			Files.deleteIfExists(destFilePath);
			Files.copy(sourceFilePath,destFilePath,StandardCopyOption.REPLACE_EXISTING,StandardCopyOption.COPY_ATTRIBUTES);
		}
		catch (IOException e)
		{
			logger.error("Cannot create the application copy notes: " + e.getMessage());
		}
	}

	/**
	 * start point to collect the spot logs.
	 * 
	 */
	public void startCollection()
	{
		model.setCollectionStarted();
		stateManager.startCollection( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class ProgressObserver implements SPOTLogCollectorProgressObserver
	{
		@Override
		public void notifyConnected ()
		{
		}
		@Override
		public void notifyProgress ()
		{
			view.showProgress( );
		}
		@Override
		public void notifySuccess ()
		{
			copySPOTLogCollectorNotes( );
			copySPOTLogCollectorLog( );
			view.showSuccess( "success" );
		}
		@Override
		public void notifyFailed ()
		{
			copySPOTLogCollectorNotes( );
			copySPOTLogCollectorLog( );
			view.showError( "error" );
		}
		@Override
		public void notifyDisconnected ()
		{
		}
		
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}

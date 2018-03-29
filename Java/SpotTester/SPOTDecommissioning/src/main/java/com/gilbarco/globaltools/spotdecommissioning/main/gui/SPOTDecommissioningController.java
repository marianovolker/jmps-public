
package com.gilbarco.globaltools.spotdecommissioning.main.gui;

import java.awt.Color;
import javax.swing.JMenu;
import javax.swing.JOptionPane;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfigPropertyException;
import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.spotdecommissioning.model.SPOTDecommissioningContext;
import com.gilbarco.globaltools.spotdecommissioning.state.*;
import com.gilbarco.globaltools.spotdecommissioning.widgets.JSPOTDecommissioningDialog;

public class SPOTDecommissioningController extends EdtAppController<SPOTDecommissioningView, SPOTDecommissioningModel>
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log		logger	= LogFactory.getLog( SPOTDecommissioningController.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected SPOTDecommissioningModel			model;
	protected SPOTDecommissioningStateManager	stateManager;
	protected SPOTDecommissioningMessageBroker	messageBroker;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SPOTDecommissioningController(SPOTDecommissioningView view)
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
			model = new SPOTDecommissioningModel( );
			setAppModel( model );
			setOptionPaneProperties( );
			SPOTDecommissioningContext.setAppLocale( this.getAppLocale( ) );

			stateManager = new SPOTDecommissioningStateManager( );
			messageBroker = new SPOTDecommissioningMessageBroker( this.model , new ProgressObserver() );

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
	public SPOTDecommissioningModel getModel()
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
		UIManager.put( "OptionPane.messageFont", JSPOTDecommissioningDialog.getDefaultFont( ) );
		UIManager.put( "OptionPane.buttonFont", JSPOTDecommissioningDialog.getDefaultFont( ) );
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
	protected void onEdtAppModelChanged(SPOTDecommissioningModel model)
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
		int opcion = view.showConfirmDialog( "Exit SPOTDecommissioning? " );
		if (opcion == JOptionPane.YES_OPTION)
		{
			logger.info( view.getAppName( ) + " Exit." );
			getModel( ).saveProperties( );
			System.exit( errorCode );
		}
	}


	/**
	 * start point to cleark keys.
	 * 
	 */
	public void startDecommissioning()
	{
		stateManager.startDecommissioning( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class ProgressObserver implements SPOTDecommissioningProgressObserver
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
			view.showSuccess( "Success - All Data Cleared" );
			logger.info(  "Success - All Data Cleared"  );
		}
		@Override
		public void notifyFailed (String error)
		{
			view.showError( "Error - " + error );
			logger.info(  "Error - " + error  );
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

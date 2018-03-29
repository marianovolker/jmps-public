
package com.gilbarco.globaltools.flexpay.main.gui;

import java.awt.Image;
import java.awt.Window;
import java.util.List;

import javax.swing.JMenu;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfigPropertyException;
import com.gilbarco.globaltools.edt.gui.EdtMainView;
import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;
import com.gilbarco.globaltools.flexpay.maintenance.gui.FlexPayMaintenanceView;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklModel;
import com.gilbarco.globaltools.flexpay.rkl.gui.FlexPayRklView;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.strategies.ChallengeAuthenticationStrategy;
/* TODO: TOKEN_COMMENTED_CODE
import com.gilbarco.globaltools.flexpay.token.engines.strategies.TokenAuthenticationStrategy;
*/
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies.IAuthenticationStrategy;

public class FlexPayController extends EdtAppController<FlexPayView, FlexPayModel>
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	
	/** The Constant logger. */
	private static final transient Log						logger					= LogFactory.getLog( FlexPayController.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected FlexPayModel			model;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private FlexPayMaintenanceView	payMaintenanceView;
	private FlexPayRklView			rklView;

	private IAuthenticationStrategy	authenticationStrategy;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public FlexPayController(FlexPayView view)
	{
		super( view );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Override
	public void initController()
	{
		this.model = new FlexPayModel();
		this.setAppModel( model );
		FlexPayContext.setAppLocale( this.getAppLocale( ) );
		authenticationStrategy = null;
	}

	public FlexPayModel getModel() throws EdtAppConfigPropertyException
	{
		this.updateModel( );
		return this.model;
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
	protected IAuthenticationStrategy getAuthenticationStrategy ()
	{
		if (authenticationStrategy == null)
		{
		/* TODO: TOKEN_COMMENTED_CODE
			authenticationStrategy = new TokenAuthenticationStrategy();
		 */
			authenticationStrategy = new ChallengeAuthenticationStrategy(null);
		}
		return authenticationStrategy;
	}

	protected IAuthenticationStrategy getAuthenticationStrategy (List<Image> lIcons)
	{
		if (authenticationStrategy == null)
		{
		/* TODO: TOKEN_COMMENTED_CODE
			authenticationStrategy = new TokenAuthenticationStrategy();
		 */
			authenticationStrategy = new ChallengeAuthenticationStrategy(lIcons);
		}
		else
		{
			authenticationStrategy.setIconImages(lIcons);
		}
		return authenticationStrategy;
	}
	
	@Override
	protected void onEdtAppModelChanged(FlexPayModel model)
	{

		this.updateButtons( model.isLogged( ), model.isStarted( ) );
	}

	protected void updateModel()
	{
	}

	protected void updateButtons(boolean logged, boolean started)
	{
	}

	protected void exitApplication(int errorCode)
	{
		if (model.isLogged( ))
		{
		}

		if (payMaintenanceView != null && !payMaintenanceView.isCloseEnabled( ))
		{
			return;
		}

		if (rklView != null && !rklView.isCloseEnabled( ))
		{
			return;
		}

		FlexPayAppConfig.saveProperties( );
		logger.info( view.getAppName( ) + " End." );
		System.exit( errorCode );
	}

	protected void launchFlexPayMaintenanceView(EdtMainView rootApp, String sTechnicianNo, String appName)
	{		
		List<EdtAppView<?>> list = rootApp.getController( ).getAllApps();
		boolean found = false;
		
		for (EdtAppView<?> edtAppView : list)
		{
				System.out.println( "class" +  edtAppView.getClass( ).getName( ));
				if( edtAppView instanceof FlexPayMaintenanceView )
					found = true;
		}
		if(!found)
		{
			payMaintenanceView = new FlexPayMaintenanceView(getAuthenticationStrategy(rootApp.getIconImages()));
			rootApp.addApp( payMaintenanceView );
		}

		getAuthenticationStrategy().setTechnicianID( sTechnicianNo );
	}

	protected void launchRKLView(EdtMainView rootApp, String sTechnicianNo, String appName)
	{
		FlexPayRklModel.setTechnicianID( sTechnicianNo );

		List<EdtAppView<?>> list = rootApp.getController( ).getAllApps( );
		boolean found = false;

		for (EdtAppView<?> edtAppView : list)
		{
			System.out.println( "class" + edtAppView.getClass( ).getName( ) );
			if (edtAppView instanceof FlexPayRklView) found = true;
		}
		if (!found)
		{
			rklView = new FlexPayRklView( );
			rootApp.addApp( rklView );
		}
	}

	protected boolean hasAuthenticationManagerView ()
	{
		return getAuthenticationStrategy().hasAuthenticationManagerView();
	}

	protected void launchAuthenticationManagerView(EdtMainView mainView, String sTechnicianNo, String sAppName)
	{
		if (getAuthenticationStrategy().hasAuthenticationManagerView())
		{
			List<EdtAppView<?>> list = mainView.getController( ).getAllApps();
			boolean found = false;
			
			for (EdtAppView<?> edtAppView : list)
			{
				System.out.println( "class" +  edtAppView.getClass( ).getName( ));
				found = getAuthenticationStrategy().isInstanceOfCurrentAuthenticationManagerView(edtAppView);
			}
			if(!found)
			{
				mainView.addApp( getAuthenticationStrategy(mainView.getIconImages()).getAuthenticationManagerView() );
			}
			getAuthenticationStrategy().setTechnicianID( sTechnicianNo );
		}
	}

	/**
	 * Gets the title app name.
	 *
	 * @return the title app name
	 */
	public String getTitleAppName()
	{
		return model.getTitleAppName();
	}	

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}

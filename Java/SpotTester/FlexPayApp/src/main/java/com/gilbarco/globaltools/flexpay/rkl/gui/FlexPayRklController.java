
package com.gilbarco.globaltools.flexpay.rkl.gui;

import javax.swing.JMenu;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import com.gilbarco.globaltools.edt.gui.app.EdtAppController;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.rkl.context.RKLContext;
import com.gilbarco.globaltools.flexpay.rkl.state.RKLStateEventHandler;

public class FlexPayRklController extends EdtAppController<FlexPayRklView, FlexPayRklModel>
{
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public FlexPayRklController(FlexPayRklView view)
	{
		super( view );
	}

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( FlexPayRklController.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected FlexPayRklModel			rklModel;
	protected RKLStateEventHandler		stateManager;
	protected RKLContext				rklContext;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************
	@Override
	public void addMenuItems(JMenu menu)
	{

	}

	@Override
	public void updateMenuItemsState(JMenu menu)
	{

	}

	@Override
	public void initController()
	{
		this.rklModel = new FlexPayRklModel( );
		this.setAppModel( rklModel );
		this.rklContext = (RKLContext) RKLContext.getInstance( rklModel );
		FlexPayContext.setAppLocale( this.getAppLocale( ) );
	}

	@Override
	protected void onEdtAppModelChanged(FlexPayRklModel model)
	{
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public void startRKL()
	{

		try
		{
			String spotIp = view.getIpInserted( );
			String rklServerIp = this.rklModel.getRKLServer( );
			String rklPort = this.rklModel.getRKLPort( );
			logger.info( "Starting RKL.." );
			logger.info( "Connecting - " + view.getIpInserted( ) + ":" + rklModel.getTerminalPort( ) );

			rklContext.init( new String[]
			{ spotIp, rklPort, rklServerIp, rklPort }, this );
		}
		catch (EdtServiceException e)
		{
			logger.error( "startRKL - EdtServiceException - " + e.getMessage( ) );
			showRKLError( );
		}
	}

	public void mockUpdateRklInfo()
	{
		view.setHashValue( "123456" );
		view.setHostKey( "buyu1-t3" );
		view.setSerialNumber( "999999999" );
		view.setDeviceType( "Vanguard" );
		view.rklShowSuccess( "" );
	}

	public void showRKLError()
	{
		view.rklShowError( FlexPayContext.getLocaleText( "RKLFailed" ) );
	}

	public void showRKLAlert(String info)
	{
		view.rklShowAlert( info );
	}

	public void showRKLSuccess(String key)
	{
		view.rklShowSuccess( FlexPayContext.getLocaleText( "RKLSuccessful" ) + " - " + key );
	}

	public void setCurrentTerminalIP(String joinIP)
	{
		this.rklModel.setCurrentTerminalIP(joinIP);
	}

	public String getCurrentTerminalIP()
	{
		return this.rklModel.getTerminalIP( );
	}


}

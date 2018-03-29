
package com.gilbarco.globaltools.flexpay.main.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;

public class FlexPayModel extends EdtAppModel
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	// private static final transient Log logger = LogFactory.getLog(GCMICCModel.class);

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	private boolean	logged;
	private boolean	started;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************


	public boolean isLogged()
	{
		return logged;
	}

	public void setLogged(boolean logged)
	{
		this.logged = logged;
	}

	public boolean isStarted()
	{
		return started;
	}

	public void setStarted(boolean started)
	{
		this.started = started;
	}

	@Override
	public String toString()
	{
		StringBuilder sb = new StringBuilder( );
		return sb.toString( );
	}

	public void modelUpdated()
	{
		this.setChanged( );
		this.notifyObservers( );
	}

	/**
	 * Gets the title app name.
	 *
	 * @return the title app name
	 */
	public String getTitleAppName()
	{
		return FlexPayAppConfig.getTitleAppName( );
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

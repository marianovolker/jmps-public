
package com.gilbarco.globaltools.spotdecommissioning.main.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.spotdecommissioning.config.SPOTDecommissioningAppConfig;

public class SPOTDecommissioningModel extends EdtAppModel
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

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

	public String getCurrentIp()
	{
		return SPOTDecommissioningAppConfig.getCurrentTerminalIP( );
	}

	public void setCurrentIp(String data)
	{
		SPOTDecommissioningAppConfig.setCurrentTerminalIP( data );
	}

	public int getCurrentPort()
	{
		return SPOTDecommissioningAppConfig.getTerminalPort( );
	}

	public void saveProperties()
	{
		SPOTDecommissioningAppConfig.saveProperties( );
	}


	public String getSPOTDecommissioningLogName()
	{
		return SPOTDecommissioningAppConfig.getDecommissioningLogName();
	}

	public byte getSpotClientID()
	{
		return SPOTDecommissioningAppConfig.getSpotClientID();
	}


	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

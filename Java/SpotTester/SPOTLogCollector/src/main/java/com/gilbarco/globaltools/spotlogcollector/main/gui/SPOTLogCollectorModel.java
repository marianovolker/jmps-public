
package com.gilbarco.globaltools.spotlogcollector.main.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.spotlogcollector.config.SPOTLogCollectorAppConfig;

public class SPOTLogCollectorModel extends EdtAppModel
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
	private boolean	collectionDone;

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
		return SPOTLogCollectorAppConfig.getCurrentTerminalIP( );
	}

	public void setCurrentIp(String data)
	{
		SPOTLogCollectorAppConfig.setCurrentTerminalIP( data );
	}

	public int getCurrentPort()
	{
		return SPOTLogCollectorAppConfig.getTerminalPort( );
	}

	public void saveProperties()
	{
		SPOTLogCollectorAppConfig.saveProperties( );
	}

	public String getLogsDownloadPath()
	{
		return SPOTLogCollectorAppConfig.getLogsDownloadPath( );
	}

	public void setLogsDownloadPath(String absolutePath)
	{
		SPOTLogCollectorAppConfig.setLogsDownloadPath( absolutePath );
	}

	public String getSPOTLogCollectorLogName()
	{
		return SPOTLogCollectorAppConfig.getSPOTLogCollectorLogName();
	}

	public String getSPOTLogCollectorOriginalLogName()
	{
		return SPOTLogCollectorAppConfig.getSPOTLogCollectorOriginalLogName();
	}

	public String getSPOTLogCollectorNotesFileName()
	{
		return SPOTLogCollectorAppConfig.getSPOTLogCollectorNotesFileName();
	}

	public byte getSpotClientID()
	{
		return SPOTLogCollectorAppConfig.getSpotClientID();
	}

	public boolean collectionStarted()
	{
		return collectionDone;
	}

	public boolean setCollectionStarted()
	{
		return collectionDone = true;
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file FlexPayMaintenanceModel.java
 * @author mgrieco
 * @date 27 Ago 2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */
package com.gilbarco.globaltools.flexpay.maintenance.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;

/**
 * The Class FlexPayMaintenanceModel.
 */
public class FlexPayMaintenanceModel extends EdtAppModel
{
	// ******************************************************************
	// STATIC FIELDS
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

	/** the maintenance action is logged. */
	private boolean	logged;
	
	/** the maintenance action is started. */
	private boolean	started;


	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	/**
	 * Gets the version folder.
	 *
	 * @return the version folder
	 */
	public String getVersionFolder()
	{
		return FlexPayAppConfig.getInstallAppsPath( );
	}
	
	/**
	 * Gets the resources folder.
	 *
	 * @return the resources folder
	 */
	public static String getResourcesFolder()
	{
		return FlexPayAppConfig.getInstallResourcesPath( );
	}

	/**
	 * Gets the terminal type.
	 *
	 * @return the terminal type
	 */
	public static String getTerminalType()
	{
		return FlexPayAppConfig.getTerminalType();
	}

	/**
	 * Gets the current terminal ip.
	 *
	 * @return the current terminal ip
	 */
	public String getCurrentTerminalIP()
	{
		return FlexPayAppConfig.getCurrentTerminalIP();
	}

	
	/**
	 * Sets the current terminal ip.
	 *
	 * @param joinIP the new current terminal ip
	 */
	public void setCurrentTerminalIP(String joinIP)
	{
		FlexPayAppConfig.setCurrentTerminalIP( joinIP );		
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString()
	{
		StringBuilder sb = new StringBuilder( );

		return sb.toString( );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	/**
	 * Checks if is logged.
	 *
	 * @return true, if is logged
	 */
	public boolean isLogged()
	{
		return logged;
	}

	/**
	 * Sets the logged.
	 *
	 * @param logged the new logged
	 */
	public void setLogged(boolean logged)
	{
		this.logged = logged;
	}

	/**
	 * Checks if is started.
	 *
	 * @return true, if is started
	 */
	public boolean isStarted()
	{
		return started;
	}

	/**
	 * Sets the started.
	 *
	 * @param started the new started
	 */
	public void setStarted(boolean started)
	{
		this.started = started;
	}

	/**
	 * Model updated.
	 */
	public void modelUpdated()
	{
		this.setChanged( );
		this.notifyObservers( );
	}

	/**
	 * Gets the standard logs folder.
	 *
	 * @return the standard logs folder
	 */
	public String getStandardLogsFolder()
	{
		return FlexPayAppConfig.getStandardLogsFolder();
	}

	/**
	 * Sets the standard logs folder.
	 *
	 * @param folder the standard logs folder to save
	 */
	public void setStandardLogsFolder(String folder)
	{
		FlexPayAppConfig.setStandardLogsFolder(folder);
	}
	
	/**
	 * Gets the pCI logs folder.
	 *
	 * @return the pCI logs folder
	 */
	public String getSecureLogsFolder()
	{
		return FlexPayAppConfig.getPCILogsFolder();
	}
	
	/**
	 * Sets the pci logs folder.
	 *
	 * @param folder the pci logs folder to save
	 */
	public void setSecureLogsFolder(String folder)
	{
		FlexPayAppConfig.setSecureLogsFolder(folder);		
	}


	/**
	 * Gets the terminal port.
	 *
	 * @return the terminal port
	 */
	public int getTerminalPort()
	{
		return FlexPayAppConfig.getTerminalPort();
	}

	/**
	 * Gets the spot client id.
	 *
	 * @return the spot client id
	 */
	public byte getSpotClientId()
	{
		return FlexPayAppConfig.getSpotClientId();
	}

	/**
	 * Gets the private key filename.
	 *
	 * @return the private key filename
	 */
	public String getFlexpayPrivateKeyFilename()
	{
		return FlexPayAppConfig.getFlexpayPrivateKeyFilename();
	}

	/**
	 * Gets the sas certificate filename.
	 *
	 * @return the sas certificate filename
	 */
	public String getSasCertificateFilename()
	{
		return FlexPayAppConfig.getSasCertificateFilename();
	}

	/**
	 * Gets the application log file name.
	 *
	 * @return the application log file name
	 */
	public String getApplicationLogFilename()
	{
		return FlexPayAppConfig.getApplicationLogFile();
	}

	/**
	 * Checks if is a vanguard device.
	 * 
	 * @return true, if is a vanguard device
	 */
	public static boolean isVanguardDevice()
	{
		if (getTerminalType( ).equalsIgnoreCase( "VANGUARD" ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Gets the usb firmware path.
	 *
	 * @return the usb firmware path
	 */
	public static String getUsbFirmwarePath()
	{
		return FlexPayAppConfig.getUsbFirmwarePath( );
	}

	/**
	 * Gets the usb application path.
	 *
	 * @return the usb application path
	 */
	public static String getUsbApplicationPath()
	{
		return FlexPayAppConfig.getUsbApplicationPath( );
	}

	/**
	 * Gets the usb secure log path.
	 *
	 * @return the usb secure log path
	 */
	public static String getUsbSecureLogPath()
	{
		return FlexPayAppConfig.getUsbSecureLogPath( );
	}

	/**
	 * Gets the usb standard log path.
	 *
	 * @return the usb standard log path
	 */
	public static String getUsbStandardLogPath()
	{
		return FlexPayAppConfig.getUsbStandardLogPath( );
	}

	/**
	 * Gets the title app name.
	 *
	 * @return the title app name
	 */
	public String getTitleAppName()
	{
		return FlexPayAppConfig.getTitleAppName();
	}

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

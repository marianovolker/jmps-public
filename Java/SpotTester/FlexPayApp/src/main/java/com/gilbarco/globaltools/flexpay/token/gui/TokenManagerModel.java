
package com.gilbarco.globaltools.flexpay.token.gui;

import com.gilbarco.globaltools.edt.gui.app.EdtAppModel;
import com.gilbarco.globaltools.edt.util.EdtSignData;
import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;

public class TokenManagerModel extends EdtAppModel
{
	// ******************************************************************
	// INNER TYPES.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static String		strTechnicianID			= "";
	private static String		strUSBdrive				= "";
	private static String		strTerminalPort;
// TODO: TOKEN_COMMENTED_CODE /////
	private static String		strTokenServerPort;
	private static String		strTokenServerIp2;
	private static String		strTokenServerIp1;
///////////////////////////////////

	private static final String	DEFAULT_FUEL_POS		= "0";

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private boolean				bllogged				= false;
	// Indicates if the User has published a certificate successfully
	private boolean				certificatePublished	= false;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public TokenManagerModel()
	{
		initializeKeys( );
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static String getCertifDefaultFolder()
	{
		return FlexPayAppConfig.getCertifFolder( );
	}

	public static String getDefaultCommonName()
	{
		return FlexPayAppConfig.getCertifCommonName( );
	}

	public static String getDefaultOrganization()
	{
		return FlexPayAppConfig.getCertifOrg( );
	}

	public static long getDefaultExpirationDays()
	{
		return FlexPayAppConfig.getCertifExpDays( );
	}

	public static String getDefaultMail()
	{
		return FlexPayAppConfig.getCertifTechMail( );
	}

	public static int getDefaultTerminalPort()
	{
		return FlexPayAppConfig.getDefaultTerminalPort( );
	}

// TODO: TOKEN_COMMENTED_CODE /////
	public static int getDefaultTokenServerPort()
	{
		return FlexPayAppConfig.getDefaultTokenServerPort( );
	}

	public static String getDefaultTokenIp1()
	{
		return FlexPayAppConfig.getTokenServerIp1( );
	}

	public static String getDefaultTokenIp2()
	{
		return FlexPayAppConfig.getTokenServerIp2( );
	}
///////////////////////////////////

	public static String getSASCertificateFilename()
	{
		return FlexPayAppConfig.getSasCertificateFilename( );
	}

	public static String getDefaultCountry()
	{
		return FlexPayAppConfig.getDefaultCertifCountry( );
	}

	public static String getDefaultState()
	{
		return FlexPayAppConfig.getDefaultCertifState( );
	}

	public static String getDefaultLocality()
	{
		return FlexPayAppConfig.getDefaultCertifLocality( );
	}

	/**
	 * @return the strTechnicianID
	 */
	public static String getTechnicianID()
	{
		return strTechnicianID;
	}

	/**
	 * @param strTechnicianID
	 *            the strTechnicianID to set
	 */
	public static void setTechnicianID(String strTechnicianID)
	{
		TokenManagerModel.strTechnicianID = strTechnicianID;
	}

	/**
	 * @return The default token directory path for storing in USB drives
	 */
	public static String getUsbTokenPath()
	{
// TODO: TOKEN_COMMENTED_CODE /////
		return FlexPayAppConfig.getUsbTokenPath( );
///////////////////////////////////
	}

	public static String getUsbFirmwarePath()
	{
		return FlexPayAppConfig.getUsbFirmwarePath( );
	}

	public static String getUsbApplicationPath()
	{
		return FlexPayAppConfig.getUsbApplicationPath( );
	}

	public static String getUsbSecureLogPath()
	{
		return FlexPayAppConfig.getUsbSecureLogPath( );
	}

	public static String getUsbStandardLogPath()
	{
		return FlexPayAppConfig.getUsbStandardLogPath( );
	}

	/**
	 * @return the string representing the USB drive letter.
	 */
	protected static String getUSBdrive()
	{
		if (strUSBdrive == null)
		{
			strUSBdrive = FlexPayAppConfig.getUsbDrive( );
		}
		return strUSBdrive;
	}

	/**
	 * @param strUSBdrive
	 *            the string representing the USB drive letter to set.
	 */
	protected static void setUSBdrive(String sUSBdrive)
	{
		if (sUSBdrive == null)
			strUSBdrive = "";
		else
			strUSBdrive = sUSBdrive;
		FlexPayAppConfig.setUsbDrive( strUSBdrive );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	/**
	 * @return the certificatePublished
	 */
	public boolean isCertificatePublished()
	{
		return certificatePublished;
	}

	/**
	 * @param certificatePublished
	 *            the certificatePublished to set
	 */
	public void setCertificatePublished(boolean certificatePublished)
	{
		this.certificatePublished = certificatePublished;
	}

	public boolean isLogged()
	{
		return bllogged;
	}

	public void setLogged(boolean logged)
	{
		this.bllogged = logged;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	/**
	 * Loads the SAS public key from file.
	 */
	private void initializeKeys()
	{
		EdtSignData.getInstance( ).setPrivateKeyFilename( FlexPayAppConfig.getFlexpayPrivateKeyFilename( ) );
		EdtSignData.getInstance( ).setCertificateFilename( FlexPayAppConfig.getSasCertificateFilename( ) );

	}

// TODO: TOKEN_COMMENTED_CODE /////
	public static int getTokenServerPort()
	{
		int port;
		if (strTokenServerPort == null)
		{
			port = FlexPayAppConfig.getTokenServerPort( );
			strTokenServerPort = Integer.toString( port );
		}
		else
		{
			port = Integer.parseInt( strTokenServerPort );
		}
		return port;
	}

	public static String getTokenServerIp1()
	{
		if (strTokenServerIp1 == null) strTokenServerIp1 = getDefaultTokenIp1( );
		return strTokenServerIp1;
	}

	public static String getTokenServerIp2()
	{
		if (strTokenServerIp2 == null) strTokenServerIp2 = getDefaultTokenIp2( );
		return strTokenServerIp2;
	}
///////////////////////////////////

	public static int getTerminalPort()
	{
		int port;
		if (strTerminalPort == null)
		{
			port = FlexPayAppConfig.getTerminalPort( );
			strTerminalPort = Integer.toString( port );
		}
		else
		{
			port = Integer.parseInt( strTerminalPort );
		}
		return port;

	}

// TODO: TOKEN_COMMENTED_CODE /////
	public static void setTokenServerPort(String port)
	{
		strTokenServerPort = port;
		FlexPayAppConfig.setTokenServerPort( Integer.parseInt( port ) );
	}

	public static void setTokenServerIp1(String ip)
	{
		strTokenServerIp1 = ip;
		FlexPayAppConfig.setTokenServerIp1( ip );
	}

	public static void setTokenServerIp2(String ip)
	{
		strTokenServerIp2 = ip;
		FlexPayAppConfig.setTokenServerIp2( ip );
	}
///////////////////////////////////

	public static void setTerminalPort(String port)
	{
		strTerminalPort = port;
		FlexPayAppConfig.setTerminalPort( Integer.parseInt( port ) );
	}

	public static String getDefaultFuelPos()
	{
		return DEFAULT_FUEL_POS;
	}

	public static String getVersionsFolder()
	{
		return FlexPayAppConfig.getInstallAppsPath( );
	}

	public static String getCurrentTerminalIP()
	{
		return FlexPayAppConfig.getCurrentTerminalIP();
	}


	public byte getSpotClientId()
	{
		return FlexPayAppConfig.getSpotClientId( );
	}
}

/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file FlexPayAppConfig.java
 * @author mgrieco
 * @date 27 Ago 2013
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.config;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfig;
import com.gilbarco.globaltools.edt.app.config.EdtAppConfigPropertyException;
import com.gilbarco.globaltools.edt.util.EdtSortedProperties;
import com.gilbarco.globaltools.edt.util.EdtUtil;

public class FlexPayAppConfig
{
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	public enum FlexPayAppProperty
	{
		// ======================================================================================================================
		// Technician Certificate settings
		P_CERTIF_COMMON_NAMES(
						"vanguard.settings.default_certificate_common_name",
						"Optional",
						"String",
						DEFAULT_CERTIF_COMMON_NAME,
						"helpstring"),

		P_CERTIF_FOLDER(
						"vanguard.settings.default_certificate_folder",
						"Optional",
						"String",
						DEFAULT_CERTIF_FOLDER,
						"helpstring"),

		P_CERTIF_TECH_MAIL(
						"vanguard.settings.default_certificate_tech_mail",
						"Optional",
						"String",
						DEFAULT_CERTIF_TECH_MAIL,
						"helpstring"),

		P_CERTIF_EXPIR_DAYS(
						"vanguard.settings.default_certificate_expiration_days",
						"Optional",
						"Integer",
						"" + DEFAULT_CERTIF_EXPIR_DAYS,
						"helpstring"),

		P_CERTIF_ORG(
						"vanguard.settings.default_certificate_org",
						"Optional",
						"String",
						DEFAULT_CERTIF_ORG,
						"helpstring"),

		P_CERTIF_COUNTRY(
						"vanguard.settings.default_certificate_country",
						"Optional",
						"String",
						DEFAULT_CERTIF_COUNTRY,
						"helpstring"),

		P_CERTIF_STATE(
						"vanguard.settings.default_certificate_state",
						"Optional",
						"String",
						DEFAULT_CERTIF_STATE,
						"helpstring"),

		P_CERTIF_LOCALITY(
						"vanguard.settings.default_certificate_locality",
						"Optional",
						"String",
						DEFAULT_CERTIF_LOCALITY,
						"helpstring"),

		// ======================================================================================================================
		// OPT Terminal connection settings
		P_TERMINAL_PORT(
						"vanguard.communication.terminal.port.default",
						"required",
						"Integer",
						"" + DEFAULT_TERMINAL_PORT,
						"helpstring"),
		P_TERMINAL_IP(
						"vanguard.communication.terminal.ip",
						"required",
						"String",
						DEFAULT_TERMINAL_IP,
						"helpstring"),

		P_TERMINAL_TYPE(
						"vanguard.settings.terminal_device.type",
						"Optional",
						"String",
						DEFAULT_TERMINAL_TYPE,
						"helpstring"),

		// TODO: TOKEN_COMMENTED_CODE /////
		// ======================================================================================================================
		// SAS Server connection settings
		P_TOKEN_SERVER_PORT(
						"vanguard.communication.token.server.port",
						"Optional",
						"Integer",
						"" + DEFAULT_TOKEN_SERVER_PORT,
						"helpstring"),

		P_TOKEN_SERVER_IP_1(
						"vanguard.communication.token.server.ip1",
						"Optional",
						"String",
						DEFAULT_TOKEN_SERVER_IP_1,
						"helpstring"),

		P_TOKEN_SERVER_IP_2(
						"vanguard.communication.token.server.ip2",
						"Optional",
						"String",
						DEFAULT_TOKEN_SERVER_IP_2,
						"helpstring"),
		// ======================================================================================================================
		// Generated USB token Bundle Paths
		P_USB_TOKEN_PATH(
						"vanguard.settings.usb_token_path",
						"Optional",
						"String",
						DEFAULT_USB_TOKEN_PATH,
						"helpstring"),
		///////////////////////////////////
		// ======================================================================================================================
		// Generated USB Bundle Paths
		P_USB_DRIVE(
						"vanguard.settings.usb_drive",
						"Optional",
						"String",
						"" + "",
						"helpstring"),
						
		P_USB_FIRMWARE_PATH(
						"vanguard.settings.usb_firmware_path",
						"required",
						"String",
						DEFAULT_USB_FIRMWARE_PATH,
						"helpstring"),

		P_USB_SECURE_LOG_PATH(
						"vanguard.settings.usb_secure_log_path",
						"Optional",
						"String",
						DEFAULT_USB_SECURE_LOG_PATH,
						"helpstring"),

		P_USB_APPLICATIONS_PATH(
						"vanguard.settings.usb_application_path",
						"required",
						"String",
						DEFAULT_USB_APPLICATION_PATH,
						"helpstring"),

		P_USB_STANDARD_LOG_PATH(
						"vanguard.settings.usb_standard_log_path",
						"Optional",
						"String",
						DEFAULT_USB_STANDARD_LOG_PATH,
						"helpstring"),

		// ======================================================================================================================

		P_INSTALL_APPLICATIONS_PATH(
						"vanguard.communication.install.application_path",
						"required",
						"String",
						DEFAULT_INSTALL_APPLICATIONS_PATH,
						"helpstring"),

		P_INSTALL_RESOURCES_PATH(
						"vanguard.communication.install.resources_path",
						"Optional",
						"String",
						DEFAULT_INSTALL_RESOURCES_PATH,
						"helpstring"),

		// ======================================================================================================================

		P_RKL_TERMINAL_PORT(
						"vanguard.communication.rkl.eppport",
						"Optional",
						"Integer",
						"" + DEFAULT_RKL_TERMINAL_PORT,
						"helpstring"),

		// ======================================================================================================================
		// Security Authentication Server data
		P_SAS_CERT_FILENAME(
						"vanguard.settings.sas.certificate_filename",
						"Optional",
						"String",
						DEFAULT_SAS_CERTIFICATE_FILENAME,
						"helpstring"),

		P_FLEXPAY_PRIVATE_KEY_FILENAME(
						"vanguard.settings.vst.privatekey_filename",
						"Optional",
						"String",
						DEFAULT_FLEXPAY_PRIVATE_KEY_FILENAME,
						"helpstring"),

		P_FLEXPAY_PUBLIC_KEY_FILENAME(
						"vanguard.settings.vst.publickey_filename",
						"Optional",
						"String",
						DEFAULT_FLEXPAY_PUBLIC_KEY_FILENAME,
						"helpstring"),

		P_FLEXPAY_CERT_FILENAME(
						"vanguard.settings.vst.certificate_filename",
						"Optional",
						"String",
						DEFAULT_FLEXPAY_CERTIFICATE_FILENAME,
						"helpstring"),

		P_CA_CERT_FILENAME(
						"vanguard.settings.ca.certificate_filename",
						"Optional",
						"String",
						DEFAULT_HANDSHAKE_CA_FILENAME,
						"helpstring"),

		P_SAS_CERT_COMMON_NAME(
						"vanguard.settings.sas.certificate_common_name",
						"Optional",
						"String",
						DEFAULT_SAS_CERTIFICATE_COMMON_NAME,
						"helpstring"),
			
		P_SAS_CERT_DNS_NAME(
						"vanguard.settings.sas.certificate_dnsname",
						"Optional",
						"String",
						DEFAULT_SAS_CERTIFICATE_DNSNAME,
						"helpstring"),
				
		P_STANDARD_LOGS_FOLDER(
						"vanguard.settings.standard_logs.folder",
						"required",
						"String",
						DEFAULT_STANDARD_LOGS_FOLDER,
						"helpstring"),
						
		P_SECURE_LOGS_FOLDER(
						"vanguard.settings.secure_logs.folder",
						"required",
						"String",
						DEFAULT_SECURE_LOGS_FOLDER,
						"helpstring"),

		// ======================================================================================================================
		// end
		;

		private String	opReqStr	= null;
		private String	propertyKey	= null;
		private String	typeStr		= null;
		private String	defValStr	= null;
		private String	helpStr		= null;

		// Ctor.
		private FlexPayAppProperty(String key, String opReq, String type, String defVal, String help)
		{
			propertyKey = key;
			opReqStr = opReq;
			typeStr = type;
			defValStr = defVal;
			helpStr = help;
		}

		/**
		 * Get optional / required string.
		 * <p>
		 * 
		 * @return The value.
		 */
		public String getOpRegStr()
		{
			return opReqStr;
		}

		/**
		 * Get type string.
		 * <p>
		 * 
		 * @return The value.
		 */
		public String getTypeStr()
		{
			return typeStr;
		}

		/**
		 * Get default value string.
		 * <p>
		 * 
		 * @return The value.
		 */
		public String getDefValueStr()
		{
			return defValStr;
		}

		/**
		 * Get help string.
		 * <p>
		 * 
		 * @return The value.
		 */
		public String getHelpStr()
		{
			return helpStr;
		}

		public String getFormatStr()
		{
			return propertyKey;
		}
	}

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger									= LogFactory.getLog( FlexPayAppConfig.class );

	private static final String			DEFAULT_CERTIF_TECH_MAIL				= "technician@gilbarco.com";
	private static final int			DEFAULT_CERTIF_EXPIR_DAYS				= 100000;
	private static final String			DEFAULT_CERTIF_ORG						= "Gilbarco Inc.";
	private static final String			DEFAULT_CERTIF_COUNTRY					= "US";
	private static final String			DEFAULT_CERTIF_STATE					= "NC";
	private static final String			DEFAULT_CERTIF_LOCALITY					= "Greensboro";
	private static final String			DEFAULT_CERTIF_COMMON_NAME				= "rsa-vst";
	private static final String			DEFAULT_CERTIF_FOLDER					= "certificates";

	private static final int			DEFAULT_TERMINAL_PORT					= 5200;
	private static final String			DEFAULT_TERMINAL_IP						= "10.5.48.169";
	private static final String			DEFAULT_TERMINAL_TYPE					= "VANGUARD";

	// TODO: TOKEN_COMMENTED_CODE /////
	private static final int			DEFAULT_TOKEN_SERVER_PORT				= 7001;
	private static final String			DEFAULT_TOKEN_SERVER_IP_1				= "10.28.120.2";
	private static final String			DEFAULT_TOKEN_SERVER_IP_2				= "10.28.120.2";
	private static final String			DEFAULT_USB_TOKEN_PATH					= "gvr//DualAuthentication//token//";
	///////////////////////////////////
	
	private static final String			DEFAULT_USB_FIRMWARE_PATH				= "gvr//DualAuthentication//firmware//";
	private static final String			DEFAULT_USB_SECURE_LOG_PATH				= "gvr//DualAuthentication//secure_log//";
	private static final String			DEFAULT_USB_APPLICATION_PATH			= "gvr//application//";
	private static final String			DEFAULT_USB_STANDARD_LOG_PATH			= "gvr//standard_log//";

	private static final String			DEFAULT_INSTALL_APPLICATIONS_PATH		= "C://SPOT//Versions//";
	private static final String			DEFAULT_INSTALL_RESOURCES_PATH			= "C://SPOT//ResourcesToUpload//";

	private static final String			DEFAULT_STANDARD_LOGS_FOLDER			= "C:\\SPOT\\Logs\\standard_log\\";
	private static final String			DEFAULT_SECURE_LOGS_FOLDER				= "C:\\SPOT\\Logs\\secure_log\\";
	
	private static final int			DEFAULT_RKL_TERMINAL_PORT				= 55200;

	private static final String			DEFAULT_SAS_CERTIFICATE_COMMON_NAME		= "Gilbarco SAS test certificate";
	private static final String			DEFAULT_SAS_CERTIFICATE_DNSNAME			= "gsas.gilbarco.com";

	private static final String			DEFAULT_HANDSHAKE_CA_FILENAME			= "certificates//x509-ca.pem";
	private static final String			DEFAULT_SAS_CERTIFICATE_FILENAME		= "certificates//x509-server.pem";
	private static final String			DEFAULT_FLEXPAY_PRIVATE_KEY_FILENAME	= "certificates//rsa-vst.private.key";
	private static final String			DEFAULT_FLEXPAY_PUBLIC_KEY_FILENAME		= "certificates//rsa-vst.public.key";
	private static final String			DEFAULT_FLEXPAY_CERTIFICATE_FILENAME	= "certificates//rsa-vst.pem";

	private static final byte			FLEXPAY_SPOT_CLIENT_ID					= (byte) 0x84;

	private static String				certifTechMail							= DEFAULT_CERTIF_TECH_MAIL;
	private static long					certifExpDays							= DEFAULT_CERTIF_EXPIR_DAYS;
	private static String				certifOrg								= DEFAULT_CERTIF_ORG;
	private static String				strCountry								= DEFAULT_CERTIF_COUNTRY;
	private static String				strState								= DEFAULT_CERTIF_STATE;
	private static String				strLocality								= DEFAULT_CERTIF_LOCALITY;
	private static String				certifCommonName						= DEFAULT_CERTIF_COMMON_NAME;
	private static String				certifFolder							= DEFAULT_CERTIF_FOLDER;

	private static int					terminalPort							= DEFAULT_TERMINAL_PORT;
	private static String				currentTerminalIP						= DEFAULT_TERMINAL_IP;
	private static String				terminalType							= DEFAULT_TERMINAL_TYPE;

	// TODO: TOKEN_COMMENTED_CODE /////
	private static int					tokenServerPort							= DEFAULT_TOKEN_SERVER_PORT;
	private static String				tokenServerIp1							= DEFAULT_TOKEN_SERVER_IP_1;
	private static String				tokenServerIp2							= DEFAULT_TOKEN_SERVER_IP_2;
	private static String				usbTokenPath							= DEFAULT_USB_TOKEN_PATH;
	///////////////////////////////////

	private static String				usbDrive								= null;
	private static String				usbFirmwarePath							= DEFAULT_USB_FIRMWARE_PATH;
	private static String				usbSecureLogPath						= DEFAULT_USB_SECURE_LOG_PATH;
	private static String				usbApplicationPath						= DEFAULT_USB_APPLICATION_PATH;
	private static String				usbStandardLogPath						= DEFAULT_USB_STANDARD_LOG_PATH;

	private static String				installAppsPath							= DEFAULT_INSTALL_APPLICATIONS_PATH;
	private static String				installResourcesPath					= DEFAULT_INSTALL_RESOURCES_PATH;

	private static int					rklTerminalPort							= DEFAULT_RKL_TERMINAL_PORT;

	private static String				sasCertificateDNSName					= DEFAULT_SAS_CERTIFICATE_DNSNAME;
	private static String				sasCertificateCommonName				= DEFAULT_SAS_CERTIFICATE_COMMON_NAME;
	private static String				sasCertificateFilename					= DEFAULT_SAS_CERTIFICATE_FILENAME;
	private static String				flexpayPrivateKeyFilename				= DEFAULT_FLEXPAY_PRIVATE_KEY_FILENAME;
	private static String				flexpayPublicKeyFilename				= DEFAULT_FLEXPAY_PUBLIC_KEY_FILENAME;
	private static String				flexpayCertificateFilename				= DEFAULT_FLEXPAY_CERTIFICATE_FILENAME;
	private static String				handshakeCaPemFile						= DEFAULT_HANDSHAKE_CA_FILENAME;

	private static String				standardLogsFolder						= DEFAULT_STANDARD_LOGS_FOLDER;
	private static String				secureLogsFolder						= DEFAULT_SECURE_LOGS_FOLDER;

	private static Properties			properties;
	private static FlexPayAppConfig		instance								= null;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private FlexPayAppConfig(Properties mainAppProperties)
	{
		// Parse properties.
		try
		{
			properties = mainAppProperties;
			parseProperties( mainAppProperties );
		}
		catch (EdtAppConfigPropertyException e)
		{
			logger.error( "Error loading FlexPayAppConfig from properties file " + e.getMessage( ) );
		}
	}

	public static FlexPayAppConfig getInstance(Properties mainAppProperties)
	{
		if (instance == null)
		{
			instance = new FlexPayAppConfig( mainAppProperties );
		}

		return instance;
	}

	// ******************************************************************
	// FINALIZER.
	// ******************************************************************

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	public void finalize() throws Throwable
	{
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface)
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public String toString()
	{
		StringBuilder sb = new StringBuilder( 100 );

		sb.append( EdtUtil.getClassName( this ) + ":" );

		// ...//
		
		return sb.toString( );
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	public static String getPropertiesUsage()
	{
		StringBuilder sb = new StringBuilder( 200 );

		int count = 0;
		FlexPayAppProperty[] aps = FlexPayAppProperty.values( );
		for (FlexPayAppProperty ap : aps)
		{
			sb.append( "Required : " + ap.getOpRegStr( ) + "\n" );
			sb.append( "Type     : " + ap.getTypeStr( ) + "\n" );
			sb.append( "Default  : " + ap.getDefValueStr( ) + "\n" );
			sb.append( "Help     : " + ap.getHelpStr( ) + "\n" );

			count++;
			if (count < aps.length)
			{
				sb.append( "\n" );
			}
		} // for
		sb.append( "\n" );
		return sb.toString( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	/**
	 * Format and get a property string.
	 * <p>
	 * An exception can be set to be thrown if the property is not found.
	 * 
	 * @param lpStr
	 *            property
	 * @param expStr
	 *            Exception string (thrown if property is not found) or null (no exception thrown).
	 * @param args
	 *            Format args.
	 * @return String or null.
	 * @throws RKLAppConfigPropertyException
	 *             If config problem.
	 */
	private String getStringProp(Properties props, FlexPayAppProperty lpStr, String expStr,
									Object... args)
	{
		// Format property string.
		String fPropStr = String.format( lpStr.getFormatStr( ), args );

		// Property.
		String retval = props.getProperty( fPropStr );

		// Throw exception if needed.
		if ((retval == null) && (expStr != null))
		{
			if(expStr.equalsIgnoreCase( "required" ))
			{
				logger.error( fPropStr + " " + expStr );
			}
			return lpStr.getDefValueStr( );
		}
		else if (expStr == null)
		{
			retval = lpStr.getDefValueStr( );
		}

		return retval;
	}

	/**
	 * parse a property string.
	 * <p>
	 * An exception can be set to be thrown if the property is not found.
	 * 
	 * @param lpStr
	 *            property
	 * @param expStr
	 *            Exception string (thrown if property is not found) or null (no exception thrown).
	 * @param args
	 *            Format args.
	 * @return int or 0.
	 * @throws RKLAppConfigPropertyException
	 *             If config problem.
	 */
	private int getIntegerProp(Properties props, FlexPayAppProperty prop, String exceptionstr) throws EdtAppConfigPropertyException
	{
		int iProp = 0;
		try
		{
			return Integer.parseInt( getStringProp( props, prop, exceptionstr ) );
		}
		catch (NumberFormatException e)
		{
			logger.debug( "Error parsing value of " + prop.getFormatStr( ) + " property. "
							+ " Data type:" + prop.getTypeStr( ) + ". Help:" + prop.getHelpStr( ) );
			try
			{
				return Integer.parseInt( prop.getDefValueStr( ) );
			}
			catch (NumberFormatException e2)
			{
			}
		}
		return iProp;
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	/**
	 * Parse properties.
	 * <p>
	 * 
	 * @param props2
	 * 
	 * @throws RKLAppConfigPropertyException
	 *             If config problem.
	 */
	private void parseProperties(Properties props) throws EdtAppConfigPropertyException
	{
		// ----------------------------------------
		// Basic.
		// ----------------------------------------

		// ===========================================================================================================================
		// --------------------
		// CERTIF_TECH_MAIL = "technician@gilbarco.com";
		// --------------------
		certifTechMail = getStringProp( props, FlexPayAppProperty.P_CERTIF_TECH_MAIL, FlexPayAppProperty.P_CERTIF_TECH_MAIL.getOpRegStr( ) );

		// --------------------
		// CERTIF_EXPIR_DAYS
		// --------------------
		certifExpDays = getIntegerProp( props, FlexPayAppProperty.P_CERTIF_EXPIR_DAYS, FlexPayAppProperty.P_CERTIF_EXPIR_DAYS.getOpRegStr( ));

		// --------------------
		// CERTIF_FOLDER
		// --------------------
		certifFolder = getStringProp( props, FlexPayAppProperty.P_CERTIF_FOLDER, FlexPayAppProperty.P_CERTIF_FOLDER.getOpRegStr( ) );

		// --------------------
		// CERTIF_ORG
		// --------------------
		certifOrg = getStringProp( props, FlexPayAppProperty.P_CERTIF_ORG, FlexPayAppProperty.P_CERTIF_ORG.getOpRegStr( ) );

		// --------------------
		// CERTIF_COUNTRY
		// --------------------
		strCountry = getStringProp( props, FlexPayAppProperty.P_CERTIF_COUNTRY, FlexPayAppProperty.P_CERTIF_COUNTRY.getOpRegStr( ) );

		// --------------------
		// CERTIF_STATE
		// --------------------
		strState = getStringProp( props, FlexPayAppProperty.P_CERTIF_STATE, FlexPayAppProperty.P_CERTIF_STATE.getOpRegStr( ) );

		// --------------------
		// CERTIF_LOCALITY
		// --------------------
		strLocality = getStringProp( props, FlexPayAppProperty.P_CERTIF_LOCALITY, FlexPayAppProperty.P_CERTIF_LOCALITY.getOpRegStr( ) );
		// ===========================================================================================================================

		// ===========================================================================================================================
		// --------------------
		// TERMINAL_PORT = 5200;
		// --------------------
		terminalPort = getIntegerProp( props, FlexPayAppProperty.P_TERMINAL_PORT, FlexPayAppProperty.P_TERMINAL_PORT.getOpRegStr( ) );

		// --------------------
		// TERMINAL_IP
		// --------------------
		currentTerminalIP = getStringProp( props, FlexPayAppProperty.P_TERMINAL_IP, FlexPayAppProperty.P_TERMINAL_IP.getOpRegStr( ) );

		// --------------------
		// TERMINAL_TYPE
		// --------------------
		terminalType = getStringProp( props, FlexPayAppProperty.P_TERMINAL_TYPE, FlexPayAppProperty.P_TERMINAL_TYPE.getOpRegStr( ) );

		// ===========================================================================================================================
		// TODO: TOKEN_COMMENTED_CODE /////
		// --------------------
		// SERVER_PORT
		// --------------------
		tokenServerPort = getIntegerProp( props, FlexPayAppProperty.P_TOKEN_SERVER_PORT, FlexPayAppProperty.P_TOKEN_SERVER_PORT.getOpRegStr( ));
		// --------------------
		// SERVER_IP_1
		// --------------------
		tokenServerIp1 = getStringProp( props, FlexPayAppProperty.P_TOKEN_SERVER_IP_1, FlexPayAppProperty.P_TOKEN_SERVER_IP_1.getOpRegStr( ) );
		// --------------------
		// SERVER_IP_2
		// --------------------
		tokenServerIp2 = getStringProp( props, FlexPayAppProperty.P_TOKEN_SERVER_IP_2, FlexPayAppProperty.P_TOKEN_SERVER_IP_2.getOpRegStr( ) );
		// --------------------
		// USB_TOKEN_PATH
		// --------------------
		usbTokenPath = getStringProp( props, FlexPayAppProperty.P_USB_TOKEN_PATH, FlexPayAppProperty.P_USB_TOKEN_PATH.getOpRegStr( ) );
		///////////////////////////////////
		// ===========================================================================================================================

		// ===========================================================================================================================
		// --------------------
		// saved USB Drive
		// --------------------
		usbDrive = getStringProp( props, FlexPayAppProperty.P_USB_DRIVE, FlexPayAppProperty.P_USB_DRIVE.getOpRegStr( ) );

		// --------------------
		// USB_FIRMWARE_PATH
		// --------------------
		usbFirmwarePath = getStringProp( props, FlexPayAppProperty.P_USB_FIRMWARE_PATH,FlexPayAppProperty.P_USB_FIRMWARE_PATH.getOpRegStr( ) );

		// --------------------
		// USB_SECURE_LOG_PATH
		// --------------------
		usbSecureLogPath = getStringProp( props, FlexPayAppProperty.P_USB_SECURE_LOG_PATH, FlexPayAppProperty.P_USB_SECURE_LOG_PATH.getOpRegStr( ) );

		// --------------------
		// USB_APPLICATIONS_PATH
		// --------------------
		usbApplicationPath = getStringProp( props, FlexPayAppProperty.P_USB_APPLICATIONS_PATH, FlexPayAppProperty.P_USB_APPLICATIONS_PATH.getOpRegStr( ) );

		// --------------------
		// USB_STANDARD_LOG_PATH
		// --------------------
		usbStandardLogPath = getStringProp( props, FlexPayAppProperty.P_USB_STANDARD_LOG_PATH, FlexPayAppProperty.P_USB_STANDARD_LOG_PATH.getOpRegStr( ) );

		// --------------------
		// install applications path
		// --------------------
		installAppsPath = getStringProp( props, FlexPayAppProperty.P_INSTALL_APPLICATIONS_PATH, FlexPayAppProperty.P_INSTALL_APPLICATIONS_PATH.getOpRegStr( ) );

		// --------------------
		// install resources path
		// --------------------
		installResourcesPath = getStringProp( props, FlexPayAppProperty.P_INSTALL_RESOURCES_PATH, FlexPayAppProperty.P_INSTALL_RESOURCES_PATH.getOpRegStr( ) );
		// ===========================================================================================================================

		// --------------------
		// RKL TERMINAL_PORT = 55200
		// --------------------
		rklTerminalPort = getIntegerProp( props, FlexPayAppProperty.P_RKL_TERMINAL_PORT, FlexPayAppProperty.P_RKL_TERMINAL_PORT.getOpRegStr( ) );
		// ===========================================================================================================================

		// ===========================================================================================================================
		// --------------------
		// SAS_CERT_FILENAME
		// --------------------
		sasCertificateFilename = getStringProp( props, FlexPayAppProperty.P_SAS_CERT_FILENAME, FlexPayAppProperty.P_SAS_CERT_FILENAME.getOpRegStr( ) );

		// --------------------
		// CA_CERT_FILENAME
		// --------------------
		handshakeCaPemFile = getStringProp( props, FlexPayAppProperty.P_CA_CERT_FILENAME, FlexPayAppProperty.P_CA_CERT_FILENAME.getOpRegStr( ) );

		// --------------------
		// SAS_CERT_COMMON_NAME
		// --------------------
		sasCertificateCommonName = getStringProp( props, FlexPayAppProperty.P_SAS_CERT_COMMON_NAME,FlexPayAppProperty.P_SAS_CERT_COMMON_NAME.getOpRegStr( ) );

		// --------------------
		// SAS_CERT_DNSNAME
		// --------------------
		sasCertificateDNSName = getStringProp( props, FlexPayAppProperty.P_SAS_CERT_DNS_NAME, FlexPayAppProperty.P_SAS_CERT_DNS_NAME.getOpRegStr( ) );

		// ===========================================================================================================================
		// --------------------
		// STANDARD_LOGS_FOLDER
		// --------------------
		standardLogsFolder = getStringProp(props, FlexPayAppProperty.P_STANDARD_LOGS_FOLDER, FlexPayAppProperty.P_STANDARD_LOGS_FOLDER.getOpRegStr( ));
		
		// --------------------
		// SECURE_LOGS_FOLDER
		// --------------------
		secureLogsFolder = getStringProp(props, FlexPayAppProperty.P_SECURE_LOGS_FOLDER, FlexPayAppProperty.P_SECURE_LOGS_FOLDER.getOpRegStr( ));
		
		// ===========================================================================================================================
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	/**
	 * Save the current properties values.
	 * <p>
	 * 
	 * @param props2
	 * 
	 * @throws IOException
	 *             If problem saving properties.
	 */
	public static void saveProperties()
	{
		try
		{
			EdtSortedProperties sp = new EdtSortedProperties( properties );

			sp.store( new FileOutputStream( EdtAppConfig.getPropertiesPath( ) ), null );
		}
		catch (IOException e)
		{
				logger.error( "Properties not saved." );
		}
	}

	// --------------------------------------------------------------
	// getters
	// --------------------------------------------------------------

	// ===========================================================================================================================
	public static String getDefaultCertifCommonName()
	{
		return DEFAULT_CERTIF_COMMON_NAME;
	}

	public static String getDefaultCertifFolder()
	{
		return DEFAULT_CERTIF_FOLDER;
	}

	public static String getDefaultCertifTechMail()
	{
		return DEFAULT_CERTIF_TECH_MAIL;
	}

	public static int getDefaultCertifExpirDays()
	{
		return DEFAULT_CERTIF_EXPIR_DAYS;
	}

	public static String getDefaultCertifOrg()
	{
		return DEFAULT_CERTIF_ORG;
	}

	public static String getDefaultCertifCountry()
	{
		return DEFAULT_CERTIF_COUNTRY;
	}

	public static String getDefaultCertifState()
	{
		return DEFAULT_CERTIF_STATE;
	}

	public static String getDefaultCertifLocality()
	{
		return DEFAULT_CERTIF_LOCALITY;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static int getDefaultTerminalPort()
	{
		return DEFAULT_TERMINAL_PORT;
	}

	public static String getDefaultTerminalIp()
	{
		return DEFAULT_TERMINAL_IP;
	}

	public static String getDefaultTerminalType()
	{
		return DEFAULT_TERMINAL_TYPE;
	}
	// ===========================================================================================================================

	// ===========================================================================================================================
	// TODO: TOKEN_COMMENTED_CODE /////
	public static int getDefaultTokenServerPort()
	{
		return DEFAULT_TOKEN_SERVER_PORT;
	}

	public static String getDefaultTokenServerIp1()
	{
		return DEFAULT_TOKEN_SERVER_IP_1;
	}

	public static String getDefaultTokenServerIp2()
	{
		return DEFAULT_TOKEN_SERVER_IP_2;
	}
	// ===========================================================================================================================

	// ===========================================================================================================================
	public static String getDefaultUsbTokenPath()
	{
		return DEFAULT_USB_TOKEN_PATH;
	}
	// ===========================================================================================================================

	// ===========================================================================================================================
///////////////////////////////////
	public static String getDefaultUsbFirmwarePath()
	{
		return DEFAULT_USB_FIRMWARE_PATH;
	}

	public static String getDefaultUsbSecureLogPath()
	{
		return DEFAULT_USB_SECURE_LOG_PATH;
	}

	public static String getDefaultUsbApplicationPath()
	{
		return DEFAULT_USB_APPLICATION_PATH;
	}

	public static String getDefaultUsbStandardLogPath()
	{
		return DEFAULT_USB_STANDARD_LOG_PATH;
	}

	public static String getDefaultInstallApplicationsPath()
	{
		return DEFAULT_INSTALL_APPLICATIONS_PATH;
	}

	public static String getDefaultInstallResourcesPath()
	{
		return DEFAULT_INSTALL_RESOURCES_PATH;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static int getDefaultRklTerminalPort()
	{
		return DEFAULT_RKL_TERMINAL_PORT;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static String getDefaultSasCertificateFilename()
	{
		return DEFAULT_SAS_CERTIFICATE_FILENAME;
	}

	public static String getDefaultFlexpayPrivateKeyFilename()
	{
		return DEFAULT_FLEXPAY_PRIVATE_KEY_FILENAME;
	}

	public static String getDefaultFlexpayPublicKeyFilename()
	{
		return DEFAULT_FLEXPAY_PUBLIC_KEY_FILENAME;
	}

	public static String getDefaultFlexpayCertificateFilename()
	{
		return DEFAULT_FLEXPAY_CERTIFICATE_FILENAME;
	}

	public static String getDefaultHandshakeCaFilename()
	{
		return DEFAULT_HANDSHAKE_CA_FILENAME;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static String getCertifCommonName()
	{
		return certifCommonName;
	}

	public static String getCertifFolder()
	{
		return certifFolder;
	}

	public static String getCertifTechMail()
	{
		return certifTechMail;
	}

	public static long getCertifExpDays()
	{
		return certifExpDays;
	}

	public static String getCertifOrg()
	{
		return certifOrg;
	}

	public static String getStrCountry()
	{
		return strCountry;
	}

	public static String getStrState()
	{
		return strState;
	}

	public static String getStrLocality()
	{
		return strLocality;
	}

	// ===========================================================================================================================

	//server certificate data 
	
	public static String getServerCertifOrg()
	{
		return getCertifOrg(); //upn is the same for tools and server
	}

	public static String getServerCertifCommonName()
	{
		return sasCertificateCommonName;
	}

	public static String getServerCertifDNSName()
	{
		return sasCertificateDNSName;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static int getTerminalPort()
	{
		return terminalPort;
	}

	public static String getCurrentTerminalIP()
	{
		return currentTerminalIP;
	}

	public static String getTerminalType()
	{
		return terminalType;
	}

	// ===========================================================================================================================
	// TODO: TOKEN_COMMENTED_CODE /////
	public static int getTokenServerPort()
	{
		return tokenServerPort;
	}

	public static String getTokenServerIp1()
	{
		return tokenServerIp1;
	}

	public static String getTokenServerIp2()
	{
		return tokenServerIp2;
	}
	// ===========================================================================================================================

	// ===========================================================================================================================
	public static String getUsbTokenPath()
	{
		return usbTokenPath;
	}
	///////////////////////////////////

	// ===========================================================================================================================
	public static String getStandardLogsFolder()
	{
		return standardLogsFolder;
	}
	
	public static String getPCILogsFolder()
	{
		return secureLogsFolder;
	}
	// ===========================================================================================================================
	public static String getUsbDrive()
	{
		return usbDrive;
	}

	public static String getUsbFirmwarePath()
	{
		return usbFirmwarePath;
	}

	public static String getUsbSecureLogPath()
	{
		return usbSecureLogPath;
	}

	public static String getUsbApplicationPath()
	{
		return usbApplicationPath;
	}

	public static String getUsbStandardLogPath()
	{
		return usbStandardLogPath;
	}

	public static String getInstallAppsPath()
	{
		return installAppsPath;
	}

	public static String getInstallResourcesPath()
	{
		return installResourcesPath;
	}

	// ===========================================================================================================================
	public static int getRklTerminalPort()
	{
		return rklTerminalPort;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static String getSasCertificateFilename()
	{
		return sasCertificateFilename;
	}

	public static String getFlexpayPrivateKeyFilename()
	{
		return flexpayPrivateKeyFilename;
	}

	public static String getFlexpayPublicKeyFilename()
	{
		return flexpayPublicKeyFilename;
	}

	public static String getFlexpayCertificateFilename()
	{
		return flexpayCertificateFilename;
	}

	public static String getHandshakeCaPemFile()
	{
		return handshakeCaPemFile;
	}

	// ===========================================================================================================================

	// --------------------------------------------------------------
	// setters
	// --------------------------------------------------------------
	// ===========================================================================================================================
	public static void setCertifCommonName(String data)
	{
		certifCommonName = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_COMMON_NAMES.getFormatStr( ), data );
	}

	public static void setCertifFolder(String data)
	{
		certifFolder = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_FOLDER.getFormatStr( ), data );
	}

	public static void setCertifTechMail(String data)
	{
		certifTechMail = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_TECH_MAIL.getFormatStr( ), data );
	}

	public static void setCertifExpDays(int data)
	{
		certifExpDays = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_EXPIR_DAYS.getFormatStr( ), Integer.toString( data ) );
	}

	public static void setCertifOrg(String data)
	{
		certifOrg = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_ORG.getFormatStr( ), data );
	}

	public static void setStrCountry(String data)
	{
		strCountry = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_COUNTRY.getFormatStr( ), data );
	}

	public static void setStrState(String data)
	{
		strState = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_STATE.getFormatStr( ), data );
	}

	public static void setStrLocality(String data)
	{
		strLocality = data;
		properties.setProperty( FlexPayAppProperty.P_CERTIF_LOCALITY.getFormatStr( ), data );
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static void setTerminalPort(int data)
	{
		terminalPort = data;
		properties.setProperty( FlexPayAppProperty.P_TERMINAL_PORT.getFormatStr( ), Integer.toString( data ) );
	}

	public static void setCurrentTerminalIP(String data)
	{
		currentTerminalIP = data;
		properties.setProperty( FlexPayAppProperty.P_TERMINAL_IP.getFormatStr( ), data );
	}

	public static void setTerminalType(String data)
	{
		terminalType = data;
		properties.setProperty( FlexPayAppProperty.P_TERMINAL_TYPE.getFormatStr( ), data );
	}

	// ===========================================================================================================================
	// TODO: TOKEN_COMMENTED_CODE /////
	public static void setTokenServerPort(int data)
	{
		tokenServerPort = data;
		properties.setProperty( FlexPayAppProperty.P_TOKEN_SERVER_PORT.getFormatStr( ), Integer.toString( data ) );
	}

	public static void setTokenServerIp1(String data)
	{
		tokenServerIp1 = data;
		properties.setProperty( FlexPayAppProperty.P_TOKEN_SERVER_IP_1.getFormatStr( ), data );
	}

	public static void setTokenServerIp2(String data)
	{
		tokenServerIp2 = data;
		properties.setProperty( FlexPayAppProperty.P_TOKEN_SERVER_IP_2.getFormatStr( ), data );
	}
	// ===========================================================================================================================

	// ===========================================================================================================================
	public static void setUsbTokenPath(String data)
	{
		usbTokenPath = data;
		properties.setProperty( FlexPayAppProperty.P_USB_TOKEN_PATH.getFormatStr( ), data );
	}
	///////////////////////////////////
	// ===========================================================================================================================

	// ===========================================================================================================================
	public static void setUsbDrive(String data)
	{
		usbDrive = data;
		properties.setProperty( FlexPayAppProperty.P_USB_DRIVE.getFormatStr( ), data );
	}

	public static void setInstallAppsPath(String data)
	{
		installAppsPath = data;
		properties.setProperty( FlexPayAppProperty.P_INSTALL_APPLICATIONS_PATH.getFormatStr( ), data );
	}

	public static void setInstallResourcesPath(String data)
	{
		installResourcesPath = data;
		properties.setProperty( FlexPayAppProperty.P_INSTALL_RESOURCES_PATH.getFormatStr( ), data );
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static void setRklTerminalPort(int data)
	{
		rklTerminalPort = data;
		properties.setProperty( FlexPayAppProperty.P_RKL_TERMINAL_PORT.getFormatStr( ), Integer.toString( data ) );
	}

	// ===========================================================================================================================

	// ===========================================================================================================================
	public static void setSasCertificateFilename(String data)
	{
		sasCertificateFilename = data;
		properties.setProperty( FlexPayAppProperty.P_SAS_CERT_FILENAME.getFormatStr( ), data );
	}

	public static void setFlexpayPrivateKeyFilename(String data)
	{
		flexpayPrivateKeyFilename = data;
		properties.setProperty( FlexPayAppProperty.P_FLEXPAY_PRIVATE_KEY_FILENAME.getFormatStr( ), data );
	}

	public static void setFlexpayPublicKeyFilename(String data)
	{
		flexpayPublicKeyFilename = data;
		properties.setProperty( FlexPayAppProperty.P_FLEXPAY_PUBLIC_KEY_FILENAME.getFormatStr( ), data );
	}

	public static void setFlexpayCertificateFilename(String data)
	{
		flexpayCertificateFilename = data;
		properties.setProperty( FlexPayAppProperty.P_FLEXPAY_CERT_FILENAME.getFormatStr( ), data );
	}

	public static void setHandshakeCaPemFile(String data)
	{
		handshakeCaPemFile = data;
		properties.setProperty( FlexPayAppProperty.P_CA_CERT_FILENAME.getFormatStr( ), data );
	}
	
	public static void setStandardLogsFolder(String data)
	{
		standardLogsFolder = data;
		properties.setProperty( FlexPayAppProperty.P_STANDARD_LOGS_FOLDER.getFormatStr( ), data );		
	}
	
	public static void setSecureLogsFolder(String data)
	{
		secureLogsFolder = data;
		properties.setProperty( FlexPayAppProperty.P_SECURE_LOGS_FOLDER.getFormatStr( ), data );		
	}
	

	// ===========================================================================================================================

	public static byte getSpotClientId()
	{
		return FLEXPAY_SPOT_CLIENT_ID;
	}

	public static String getApplicationLogFile()
	{
		return "EdtApp.log";
	}

	/**
	 * Gets the title app name.
	 *
	 * @return the title app name
	 */
	public static String getTitleAppName()
	{
		return "Simplified Vanguard Service Tool - SVST v0.0.09";
	}
}

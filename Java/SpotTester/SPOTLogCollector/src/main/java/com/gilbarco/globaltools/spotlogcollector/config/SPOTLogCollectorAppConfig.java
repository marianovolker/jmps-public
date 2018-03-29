/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.spotlogcollector.config;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfig;
import com.gilbarco.globaltools.edt.app.config.EdtAppConfigPropertyException;
import com.gilbarco.globaltools.edt.util.EdtSortedProperties;
import com.gilbarco.globaltools.edt.util.EdtUtil;

public class SPOTLogCollectorAppConfig
{
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	public enum SPOTLogCollectorAppProperty
	{
		// ======================================================================================================================

		// ======================================================================================================================
		// OPT Terminal connection settings
		P_TERMINAL_PORT(
						"spotlogcollector.communication.terminal.port",
						"Optional",
						"Integer",
						"" + DEFAULT_TERMINAL_PORT,
						"helpstring"),

		P_TERMINAL_IP(
						"spotlogcollector.communication.terminal.ip",
						"Optional",
						"String",
						DEFAULT_TERMINAL_IP,
						"helpstring"),

		P_LOGS_DOWNLOAD_PATH(
						"spotlogcollector.settings.logs_download_path",
						"Optional",
						"String",
						DEFAULT_LOGS_DOWNLOAD_PATH,
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
		private SPOTLogCollectorAppProperty(String key, String opReq, String type, String defVal, String help)
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
	private static final transient Log			logger							= LogFactory.getLog( SPOTLogCollectorAppConfig.class );

	private static final int					DEFAULT_TERMINAL_PORT			= 5200;
	private static final String					DEFAULT_TERMINAL_IP				= "";
	private static final String					DEFAULT_LOGS_DOWNLOAD_PATH		= "";
	private static final String					APPLICATION_LOGNAME				= "SPOTLogCollector.log";
	private static final String					APPLICATION_ORIGINAL_LOGNAME	= "EdtApp.log";
	private static final String					APPLICATION_NOTES_FILE_NAME		= "NOTES.txt";

	private static final byte					SPOT_CLIENT_ID					= (byte) 0x83;

	private static int							terminalPort					= DEFAULT_TERMINAL_PORT;
	private static String						currentTerminalIP				= DEFAULT_TERMINAL_IP;
	private static String						logsDownloadPath				= DEFAULT_LOGS_DOWNLOAD_PATH;

	private static Properties					properties;
	private static SPOTLogCollectorAppConfig	instance						= null;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private SPOTLogCollectorAppConfig(Properties mainAppProperties)
	{
		// Parse properties.
		try
		{
			properties = mainAppProperties;
			parseProperties( mainAppProperties );
		}
		catch (EdtAppConfigPropertyException e)
		{
			logger.error( "Error loading SPOTLogCollectorAppConfig from properties file " + e.getMessage( ) );
		}
	}

	public static SPOTLogCollectorAppConfig getInstance(Properties mainAppProperties)
	{
		if (instance == null)
		{
			instance = new SPOTLogCollectorAppConfig( mainAppProperties );
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
		SPOTLogCollectorAppProperty[] aps = SPOTLogCollectorAppProperty.values( );
		for (SPOTLogCollectorAppProperty ap : aps)
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
	private String getStringProp(Properties props, SPOTLogCollectorAppProperty lpStr, String expStr,
									Object... args)
	{
		// Format property string.
		String fPropStr = String.format( lpStr.getFormatStr( ), args );

		// Property.
		String retval = props.getProperty( fPropStr );

		// Throw exception if needed.
		if ((retval == null) && (expStr != null))
		{
			logger.error( fPropStr + " " + expStr );
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
	private int getIntegerProp(Properties props, SPOTLogCollectorAppProperty prop, String exceptionstr) throws EdtAppConfigPropertyException
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
		// TERMINAL_PORT = 5200;
		// --------------------
		terminalPort = getIntegerProp( props, SPOTLogCollectorAppProperty.P_TERMINAL_PORT, "optional" );

		// --------------------
		// TERMINAL_IP
		// --------------------
		currentTerminalIP = getStringProp( props, SPOTLogCollectorAppProperty.P_TERMINAL_IP, "optional." );

		// --------------------
		// TERMINAL_IP
		// --------------------
		logsDownloadPath = getStringProp( props, SPOTLogCollectorAppProperty.P_LOGS_DOWNLOAD_PATH, "optional." );

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
	public static int getDefaultTerminalPort()
	{
		return DEFAULT_TERMINAL_PORT;
	}

	public static String getDefaultTerminalIp()
	{
		return DEFAULT_TERMINAL_IP;
	}

	public static String getDefaultLogsDownloadPath()
	{
		return DEFAULT_LOGS_DOWNLOAD_PATH;
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

	public static String getLogsDownloadPath()
	{
		return logsDownloadPath;
	}

	// ===========================================================================================================================

	// --------------------------------------------------------------
	// setters
	// --------------------------------------------------------------

	// ===========================================================================================================================
	public static void setTerminalPort(int data)
	{
		terminalPort = data;
		properties.setProperty( SPOTLogCollectorAppProperty.P_TERMINAL_PORT.getFormatStr( ), Integer.toString( data ) );
	}

	public static void setCurrentTerminalIP(String data)
	{
		currentTerminalIP = data;
		properties.setProperty( SPOTLogCollectorAppProperty.P_TERMINAL_IP.getFormatStr( ), data );
	}

	// ===========================================================================================================================

	// ===========================================================================================================================

	public static void setLogsDownloadPath(String data)
	{
		logsDownloadPath = data;
		properties.setProperty( SPOTLogCollectorAppProperty.P_LOGS_DOWNLOAD_PATH.getFormatStr( ), data );
	}

	public static String getSPOTLogCollectorLogName()
	{
		return APPLICATION_LOGNAME;
	}

	public static String getSPOTLogCollectorOriginalLogName()
	{
		return APPLICATION_ORIGINAL_LOGNAME;
	}

	public static String getSPOTLogCollectorNotesFileName()
	{
		return APPLICATION_NOTES_FILE_NAME;
	}

	public static byte getSpotClientID()
	{
		return SPOT_CLIENT_ID;
	}

	// ===========================================================================================================================

	// ===========================================================================================================================

}


package com.gilbarco.globaltools.spotdecommissioning.config;

import java.io.FileInputStream;
import java.util.Properties;

import junit.framework.Assert;

import org.junit.Test;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfig;
import com.gilbarco.globaltools.spotdecommissioning.config.SPOTDecommissioningAppConfig;

public class SPOTDecommissioningAppConfigTest
{
	@Test
	public void testDefaults()
	{
		Properties mainAppProperties;
		try (FileInputStream fis = new FileInputStream( EdtAppConfig.getAppPropertiesPath( ) ))
		{
			mainAppProperties = new Properties( );
			mainAppProperties.load( fis );
		}
		catch (Exception ex)
		{
			mainAppProperties = new Properties( );
		}
		
		@SuppressWarnings("static-access")
		String defaultIP = SPOTDecommissioningAppConfig.getInstance( mainAppProperties ).getCurrentTerminalIP( );
		Assert.assertTrue( defaultIP.equals( mainAppProperties
						.getProperty( "spotdecommissioning.communication.terminal.ip" ) ) );
	}

}

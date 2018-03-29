
package com.gilbarco.globaltools.spotlogcollector.config;

import java.io.FileInputStream;
import java.util.Properties;

import junit.framework.Assert;

import org.junit.Test;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfig;

public class SPOTLogCollectorAppConfigTest
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
		String defaultIP = SPOTLogCollectorAppConfig.getInstance( mainAppProperties ).getCurrentTerminalIP( );
		Assert.assertTrue( defaultIP.equals( mainAppProperties
						.getProperty( "spotlogcollector.communication.terminal.ip" ) ) );
	}

}

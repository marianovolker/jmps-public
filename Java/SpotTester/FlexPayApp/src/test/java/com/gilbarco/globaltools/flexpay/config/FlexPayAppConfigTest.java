
package com.gilbarco.globaltools.flexpay.config;

import java.io.FileInputStream;
import java.util.Properties;

import junit.framework.Assert;

import org.junit.Test;

import com.gilbarco.globaltools.edt.app.config.EdtAppConfig;

public class FlexPayAppConfigTest
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

		FlexPayAppConfig.getInstance( mainAppProperties );

		String techMail = FlexPayAppConfig.getCurrentTerminalIP( );
		Assert.assertTrue( techMail.equals( mainAppProperties.getProperty( "vanguard.communication.terminal.ip" ) ) );
	}
}

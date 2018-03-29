
package com.gilbarco.globaltools.spotlogcollector.main.gui;

import org.junit.Assert;
import org.junit.Test;

public class SPOTLogCollectorViewTest
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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Test
	public void SPOTLogCollectorViewInitialStateTest()
	{
		try
		{
			SPOTLogCollectorView app = new SPOTLogCollectorView( );
			app.initialize( );
			
			if(app.getIpInserted( ).length( )>0 && app.getDownloadDirectory().length( )>0)
				Assert.assertTrue( app.getCollectLogsButton( ).isEnabled( ) );
			else
				Assert.assertFalse( app.getCollectLogsButton( ).isEnabled( ) );				
		}
		catch (Exception e)
		{
			Assert.fail( e.getMessage( ) );
		}
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
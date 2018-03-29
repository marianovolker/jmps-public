
package com.gilbarco.globaltools.spotdecommissioning.main.gui;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.spotdecommissioning.main.gui.SPOTDecommissioningView;

public class SPOTDecommissioningViewTest
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
	public void SPOTDecommissioningViewInitialStateTest()
	{
		try
		{
			SPOTDecommissioningView app = new SPOTDecommissioningView( );
			app.initialize( );
			
			if(app.getIpInserted( ).length( )>0 )
				Assert.assertTrue( app.getClearKeysButton( ).isEnabled( ) );
			else
				Assert.assertFalse( app.getClearKeysButton( ).isEnabled( ) );				
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

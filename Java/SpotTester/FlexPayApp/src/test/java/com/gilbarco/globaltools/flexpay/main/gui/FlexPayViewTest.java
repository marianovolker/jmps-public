
package com.gilbarco.globaltools.flexpay.main.gui;

import org.junit.Assert;
import org.junit.Test;

public class FlexPayViewTest
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
	public void FlexPayViewInitialStateTest()
	{
		try
		{
			FlexPayView app = new FlexPayView( );
			app.initialize( );

			Assert.assertTrue( app.getTechnicianNumberField( ).getText( ).length( ) == 0 );
			Assert.assertTrue( app.getTechnicianNumberField( ).isEnabled( ) );
/* TODO: TOKEN_COMMENTED_CODE
			Assert.assertFalse( app.getRequestTokenButton( ).isEnabled( ) );
*/
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
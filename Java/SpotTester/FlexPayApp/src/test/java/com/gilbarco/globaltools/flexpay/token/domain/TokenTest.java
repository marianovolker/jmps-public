/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.domain;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.flexpay.token.domain.Token;




public class TokenTest
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

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	@Test
	public void TokenCRUDTest()
	{
		try
		{
			boolean blret = false;
			Token tdb = null;
			Token t = new Token("012345678901234567890","5/31/2011 04:39:52 PM", "5/31/2011 08:39:52 PM");

			// Normal addition test
			blret = Token.addToDatabase(t);
			Assert.assertTrue( blret);
			Assert.assertTrue( Token.getFromDatabase( "012345678901234567890" ).getIdToken() > 0 );

			// Duplicated addition test
			TokenForUnitTesting tHack;
			tHack = new TokenForUnitTesting("012345678901234567890","5/31/2011 04:39:52 PM","5/31/2011 08:39:52 PM"  );
			tHack.setIdToken( t.getIdToken( ) );
			t = tHack; // token with pre-existent ID in database
			blret = Token.addToDatabase(t);
			Assert.assertFalse( blret); // We expect duplicated token to be rejected.

			// Reading database test
			t = new Token("901234567890123456789", "5/31/2011 04:39:52 PM", "5/31/2011 08:39:52 PM");
			blret = Token.addToDatabase(t);
			Assert.assertTrue( blret); // normal addition test again
			tdb = Token.getFromDatabase( "901234567890123456789" );
			Assert.assertTrue(t == tdb); // same object reference is expected
			tdb = null;
			tdb = Token.getFromDatabase( t.getIdToken( ) );
			Assert.assertTrue(t == tdb); // same object reference is expected

			//Token Permissions Test
			t.setPermissions("NetworkConfiguration");
			Assert.assertTrue(t.getPermissions( ).equals( "NetworkConfiguration" ));
			Assert.assertFalse(Token.hasSoftwareUpdatesPermissions( t ));
			t.setPermissions( "NetworkConfiguration, FirmwareUpdate" );
			Assert.assertTrue(Token.hasSoftwareUpdatesPermissions( t ));
			
			// Deleting test
			int id = t.getIdToken( );
			blret = Token.deleteFromDatabase(id);
			Assert.assertTrue( blret); // we expect to successfully delete the token in DB
			blret = Token.deleteFromDatabase(id);
			Assert.assertFalse( blret); // we expect to fail (id already deleted)
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

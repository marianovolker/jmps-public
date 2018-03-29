package com.gilbarco.globaltools.flexpay.token.domain;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.flexpay.token.domain.Location;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;

public class UnitTest
{
	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

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
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************
	@Test
	public void UnitCRUDTest()
	{
		try
		{
			boolean blret = false;
			Location l = new Location("Popshope", "GSO", "NC", "USA");
			Unit u = new Unit("150873", 1, l);

			// Normal addition test
			blret = Unit.addToDatabase(u);
			Assert.assertTrue( blret);

			// Read test
			Assert.assertTrue( u == Unit.getFromDatabase( u.getIdUnit() ) );
			Assert.assertTrue( u == Unit.getFromDatabase( "150873" ) );

			// Duplicated unit ID test
			// Duplicated addition test
			UnitForUnitTesting uHack;
			uHack = new UnitForUnitTesting("12345", 2, l);
			uHack.setIdUnit( u.getIdUnit( ) );
			u = uHack; // unit with pre-existent ID in database
			blret = Unit.addToDatabase(u);
			Assert.assertFalse( blret); // We expect duplicated unit to be rejected.

			// Deleting test
			int id = u.getIdUnit( );
			blret = Unit.deleteFromDatabase(id);
			Assert.assertTrue( blret); // we expect to successfully delete the unit in DB
			blret = Unit.deleteFromDatabase(id);
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
	// MAIN.
	// ******************************************************************

}

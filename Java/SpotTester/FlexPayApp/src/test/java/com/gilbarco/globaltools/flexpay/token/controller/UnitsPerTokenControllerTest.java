/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.controller;

import org.junit.Assert;
import org.junit.Test;

import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;
import com.gilbarco.globaltools.flexpay.token.domain.UnitsPerToken;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenUnitTableRow;

/**
 * @author gparis
 * 
 */
public class UnitsPerTokenControllerTest
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
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
//	@SuppressWarnings("deprecation")
//	@Test
	public void UnitsPerTokenCRUDControllerTest()
	{
		boolean blret = false;
		int iFirstUnitID = 0;
		DataTransferForTokenUnitTableRow dttu = new DataTransferForTokenUnitTableRow( );
		Token t = null;
		Unit u = null;
		UnitsPerToken ut = null;

		dttu.setIdToken( 1 );
		dttu.setIdUnit( -1 ); // this will be overridden by addUnitAndToken()
		dttu.setTokenCode( "01234567890123456789" );
		dttu.setTargetUnit( "12345" );
		dttu.setStartTime( "11/21/2012T08:00" );
		dttu.setEndTime( "11/21/2012T08:00" );
		dttu.setPermissions( "SecureDevicesActivation" );
		dttu.setRemoteMaintenance( "disabled" );
		dttu.setTechinicianDevice( "techiniciandevice" );
		dttu.setOperatorID( "operator1" );

		try
		{
			// Add unit per token test - new token
			blret = UnitsPerTokenController.addUnitAndToken( dttu, null );
			Assert.assertTrue( blret ); // we expect a new token has been created with ID 1

			// read tests
			iFirstUnitID = dttu.getUnitID( ); // previous call returned the generated unit ID
			ut = UnitsPerTokenController.getUnitPerToken( 1, iFirstUnitID );
			Assert.assertFalse( ut == null ); // we expect a reference to an existent ut

			t = Token.getFromDatabase( 1 );
			Assert.assertFalse( t == null ); // we expect a reference to an existent token

			u = Unit.getFromDatabase( iFirstUnitID );
			Assert.assertFalse( u == null ); // we expect a reference to an existent unit

			Assert.assertTrue( ut == t.getTokenUnit( iFirstUnitID ) );

			Assert.assertTrue( ut == u.getUnitToken( 1 ) );

			Assert.assertTrue( t.getTokenCode( ).equals( dttu.getTokenCode( ) ) );
			Assert.assertTrue( u.getPPN( ) == dttu.getPPN( ) );

			// Add the same unit per token another time
			// Use the former MUPT (multiple units per token) that checks duplication
			blret = UnitsPerTokenController.addUnitAndToken_MultipleUnitsPerToken( dttu );
			Assert.assertFalse( blret ); // we don't expect a new token creation

			// we don't expect the same unit to be created again
			Assert.assertFalse( iFirstUnitID == dttu.getUnitID( ) );
			Assert.assertFalse( ut == t.getTokenUnit( dttu.getUnitID( ) ) );

			// we expect a new unit-token was created
			ut = UnitsPerTokenController.getUnitPerToken( 1, dttu.getUnitID( ) );
			Assert.assertTrue( ut == t.getTokenUnit( dttu.getUnitID( ) ) );

			u = Unit.getFromDatabase( dttu.getUnitID( ) );
			Assert.assertFalse( u == null ); // we expect a reference to an existent unit
			Assert.assertTrue( ut == u.getUnitToken( 1 ) );
			Assert.assertTrue( t.getTokenCode( ).equals( dttu.getTokenCode( ) ) );
			Assert.assertTrue( u.getPPN( ) == dttu.getPPN( ) );

			// Finally, the deletion test
			blret = UnitsPerTokenController.deleteUnitPerToken( 1, dttu.getUnitID( ) );
			Assert.assertTrue( blret );
			blret = UnitsPerTokenController.deleteUnitPerToken( 1, dttu.getUnitID( ) );
			Assert.assertFalse( blret );


			// unit-token musn't be related with the unit anymore
			ut = u.getUnitToken( 1 );
			Assert.assertTrue( ut == null );

			// unit-token also musn't be related with the token anymore
			ut = t.getTokenUnit( dttu.getUnitID( ) );
			Assert.assertTrue( ut == null );

			u = Unit.getFromDatabase( dttu.getUnitID( ) );
			Assert.assertTrue( u == null ); // unit must also be deleted from units collection
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

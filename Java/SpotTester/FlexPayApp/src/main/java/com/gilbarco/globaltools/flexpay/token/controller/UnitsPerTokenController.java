/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.controller;

import java.util.Collection;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.flexpay.token.domain.Location;
import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.domain.TokenState;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;
import com.gilbarco.globaltools.flexpay.token.domain.UnitsPerToken;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenUnitTableRow;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;

/**
 * @author gparis
 * 
 */
public class UnitsPerTokenController
{
	// ******************************************************************
	// INNER TYPES
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( UnitsPerTokenController.class );

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

	/**
	 * @author gparis
	 * @since Jul 10 2012 Only one unit per token ( for simplified application )
	 * @param dttu
	 *            : data to populate the "database" collections
	 * @return boolean : true means new token was added, false means a pre-existent token was used to add the unit.
	 * @throws Exception
	 *             : When there is an attempt to add a pre-existent unit per token
	 */
	public static boolean addUnitAndToken(DataTransferForTokenUnitTableRow dttu, byte[] xmlBytes) throws Exception
	{
		Token t = new Token( dttu.getTokenCode( ), dttu.getStartTime( ), dttu.getEndTime( ) );
		t.setPermissions( dttu.getPermissions( ) );
		t.setRemoteMaintenance( dttu.getRemoteMaintenance( ) );
		t.setTechnicianDevice( dttu.getTechinicianDevice( ) );
		t.setOperatorID( dttu.getOperatorID( ) );
		t.setMaxDuration( dttu.getMaxDuration( ) );
		t.setTimes( dttu.getTimes( ) );

		t.setXmlBytes( xmlBytes );
		
		UnitsPerToken ut = null;
		Location l = new Location(TokenManagerModel.getDefaultOrganization(), TokenManagerModel.getDefaultLocality( ), TokenManagerModel.getDefaultState(), TokenManagerModel.getDefaultCountry( ));
		Unit u = new Unit( dttu.getPPN( ), Integer.parseInt( TokenManagerModel.getDefaultFuelPos( ) ), l );

		ut = new UnitsPerToken( t.getIdToken( ), u.getIdUnit( ) );
		t.addTokenUnit( ut );
		u.addUnitToken( ut );
		ut.setTokenState( TokenState.REQUESTED );

		Unit.addToDatabase( u );
		Token.addToDatabase( t );
		UnitsPerToken.addToDatabase( ut );

		// now everything is OK then let the GUI to keep track of the new record
		dttu.setIdToken( t.getIdToken( ) );
		dttu.setIdUnit( u.getIdUnit( ) );
		
		return true; // always create a new unit-token combination !
	}

	/**
	 * 
	 * Previous version prepared to add several units to the same token.
	 * 
	 * @deprecated : deprecated by now in new application behavior(single unit per token).
	 * @return boolean : true means new token was added, false means a pre-existent token was used to add the unit.
	 * @throws Exception
	 *             : When there is an attempt to add a pre-existent unit per token
	 */
	public static boolean addUnitAndToken_MultipleUnitsPerToken(DataTransferForTokenUnitTableRow dttu) throws Exception
	{
		boolean blNewToken = false;
		Token t = null;
		UnitsPerToken ut = null;
		Location l = new Location(TokenManagerModel.getDefaultOrganization(), TokenManagerModel.getDefaultLocality( ), TokenManagerModel.getDefaultState(), TokenManagerModel.getDefaultCountry( ));
		Unit u = new Unit( dttu.getPPN( ), Integer.parseInt( TokenManagerModel.getDefaultFuelPos( ) ), l );

		ut = UnitsPerToken.getFromDatabase( dttu.getTokenID( ), u.getIdUnit( ) );
		if (ut != null) // pre-existent unit per token
		{
			Exception e = new Exception( "pre-existent unit per token" );
			throw e; // never happens using current GUI, but possible by unit test.
		}

		t = Token.getFromDatabase( dttu.getTokenID( ) );
		if (t == null) // token inexistent, it must create one
		{
			// We're going to hijack the serial token Id in this method,
			// because at the moment we have only one token by using the current GUI
			TokenForAddingUnitPerTokenOperation tHack;
			tHack = new TokenForAddingUnitPerTokenOperation( dttu.getTokenCode( ), dttu.getStartTime( ), dttu.getEndTime( ) );
			tHack.setIdToken( dttu.getTokenID( ) );
			t = tHack;
			blNewToken = true;
		}

		ut = new UnitsPerToken( t.getIdToken( ), u.getIdUnit( ) );
		t.setTokenCode( dttu.getTokenCode( ) );
		t.addTokenUnit( ut );
		u.addUnitToken( ut );

		Unit.addToDatabase( u );
		if (blNewToken)
						Token.addToDatabase( t );

		UnitsPerToken.addToDatabase( ut );

		// now everything is OK then let the GUI to keep track of the new record
		dttu.setIdUnit( u.getIdUnit( ) );
		return blNewToken;
	}

	/**
	 * 
	 * @param DataTransferForTokenUnitTableRow
	 *            : dttu data to be updated in object's memory
	 * @return boolean : true means updated, false means unit-token not existent
	 */
	public static boolean updateUnitAndToken(DataTransferForTokenUnitTableRow dttu)
	{
		UnitsPerToken ut = UnitsPerToken.getFromDatabase( dttu.getTokenID( ), dttu.getUnitID( ) );
		if (ut == null)
						return false; // inexistent token-unit instance join in collections !

		Token t = Token.getFromDatabase( dttu.getTokenID( ) );
		if (t == null)
						return false; // inexistent token instance in collections !

		Unit u = Unit.getFromDatabase( dttu.getUnitID( ) );
		if (u == null)
						return false; // inexistent unit instance in collections !

		t.setTokenCode( dttu.getTokenCode( ) );
		t.setStartTime( dttu.getStartTime( ) ); 
		t.setEndTime( dttu.getEndTime( ) ); 
		t.setPermissions( dttu.getPermissions( ) );
		t.setRemoteMaintenance( dttu.getRemoteMaintenance( ) );
		t.setTechnicianDevice( dttu.getTechinicianDevice( ) );
		t.setOperatorID( dttu.getOperatorID( ) );
		t.setMaxDuration( dttu.getMaxDuration( ) );
		t.setTimes( dttu.getTimes( ) );
		
		ut.setTokenState( TokenState.valueOf( dttu.getTokenStatus( ) ) );
		u.setPPN( dttu.getPPN( ) );

		Location l = new Location(TokenManagerModel.getDefaultOrganization(), TokenManagerModel.getDefaultLocality( ), TokenManagerModel.getDefaultState(), TokenManagerModel.getDefaultCountry( ));
		u.setLocation( l );
		return true;
	}

	/**
	 * 
	 * @param int : idToken
	 * @param int : idUnit
	 * @return boolean : true means deleted, false means unit-token not existent.
	 */
	public static boolean deleteUnitPerToken(int idToken, int idUnit)
	{
		UnitsPerToken ut = UnitsPerToken.getFromDatabase( idToken, idUnit );

		if(ut == null)
			return false;

		Token t;
		Unit u;
		u = Unit.getFromDatabase( idUnit );
		u.deleteTokenUnit( idToken );
		t = Token.getFromDatabase( idToken );
		t.deleteTokenUnit( idUnit );
		
		UnitsPerToken.deleteFromDatabase( idToken, idUnit );
		Unit.deleteFromDatabase( idUnit );
		Token.deleteFromDatabase( idToken );

		return true;
	}

	/**
	 * 
	 * @param int : idToken
	 * @param int : idUnit
	 * @return UnitsPerToken : ut reference to memory database, null if unit-token is not existent.
	 */
	public static UnitsPerToken getUnitPerToken(int idToken, int idUnit)
	{
		return UnitsPerToken.getFromDatabase( idToken, idUnit );
	}

	/**
	 * 
	 * @return UnitsPerToken : all units per token in memory database.
	 */
	public static Collection<Map<Integer, UnitsPerToken>> getUnitsPerTokens()
	{
		return UnitsPerToken.getAllUnitsPerToken( );
	}

	/**
	 * @deprecated This family of "replace instance" methods are not used by now. They are reserved for future use.
	 * @param DataTransferForTokenUnitTableRow
	 *            : dttu data to be replaced in memory database
	 */
	public static boolean replaceUnitAndToken(DataTransferForTokenUnitTableRow dttu)
	{
		Token t = new Token( dttu.getTokenCode( ), dttu.getStartTime( ), dttu.getEndTime( ) );
		t.setPermissions( dttu.getPermissions( ) );
		t.setRemoteMaintenance( dttu.getRemoteMaintenance( ) );
		t.setTechnicianDevice( dttu.getTechinicianDevice( ) );
		t.setOperatorID( dttu.getOperatorID( ) );
		t.setMaxDuration( dttu.getMaxDuration( ) );
		t.setTimes( dttu.getTimes( ) );

		Location l = new Location(TokenManagerModel.getDefaultOrganization(), TokenManagerModel.getDefaultLocality( ), TokenManagerModel.getDefaultState(), TokenManagerModel.getDefaultCountry( ));
		Unit u = new Unit( dttu.getPPN( ), Integer.parseInt( TokenManagerModel.getDefaultFuelPos( ) ), l );
		UnitsPerToken ut = new UnitsPerToken( t.getIdToken( ), u.getIdUnit( ) );

		t.setStartTime( dttu.getStartTime( ) );

		try
		{
			UnitsPerToken.replaceInDatabase( ut );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ) );
			return false;
		}

		Token mt = Token.getFromDatabase( t.getIdToken( ) );
		Unit mu = Unit.getFromDatabase( u.getIdUnit( ) );

		if (mt == null || mu == null) // integrity broken, both objects must exist!!
										return false; // TODO: See to raise an exception

		t.setListOfUnitsPerToken( mt.getListOfUnitsPerToken( ) );
		u.setListOfTokens( mu.getListOfTokens( ) );
		Token.replaceInDatabase( t );
		Unit.replaceInDatabase( u );

		return true;
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

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

/**
 * 
 */

package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * @author stomasini 08/05/2012 domain class to represent Units per Token
 */

@SuppressWarnings("serial")
public class UnitsPerToken implements Serializable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	/**
	 * This is only an interface to access synchronously to the real static hash map.
	 * Key of Map is {@code TokenId} see {@link Token} class, Key of inner Map is {@code UnitId} see {@link Unit} class 
	 * 
	 */
	private static Map<Integer, Map<Integer, UnitsPerToken>> memoryDatabase = 
			Collections.synchronizedMap(new HashMap<Integer, Map<Integer, UnitsPerToken>>());

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	private int									idToken;
	private int									idUnit;
	private TokenState 							tokenState;


	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	/**
	 * @param newTokenUnit
	 *            Add the unit to the memory database
	 * @return boolean: true if new token is added, false if only a new unit inside a
	 *                  pre-existent token is added.
	 *                  A RuntimeException is raised if the unit-token combination
	 *                    already exists.
	 * @throws Exception 
	 */
	public static boolean addToDatabase(UnitsPerToken newTokenUnit) throws Exception
	{
		synchronized(memoryDatabase)
		{
			Integer xTokenId = Integer.valueOf(newTokenUnit.idToken);
			Integer yUnitId  = Integer.valueOf(newTokenUnit.idUnit);
			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null) // the token is a new token
			{
				mut = new HashMap<Integer, UnitsPerToken>();
				mut.put( yUnitId, newTokenUnit );
				memoryDatabase.put(xTokenId, mut);
				return true; // new token entry has been created
			}
			else // the token already exists
			{
				UnitsPerToken ut = mut.get( yUnitId );
				if(ut != null) // the unit also already exists into the pre-existing token
				{
					Exception e = new Exception("Unit-Token already exists!");
					throw e;
				}
				else // new unit into pre-existent token
				{
					mut.put( yUnitId, newTokenUnit );
					return false;
				}
			}
		}
	}

	/**
	 * @param int : idToken
	 * Deletes all the UnitsPerToken objects associated with a given tokenID
	 * from the memory database.
	 * @return boolean : true means all token-units deleted, false means token id not existent 
	 */
	public static boolean deleteFromDatabase(int idToken)
	{
		synchronized(memoryDatabase)
		{
			Integer xTokenId = Integer.valueOf( idToken);
			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null)
			{
				return false;
			}

			memoryDatabase.remove( xTokenId );
			return true;
		}
	}

	/**
	 * Deletes the specified UnitsPerToken object from the memory database.
	 * @param int : idToken
	 * @param int : idUnit
	 * @return boolean : true means deleted, false means unit-token not existent. 
	 */
	public static boolean deleteFromDatabase(int idToken, int idUnit)
	{
		synchronized(memoryDatabase)
		{
			Integer xTokenId = Integer.valueOf(idToken);
			Integer yUnitId  = Integer.valueOf(idUnit);
			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null)
			{
				return false; // nothing to do
			}

			UnitsPerToken upt = mut.remove( yUnitId );
			return (upt != null ? true : false);
		}
	}

	/**
	 * * Delegator method to deleteFromDatabase(int, String) one.
	 * Deletes the UnitsPerToken object from the memory database.
	 * @param UnitsPerToken : unitsPerToken
	 * Deletes the UnitsPerToken object from the memory database.
	 */
	public static void deleteFromDatabase(UnitsPerToken unitsPerToken)
	{
		deleteFromDatabase( unitsPerToken.getTokenID(), unitsPerToken.getIdUnit() );
	}


	/**
	 * Deletes the specified UnitsPerToken object from the memory database.
	 * @deprecated This family of "replace instance" methods are not used by now.
	 * @param updTokenUnit
	 *            Updated token-unit to update the memory database pre-existent entry
	 * @return void
	 * @throws Exception 
	 */
	public static void replaceInDatabase(UnitsPerToken updTokenUnit) throws Exception 
	{
		synchronized(memoryDatabase)
		{
			
			
			Integer xTokenId = Integer.valueOf(updTokenUnit.idToken);
			Integer yUnitId  = Integer.valueOf(updTokenUnit.idUnit);
			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null) // inexistent token being edited denotes inconsistency
			{
				Exception e = new Exception("Edited token doesn't exists!");
				throw e;
			}
			else
			{
				UnitsPerToken ut = mut.get( yUnitId );
				if(ut == null) // inexistent unit being edited denotes inconsistency
				{
					Exception e = new Exception("Edited token-unit doesn't exists!");
					throw e;
				}
				else // perform changes to the instance in the database
				{
					mut.put( yUnitId, updTokenUnit );
				}
			}
		}
	}


	/**
	 * Gets the UnitsPerToken object from the memory database
	 * @param int : idToken
	 * @param int : idUnit
	 * @return UnitsPerToken : A reference to the requested object in memory database,
	 * or null if the object key is not found.
	 */
	public static UnitsPerToken getFromDatabase(int idToken, int idUnit)
	{
		UnitsPerToken ut = null;

		synchronized(memoryDatabase)
		{
			Integer xTokenId = Integer.valueOf(idToken);
			Integer yUnitId  = Integer.valueOf(idUnit);

			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null)
				return null; // nothing in database

			ut = mut.get( yUnitId );
		}

		return ut;
	}

/*
	public static String[] getUnitListAsString(int idToken)
	{
		int i = 0;

		synchronized(memoryDatabase)
		{
			Integer xTokenId = new Integer(idToken);
			Map<Integer, UnitsPerToken> mut = memoryDatabase.get( xTokenId );
			if(mut == null)
				return new String[1]; // nothing to do

			String[] theReturnedStringList = new String[mut.size( )];

			// memoryDatabase can be viewed as an iterable collection of UnitsPerToken objects
			Collection<UnitsPerToken> coll = mut.values( );

			for (UnitsPerToken nextUnit : coll)
			{
				// TODO TO REMOVE this not-normalized UnitsPerToken table behavior
				theReturnedStringList[i] = nextUnit.getSiteName( ) + "," + nextUnit.getLocation( ) + ",FP "
								+ nextUnit.getFuelingPosition( );

				/* This is the correct way according to a normalized table
				 * TODO Future implementation
				 */
				//theReturnedList[i] = nextUnit.getTokenCode( ) + "," + nextUnit.getIdUnit( ) +
				//						nextUnit.getState();
/*
				i++;
			}

			return theReturnedStringList;
		}
	}
*/

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public UnitsPerToken(int idToken, int idUnit)
	{
		this.idToken = idToken;
		this.idUnit = idUnit;
		this.tokenState = TokenState.REQUESTED;
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public int getTokenID()
	{
		return idToken;
	}

	public int getIdUnit()
	{
		return idUnit;
	}

	public TokenState getTokenState()
	{
		return tokenState;
	}

	public void setTokenState(TokenState state)
	{
		this.tokenState = state;
	}

	public static Collection<Map<Integer, UnitsPerToken>> getAllUnitsPerToken()
	{
		return memoryDatabase.values( );
		
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
}


package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

import com.gilbarco.globaltools.flexpay.token.domain.Location;
import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;
import com.gilbarco.globaltools.flexpay.token.domain.UnitsPerToken;

/**
 * @author mvolker
 * @version 1.0
 * @created 17-May-2012 04:51:26 p.m.
 */
public class Unit implements Serializable
{
	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************

	private static AtomicInteger		idGenerator		= new AtomicInteger( 1 );

	/**
	 * This is only an <code>interface</code> to access synchronously to the real static hash map.
	 */
	private static Map<Integer, Unit>	memoryDatabase	= Collections.synchronizedMap( new HashMap<Integer, Unit>( ) );

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected int						idUnit;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	private String						iProductPartNumber;
	private int							iFuelingPosition;
	private String						strIPAddress;
	private ArrayList<UnitsPerToken>	listOfTokensPerUnit;
	private Location					location;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	/**
	 * Adds the new unit to the memory database.
	 * 
	 * @param Token
	 *            : newUnit
	 * @return boolean : true means added, false means a pre-existent unit
	 */
	public static boolean addToDatabase(Unit newUnit)
	{
		Unit mu = null;

		synchronized (memoryDatabase)
		{
			mu = memoryDatabase.get( newUnit.getIdUnit( ) );
			if (mu != null)
				return false;
			else
				memoryDatabase.put( newUnit.getIdUnit( ), newUnit );
		}

		return true;
	}

	/**
	 * Deletes the unit from the memory database.
	 * 
	 * @param int : idUnit
	 * @return boolean : true means deleted, false means a non-existent unit.
	 */
	public static boolean deleteFromDatabase(int idUnit)
	{
		Unit mu = null;

		synchronized (memoryDatabase)
		{
			mu = memoryDatabase.remove( idUnit );
		}

		return (mu != null ? true : false);
	}

	/**
	 * Replaces the unit in the memory database.
	 * 
	 * @deprecated This family of "replace instance" methods are not used by now. They are reserved for future use.
	 * @param Unit
	 *            : u (supposed to be existent)
	 * @return boolean : true means replaced, false means a non-existent unit.
	 */
	public static boolean replaceInDatabase(Unit u)
	{
		Unit mu = null;

		synchronized (memoryDatabase)
		{
			mu = memoryDatabase.get( u.getIdUnit( ) );
			if (mu == null)
				return false;
			else
				memoryDatabase.put( u.getIdUnit( ), u );
		}

		return true;
	}

	/**
	 * Gets the unit from the memory database
	 * 
	 * @param int : idUnit
	 * @return Unit : A reference to the requested unit in memory database, or null if idUnit is not found.
	 * 
	 */
	public static Unit getFromDatabase(int idUnit)
	{
		Unit u = null;

		synchronized (memoryDatabase)
		{
			u = memoryDatabase.get( idUnit );
		}

		return u;
	}

	/**
	 * Gets the unit from the memory database
	 * 
	 * @param String
	 *            : sPartNumberCode
	 * @return Unit : A reference to the requested unit in memory database, or null if sPartNumberCode is not found.
	 * 
	 */
	public static Unit getFromDatabase(String sPartNumberCode)
	{
		synchronized (memoryDatabase)
		{
			Collection<Unit> coll = memoryDatabase.values( );
			for (Unit u : coll)
				if (u.iProductPartNumber.equals( sPartNumberCode) ) return u;
		}

		return null;
	}

	public static String[] getListAsString()
	{
		synchronized (memoryDatabase)
		{
			int i = 0;
			String[] theReturnedStringList = new String[memoryDatabase.size( )];

			// memoryDatabase can be viewed as an iterable collection of UnitsPerToken objects
			Collection<Unit> coll = memoryDatabase.values( );

			for (Unit nextUnit : coll)
			{
				theReturnedStringList[i++] = nextUnit.location.getSiteName( ) + ","
												+ nextUnit.location.getCity( ) + ",FP "
												+ nextUnit.iFuelingPosition;
			}

			return theReturnedStringList;
		}
	}

	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	public Unit(String iPPN, int iFuelPos)
	{
		this( iPPN, iFuelPos, null );
	}

	public Unit(String iPPN, int iFuelPos, Location l)
	{
		idUnit = Unit.idGenerator.getAndIncrement( );
		iProductPartNumber = iPPN;
		iFuelingPosition = iFuelPos;
		listOfTokensPerUnit = null;
		location = l;
	}

	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	public void finalize() throws Throwable
	{
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************

	public final int getIdUnit()
	{
		return idUnit;
	}

	public final String getPPN()
	{
		return iProductPartNumber;
	}

	public final void setPPN(String ppn)
	{
		this.iProductPartNumber = ppn;
	}

	/**
	 * @return the iFuelingPosition
	 */
	protected int getFuelingPosition()
	{
		return iFuelingPosition;
	}

	/**
	 * @param iFuelingPosition
	 *            the iFuelingPosition to set
	 */
	protected void setFuelingPosition(int iFuelingPosition)
	{
		this.iFuelingPosition = iFuelingPosition;
	}

	/**
	 * @return the strIPAddress
	 */
	public String getIPAddress()
	{
		return strIPAddress;
	}

	/**
	 * @param strIPAddress
	 *            the strIPAddress to set
	 */
	public void setIPAddress(String sIPAddress)
	{
		if (sIPAddress != null)
		{
			strIPAddress = new String( sIPAddress );
		}
	}

	public final ArrayList<UnitsPerToken> getListOfTokens()
	{
		return listOfTokensPerUnit;
	}

	public final void setListOfTokens(ArrayList<UnitsPerToken> listOfUnits)
	{
		this.listOfTokensPerUnit = listOfUnits;
	}

	public final Location getLocation()
	{
		return location;
	}

	public final void setLocation(Location location)
	{
		this.location = location;
	}

	public void addUnitToken(UnitsPerToken ut)
	{
		if (listOfTokensPerUnit == null) listOfTokensPerUnit = new ArrayList<UnitsPerToken>( );

		listOfTokensPerUnit.add( ut );
	}

	public boolean replaceUnitToken(UnitsPerToken ut)
	{
		if (listOfTokensPerUnit == null) return false;

		for (UnitsPerToken lut : listOfTokensPerUnit)
			if (lut.getTokenID( ) == ut.getTokenID( )) // token ID found
			{
				int idx = listOfTokensPerUnit.indexOf( lut );
				listOfTokensPerUnit.set( idx, ut ); // ut now replaces old lut
				return true;
			}

		return false;
	}

	public boolean deleteTokenUnit(int idToken)
	{
		for (UnitsPerToken lut : listOfTokensPerUnit)
			if (lut.getTokenID( ) == idToken) // token ID found
			{
				int idx = listOfTokensPerUnit.indexOf( lut );
				listOfTokensPerUnit.remove( idx );
				return true;
			}

		return false;
	}

	public UnitsPerToken getUnitToken(int idToken)
	{
		for (UnitsPerToken lut : listOfTokensPerUnit)
			if (lut.getTokenID( ) == idToken) // unit ID found
			{
				return lut;
			}

		return null;
	}
}

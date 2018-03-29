/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

import com.gilbarco.globaltools.flexpay.token.domain.Token;
import com.gilbarco.globaltools.flexpay.token.domain.UnitsPerToken;

/**
 * @author mvolker
 * @version 1.0
 * @created 17-May-2012 04:51:26 p.m.
 */
public class Token implements Serializable
{
	private static final String	FIRMWARE_UPDATE_PERMISSION	= "FirmwareUpdate";

	/**
	 * 
	 */
	private static final long			serialVersionUID	= -1046613718528076618L;

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static AtomicInteger		idGenerator			= new AtomicInteger( 1 );

	/**
	 * This is only a static interface to access synchronously the real hash map
	 */
	private static Map<Integer, Token>	memoryDatabase		= Collections.synchronizedMap( new HashMap<Integer, Token>( ) );

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected int						idToken;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private String						tokenCode;
	private String						remoteMaintenance;
	private String						permissions;																			// TODO temporary String
	private String						technicianDevice;
	private String						operatorID;
	private String						maxDuration;
	private String						startTime;
	private String						endTime;
	private String						times;
	private String						tokenVersion;

	private ArrayList<UnitsPerToken>	listOfUnitsPerToken;
	private byte[]						xmlBytes;

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	/**
	 * Adds the token to the memory database.
	 * 
	 * @param Token
	 *            : newToken
	 * @return boolean : true means added, false means a pre-existent token.
	 */
	public static boolean addToDatabase(Token newToken)
	{
		Token mt = null;

		synchronized (memoryDatabase)
		{
			mt = memoryDatabase.get( newToken.getIdToken( ) );
			if (mt != null)
				return false;
			else
				memoryDatabase.put( newToken.getIdToken( ), newToken );
		}

		return true;
	}

	/**
	 * Deletes the token from the memory database
	 * 
	 * @param int : idToken
	 * @return boolean : true means deleted, false means a non-existent token.
	 */
	public static boolean deleteFromDatabase(int idToken)
	{
		Token mt = null;

		synchronized (memoryDatabase)
		{
			mt = memoryDatabase.remove( idToken );
		}

		return (mt != null ? true : false);
	}

	/**
	 * Replaces the token in the memory database.
	 * 
	 * @deprecated This family of "replace instance" methods are not used by now. They are reserved for future use.
	 * @param Token
	 *            : t (supposed to be existent)
	 * @return boolean : true means replaced, false means a non-existent token.
	 */
	public static boolean replaceInDatabase(Token t)
	{
		Token mt = null;

		synchronized (memoryDatabase)
		{
			mt = memoryDatabase.get( t.getIdToken( ) );
			if (mt == null)
				return false;
			else
				memoryDatabase.put( t.getIdToken( ), t );
		}

		return true;
	}

	/**
	 * Gets the token from the memory database
	 * 
	 * @param int : idToken
	 * @return Token : A reference to the requested token in memory database, or null if idToken is not found.
	 * 
	 */
	public static Token getFromDatabase(int idToken)
	{
		Token t = null;

		synchronized (memoryDatabase)
		{
			t = memoryDatabase.get( idToken );
		}

		return t;
	}

	/**
	 * Gets the token from the memory database
	 * 
	 * @param String
	 *            : sTokenCode
	 * @return Token : A reference to the requested token in memory database, or null if sTokenCode is not found.
	 * 
	 */
	public static Token getFromDatabase(String sTokenCode)
	{
		synchronized (memoryDatabase)
		{
			Collection<Token> coll = memoryDatabase.values( );
			for (Token t : coll)
				if (t.tokenCode.equalsIgnoreCase( sTokenCode ))
														return t;
		}

		return null;
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public Token(String sCode, String sStartTime, String sEndTime)
	{
		idToken = Token.idGenerator.getAndIncrement( );
		tokenCode = sCode;
		startTime = sStartTime;
		endTime = sEndTime;						
		remoteMaintenance = null;
		permissions = null;
		technicianDevice = null;
		operatorID = null;

		listOfUnitsPerToken = new ArrayList<UnitsPerToken>( );

		xmlBytes = null;
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

	public final int getIdToken()
	{
		return idToken;
	}

	public final String getTokenCode()
	{
		return tokenCode;
	}

	public final void setTokenCode(String sTokenCode)
	{
		tokenCode = sTokenCode;
	}

	public final String getStartTime()
	{
		return startTime;
	}

	public final void setStartTime(String sStartingDateTime)
	{
		startTime = new String( sStartingDateTime );
	}

	public void setEndTime(String sEndTime)
	{
		this.endTime = sEndTime;		
	}

	public String getEndTime()
	{
		return this.endTime;		
	}

	public final ArrayList<UnitsPerToken> getListOfUnitsPerToken()
	{
		return listOfUnitsPerToken;
	}

	public final void setListOfUnitsPerToken(ArrayList<UnitsPerToken> listOfUnitsPerToken)
	{
		this.listOfUnitsPerToken = listOfUnitsPerToken;
	}

	public final byte[] getXmlBytes()
	{
		return xmlBytes;
	}

	public final void setXmlBytes(byte[] xmlBytes)
	{
		this.xmlBytes = xmlBytes;
	}

	public final void addTokenUnit(UnitsPerToken ut)
	{
		listOfUnitsPerToken.add( ut );
	}

	public UnitsPerToken getTokenUnit(int idUnit)
	{
		for (UnitsPerToken lut : listOfUnitsPerToken)
			if (lut.getIdUnit( ) == idUnit) // unit ID found
			{
				return lut;
			}

		return null;
	}

	public boolean replaceTokenUnit(UnitsPerToken ut)
	{
		for (UnitsPerToken lut : listOfUnitsPerToken)
			if (lut.getIdUnit( ) == ut.getIdUnit( )) // unit ID found
			{
				int idx = listOfUnitsPerToken.indexOf( lut );
				listOfUnitsPerToken.set( idx, ut ); // ut now replaces old lut
				return true;
			}

		return false;
	}

	public boolean deleteTokenUnit(int idUnit)
	{
		for (UnitsPerToken lut : listOfUnitsPerToken)
			if (lut.getIdUnit( ) == idUnit) // unit ID found
			{
				int idx = listOfUnitsPerToken.indexOf( lut );
				listOfUnitsPerToken.remove( idx );
				return true;
			}

		return false;
	}

	public static boolean isTokenCodeDigitString(String text)
	{
		for (int i = 0; i < text.length( ); ++i)
		{
			if (!isTokenCodeDigit( text.charAt( i ) ))
			{
				return false;
			}
		}
		return true;
	}

	public static boolean isTokenCodeDigit(char code)
	{
		return (code >= 'A' && code <= 'Z') || (code >= '2' && code <= '7');
	}

	// Helper functions

	protected static String getFirstWord(String strPhrase)
	{
		int iSeparator = strPhrase.indexOf( ' ' );
		if (iSeparator < 0)
			return strPhrase;
		else
			return strPhrase.substring( 0, iSeparator );
	}

	protected static String getLastPortion(String strPhrase)
	{
		int iSeparator = strPhrase.indexOf( ' ' );
		if (iSeparator < 0)
			return "";
		else
			return strPhrase.substring( 1 + iSeparator, strPhrase.length( ) );
	}

	public String getRemoteMaintenance()
	{
		return remoteMaintenance;
	}

	public void setRemoteMaintenance(String remoteMaintenance)
	{
		this.remoteMaintenance = remoteMaintenance;
	}

	public String getPermissions()
	{
		return permissions;
	}

	public void setPermissions(String permissions)
	{
		this.permissions = permissions;
	}

	public String getTechnicianDevice()
	{
		return technicianDevice;
	}

	public void setTechnicianDevice(String technicianDevice)
	{
		this.technicianDevice = technicianDevice;
	}

	public String getOperatorID()
	{
		return operatorID;
	}

	public void setOperatorID(String operatorID)
	{
		this.operatorID = operatorID;
	}

	public String getMaxDuration()
	{
		return maxDuration;
	}

	public void setMaxDuration(String maxDuration)
	{
		this.maxDuration = maxDuration;
	}

	public String getTimes()
	{
		return times;
	}

	public void setTimes(String times)
	{
		this.times = times;
	}

	public String getTokenVersion()
	{
		return tokenVersion;
	}

	public void setTokenVersion(String tokenVersion)
	{
		this.tokenVersion = tokenVersion;
	}

	 /**
     * Determine if the selected token has permissions to update software unit. <p>
     * @return true if selected has that permissions, false otherwise.
     */	
	public static boolean hasSoftwareUpdatesPermissions(Token token)
	{		
		String permissions = token.getPermissions( );
		return permissions.contains(FIRMWARE_UPDATE_PERMISSION);
	}
}

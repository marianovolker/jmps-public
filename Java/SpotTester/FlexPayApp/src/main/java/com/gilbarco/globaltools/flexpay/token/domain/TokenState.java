package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;

import com.gilbarco.globaltools.flexpay.token.domain.StateConstants;
import com.gilbarco.globaltools.flexpay.token.domain.TokenState;


/**
 * This is a helper class holding constants used in the enumeration below.
 * @author gparis
 */
class StateConstants
{
	public static int	MAX_STATE_ID	= 3;
}


/**
 * This enumeration models the different states the token entity has, along its whole life.
 * @author gparis
 */
public enum TokenState implements Serializable
{
	// enumeration's object values
	VOID(0),
	REQUESTED(1),
	STORED(2),
	PRESENTED(StateConstants.MAX_STATE_ID);


	private final static int	MAXID		= StateConstants.MAX_STATE_ID;
	private final static char	acStates[]	=
		{ 'V', 'C', 'U', 'T', 'A', 'S', 'P' };
	private final static String	asStates[]	=
		{ "Void", "Requested", "Unit assigned", "Token assigned", "Authorized", "Stored", "Presented" };
	private final static String	asClues[]	=
		{ "void", "requested", "unit", "token","authorized", "stored", "presented" };

	public static int guessId(String strTokenState)
	{
		String s = new String( strTokenState.toLowerCase( ) );
		for (int i = 0; i <= MAXID; i++)
			if (s.contains( asClues[i] ))
			{
				return i;
			}

		return 0;
	}

	public static TokenState intToTokenState(int i)
	{
		switch(i)
		{
			case 0 :
				return VOID;
			case 1 :
				return REQUESTED;
			case 2 :
				return STORED;
			case 3 :
				return PRESENTED;
			default :
				return VOID;
		}
	}


	// the only field of this enumeration
	private int	id	= 0;

	// Enumeration constructor
	TokenState(int i)
	{
		fromInt( i );
	}

	// Helper conversion methods

	public void fromInt(int i)
	{
		if (i < 0 || i > MAXID)
			id = 0;
		else
			id = i;
	}

	public void fromString(String strTokenState)
	{
		fromInt( guessId(strTokenState) );
	}

	public int toInt()
	{
		return id;
	}

	public char toChar()
	{
		return acStates[id];
	}

	@Override
	public String toString()
	{
		return asStates[id];
	}

} // end of enumeration class



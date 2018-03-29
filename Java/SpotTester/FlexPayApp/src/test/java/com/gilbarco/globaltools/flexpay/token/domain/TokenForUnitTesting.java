/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.domain;

import com.gilbarco.globaltools.flexpay.token.domain.Token;



/**
 * @author gparis
 * This class is intended to be created ONLY for unit testing of domain classes.
 * Thus it is located in com.gilbarco.FlexPay.domain package.
 * This class allow to hijack the auto-generated idToken field,
 * by writing it using the setIdToken() method that is unavailable
 * in Token class.
 */
public class TokenForUnitTesting extends Token
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
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * 
	 */
	private static final long	serialVersionUID	= -4800174575477522944L;

	/**
	 * @param sCode
	 * @param sType
	 * @param sStartDate
	 */
	protected TokenForUnitTesting(String sCode, String sStartDate, String sEndSate)
	{
		super( sCode, sStartDate, sEndSate );
	}


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void setIdToken(int idToken)
	{
		this.idToken = idToken;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

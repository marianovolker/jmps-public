/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.controller;

import com.gilbarco.globaltools.flexpay.token.domain.Token;



/**
 * @author gparis
 * This class is intended to be created ONLY BY a CONTROLLER class.
 * Thus it is located in com.gilbarco.FlexPay.controller package.
 * This class allow to hijack the auto-generated idToken field,
 * by writing it using the setIdToken() method that is unavailable
 * in Token class.
 */
public class TokenForAddingUnitPerTokenOperation extends Token
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
	 * @param sCode
	 * @param sType
	 * @param sExpDate
	 */
	protected TokenForAddingUnitPerTokenOperation(String sCode, String sStartDate, String sEndDate)
	{
		super( sCode, sStartDate, sEndDate );
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

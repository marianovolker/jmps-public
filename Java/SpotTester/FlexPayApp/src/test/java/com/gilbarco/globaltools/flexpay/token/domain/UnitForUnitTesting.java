/**
 * 
 */
package com.gilbarco.globaltools.flexpay.token.domain;

import com.gilbarco.globaltools.flexpay.token.domain.Location;
import com.gilbarco.globaltools.flexpay.token.domain.Unit;


/**
 * @author gparis
 * This class is intended to be created ONLY for unit testing of domain classes.
 * Thus it is located in com.gilbarco.FlexPay.domain package.
 * This class allow to hijack the auto-generated idUnit field,
 * by writing it using the setIdUnit() method that is unavailable
 * in Unit class.
 */
public class UnitForUnitTesting extends Unit
{
	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS 
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
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * 
	 */
	private static final long	serialVersionUID	= 8808555710774310001L;


	protected UnitForUnitTesting(String sPPN, int iFuelPos)
	{
		super( sPPN, iFuelPos );
	}

	protected UnitForUnitTesting(String sPPN, int iFuelPos, Location l)
	{
		super( sPPN, iFuelPos, l );
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
	protected void setIdUnit(int idUnit)
	{
		this.idUnit = idUnit;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

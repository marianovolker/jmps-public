
package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;
import java.util.concurrent.atomic.AtomicInteger;

import com.gilbarco.globaltools.flexpay.token.domain.Location;

/**
 * @author mvolker
 * @version 1.0
 * @created 17-May-2012 04:51:27 p.m.
 */
public class Location implements Serializable
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static AtomicInteger		idGenerator = new AtomicInteger(1);

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************

	// TODO Use these "coded" fields when application data model be mature
	/*
	private int	City;
	private int	Country;
	private int	IdLocation;
	private int	State;
	*/

	// At the moment we use these "not-normalized" fields
	private int		idLocation;
	private String	strSiteName;
	private String	strCity;
	private String	strCountry;
	private String	strState;


	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public Location(String sName, String sCity, String sState, String sCountry)
	{
		idLocation = Location.idGenerator.getAndIncrement( );
		strSiteName = new String(sName);
		strCity = new String(sCity);
		strState = new String(sState);
		strCountry = new String(sCountry);
	}


	// ******************************************************************
	// OVERRIDDEN METHODS (invoked from polymorphic interface).
	// ******************************************************************

	public void finalize() throws Throwable
	{
	}


	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	// At the moment we use these "not-normalized" getters/setters

	public final int getIdLocation()
	{
		return idLocation;
	}

	/**
	 * @return the strSiteName
	 */
	protected String getSiteName()
	{
		return strSiteName;
	}


	/**
	 * @param strSiteName the strSiteName to set
	 */
	protected void setSiteName(String strSiteName)
	{
		this.strSiteName = strSiteName;
	}


	public final String getCity()
	{
		return strCity;
	}

	public final void setCity(String sCity)
	{
		strCity = sCity;
	}

	public final String getCountry()
	{
		return strCountry;
	}

	public final void setCountry(String sCountry)
	{
		strCountry = sCountry;
	}

	public final String getState()
	{
		return strState;
	}

	public final void setState(String sState)
	{
		strState = sState;
	}

	// TODO Use these "coded" getters/setters when application data model be mature
	/*
	public final int getCity()
	{
		return City;
	}

	public final void setCity(int city)
	{
		City = city;
	}

	public final int getCountry()
	{
		return Country;
	}

	public final void setCountry(int country)
	{
		Country = country;
	}

	public final int getIdLocation()
	{
		return IdLocation;
	}

	public final void setIdLocation(int idLocation)
	{
		IdLocation = idLocation;
	}

	public final int getState()
	{
		return State;
	}

	public final void setState(int state)
	{
		State = state;
	}
	*/
}

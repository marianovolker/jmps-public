
package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;

import com.gilbarco.globaltools.flexpay.token.domain.Technician;

/**
 * @author mvolker
 * @version 1.0
 * @created 17-May-2012 04:51:26 p.m.
 */
public class Certificate implements Serializable
{
	private char		file;
	private int			IdCertificate;
	private int			state;
	private Technician	oTechnician;
	
	public Certificate()
	{
	}

	public void finalize() throws Throwable
	{
	}	

	/**
	 * @return the file
	 */
	public final char getFile()
	{
		return file;
	}

	/**
	 * @param file the file to set
	 */
	public final void setFile(char file)
	{
		this.file = file;
	}

	/**
	 * @return the idCertificate
	 */
	public final int getIdCertificate()
	{
		return IdCertificate;
	}

	/**
	 * @param idCertificate the idCertificate to set
	 */
	public final void setIdCertificate(int idCertificate)
	{
		IdCertificate = idCertificate;
	}

	/**
	 * @return the state
	 */
	public final int getState()
	{
		return state;
	}

	/**
	 * @param state the state to set
	 */
	public final void setState(int state)
	{
		this.state = state;
	}

	/**
	 * @return the oTechnician
	 */
	public final Technician getoTechnician()
	{
		return oTechnician;
	}

	/**
	 * @param oTechnician the oTechnician to set
	 */
	public final void setoTechnician(Technician oTechnician)
	{
		this.oTechnician = oTechnician;
	}
}

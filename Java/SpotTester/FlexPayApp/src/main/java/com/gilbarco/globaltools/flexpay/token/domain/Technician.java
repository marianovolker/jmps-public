
package com.gilbarco.globaltools.flexpay.token.domain;

import java.io.Serializable;

import com.gilbarco.globaltools.flexpay.token.domain.Certificate;

/**
 * @author mvolker
 * @version 1.0
 * @created 17-May-2012 04:51:25 p.m.
 */
public class Technician implements Serializable
{
	private int			IdTechnician;
	private char		Nombre;
	private int			TechnicianCode;
	private Certificate	oCertificate;
	
	public Technician()
	{
	}

	public void finalize() throws Throwable
	{
	}	

	/**
	 * @return the idTechnician
	 */
	public final int getIdTechnician()
	{
		return IdTechnician;
	}

	/**
	 * @param idTechnician the idTechnician to set
	 */
	public final void setIdTechnician(int idTechnician)
	{
		IdTechnician = idTechnician;
	}

	/**
	 * @return the nombre
	 */
	public final char getNombre()
	{
		return Nombre;
	}

	/**
	 * @param nombre the nombre to set
	 */
	public final void setNombre(char nombre)
	{
		Nombre = nombre;
	}

	/**
	 * @return the technicianCode
	 */
	public final int getTechnicianCode()
	{
		return TechnicianCode;
	}

	/**
	 * @param technicianCode the technicianCode to set
	 */
	public final void setTechnicianCode(int technicianCode)
	{
		TechnicianCode = technicianCode;
	}

	/**
	 * @return the oCertificate
	 */
	public final Certificate getoCertificate()
	{
		return oCertificate;
	}

	/**
	 * @param oCertificate the oCertificate to set
	 */
	public final void setoCertificate(Certificate oCertificate)
	{
		this.oCertificate = oCertificate;
	}

}

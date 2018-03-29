/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.engines;

/**
 * @author gparis
 *
 */
public class UploadNotification
{
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
	// says if there have been any error till now
	private boolean blError = false;
	
	// total number of files to upload.
	private int iTotalFiles = 0;

	// partial number of files that have already been updated.
	private int iUploaded = 0;


	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	public UploadNotification(boolean bl, int ntot, int nparc)
	{
		blError = bl;
		iTotalFiles = ntot;
		iUploaded = nparc;
	}


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	/**
	 * @return false if there have been errors
	 */
	public boolean hasErrors() 
	{
		return blError;
	}

	/**
	 * @return the iTotalFiles
	 */
	public int getTotal()
	{
		return iTotalFiles;
	}

	/**
	 * @return the iUploaded
	 */
	public int getUploaded()
	{
		return iUploaded;
	}



	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// STATIC METHODS 
	// ******************************************************************
}



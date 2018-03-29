/**
 * 
 */
package com.gilbarco.globaltools.flexpay.maintenance.romfs;

import java.io.File;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.flexpay.maintenance.resources.DownloadableFile;


/**
 * It extends the File functionality to a generic (M3/M5 oriented) ROMfs package file, "having" a
 * File reference inside (not using inheritance).
 * 
 * @author gparis
 *
 */
public class ROMfsPackage extends DownloadableFile
{

	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( ROMfsPackage.class );

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	protected ROMfsCertificate	certificate = null;
	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private int					nBlock = 0;


	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * @param pathname
	 */
	public ROMfsPackage(String pathname)
	{
		super(pathname);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public ROMfsPackage(String parent, String child)
	{
		super(parent, child);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public ROMfsPackage(File parent, String child)
	{
		super(parent, child);
	}

	/**
	 * @param file : a pre-existent file object
	 */
	public ROMfsPackage(File file)
	{
		super(file);
	}


	public ROMfsPackage(ROMfsCertificate cert)
	{
		super( cert.getFile() );
		certificate = cert;
	}


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	public long length()
	{
		if(file == null)
			return -1;

		return file.length();
	}


	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

	public byte[] getNextBlock(int iBlockSize)
	{
		if( eof() )
			return null;

		return getDataBlock(nBlock++ * iBlockSize, iBlockSize);
	}

	/**
	 * @return the ROMfs certificate size
	 */
	public int getCertificateSize()
	{
		if(certificate == null)
			return 0;

		return certificate.getCertificateSize();
	}

	/**
	 * @return the ROMfs certificate data
	 */
	public byte[] getCertificateData()
	{
		if(certificate == null)
			return null;

		return certificate.getCertificateData();
	}

	public byte[] getSignatureData()
	{
		if(certificate == null)
			return null;

		return certificate.getSignatureData();
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

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}

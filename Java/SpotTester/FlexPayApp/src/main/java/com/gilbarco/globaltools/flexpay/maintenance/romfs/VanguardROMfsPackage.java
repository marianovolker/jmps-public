/**
 * 
 */
package com.gilbarco.globaltools.flexpay.maintenance.romfs;

import java.io.File;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.flexpay.maintenance.resources.DownloadableFile;


/**
 * It extends the ROMfs package functionality to a Vanguard ROMfs package.
 * 
 * @author gparis
 *
 */
public class VanguardROMfsPackage extends ROMfsPackage
{

	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( VanguardROMfsPackage.class );

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
	 * @param pathname
	 */
	public VanguardROMfsPackage(String pathname)
	{
		super(pathname);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public VanguardROMfsPackage(String parent, String child)
	{
		super(parent, child);
	}

	/**
	 * @param parent
	 * @param child
	 */
	public VanguardROMfsPackage(File parent, String child)
	{
		super(parent, child);
	}

	/**
	 * @param file : a pre-existent file object
	 */
	public VanguardROMfsPackage(File file)
	{
		super(file);
	}


	public VanguardROMfsPackage(ROMfsCertificate cert)
	{
		super( cert );
	}


	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	@Override
	public long length()
	{
		if(file == null)
			return -1;

		if(certificate == null)
			return file.length();

		return file.length() - certificate.getCertificateSize() - certificate.getSignatureSize();
	}


	// ******************************************************************
	// PUBLIC METHODS	  (general, getter, setter, interface imp)
	// ******************************************************************

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

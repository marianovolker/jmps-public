/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file VanguardROMfsCertificate.java
 * @copyright © 2012, 2013 Gilbarco Inc. Confidential and Propietary
 *
 */

package com.gilbarco.globaltools.flexpay.maintenance.romfs;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.Formatter;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * The Class VanguardROMfsCertificate.
 *
 * @class
 * @author mvolker
 */
public class VanguardROMfsCertificate extends ROMfsCertificate
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The Constant logger. */
	private static final transient Log	logger	= LogFactory.getLog( VanguardROMfsCertificate.class );


	/** The Constants for each certificate field data*/
	private static final String			TYPE_KEY					= "TYPE";
	private static final String			PCI_KEY						= "PCI";
	private static final String			FILE_ID_KEY					= "FILE_ID";
	private static final String			VENDOR_NAME_KEY				= "VENDOR_NAME";
	private static final String			PRODUCT_BOARD_NAME_KEY		= "PRODUCT_BOARD_NAME";
	private static final String			APPLICATION_NAME_KEY		= "APPLICATION_NAME";
	private static final String			APPLICATION_VERSION_KEY		= "APPLICATION_VERSION";
	private static final String			DATE_AND_TIME_KEY			= "DATE_AND_TIME";
	private static final String			HARDWARE_COMPATIBILITY_KEY	= "HARDWARE_COMPATIBILITY";

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	/** The start certif label. */
	private byte[]	startCertifLabel;

	/** The file type field. */
	private byte[]	fileType;

	/** The pci file field. */
	private byte[]	pciValue;

	/** The hardware compatibility file field. */
	protected byte[] hardwareCompatibility = null;


	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * Instantiates a new vanguard file certificate.
	 *
	 * @param file the file
	 */
	public VanguardROMfsCertificate ( File file )
	{
		super( file );
	}

	/**
	 * Instantiates a new vanguard file certificate.
	 *
	 * @param certificate the certificate
	 */
	public VanguardROMfsCertificate ( byte[] certificate )
	{
		super( certificate );
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface impl)
	// ******************************************************************

	/**
	 * Gets the hardware compatibility.
	 *
	 * @return the hardware compatibility file field.
	 */
	public byte[] getHardwareCompatibility()
	{
		return hardwareCompatibility;
	}


	// ******************************************************************
	// OVERRIDDEN METHODS.
	// ******************************************************************


	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate#calculateCertificateAndSignaturePosition(java.io.FileInputStream)
	 */
	@Override
	protected void calculateCertificateAndSignaturePosition(FileInputStream in) throws IOException
	{
		int iMSB = 0, iLSB = 0;
		int fileSize = (int) getFile().length();
		iSignatureLength = ROMFS_SIGNATURE_FIXEDSIZE;
		iSignatureOffset = fileSize - iSignatureLength;

		if(in.getChannel().position() > 0)
		{
			in.getChannel().position(0); // file must be scanned from the beginning
		}

		// reads the certificate length
		in.skip(iSignatureOffset - 2); // it positions over the certificate length field
		iMSB = in.read();
		if( iMSB < 0 ) iMSB += 256;
		iLSB = in.read();
		if( iLSB < 0 ) iLSB += 256;

		// file position must be restored to the beginning for proper further scanning
		in.getChannel().position(0);

		iCertificateLength = 256 * iMSB + iLSB;
		iCertificateOffset = iSignatureOffset - iCertificateLength;

		return;
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate#AreThereNullFields()
	 */
	@Override
	protected boolean AreThereNullFields()
	{
		if( fileId == null )
			return true;
		if (fileType == null)
			return true;
		else if( vendor == null )
			return true;
		else if( boardProduct == null )
			return true;
		else if( appName == null )
			return true;
		else if( appVersion == null )
			return true;
		else if( dateAndTime == null )
			return true;
		else if( hardwareCompatibility == null )
			return true;
		else
			return false;
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate#scanCertificate()
	 */
	@Override
	protected void scanCertificate() 
	{
		// first field not valuable
		nextField( true );
		// initial certificate label
		startCertifLabel = nextField( false );

		// data fields
		while (hasNextField( ))
		{
			byte[] newfield = nextField( false );
			if (newfield == null) // no more certificates.
				break;

			String data = getFieldData( newfield );
			int indexOf = data.indexOf( "=" );
			if (indexOf > 0)
			{
				String key = data.substring( 0, indexOf );
				newfield = data.substring( indexOf + 1 ).getBytes( );
				switch(key)
				{
					case TYPE_KEY :
						fileType = newfield;
						break;
					case PCI_KEY :
						pciValue = newfield;
						break;
					case FILE_ID_KEY :
						fileId = newfield;
						break;
					case VENDOR_NAME_KEY :
						vendor = newfield;
						break;
					case PRODUCT_BOARD_NAME_KEY :
						boardProduct = newfield;
						break;
					case APPLICATION_NAME_KEY :
						appName = newfield;
						break;
					case APPLICATION_VERSION_KEY :
						appVersion = newfield;
						break;
					case DATE_AND_TIME_KEY :
						dateAndTime = newfield;
						break;
					case HARDWARE_COMPATIBILITY_KEY :
						hardwareCompatibility = newfield;
						break;
					default :
						logger.warn( "Invalid certificate field");
						break; //invalid file, abort.
				}
			}
		}
	}

	/**
	 * Checks for next field possibility.
	 *
	 * @return true, if there are data for a next field.
	 */
	private boolean hasNextField()
	{
		return this.offsetInsideCertificate < iCertificateLength;
	}

	/**
	 * Gets the data of a bytes array field.
	 *
	 * @param data the bytes array field
	 * @return the data
	 */
	private String getFieldData(byte[] data) 
	{
		try
		{
			return new String(data ,"UTF-8");
		}
		catch (UnsupportedEncodingException e)
		{
			logger.error( e.getMessage( ) );
		}
		return "";
	}


	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate#nextField(boolean)
	 */
	@Override
	protected byte[] nextField( boolean bStart ) 
	{
		if( bStart )
		{
			this.offsetInsideCertificate = 0;
		}
		
		byte[] buf = new byte[iCertificateLength];
		int offsetInsideTemporalBuf = 0;
		boolean bValidField = false;
		
		while( this.offsetInsideCertificate < iCertificateLength )
		{
			byte aByte = this.certificate[this.offsetInsideCertificate];
			this.offsetInsideCertificate++;
			if( aByte != 0 )
			{
				buf[offsetInsideTemporalBuf] = aByte;
				offsetInsideTemporalBuf++;
				bValidField = true;
			}
			else
			{
				break;
			}
		}
		if( bValidField )
		{
			return Arrays.copyOf(buf, offsetInsideTemporalBuf);			
		}
		else
		{
			return null;
		}
	}	
	
	
	/**
	 * Overridable method.
	 * @return boolean saying Ok or failed.
	 */
	@Override
	public boolean checkCertificate() // overridable method
	{
		if (AreThereNullFields( )) // overridable null field evaluation
		{
			return false;
		}

		String sStartCertifLabel, sFileType, sPciValue, sFileID, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime, sHardwareCompatibility;
		try
		{
			sStartCertifLabel = new String( startCertifLabel, "UTF-8" );
			if (pciValue != null && pciValue.length > 0)
			{
				sPciValue = new String( pciValue, "UTF-8" );
			}
			else
				sPciValue = "";
			sFileType = new String( fileType, "UTF-8" );
			sFileID = new String( fileId, "UTF-8" );
			sVendor = new String( vendor, "UTF-8" );
			sBoardProduct = new String( boardProduct, "UTF-8" );
			sAppName = new String( appName, "UTF-8" );
			sAppVersion = new String( appVersion, "UTF-8" );
			sDateAndTime = new String( dateAndTime, "UTF-8" );
			sHardwareCompatibility = new String( hardwareCompatibility, "UTF-8" );
		}
		catch (UnsupportedEncodingException e)
		{
			return false;
		}

		StringBuilder sMsgBuffer = new StringBuilder( );
		Formatter f = new Formatter( sMsgBuffer );
		String sTempl;
		String sMsg;
		if (certificateFile != null)
		{
			sTempl = "from filename [%s] = fileType [%s], pciValue [%s], fileId [%s], vendor [%s], boardProduct [%s], appName [%s], appVersion [%s], dateAndTime [%s].";
			sMsg = f.format( sTempl, this.certificateFile.getName( ), sFileType, sPciValue, sFileID, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime ).out( ).toString( );
		}
		else
		{
			sTempl = "from SPOT = fileType [%s], pciValue [%s], fileId [%s], vendor [%s], boardProduct [%s], appName [%s], appVersion [%s], dateAndTime [%s].";
			sMsg = f.format( sTempl, sFileType, sPciValue, sFileID, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime ).out( ).toString( );
		}

		f.close( );
		logger.debug( sMsg );

		if (!sStartCertifLabel.equalsIgnoreCase( "GILBARCO CERTIFICATE" )
			|| (!sFileType.equalsIgnoreCase( "APP" ) && !sFileType.equalsIgnoreCase( "FWR" ))
			|| (sFileType.equalsIgnoreCase( "APP" ) && !sFileID.equalsIgnoreCase( "EMV" ) && !sFileID.equalsIgnoreCase( "TOOLS" ) && !sFileID.equalsIgnoreCase( "OPT" ))
			|| (sFileType.equalsIgnoreCase( "FWR" ) && !sFileID.equalsIgnoreCase( "Kernel" ) && !sFileID.equalsIgnoreCase( "Boot" ) && !sFileID.equalsIgnoreCase( "RESOURCES" )))
		{
			logger.error( "Invalid Certificate Fromat - TYPE, ID, etc." );
			return false;
		}

		return true;
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

}

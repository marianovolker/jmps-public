/**
 * © 2012, 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file ROMfsCertificate.java
 * @author mvolker
 * @date 2012
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

import com.gilbarco.globaltools.flexpay.maintenance.gui.FlexPayMaintenanceModel;

/**
 * The Class ROMfsCertificate.
 *
 * @author mvolker
 */
public class ROMfsCertificate
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	/** The Constant ROMFS_CERTIFICATE_FIXEDSIZE. */
	protected static final int ROMFS_CERTIFICATE_FIXEDSIZE = 248;
	
	/** The Constant ROMFS_SIGNATURE_FIXEDSIZE. */
	protected static final int ROMFS_SIGNATURE_FIXEDSIZE = 256;
	
	/** The Constant logger. */
	private static final transient Log	logger	= LogFactory.getLog( ROMfsCertificate.class );

	/*	The file certificate contain information about version, FWR, APPL, HWR, etc. and signature.
		The max length is 512 bytes. The structure is:
		========================================================================
		VANGUARD Certificate layout      size = variable(LEN_MSB,LEN_LSB) + 256
		<0x00> START CERTIFICATE "GILBARCO VANGUARD CERTIFICATE"
		<0x00> FILE_ID={"recovery","ROMfs1","ROMfs2","ROMfs3","ROMfs4","ROMfs5","ROMfs6","ROMfs7","ROMfs8"}
		<0x00> VENDOR_NAME=
		<0x00> PRODUCT_BOARD_NAME=
		<0x00> APPLICATION_NAME=
		<0x00> APPLICATION_VERSION=
		<0x00> DATE_AND_TIME=
		<0x00> HARDWARE_COMPATIBILITY=
		<0x00>
		<0x00>
			LEN_MSB
			LEN_LSB                 len is from [START_CERTIFICATE ...... Signature]
			Signature [256 bytes]
		========================================================================
		Classic M3 and M5 Certificate layout is fixed size 248 (0xF0)
		<0x00> File Id {"kernel","ROMfs1","ROMfs2","ROMfs3","ROMfs4","ROMfs5","ROMfs6","ROMfs7","ROMfs8"}
		<0x00> File Hash		[32] (MD5)
		<0x00> Vendor Name
		<0x00> Product/Board Name
		<0x00> Application Name
		<0x00> Application Version
		<0x00> DateAndTime
		<0x00> Optional File Hash
		<0x00> Filler
		<0x00> MAC Certificate [4] (X9.19 with MSK)
		<0x00>
		========================================================================
	*/

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************
	/** The certificate length. */
	protected int iCertificateLength = ROMFS_CERTIFICATE_FIXEDSIZE;
	
	/** The certificate offset. */
	protected int iCertificateOffset = 0;
	
	/** The signature length. */
	protected int iSignatureLength = 0; // VANGUARD field
	
	/** The signature offset. */
	protected int iSignatureOffset = 0; // VANGUARD field
	
	/** The file id. */
	protected byte[] fileId;
	
	/** The file hash. */
	protected byte[] fileHash; // M3M5 field
	
	/** The vendor. */
	protected byte[] vendor;
	
	/** The board product. */
	protected byte[] boardProduct;
	
	/** The app name. */
	protected byte[] appName;
	
	/** The app version. */
	protected byte[] appVersion;
	
	/** The date and time. */
	protected byte[] dateAndTime;
	
	/** The firmware mac. */
	protected byte[] firmwareMAC;      // M3M5 field
	
	/** The optional file hash. */
	protected byte[] optionalFileHash; // M3M5 field
	
	/** The offset inside certificate. */
	protected int offsetInsideCertificate = 0;
	
	/** The certificate. */
	protected byte[] certificate;
	
	/** The certificate file. */
	protected File certificateFile;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	/** The file name. */
	private String fileName;
	
	/** The signature. */
	private byte[] signature;


	// ******************************************************************
	// CONSTRUCTORS.
	// ******************************************************************

	/**
	 * Instantiates a new certificate.
	 *
	 * @param file the file for the certificate
	 */
	public ROMfsCertificate ( File file )
	{
		this.fileName = file.getAbsolutePath( );
		this.certificateFile = file; 
		this.certificate = null;
	}

	/**
	 * Instantiates a new certificate.
	 *
	 * @param certificate the certificate data
	 */
	public ROMfsCertificate ( byte[] certificate )
	{
		this.fileName = null;
		this.certificateFile = null;
		this.certificate = certificate;
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface impl)
	// ******************************************************************

	/**
	 * Gets the certificate size.
	 *
	 * @return the ROMfs certificate size
	 */
	public int getCertificateSize()
	{
		return iCertificateLength;
	}

	/**
	 * Gets the signature size.
	 *
	 * @return the ROMfsCertificateSize
	 */
	public int getSignatureSize()
	{
		return iSignatureLength;
	}

	/**
	 * Gets the file name.
	 *
	 * @return the fileName
	 */
	public String getFileName()
	{
		return fileName;
	}

	/**
	 * Gets the file.
	 *
	 * @return the inner file object
	 */
	public File getFile()
	{
		return certificateFile;
	}

	/**
	 * Gets the certificate data.
	 *
	 * @return the binary certificate ( byte array of the certificate )
	 */
	public byte[] getCertificateData()
	{
		return certificate;
	}

	/**
	 * Gets the signature data.
	 *
	 * @return the binary signature ( byte array of the signature )
	 */
	public byte[] getSignatureData()
	{
		return signature;
	}

	/**
	 * Gets the file id.
	 *
	 * @return the fileId
	 */
	public byte[] getFileId()
	{
		return fileId;
	}

	/**
	 * Gets the vendor.
	 *
	 * @return the vendor
	 */
	public byte[] getVendor()
	{
		return vendor;
	}

	/**
	 * Gets the board product.
	 *
	 * @return the boardProduct
	 */
	public byte[] getBoardProduct()
	{
		return boardProduct;
	}

	/**
	 * Gets the app name.
	 *
	 * @return the appName
	 */
	public byte[] getAppName()
	{
		return appName;
	}

	/**
	 * Gets the app version.
	 *
	 * @return the appVersion
	 */
	public byte[] getAppVersion()
	{
		return appVersion;
	}

	/**
	 * Gets the date and time.
	 *
	 * @return the dateAndTime
	 */
	public byte[] getDateAndTime()
	{
		return dateAndTime;
	}

	/**
	 * Gets the firmware mac.
	 *
	 * @return the firmwareMAC
	 */
	public byte[] getFirmwareMac()
	{
		return firmwareMAC;
	}

	/**
	 * Gets the file hash.
	 *
	 * @return the fileHash
	 */
	public byte[] getFileHash()
	{
		return fileHash;
	}

	/**
	 * Gets the optional file hash.
	 *
	 * @return the optionalFileHash
	 */
	public byte[] getOptionalFileHash()
	{
		return optionalFileHash;
	}

	/**
	 * Load certificate.
	 *
	 * @param blParseIt the bl parse it
	 * @return true, if successful
	 */
	public boolean loadCertificate(boolean blParseIt) 
	{
		if( certificate == null )
		{
			try
			{
				FileInputStream in = new FileInputStream(fileName);
				calculateCertificateAndSignaturePosition(in); // overridable method

				in.skip( iCertificateOffset );
				certificate = new byte[iCertificateLength];
				in.read( certificate );
				if( iSignatureLength > 0) // Vanguard only
				{
					signature = new byte[iSignatureLength];
					in.read( signature );
				}
				in.close();
			}
			catch (IOException e)
			{
				return false;
			}
		}

		if( certificate.length <= 0 )
		{
			return false;
		}

		if( !blParseIt ) // do not parse the binary certificate
			return true;

		scanCertificate(); // overridable method

		return checkCertificate(); // overridable method
	}

	/**
	 * Validate the certificate data.
	 *
	 * @return true if it is valid, false in other case.
	 */
	public boolean checkCertificate() // overridable method
	{
		if( AreThereNullFields() ) // overridable null field evaluation
		{
			return false;
		}
		
		String sFileID, sFileHash, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime;
		try
		{
			sFileID = new String(fileId ,"UTF-8");
			// Field present on M3M5 only
			sFileHash = ( (fileHash == null) ? "" : new String(fileHash ,"UTF-8") );
			sVendor = new String(vendor ,"UTF-8");
			sBoardProduct = new String(boardProduct ,"UTF-8");
			sAppName = new String(appName ,"UTF-8");
			sAppVersion = new String(appVersion ,"UTF-8");
			sDateAndTime = new String(dateAndTime ,"UTF-8");
		}
		catch (UnsupportedEncodingException e)
		{
			return false;
		}
		
		if ((!sFileID.equalsIgnoreCase( "Kernel" ) && !sFileID.equalsIgnoreCase( "Boot" ) && !sFileID.equalsIgnoreCase( "BootRecovery" ) &&
				!sFileID.equalsIgnoreCase( "RecoveryData" ) && !sFileID.equalsIgnoreCase( "ROMfs1" ) && !sFileID.equalsIgnoreCase( "ROMfs2" ) &&
				!sFileID.equalsIgnoreCase( "ROMfs3" ) && !sFileID.equalsIgnoreCase( "ROMfs3" ) && !sFileID.equalsIgnoreCase( "ROMfs4" ) &&
				!sFileID.equalsIgnoreCase( "ROMfs5" ) && !sFileID.equalsIgnoreCase( "ROMfs6" ) && !sFileID.equalsIgnoreCase( "ROMfs7" ) &&
				!sFileID.equalsIgnoreCase( "ROMfs8" ) && !sFileID.equalsIgnoreCase( "debian" )) || (sFileID.equalsIgnoreCase( "Recovery" ) && !FlexPayMaintenanceModel.isVanguardDevice( )))
		{
			return false;
		}

		StringBuilder sMsgBuffer = new StringBuilder();
		Formatter f = new Formatter(sMsgBuffer);
		String sTempl;
		String sMsg;
		if( this.certificateFile != null )
		{
			sTempl = "from filename [%s] = fileId [%s], fileHash [%s], vendor [%s], boardProduct [%s], appName [%s], appVersion [%s], dateAndTime [%s].";
			sMsg = f.format( sTempl, this.certificateFile.getName( ), sFileID, sFileHash, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime ).out().toString();
		}
		else
		{
			sTempl = "from SPOT = fileId [%s], fileHash [%s], vendor [%s], boardProduct [%s], appName [%s], appVersion [%s], dateAndTime [%s].";
			sMsg = f.format( sTempl, sFileID, sFileHash, sVendor, sBoardProduct, sAppName, sAppVersion, sDateAndTime ).out().toString();
		}
		f.close();
		logger.debug( sMsg );
		return true;
	}


	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * Calculate certificate and signature position.
	 *
	 * @param in the input file
	 * @throws IOException Signals that an I/O exception has occurred.
	 */
	protected void calculateCertificateAndSignaturePosition(FileInputStream in) throws IOException
	{
		int fileSize = (int) certificateFile.length();
		iCertificateOffset = fileSize - iCertificateLength ;
		iCertificateLength = ROMFS_CERTIFICATE_FIXEDSIZE;
		iSignatureOffset = 0;
		iSignatureLength = 0;
		return;
	}

	/**
	 * Are there null fields for the current certificate.
	 *
	 * @return boolean
	 * @author gparis
	 */
	protected boolean AreThereNullFields() // overridable method
	{
		if( fileId == null )
			return true;
		else if( fileHash == null )
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
		else
			return false;
	}

	/**
	 * Scan the certificate fields.
	 *
	 * @return boolean
	 * @author gparis
	 */
	protected void scanCertificate() // overridable method
	{
		nextField(true);
		fileId 			 = nextField(false);
		fileHash		 = nextField(false);
		vendor 			 = nextField(false);
		boardProduct	 = nextField(false);
		appName			 = nextField(false);
		appVersion		 = nextField(false);
		dateAndTime		 = nextField(false);
		firmwareMAC		 = nextField(false);
		optionalFileHash = nextField(false);
	}

	
	/**
	 * Next field.
	 *
	 * @param bStart true if it is the first field.
	 * @return the byte[] for a new field.
	 */
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
	
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	

}

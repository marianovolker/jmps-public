/**
 * 
 */
package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.M3M5ROMfsPackage;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsPackage;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/**
 * @author gparis
 *
 */
public class M3M5ROMfsPackageUploadStrategy implements IROMfsPackageUploadStrategy
{
	// ******************************************************************
	// INNER TYPES 
	// ******************************************************************

	// ******************************************************************
	// STATIC FIELDS 
	// ******************************************************************
	/**
	 * The SPOT Application ID to send ROMfs update requests.
	 */
	private static final byte PACKAGE_DOWNLOAD_REQUEST_APP_ID = 0x03;

	/**
	 * The SPOT Application ID to evaluate ROMfs update responses.
	 */
	private static final byte PACKAGE_DOWNLOAD_REPLY_APP_ID = (byte) 0x83;

	/**
	 * The timeout for a download command answer is set to 5000 milliseconds
	 * for M3/M5 OPTs
	 */
	private static final long DOWNLOAD_COMMAND_REPLY_TIMEOUT = 5000;

	/**
	 * The timeout for a package activation command answer is set to 10000 milliseconds
	 * for M3/M5 OPTs
	 */
	private static final long PACKAGE_ACTIVATION_REPLY_TIMEOUT = 5000;

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
	// CONSTRUCTOR.
	// ******************************************************************

	// ******************************************************************
	// OVERRIDDEN METHODS  (invoked from polymorphic interface).
	// ******************************************************************

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#createROMfsPackage()
	 */
	@Override
	public ROMfsPackage createROMfsPackage(ROMfsCertificate rcert)
	{
		return new M3M5ROMfsPackage(rcert);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getPackageActivationRequest()
	 */
	@Override
	public byte[] getPackageActivationRequest(String sActivationTime)
	{
		return SpotMessages.getInstance().getM3M5PackageActivationRequest(sActivationTime);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getFileDownloadRequestMsgBytes(int, int, int, byte[], byte[])
	 */
	@Override
	public byte[] getFileDownloadRequestMsgBytes(int length, int iCertificateSize,
							int iCommand,
							int iOption, byte[] certificateData, byte[] signatureData)
	{
		return SpotMessages.getInstance( )
				.getM3M5ROMfsFileDownloadRequestData
				(
					length,
					iCommand, // save or verify package
					iOption, // start or resume download operation
					certificateData // fixed 248 bytes length, iCertificateSize not used here.
				);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getBlockDownloadRequestMsgBytes(int, byte[])
	 */
	@Override
	public byte[] getBlockDownloadRequestMsgBytes(int length, byte[] data)
	{
		return SpotMessages.getInstance( )
				.getM3M5ROMfsBlockDownloadRequestData( length, data );
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isPackageActivationCommandName(java.lang.String)
	 */
	@Override
	public boolean isPackageActivationCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isFileDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isFileDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isBlockDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isBlockDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isFileOrBlockDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isFileOrBlockDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD.getName()) )
			return true;

		if( sCmdName.equals(SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isVanguardDevice()
	 */
	@Override
	public boolean isVanguardDevice()
	{
		return false;
	}

	/**
	 * Calling this interface method for M3/M5 devices is conceptually incorrect and
	 * it is marked as an invalid timeout rising exceptions
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getAuthenticationPresentationTimeout()
	 */
	@Override
	public long getAuthenticationPresentationTimeout()
	{
		return -1;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getMsgAnswerTimeout()
	 */
	@Override
	public long getMsgAnswerTimeout()
	{
		return DOWNLOAD_COMMAND_REPLY_TIMEOUT;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getPackageActivationTimeout()
	 */
	@Override
	public long getPackageActivationTimeout()
	{
		return PACKAGE_ACTIVATION_REPLY_TIMEOUT;
	}

	@Override
	public byte getROMfsPackageUpdateRequestAppID()
	{
		return PACKAGE_DOWNLOAD_REQUEST_APP_ID;
	}

	@Override
	public byte getROMfsPackageUpdateResponseAppID()
	{
		return PACKAGE_DOWNLOAD_REPLY_APP_ID;
	}

	/* (non-Javadoc)
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#newPackageCertificate(java.io.File)
	 */
	@Override
	public ROMfsCertificate newPackageCertificate(File file)
	{
		return new ROMfsCertificate(file);
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
	// MAIN.
	// ******************************************************************

}

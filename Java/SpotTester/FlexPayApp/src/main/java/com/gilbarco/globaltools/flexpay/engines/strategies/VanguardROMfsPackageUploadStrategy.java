package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsPackage;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.VanguardROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.VanguardROMfsPackage;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

public class VanguardROMfsPackageUploadStrategy implements IROMfsPackageUploadStrategy
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
	private static final byte PACKAGE_DOWNLOAD_REQUEST_APP_ID = 0x01;

	/**
	 * The SPOT Application ID to evaluate ROMfs update responses.
	 */
	private static final byte PACKAGE_DOWNLOAD_REPLY_APP_ID = (byte) 0x81;

	/**
	 * The timeout for a token presentation command answer is set to 10000 milliseconds
	 * for Vanguard OPTs
	 */
	private static final long AUTHENTICATION_PRESENTATION_REPLY_TIMEOUT = 10000;

	/**
	 * The timeout of a download command answer is set to 5000 milliseconds
	 * for Vanguard OPTs
	 */
	private static final long DOWNLOAD_COMMAND_REPLY_TIMEOUT = 10000;

	/**
	 * The timeout for a package activation command answer is set to 10000 milliseconds
	 * for Vanguard OPTs
	 */
	private static final long PACKAGE_ACTIVATION_REPLY_TIMEOUT = 10000;

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
		return new VanguardROMfsPackage(rcert);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getPackageActivationRequest()
	 */
	@Override
	public byte[] getPackageActivationRequest(String sActivationTime)
	{
		return SpotMessages.getInstance().getVanguardPackageActivationRequest(sActivationTime);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getFileDownloadRequestMsgBytes(int, int, int, byte[], byte[])
	 */
	@Override
	public byte[] getFileDownloadRequestMsgBytes(int length, int iCertificateSize,
										int iCommand, int iOption,
										byte[] certificateData, byte[] signatureData)
	{
		return SpotMessages.getInstance( )
				.getVanguardROMfsFileDownloadRequestData
				(
					length, // ROMfs file length
					iCertificateSize, // certificate size
					iCommand, // save or verify package cmd
					iOption, // start downloading or resume previous operation
					certificateData, signatureData
				);
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getBlockDownloadRequestMsgBytes(int, byte[])
	 */
	@Override
	public byte[] getBlockDownloadRequestMsgBytes(int length, byte[] data)
	{
		return SpotMessages.getInstance( )
				.getVanguardROMfsBlockDownloadRequestData( length, data );
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isPackageActivationCommandName(java.lang.String)
	 */
	@Override
	public boolean isPackageActivationCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isFileDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isFileDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isBlockDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isBlockDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isFileOrBlockDownloadCommandName(java.lang.String)
	 */
	@Override
	public boolean isFileOrBlockDownloadCommandName(String sCmdName)
	{
		if( sCmdName.equals(SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD.getName()) )
			return true;

		if( sCmdName.equals(SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD.getName()) )
			return true;

		return false;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#isVanguardDevice()
	 */
	@Override
	public boolean isVanguardDevice()
	{
		return true;
	}

	/**
	 * @see com.gilbarco.globaltools.flexpay.engines.strategies.IROMfsPackageUploadStrategy#getAuthenticationPresentationTimeout()
	 */
    @Override
	public long getAuthenticationPresentationTimeout()
	{
		return AUTHENTICATION_PRESENTATION_REPLY_TIMEOUT;
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
		return new VanguardROMfsCertificate(file);
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

/**
 * 
 */
package com.gilbarco.globaltools.flexpay.engines.strategies;

import java.io.File;

import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsCertificate;
import com.gilbarco.globaltools.flexpay.maintenance.romfs.ROMfsPackage;

/**
 * Interface to force a contract having the guidelines for updating ROMfs packages
 * in the terminal OPT, according to its terminal type.
 * @author gparis
 *
 */
public interface IROMfsPackageUploadStrategy
{
	/**
	 * 
	 * @return A <b>new</b> subclass <b>instance</b> of ROMfsPackage class.
	 */
	public ROMfsPackage createROMfsPackage(ROMfsCertificate rcert);

	/**
	 * @param The desired activation time (optional) in ASCII format "YYYY.MM.DD-hh:mm"
	 * @return The <b>byte array</b> corresponding to the "Package Activation Request" SPOT command.
	 */
	public byte[] getPackageActivationRequest(String sActivationTime);

	/**
	 * 
	 * @return The <b>byte array</b> corresponding to the "Download ROMfs Request" SPOT command.
	 */
	public byte[] getFileDownloadRequestMsgBytes( int length, int iCertificateSize,
										int iCommand, int iOption,
										byte[] certificateData, byte[] signatureData);

	/**
	 * 
	 * @return The <b>byte array</b> corresponding to the "Download block Request" SPOT command.
	 */
	public byte[] getBlockDownloadRequestMsgBytes( int length, byte[] data );


	/**
	 * 
	 * @return <b>boolean</b> saying whether the parameter is the name of the
	 * "Package Activation" command, or not
	 */
	public boolean isPackageActivationCommandName(String sCmdName);

	/**
	 * 
	 * @return <b>boolean</b> saying whether the parameter is the name of the
	 * "Download ROMfs Request" command, or not
	 */
	public boolean isFileDownloadCommandName(String sCmdName);

	/**
	 * 
	 * @return <b>boolean</b> saying whether the parameter is the name of the
	 * "Download block Request" command, or not
	 */
	public boolean isBlockDownloadCommandName(String sCmdName);

	/**
	 * 
	 * @return <b>boolean</b> saying whether the parameter is the name of the
	 * "Download ROMfs Request" or "Download block Request" command, or not
	 */
	public boolean isFileOrBlockDownloadCommandName(String sCmdName);

	/**
	 * 
	 * @return <b>boolean</b> saying whether the target OPT device for this
	 * strategy is a Vanguard unit, or not
	 */
	public boolean isVanguardDevice();

	/**
	 * 
	 * @return <b>long</b> expressing the max number of milliseconds waiting for the answer
	 * of the authentication presentation command.
	 */
	public long getAuthenticationPresentationTimeout();

	/**
	 * 
	 * @return <b>long</b> expressing the max number of milliseconds waiting for an answer
	 * of the previous download commands.
	 */
	public long getMsgAnswerTimeout();

	/**
	 * 
	 * @return <b>long</b> expressing the max number of milliseconds waiting for the answer
	 * of the package activation command.
	 */
	public long getPackageActivationTimeout();

	/**
	 * 
	 * @return <b>byte</b> The SPOT Application ID to send ROMfs update requests.
	 */
	public byte getROMfsPackageUpdateRequestAppID();

	/**
	 * 
	 * @return <b>byte</b> The SPOT Application ID to evaluate ROMfs update responses.
	 */
	public byte getROMfsPackageUpdateResponseAppID();

	
	/**
	 * New package certificate according with the strategy.
	 *
	 * @param file the certificate file
	 * @return the certificate
	 */
	public ROMfsCertificate newPackageCertificate(File file);

	
}

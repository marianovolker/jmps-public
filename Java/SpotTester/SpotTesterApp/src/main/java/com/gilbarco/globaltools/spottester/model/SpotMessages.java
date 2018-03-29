
package com.gilbarco.globaltools.spottester.model;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.crypto.EdtCryptoAdapter;
import com.gilbarco.globaltools.edt.util.EdtByteUtil;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spotprotocol.utils.MAC9_19;
import com.gilbarco.globaltools.spottester.exceptions.SpotMessageException;

/**
 * @Class SpotTesterMessages
 * 
 * @Description This Class has all the messages implementation. All the getters returns the raw data to be sent to SPOT.
 * 
 * @Services <ul>
 *           <li>EdtMgpService
 *           </ul>
 * 
 * @see MgpServiceAdapter
 * @see SpotMessageType
 * 
 * @Protocol SpotProtocol 1.03 (EdtMgpService configuration).
 * 
 * @version 1.0.0
 * 
 * @author jrubstein
 * 
 * */

public class SpotMessages
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final byte[]			RSA_CERTIFICATE	= EdtConvert.hexStringToBytes( "1E22C82EED871FD38BBFF2F2CFD33A6DF6C1981639E1D3831BAE8098A5CB5237597488EFD28BF3B4457BD33B865BA5895F30097F0A923D7B5A9787AC14C0080783879468DB12759BD8E8FB4B7F632B7F124DE1E4CC0A0D0BAD1ACE3E1F72FA419972D7329CC94BA75D0150E1BC81C182993A2AF0D02E696B2349DCCA551753EE593B5C8CEB2B2FE3C1A607B0DE7CD0709171D7BD95C26D93DE92600EB06FFAA17C6A94640C67CED48C7AD38FCBAA2267BB294BF4824242454B547F1928C0EA6DB0A028ACBFC4CBE9169E5110D19248F263DCBA5FF82EFBA898886B1F9D58B7A5CE993D74F35971E95C1CC353A53A4E34F86C85F99BCB26D9" );
	private static SpotMessages			instance		= null;
	private static final transient Log	logger			= LogFactory.getLog( SpotMessages.class );

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	public static SpotMessages getInstance()
	{
		if (instance == null)
								instance = new SpotMessages( );
		return instance;
	}

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private MgpServiceAdapter	edtMgpAdapter;
	private EdtCryptoAdapter	edtCryptoAdapter;
	private byte				pingTimeout;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************

	private SpotMessages()
	{
		super( );
		try
		{
			edtMgpAdapter = (MgpServiceAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(
				SpotServiceDefinitionsFactory.getMgpDefinition( ) );
			edtCryptoAdapter = (EdtCryptoAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(
				SpotServiceDefinitionsFactory.getCryptoDefinition( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public byte[] getSpotLoginData(byte clientID)
	{
		return this.getSpotLoginData( SpotConstants.LOGINTIMEOUT, clientID, SpotConstants.PINGTIMEOUT );
	}

	public byte[] getSpotLoginData(byte timeout, byte clientID, byte pingTimeout)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			map.put( "ClientId", new byte[] { clientID } );
			map.put( "ModuloLen", new byte[] { 0x00 } );
			map.put( "PingTimeout", new byte[] { timeout } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.LOGIN.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		finally
		{
			this.pingTimeout = pingTimeout;
		}
		return null;
	}

	public byte getPingTimeoutValue()
	{
		return this.pingTimeout;
	}

	public byte[] getSpotPingData()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.PING.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getSpotLogoutData()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.LOGOUT.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getSpotScreenWithTextData(String text, byte winId)
	{

		Map<String, Object> map = new HashMap<String, Object>( );
		Map<String, Object> tag = new HashMap<String, Object>( );
		map.put( "ssk", new byte[] { 0x00 } );
		map.put( "AppId", new byte[] { 0x01 } );
		map.put( "WinId", new byte[] { winId } );
		tag.put( "TagId", new byte[] { 0x00 } );
		tag.put( "OriginX", new byte[] { 0x00, 0x00 } );
		tag.put( "OriginY", new byte[] { 0x00, 0x00 } );
		tag.put( "Width", new byte[] { 0x01, 0x40 } );
		tag.put( "Height", new byte[] { 0x00, (byte) 0xF0 } );
		tag.put( "BackRGB", new byte[] { 0x64, 0x64, (byte) 0xF0 } );
		tag.put( "BorderSize", new byte[] { 0x00 } );
		tag.put( "BorderRGB", new byte[] { (byte) 0xFF, (byte) 0xC8, 0x00 } );
		tag.put( "Visible", new byte[] { 0x00 } );
		tag.put( "TagType", new byte[] { 0x01 } );
		tag.put( "FontId", new byte[] { 0x00 } );
		tag.put( "FontRGB", new byte[] { 0x00, 0x00, 0x00 } );
		tag.put( "Alligen", new byte[] { 0x00 } );
		tag.put( "posX", new byte[] { 0x00, 0x10 } );
		tag.put( "posY", new byte[] { 0x00, 0x10 } );
		tag.put( "TextLen", new byte[] { (byte) text.length( ) } );
		tag.put( "Text", text.getBytes( ) );
		map.put( "TagsArray", tag );
		try
		{
			return edtMgpAdapter.callEncode( map, SpotMessageType.CREATE_WINDOW.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getResetRequest()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.RESET.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	/**
	 * @return default package activation ( right now! )command byte array "for M3/M5 OPTs
	 */
	public byte[] getM3M5PackageActivationRequest()
	{
		return getM3M5PackageActivationRequest( null );
	}

	/**
	 * @return package activation (according activation time) command byte array "for M3/M5 OPTs
	 */
	public byte[] getM3M5PackageActivationRequest(String sActivationTime)
	{
		return commonActivationRequest( sActivationTime, SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION );
	}

	/**
	 * @return default package activation ( right now! )command byte array "for M3/M5 OPTs
	 */
	public byte[] getVanguardPackageActivationRequest()
	{
		return getVanguardPackageActivationRequest( null );
	}

	/**
	 * @return package activation (according activation time) command byte array "for M3/M5 OPTs
	 */
	public byte[] getVanguardPackageActivationRequest(String sActivationTime)
	{
		return commonActivationRequest( sActivationTime, SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION );
	}

	private byte[] commonActivationRequest(String sActivationTime, SpotMessageType msgType)
	{
		boolean blIsVanguardOPT;

		if (msgType == SpotMessageType.VANGUARD_ROMfs_PACKAGE_ACTIVATION)
			blIsVanguardOPT = true;
		else if (msgType == SpotMessageType.M3M5_ROMfs_PACKAGE_ACTIVATION)
			blIsVanguardOPT = false;
		else
			return null; // wrong message type !

		String sTime = null;
		byte[] abActivationTime = null;

		if (sActivationTime == null)
		{
			// Activation time is right now
			abActivationTime = new byte[] { 0x30, 0x30, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x2D, 0x30, 0x30, 0x3A, 0x30, 0x30 };
		}
		else
		{
			String s = sActivationTime.trim( );
			sTime = (s.length( ) > 16 ? s.substring( 0, 15 ) : s);
			try
			{
				abActivationTime = sTime.getBytes( "US-ASCII" );
			}
			catch (UnsupportedEncodingException e)
			{
				logger.error( e.getMessage( ), e );
				return null;
			}
		}

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { (byte) (blIsVanguardOPT ? 0x01 : 0x03) } );
			map.put( "Time", abActivationTime );
			return edtMgpAdapter.callEncode( map, msgType.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getVanguardPackageInfoRequest()
	{

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "Set", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.VANGUARD_ROMfs_PACKAGE_INFO.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getM3M5PackageInfoRequest()
	{

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x03 } );
			map.put( "Set", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.M3M5_ROMfs_PACKAGE_INFO.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getVanguardROMfsFileDownloadRequestData(int length, int iCertSize,
															int iCommand, int iOption,
															byte[] abCertificate,
															byte[] abSignature)
	{
		short lenH16 = (short) (length / 65536);
		short lenL16 = (short) (length % 65536);

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			byte abFileLength[] = { (byte) (lenH16 / 256), (byte) (lenH16 % 256),
							(byte) (lenL16 / 256), (byte) (lenL16 % 256) };
			map.put( "FileSize", abFileLength );
			byte abCertSize[] = { (byte) (iCertSize / 256), (byte) (iCertSize % 256) };
			map.put( "CertificateSize", abCertSize );
			map.put( "Certificate", Arrays.copyOfRange( abCertificate, 0, iCertSize ) );
			map.put( "Signature", Arrays.copyOfRange( abSignature, 0, 256 ) );
			map.put( "VerifyOrSave", new byte[] { (byte) iCommand } );
			map.put( "Option", new byte[] { (byte) iOption } );

			return edtMgpAdapter.callEncode( map, SpotMessageType.VANGUARD_ROMfs_FILE_DOWNLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getM3M5ROMfsFileDownloadRequestData(int length, int iCommand,
														int iOption, byte[] abCertificate)
	{
		short lenH16 = (short) (length / 65536);
		short lenL16 = (short) (length % 65536);

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x03 } );
			byte ab4Bytes[] = { (byte) (lenH16 / 256), (byte) (lenH16 % 256), (byte) (lenL16 / 256), (byte) (lenL16 % 256) };
			map.put( "FileSize", ab4Bytes );
			map.put( "Certificate", Arrays.copyOfRange( abCertificate, 0, 248 ) );
			map.put( "VerifyOrSave", new byte[] { (byte) iCommand } );
			map.put( "Option", new byte[] { (byte) iOption } );

			return edtMgpAdapter.callEncode( map, SpotMessageType.M3M5_ROMfs_FILE_DOWNLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileDownloadRequestData(byte[] abData, byte fileType, byte fileId, byte authenType, String description)
	{
		try
		{
			byte[] size = new byte[4];
			EdtConvert.longToBytes( true, size, 0, size.length, abData.length, 4 );

			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "FileType", new byte[] { fileType } );
			map.put( "FileId", new byte[] { fileId } );
			map.put( "FileSz", size );
			map.put( "CRC", edtCryptoAdapter.callCRC32( abData ) );
			map.put( "AuthenType", new byte[] { authenType } );
			switch(authenType)
			{
				case 0x01 :
				case 0x02 :
					map.put( "Authent12",
						new MAC9_19( SpotServiceDefinitionsFactory.getCryptoDefinition( ) ).getValue( abData, authenType ) );
					break;
				case 0x03 :
				case 0x04 :
					map.put( "Authent34", RSA_CERTIFICATE );
					break;
			}
			map.put( "Description", description.getBytes( ) );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_DOWNLOAD_REQUEST.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileDownloadRequestData(File file, byte fileType, byte fileId, byte authenType, String description)
	{

		try
		{
			byte[] size = new byte[4];
			EdtConvert.longToBytes( true, size, 0, size.length, file.length( ), 4 );

			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "FileType", new byte[] { fileType } );
			map.put( "FileId", new byte[] { fileId } );
			map.put( "FileSz", size );
			map.put( "CRC", edtCryptoAdapter.callCRC32( file, true ) );
			map.put( "AuthenType", new byte[] { authenType } );
			switch(authenType)
			{
				case 0x01 :
				case 0x02 :
					map.put( "Authent12",
						new MAC9_19( SpotServiceDefinitionsFactory.getCryptoDefinition( ) ).getValue( file, authenType ) );
					break;
				case 0x03 :
				case 0x04 :
					map.put( "Authent34", RSA_CERTIFICATE );
					break;
			}
			map.put( "Description", description.getBytes( ) );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_DOWNLOAD_REQUEST.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getVanguardROMfsBlockDownloadRequestData(int length, byte[] data)
	{
		byte abOnly2Bytes[] = { (byte) (length / 256), (byte) (length % 256) };
		/*try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "BlkSize", abOnly2Bytes );
			if (length > 0)
			{
				map.put( "BlkData", Arrays.copyOfRange( data, 0, length ) );
			}

			// doesn't work with 8k block size.///////////////////////////////////////////////////////////
			return edtMgpAdapter.callEncode( map, SpotMessageType.VANGUARD_ROMfs_BLOCK_DOWNLOAD.getCommand( ) );
			// ///////////////////////////////////////////////////////////////////////////////////////////
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
		*/
		int msgsize = length + 8;
		byte[] first = new byte[8];

		first[0] = (byte) (msgsize / 256); //message size byte 01
		first[1] = (byte) (msgsize % 256); //message size byte 02
		first[2] = (byte) 0x01;	//AppId
		first[3] = (byte) 0x00; //ssk
		first[4] = (byte) 0x60; //command
		first[5] = (byte) 0x02; //subcommand
		first[6] = abOnly2Bytes[0]; //BlkSize byte 01
		first[7] = abOnly2Bytes[1]; //BlkSize byte 02

		//BlkData
		if (length > 0)
		{
			byte[] result = Arrays.copyOf( first, msgsize );
			System.arraycopy( data, 0, result, first.length, data.length );
			return result;
		}
		else
		{
			return first;
		}
	}

	public byte[] getM3M5ROMfsBlockDownloadRequestData(int length, byte[] data)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x03 } );
			byte abOnly2Bytes[] = { (byte) (length / 256), (byte) (length % 256) };
			map.put( "BlkSize", abOnly2Bytes );
			if (length > 0)
							map.put( "BlkData", Arrays.copyOfRange( data, 0, length ) );

			return edtMgpAdapter.callEncode( map, SpotMessageType.M3M5_ROMfs_BLOCK_DOWNLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileBlockDownloadData(int offset, int length, byte[] data)
	{
		try
		{
			byte[] offSet = new byte[4];
			EdtConvert.intToBytes( true, offSet, 0, offset );
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "BlkOffset", offSet );
			// map.put("BlkSize", EdtConvert.hexStringToBytesLeft(Integer.toHexString(length), '0'));
			byte abOnly2Bytes[] = { (byte) (length / 256), (byte) (length % 256) };
			map.put( "BlkSize", abOnly2Bytes );
			map.put( "BlkData", Arrays.copyOfRange( data, 0, length ) );

			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_BLOCK_DOWNLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileDeleteData(byte fileType, byte fileId)
	{

		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "FileType", new byte[] { fileType } );
			map.put( "FileId", new byte[] { fileId } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_DELETE.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	/*
	 * public byte[] getSoftwareInfo() {
	 * 
	 * try { Map<String, Object> map = new HashMap<String, Object>(); map.put("ssk", new byte[] { 0x00 }); map.put("AppId", new byte[] { 0x01 }); map.put("ItemCount", new byte[] { 0x00 }); return
	 * edtMgpAdapter.callEncode(map, SpotMessageType.SOFTWARE_INFO.getCommand()); } catch (EdtServiceException e) { logger.error(e.getMessage(), e); } return null; }
	 */
	public byte[] getFileBrowse(byte fileType)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "FileType", new byte[] { fileType } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_BROWSE.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getOTISetData(byte[] tag, byte[] rawData)
	{
		try
		{
			byte[] tlvData = EdtByteUtil.concatBytes( tag,
				EdtByteUtil.concatBytes( this.computeOTIValueLength( rawData ), rawData ) );
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x04 } );
			map.put( "ssk", new byte[] { 0x00 } );
			// map.put("Tag", tag);
			// map.put("TagLen", new byte[] { (byte) lv.length });
			map.put( "TagData", tlvData );
			return edtMgpAdapter.callEncode( map, SpotMessageType.OTI_SET.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		catch (SpotMessageException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getOTIGetData(byte[] tag, byte[] rawData)
	{
		try
		{
			byte[] tlvData = EdtByteUtil.concatBytes( tag,
				EdtByteUtil.concatBytes( this.computeOTIValueLength( rawData ), rawData ) );
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x04 } );
			map.put( "ssk", new byte[] { 0x00 } );
			// map.put("Tag", tag);
			// map.put("TagLen", new byte[] { (byte) data.length });
			map.put( "TagData", tlvData );
			return edtMgpAdapter.callEncode( map, SpotMessageType.OTI_GET.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getOTIDoData(byte[] tag, byte[] rawData)
	{
		try
		{
			byte[] tlvData = EdtByteUtil.concatBytes( tag,
				EdtByteUtil.concatBytes( this.computeOTIValueLength( rawData ), rawData ) );
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x04 } );
			map.put( "ssk", new byte[] { 0x00 } );
			// map.put("Tag", tag);
			// map.put("TagLen", new byte[] { (byte) tlvData.length });
			map.put( "TagData", tlvData );
			return edtMgpAdapter.callEncode( map, SpotMessageType.OTI_DO.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getEnableCLReader(byte[] data)
	{
		return this.getEnableCLReader( data, null, null, null, null );
	}

	public byte[] getEnableCLReader(byte[] data, byte[] amount, byte[] transactionDate, byte[] transactionType,
									byte[] transactionTime)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x04 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "ApplicationMask", data );
			if (amount != null)
								map.put( "Amount", amount );
			if (transactionDate != null)
										map.put( "TransactionDate", transactionDate );
			if (transactionType != null)
										map.put( "TransactionType", transactionType );
			if (transactionTime != null)
										map.put( "TransactionTime", transactionTime );
			return edtMgpAdapter.callEncode( map, SpotMessageType.ENABLE_CL_READER.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getOTIDisableCLReader()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x04 } );
			map.put( "ssk", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.DISABLE_CL_READER.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getShowScreen(byte winId)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			Map<String, Object> tag = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "WinId", new byte[] { winId } );
			tag.put( "TagId", new byte[] { 0x00 } );
			tag.put( "Zorder", new byte[] { 0x00 } );
			map.put( "TagsArray", tag );
			return edtMgpAdapter.callEncode( map, SpotMessageType.SHOW_WINDOW.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getHideScreen(byte winId)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			Map<String, Object> tag = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "WinId", new byte[] { winId } );
			tag.put( "TagId", new byte[] { 0x00 } );
			map.put( "TagsArray", tag );
			return edtMgpAdapter.callEncode( map, SpotMessageType.HIDE_WINDOW.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getMaintenance(byte tokenId, byte[] tokenData)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x00 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "TokenId", new byte[] { tokenId } );
			map.put( "TokenData", Arrays.copyOfRange( tokenData, 0, tokenData.length ) );

			return edtMgpAdapter.callEncode( map, SpotMessageType.VANGUARD_MAINTENANCE.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getHardwareInfo()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "ItemCount", new byte[] { 0x00 } );

			return edtMgpAdapter.callEncode( map, SpotMessageType.HARDWARE_INFO.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getPPNbyHardwareInfo()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "ItemCount", new byte[] { 0x01 } );
			map.put( "ItemIDs", new byte[] { 0x01 } );

			return edtMgpAdapter.callEncode( map, SpotMessageType.HARDWARE_INFO.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileUploadRequestData(byte fileType, byte fileId)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "FileType", new byte[] { fileType } );
			map.put( "FileId", new byte[] { fileId } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_UPLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getFileBlockUploadData(int offset)
	{
		try
		{
			byte[] offSet = new byte[4];
			EdtConvert.intToBytes( true, offSet, 0, offset );
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "BlkOffset", offSet );
			return edtMgpAdapter.callEncode( map, SpotMessageType.FILE_BLOCK_UPLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}

	public byte[] getKeyStatusRequestData()
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			return edtMgpAdapter.callEncode( map, SpotMessageType.KEY_STATUS.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( "KeyStatusRequestData - " + e.getMessage( ) );
		}
		return null;
	}

	public byte[] getApplicationKeyDownloadRequestData(int keyId, byte keyUse, byte transportKeyId, byte keyType, byte[] keyData)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "AppId", new byte[] { 0x01 } );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "KeyId", new byte[] { (byte) keyId } );
			map.put( "KeyUse", new byte[] { keyUse } );
			map.put( "TransportKeyId", new byte[] { transportKeyId } );
			map.put( "KeyType", new byte[] { keyType } );
			map.put( "KeyData", keyData );
			return edtMgpAdapter.callEncode( map, SpotMessageType.APPLICATION_KEY_DOWNLOAD.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( "ApplicationKeyDownloadRequestData - " + e.getMessage( ) );
		}
		return null;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * Calculates the (L)ength of a (V)alue for an OTI TLV format.
	 * 
	 * @param value
	 *            is the V in the OTI TLV format
	 * @return the L in the OTI TLV format
	 */
	protected byte[] computeOTIValueLength(byte[] value) throws SpotMessageException
	{
		int l = value.length;

		if (l > 65535)
						throw new SpotMessageException( "Value in OTI TLV can not be larger than 65535" );
		if (l > 255)
		{
			byte[] len = new byte[2];
			EdtConvert.longToBytes( true, len, 0, len.length, l & 0xFFFFFFFF, len.length );
			return EdtByteUtil.concatBytes( (byte) 0x82, len );
		}
		if (l > 127)
		{
			byte[] len = new byte[1];
			EdtConvert.longToBytes( true, len, 0, len.length, l & 0xFFFFFFFF, len.length );
			return EdtByteUtil.concatBytes( (byte) 0x81, len );
		}

		byte[] len = new byte[1];
		EdtConvert.longToBytes( true, len, 0, len.length, l & 0xFFFFFFFF, len.length );
		return len;
	}
	
	/**
	 * Gets the bytes to sent a service menu switch message to the unit.
	 *
	 * @param activate: true to cause the VGD to activate the User Interface on UPM display, false to deactivate.
	 * @return the service menu switch data message.
	 * 		SwitchType[1]	0x01 = Switch command ON 
	 * 						0x02 = Switch command OFF
	 */
	public byte[] getServiceMenuSwitchData(boolean activate)
	{
		try
		{
			byte switchtype = activate ? ((byte)0x01) : ((byte)0x02);
			
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			map.put( "SwitchType", new byte[] { switchtype } ); 
			return edtMgpAdapter.callEncode( map, SpotMessageType.SERVICE_MENU_SWITCH.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}
	
	
	/**
	 * Gets the GET Challenge command data to send to the unit.
	 *
	 * @param techid the technician id string value
	 * @return the get_challenge data
	 */
	public byte[] getGetChallengeData(String techid)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( "ssk", new byte[] { 0x00 } );
			map.put( "AppId", new byte[] { 0x00 } );
			map.put( "TechIdLen", new byte[] { (byte) techid.length( ) } ); 
			map.put( "TechnicianId", techid.getBytes( ) ); 
			return edtMgpAdapter.callEncode( map, SpotMessageType.GET_CHALLENGE.getCommand( ) );
		}
		catch (Exception e)
		{
			logger.error( e.getMessage( ), e );
		}
		return null;
	}
	
	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************
}

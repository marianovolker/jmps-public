
package com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.bootstrap.EdtServiceContext;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtByteUtil;
import com.gilbarco.globaltools.flexpay.rkl.context.RKLServiceDefinitionFactory;
import com.gilbarco.globaltools.mgp.edt.service.MgpServiceAdapter;
import com.gilbarco.globaltools.rklnativeprotocol.api.RklNativeMessageType;

public class RKLNativeMessages
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static RKLNativeMessages	instance	= null;
	private static final transient Log	logger		= LogFactory.getLog( RKLNativeMessages.class );

	// Literals
	public static final String			CONTROL		= "Control";

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************
	public static RKLNativeMessages getInstance()
	{
		if (instance == null) instance = new RKLNativeMessages( );
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

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private RKLNativeMessages()
	{
		super( );
		try
		{
			edtMgpAdapter = (MgpServiceAdapter) EdtServiceContext.getServiceManager( ).getServiceAdapter(
				RKLServiceDefinitionFactory.getRKLMgpDefinition( ) );

		}
		catch (EdtServiceException e)
		{
			logger.error( "RKLNativeMessages - " + e.getMessage( ) );
		}
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public byte[] getSpotIdentification(byte control)
	{
		return new byte[] {};
	}

	public byte[] getSendSpotIdentification(byte control, byte[] serialNumber)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( CONTROL, new byte[]
			{ control } );
			map.put( "FunctionID", "AOPEDI".getBytes( ) );
			map.put( "DeviceGroupName", "DG1234".getBytes( ) );
			map.put( "SerialNumber", EdtByteUtil.concatBytes( "SN".getBytes( ), serialNumber ) );
			map.put( "RandomData", "RD1234".getBytes( ) );
			map.put( "HashAlgorithm", "RG0".getBytes( ) );

			return edtMgpAdapter.callEncode( map, RklNativeMessageType.SEND_IDENTIFICATION.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( "getSendSpotIdentification - " + e.getMessage( ) );
		}
		return null;
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	public byte[] sendGetKeysToSrvr(byte control, byte[] keydata)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendGetKeyManagementToSrvr(byte control)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendSpotCertificateToSrvr(byte control, byte[] certif)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] getSpotCertificate(byte control)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendCertificateVerificationToSpot(byte control, byte[] certif)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendHostCertificateToSpot(byte control, byte[] certif)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendKeyResponseToSpot(byte control, byte[] bs)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendKeyManagementDataToSpot(byte control, byte[] bs)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] getSpotNonceData(byte control)
	{
		// TODO Auto-generated method stub
		return null;
	}

	public byte[] sendCloseKeySessionToSrvr(byte control, byte[] certif)
	{
		try
		{
			Map<String, Object> map = new HashMap<String, Object>( );
			map.put( CONTROL, new byte[]
			{ control } );
			map.put( "FunctionID", "AOPEDV".getBytes( ) );
			map.put( "KeyType", "KT1234".getBytes( ) );
			map.put( "SlotNumber", "KS34".getBytes( ) );
			map.put( "EncryptPaddingType", "KE1234".getBytes( ) );
			map.put( "EncryptedKey", "CE1234".getBytes( ) );
			map.put( "RSASignature", "KA123".getBytes( ) );

			return edtMgpAdapter.callEncode( map, RklNativeMessageType.CLOSE_KEY_VERIFICATION_REQUEST.getCommand( ) );
		}
		catch (EdtServiceException e)
		{
			logger.error( "sendCloseKeySessionToSrvr - " + e.getMessage( ) );
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

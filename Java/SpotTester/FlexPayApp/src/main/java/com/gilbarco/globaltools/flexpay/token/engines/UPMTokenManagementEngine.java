/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Observable;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.flexpay.model.FlexPayContext;
import com.gilbarco.globaltools.flexpay.targetunit.cnxnmgnt.engines.swing.adapters.ConnectionEngineSwingAdapter;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;
import com.gilbarco.globaltools.mgp.domain.ArrayContainer;
import com.gilbarco.globaltools.mgp.domain.ChildElement;
import com.gilbarco.globaltools.mgp.domain.Container;
import com.gilbarco.globaltools.mgp.domain.ContainerType;
import com.gilbarco.globaltools.mgp.domain.Field;
import com.gilbarco.globaltools.mgp.domain.Message;
import com.gilbarco.globaltools.mgp.domain.Protocol;
import com.gilbarco.globaltools.mgp.edt.service.MgpEvent.EdtMgpType;
import com.gilbarco.globaltools.mgp.edt.service.MgpResultEvent;
import com.gilbarco.globaltools.mgp.exceptions.MessageDataException;
import com.gilbarco.globaltools.mgp.exceptions.PredicateException;
import com.gilbarco.globaltools.spotprotocol.SpotProtocol;
import com.gilbarco.globaltools.spotprotocol.api.SpotMessageType;
import com.gilbarco.globaltools.spottester.model.SpotMessages;

/* TODO: TOKEN_COMMENTED_CODE
This class is never used as token management was removed.
*/
public class UPMTokenManagementEngine extends Observable
{
	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************
	private static volatile UPMTokenManagementEngine 	instance 				= null;
	private static final transient Log					logger					= LogFactory.getLog( UPMTokenManagementEngine.class );
	private boolean										receivedResponse		= false;
	private boolean										errorInResponse			= false;
	private AckCodeTokenResponse						responseCode			= AckCodeTokenResponse.SPOT_ACKCODE_UNKNOWN;
	private byte[]										xmlToken;
	private String 										strPPN;
	private final long									ENGINE_SENDING_TIMEOUT	= 10000L;
	
	
	// ******************************************************************
	// PRIVATE METHODS
	// ******************************************************************
	private static synchronized UPMTokenManagementEngine createInstance() throws EdtServiceException
	{
		if( instance == null )
		{
			instance = new UPMTokenManagementEngine();
		}

		return instance;
	}
	
	private void logMsjInAllLogs(String str)
	{
		String technicianID = TokenManagerModel.getTechnicianID( );
		logger.info( "technicianID [" + technicianID + "], " + str );
	}

	private void logErrorInAllLogs(String str)
	{
		String technicianID = TokenManagerModel.getTechnicianID( );
		logger.error( "technicianID [" + technicianID + "], " + str );
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private UPMTokenManagementEngine() throws EdtServiceException
	{
		super( );
		subscribeToEvents( );
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public enum AckCodeTokenResponse
	{
		/*
		 * Values at 29/11/2012.
		 * 
		0x00	ACK - dual control procedure starts 
		0x01	generic authentication failure
		0x02	unit enters in dual control - unsolicited message to other clients
		0x03	Token digital signature verification failed
		0x04	Token is out of date / expired
		0x05	Token is already used max number of times
		0x06	Token version not supported
		0x07	Remote maintenance is not allowed by token (network only)
		…	
		0x20	Token XML formal error - generic
		0x21	Token PPN field is missing
		0x22	Token Permissions field is missing
		0x23	Token Version field is missing
		0x24	Token Type field is missing
		0x25	Token Code field is missing
		0x26	Token Start Date field is missing
		0x27	Token End Date field is missing
		0x28	Token Times field is missing
		0x29	Token Max Duration field is missing
		0x2A	Token Signature is missing
		0x2B	Token Remote Maintenance is missing (network only)
		0x2C	Token Technician Identifier is missing
		0x2D	Token Technician Device is missing
		0x2E	Token Operator Identifier is missing
		…	
		0x40	Token PPN field does not match with unit part number
		0x41	Token Permissions field is bad formatted
		0x42	Token Version field is bad formatted
		0x43	Token Type field is bad formatted
		0x44	Token Code field is bad formatted
		0x45	Token Start Date field is bad formatted
		0x46	Token End Date field is bad formatted
		0x47	Token Times field is bad formatted
		0x48	Token Max Duration field is bad formatted
		0x49	Token Remote Maintenance field is bad formatted (network only)
		0x4A	Token Technician Identifier field is bad formatted
		0x4B	Token Technician Device field is bad formatted
		0x4C	Token Operator Identifier field is bad formatted
		…	
		0x60	Token Start Date field is greater than End Date field
		0x61	Token Start Date field is greater than Current Date field
		0x62	Token Current Date field is greater than End Date field
		*/		
		
		SPOT_ACKCODE_NOERROR															(0x00, "ACK - dual control procedure starts"),
		SPOT_ACKCODE_GENERIC_AUTHENTICATION_FAILURE										(0x01, "generic authentication failure"),
		SPOT_ACKCODE_UNIT_ENTERS_IN_DUAL_CONTROL_UNSOLICITED_MESSAGE_TO_OTHER_CLIENTS	(0x02, "unit enters in dual control - unsolicited message to other clients"),
		SPOT_ACKCODE_TOKEN_DIGITAL_SIGNATURE_VERIFICATION_FAILED						(0x03, "Token digital signature verification failed"),
		SPOT_ACKCODE_TOKEN_IS_OUT_OF_DATE_EXPIRED										(0x04, "Token is out of date / expired"),
		SPOT_ACKCODE_TOKEN_IS_ALREADY_USED_MAX_NUMBER_OF_TIMES							(0x05, "Token is already used max number of times"),
		SPOT_ACKCODE_TOKEN_VERSION_NOT_SUPPORTED										(0x06, "Token version not supported"),
		SPOT_ACKCODE_REMOTE_MAINTENANCE_IS_NOT_ALLOWED_BY_TOKEN        					(0x07, "Remote maintenance is not allowed by token (network only)"),
		SPOT_ACKCODE_TOKEN_XML_FORMAL_ERROR_GENERIC										(0x20, "Token XML formal error - generic"),
		SPOT_ACKCODE_TOKEN_PPN_FIELD_IS_MISSING         								(0x21, "Token PPN field is missing"),
		SPOT_ACKCODE_TOKEN_PERMISSIONS_FIELD_IS_MISSING									(0x22, "Token Permissions field is missing"),
		SPOT_ACKCODE_TOKEN_VERSION_FIELD_IS_MISSING										(0x23, "Token Version field is missing"),
		SPOT_ACKCODE_TOKEN_TYPE_FIELD_IS_MISSING										(0x24, "Token Type field is missing"),
		SPOT_ACKCODE_TOKEN_CODE_FIELD_IS_MISSING										(0x25, "Token Code field is missing"),
		SPOT_ACKCODE_TOKEN_START_DATE_FIELD_IS_MISSING									(0x26, "Token Start Date field is missing"),
		SPOT_ACKCODE_TOKEN_END_DATE_FIELD_IS_MISSING									(0x27, "Token End Date field is missing"),
		SPOT_ACKCODE_TOKEN_TIMES_FIELD_IS_MISSING										(0x28, "Token Times field is missing"),
		SPOT_ACKCODE_TOKEN_MAX_DURATION_FIELD_IS_MISSING								(0x29, "Token Max Duration field is missing"),
		SPOT_ACKCODE_TOKEN_SIGNATURE_IS_MISSING											(0x2A, "Token Signature is missing"),
		SPOT_ACKCODE_TOKEN_REMOTE_MAINTENANCE_IS_MISSING								(0x2B, "Token Remote Maintenance is missing (network only)"),
		SPOT_ACKCODE_TOKEN_TECHNICIAN_IDENTIFIER_IS_MISSING								(0x2C, "Token Technician Identifier is missing"),
		SPOT_ACKCODE_TOKEN_TECHNICIAN_DEVICE_IS_MISSING									(0x2D, "Token Technician Device is missing"),
		SPOT_ACKCODE_TOKEN_OPERATOR_IDENTIFIER_IS_MISSING								(0x2E, "Token Operator Identifier is missing"),
		SPOT_ACKCODE_TOKEN_PPN_FIELD_DOES_NOT_MATCH_WITH_UNIT_PART_NUMBER				(0x40, "Token PPN field does not match with unit part number"),
		SPOT_ACKCODE_TOKEN_PERMISSIONS_FIELD_IS_BAD_FORMATTED							(0x41, "Token Permissions field is bad formatted"),
		SPOT_ACKCODE_TOKEN_VERSION_FIELD_IS_BAD_FORMATTED								(0x42, "Token Version field is bad formatted"),
		SPOT_ACKCODE_TOKEN_TYPE_FIELD_IS_BAD_FORMATTED									(0x43, "Token Type field is bad formatted"),
		SPOT_ACKCODE_TOKEN_CODE_FIELD_IS_BAD_FORMATTED									(0x44, "Token Code field is bad formatted"),
		SPOT_ACKCODE_TOKEN_START_DATE_FIELD_IS_BAD_FORMATTED							(0x45, "Token Start Date field is bad formatted"),
		SPOT_ACKCODE_TOKEN_END_DATE_FIELD_IS_BAD_FORMATTED								(0x46, "Token End Date field is bad formatted"),
		SPOT_ACKCODE_TOKEN_TIMES_FIELD_IS_BAD_FORMATTED									(0x47, "Token Times field is bad formatted"),
		SPOT_ACKCODE_TOKEN_MAX_DURATION_FIELD_IS_BAD_FORMATTED							(0x48, "Token Max Duration field is bad formatted"),
		SPOT_ACKCODE_TOKEN_REMOTE_MAINTENANCE_FIELD_IS_BAD_FORMATTED					(0x49, "Token Remote Maintenance field is bad formatted (network only)"),
		SPOT_ACKCODE_TOKEN_TECHNICIAN_IDENTIFIER_FIELD_IS_BAD_FORMATTED					(0x4A, "Token Technician Identifier field is bad formatted"),
		SPOT_ACKCODE_TOKEN_TECHNICIAN_DEVICE_FIELD_IS_BAD_FORMATTED						(0x4B, "Token Technician Device field is bad formatted"),
		SPOT_ACKCODE_TOKEN_OPERATOR_IDENTIFIER_FIELD_IS_BAD_FORMATTED					(0x4C, "Token Operator Identifier field is bad formatted"),
		SPOT_ACKCODE_TOKEN_START_DATE_FIELD_IS_GREATER_THAN_END_DATE_FIELD				(0x60, "Token Start Date field is greater than End Date field"),
		SPOT_ACKCODE_TOKEN_START_DATE_FIELD_IS_GREATER_THAN_CURRENT_DATE_FIELD			(0x61, "Token Start Date field is greater than Current Date field"),
		SPOT_ACKCODE_TOKEN_CURRENT_DATE_FIELD_IS_GREATER_THAN_END_DATE_FIELD			(0x62, "Token Current Date field is greater than End Date field"),		
		SPOT_ACKCODE_UNKNOWN															(0xFF, "The response is not recognized");
		

		private int	ackCode;
		private String	ackDescription;

		public int getAckCode()
		{
			return ackCode;
		}

		public String getAckDescription()
		{
			return ackDescription;
		}

		private AckCodeTokenResponse(int ackCode, String ackDescription)
		{
			this.ackCode = ackCode;
			this.ackDescription = ackDescription;
		}
		
		public static AckCodeTokenResponse fromInt(int code)
		{
			for ( AckCodeTokenResponse value : AckCodeTokenResponse.values( ) ) 
			{
				if ( value.getAckCode( ) == code )
				{
					return value;
				}
			}
			
			return SPOT_ACKCODE_UNKNOWN;
		}
	}	
	
	public static UPMTokenManagementEngine getInstance() throws EdtServiceException
	{
		if(instance != null)
		{
			return instance;
		}

		return createInstance( );
	}
	
	public boolean hasErrors()
	{
		return errorInResponse;
	}	
	
	public String getTargetUnit()
	{
		return strPPN;
	}

	public AckCodeTokenResponse getResponseCode()
	{
		return responseCode;
	}

	public void setXmlToken(byte[] xmlToken)
	{
		this.xmlToken = Arrays.copyOfRange( xmlToken, 0, xmlToken.length );
	}

	public void doPresentToken() throws InterruptedException
	{
		errorInResponse  = false;
		receivedResponse = false;
		responseCode	 = AckCodeTokenResponse.SPOT_ACKCODE_UNKNOWN;

		logMsjInAllLogs( "The message [PRESENT_TOKEN] is being sent now..." );
		
		presentToken( );

		doActionAndWaitResponse();

		if( !hasErrors( ) )
		{
			logMsjInAllLogs( "The message PRESENT_TOKEN has been sent successfully!" );
		}
	}
	
	public void doObtainThePPN() throws InterruptedException
	{
		errorInResponse  = false;
		receivedResponse = false;
		responseCode	 = AckCodeTokenResponse.SPOT_ACKCODE_UNKNOWN;

		logMsjInAllLogs( "The message [GET_TARGET_UNIT] is being sent now..." );
		
		sendGetPPNByHardwareInfo( );
		
		doActionAndWaitResponse();

		if( !hasErrors( ) )
		{
			logMsjInAllLogs( "The message GET_TARGET_UNIT has been sent successfully!" );
		}		
	}	

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected synchronized void wakeup()
	{
		this.notify( );
	}

	protected void presentToken()
	{
		final byte authMaintenanceId = 0x01; // This is an authenticated Maintenance
		byte[] message = SpotMessages.getInstance( ).getMaintenance( authMaintenanceId, xmlToken );
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}
	
	protected void sendGetPPNByHardwareInfo()
	{
		byte[] message = SpotMessages.getInstance( ).getPPNbyHardwareInfo( );
		logger.debug( "Request message : " + EdtConvert.bytesToHexString( message ) );
		try
		{
			ConnectionEngineSwingAdapter.getInstance().send(message);
		}
		catch (NullPointerException e)
		{
			logger.error("Error. Exception thrown while sending synchronouse data to target unit.\nEdtServiceException: '" + e.getMessage() + "'.");
		}
	}
	
	protected void doActionAndWaitResponse() throws InterruptedException
	{
		synchronized (this)
		{
			long t0 = Calendar.getInstance( ).getTimeInMillis( );
			long t1;
			while (!receivedResponse)
			{
				wait( ENGINE_SENDING_TIMEOUT );
				t1 = Calendar.getInstance( ).getTimeInMillis( );
				if ( (t1 - t0) >= ENGINE_SENDING_TIMEOUT)
				{
					break;
				}
			}

			if (!receivedResponse)
			{
				errorInResponse = true;
				return;
			}

			if (!errorInResponse)
			{
				responseCode = AckCodeTokenResponse.SPOT_ACKCODE_NOERROR;
			}
		}
	}	

	protected void subscribeToEvents()
	{
		try
		{
			SERVICE_MANAGER.subscribe( new EdtEventAction<MgpResultEvent>( )
			{
				@Override
				public void onInvokeEvent(MgpResultEvent event)
				{
					receivedResponse = true;
					errorInResponse  = true;
					
					// Response of PRESENT_TOKEN from UPM host. 
					// -----------------------------------------------------------------------------------------------------
					if (event.getMessageName( ).equals( SpotMessageType.VANGUARD_MAINTENANCE.getName( ) ))
					{
						@SuppressWarnings("unchecked")
						Map<String, Object> dataResponse = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );					
						
						int ackCode = EdtConvert.stringToInt( ((String) dataResponse.get("AckCode")) );
						responseCode = AckCodeTokenResponse.fromInt( ackCode );
						
						if ( event.isValid( ) && responseCode.equals( AckCodeTokenResponse.SPOT_ACKCODE_NOERROR ) )
						{
							errorInResponse = false;
						}
						else
						{
							logErrorInAllLogs( "ERROR getting response from UPM for message PRESENT_TOKEN [" + responseCode.getAckDescription( ) + "]." );
						}

						wakeup( );
					}
					// -----------------------------------------------------------------------------------------------------
					
					// Response of HARDWARE_INFO from UPM host.
					// -----------------------------------------------------------------------------------------------------
					if (event.getMessageName( ).equals( SpotMessageType.HARDWARE_INFO.getName( ) ))
					{
						@SuppressWarnings("unchecked")
						Map<String, Object> dataResponse = (Map<String, Object>) event.getDecodeMap( ).get( event.getDecodeMap( ).keySet( ).iterator( ).next( ) );					
						
						int numItems = EdtConvert.stringToInt( ((String) dataResponse.get("NumItems")) );
						if ( event.isValid( ) && numItems > 0 )
						{
							Protocol spotProtocol;
							SpotProtocol sp = new SpotProtocol( );
							try
							{
								spotProtocol = sp.buildSpotProtocol( );
								List<Message> messages = spotProtocol.findMessage( SpotMessageType.HARDWARE_INFO.getName( ), ContainerType.RESPONSE );

								Message msgCopy = messages.get( 0 ).getWorkingCopy( );
								msgCopy.decode( event.getRawData( ) );

								for (ChildElement child : msgCopy.getChildren( ))
								{
									if (Container.class.isInstance( child ))
									{
										@SuppressWarnings("unchecked")
										ArrayContainer<Container> itemArray = (ArrayContainer<Container>) child;
										Collection<ChildElement> col = itemArray.getChildren( );

										for (ChildElement childElement : col)
										{
											if (childElement instanceof Container)
											{
												Collection<Field> colItem = ((Container) childElement).findChildElement( "ItemID" );
												Field fldPPNId = (Field) colItem.toArray( )[0];
												byte[] fileId = (fldPPNId != null)? (fldPPNId.getData( )):(null);

												if( (fileId.length > 0) && ((byte)fileId[0]) == ((byte)0x01) )
												{
													colItem = ((Container) childElement).findChildElement( "ItemData" );
													Field fldPPNData = (Field) colItem.toArray( )[0];
													String sHexData;
													try
													{
														sHexData = new String(fldPPNData.getData( ), "US-ASCII");
														int index = (sHexData.length( )/2);
														strPPN = sHexData.substring( index );
														logMsjInAllLogs( "PPN : " + strPPN );
														errorInResponse = false;
														break;
													}
													catch (UnsupportedEncodingException e)
													{
														logErrorInAllLogs( "ERROR getting response from UPM [" + e.getMessage( ) + "]." );
													}
												}
											}
										}
									}
								}
							}
							catch (PredicateException|MessageDataException e)
							{
								logErrorInAllLogs( "ERROR getting response from UPM [" + e.getMessage( ) + "]." );
							}
						}

						wakeup( );
					}
					// -----------------------------------------------------------------------------------------------------
					
				}

				@Override
				public Class<MgpResultEvent> getEventClass()
				{
					return MgpResultEvent.class;
				}

				@Override
				public String getSubscriptionFilter()
				{
					return EdtEvent.EVENT_TYPE_FILTER + "='" + EdtMgpType.RESPONSE_RESULT.name( ) + "'";
				}

				@Override
				public boolean justOnce()
				{
					return false;
				}
			} );

		}
		catch (EdtServiceException e)
		{
			logErrorInAllLogs( "ERROR getting response from UPM [" + e.getMessage( ) + "]." );
		}
	}
}

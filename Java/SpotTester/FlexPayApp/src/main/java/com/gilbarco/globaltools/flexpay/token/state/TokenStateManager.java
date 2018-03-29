/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.state;

/**
 * 
 * @Description State machine manager for the Token application.
 * 
 * @see TokenStateManager
 * 
 * @version 1.0.0
 */

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;
import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.TIMER_ADAPTER;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.xml.stream.XMLStreamException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.services.crypto.X509SelfSignCertificate;
import com.gilbarco.globaltools.edt.services.io.socket.EdtSocketEvent.SocketEventType;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSClientSocketEvent;
import com.gilbarco.globaltools.edt.services.io.tlssocket.EdtTLSSocketEvent.TLSSocketEventType;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent;
import com.gilbarco.globaltools.edt.services.timer.EdtTimerEvent.EdtTimerMode;
import com.gilbarco.globaltools.edt.util.EdtStringUtil;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerController;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement.SasElementTags;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasMessage;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg.SasErrorCodeType;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg.SasResponseType;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasPublishCertificateResponse;
import com.gilbarco.globaltools.sasprotocol.xml.services.SasStaxXmlImpl;

public class TokenStateManager
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger					= LogFactory.getLog( TokenStateManager.class );
	private static final String			MESSAGE_SENDING_TIMEOUT	= "MESSAGE_SENDING_TIMEOUT";

	// ******************************************************************
	// STATIC METHODS
	// ******************************************************************

	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PROTECTED FIELDS.
	// ******************************************************************

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private static volatile TokenStateManager	instance	= null;
	private Issuer								issuer;
	private EdtEventAction<EdtTimerEvent>		timeoutAction;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private TokenStateManager() throws EdtServiceException
	{
		super( );
		issuer = new Issuer( this.getClass( ) );
		timeoutAction = new StateTimeOut( );

		subscribeToEvents( );
		changeState( null, TokenAppState.IDLE_STATE.name( ), "Ready" );
	}


	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	public static TokenStateManager getInstance() throws EdtServiceException
	{
		if (instance != null)
		{
			return instance;
		}

		return createInstance( );
	}

	public void requestTokenMessageEvent(String tokencode)
	{
		Map<String, Object> eventProperties = new HashMap<String, Object>( );

		eventProperties.put( SasElementTags.TOKEN_CODE.name( ), tokencode );
		changeState( null, TokenAppState.TOKEN_REQUEST_STATE.name( ), "RequestToken", eventProperties );
	}

	public void publishCertificate()
	{
		Map<String, Object> eventProperties = new HashMap<String, Object>( );

		String certifname = TokenManagerController.getCertifDefaultFolder( ) + TokenManagerController.getDefaultCommonName( ) + ".pem";
		String certifdata = X509SelfSignCertificate.getFileCertificate( certifname );
		eventProperties.put( SasElementTags.CERTIFICATE.name( ), certifdata );
		changeState( null, TokenAppState.PUBLISH_CERTIFICATE_REQUEST_STATE.name( ), "PublishCertificate", eventProperties );
	}

	public void requestTokenQueryMessageEvent(String targetUnit)
	{
		Map<String, Object> eventProperties = new HashMap<String, Object>( );

		eventProperties.put( SasElementTags.TARGET_UNIT.name( ), targetUnit );
		changeState( null, TokenAppState.TOKEN_QUERY_REQUEST_STATE.name( ), "TokenQueryRequest", eventProperties );
	}
	
	public void startStateTimer(long timeout)
	{
		TIMER_ADAPTER.callElapsedTimeAction( MESSAGE_SENDING_TIMEOUT, timeout, EdtTimerMode.ONE_SHOT, timeoutAction, issuer );
		logger.info( "begin timer" );
	}

	public void stopStateTimer()
	{
		TIMER_ADAPTER.callStopTimerAction( MESSAGE_SENDING_TIMEOUT );
		logger.info( "timer stopped" );
	}	
	

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new ProcessMover( ) );
	}
	

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private static synchronized TokenStateManager createInstance() throws EdtServiceException
	{
		if (instance == null)
		{
			instance = new TokenStateManager( );
		}

		return instance;
	}

	/**
	 * delegator method that calls changeState(String, String, String, Map<String, Object>) instead
	 * 
	 * @param from
	 *            : String describing the supposed initial state
	 * @param to
	 *            : String describing the desired final state to go.
	 * @param description
	 *            : String describing the desired/requested operation.
	 */
	private void changeState(String from, String to, String description)
	{
		changeState( from, to, description, null );
	}

	/**
	 * If the supposed initial state is supplied as parameter, then this MUST BE THE SAME as the string description of the current state. Otherwise this method shall fail.
	 * 
	 * @param from
	 *            : String describing the supposed initial state
	 * @param to
	 *            : String describing the desired final state to go.
	 * @param description
	 *            : String describing the desired/requested operation.
	 * @param properties
	 *            : map of event parameters (Object references)
	 */
	private void changeState(String from, String to, String description, Map<String, Object> properties)
	{
		try
		{
			if ((EdtStringUtil.isNullOrEmpty( from ) || PROCESS_ADAPTER.getCurrentState( ).equals( from )))
			{
				String info = "move from state [" + from + "] to [" + to + "] actual state is [" + PROCESS_ADAPTER.getCurrentState( ) + "], description: [" + description + "]";
				logger.info( info );
				
				PROCESS_ADAPTER.callChangeToState( PROCESS_ADAPTER.getCurrentState( ), to, this.issuer, description, properties );
			}
			else
			{
				String error = "WRONG process event, can't move from state [" + from + "] to [" + to + "] actual state is [" + PROCESS_ADAPTER.getCurrentState( ) + "]";
				logger.error( "error: " + error );
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( e.getMessage( ) );
		}
	}

	private void processErrorCodeMessage(SasErrorCodeType errorCode, String errorDescription)
	{
		Map<String, Object> eventProperties = new HashMap<String, Object>( );
		eventProperties.put( SasElementTags.CODE.name( ), errorCode );
		eventProperties.put( SasElementTags.DESCRIPTION.name( ), errorDescription );
			
		TokenAppState currentState;
		try
		{
			currentState = TokenAppState.valueOf( PROCESS_ADAPTER.getCurrentState( ) );
			logger.info( "State:" + currentState + ", ErrorCode: " + errorCode.toString( ) + ", ErrorDescription: " + errorDescription );
		}
		catch (EdtServiceException e)
		{
			currentState = TokenAppState.UNKNOW_STATE;
		}

		switch(currentState)
		{
			case TOKEN_REQUEST_STATE :
			{
				changeState( TokenAppState.TOKEN_REQUEST_STATE.name( ), TokenAppState.TOKEN_RESPONSE_STATE.name( ), "TokenResponse", eventProperties );
			}
			break;

			case PUBLISH_CERTIFICATE_REQUEST_STATE :
			{
				changeState( TokenAppState.PUBLISH_CERTIFICATE_REQUEST_STATE.name( ), TokenAppState.PUBLISH_CERTIFICATE_RESPONSE_STATE.name( ), "CertificateResponse", eventProperties );
			}
			break;

			case TOKEN_QUERY_REQUEST_STATE :
			{
				changeState( TokenAppState.TOKEN_QUERY_REQUEST_STATE.name( ), TokenAppState.TOKEN_QUERY_RESPONSE_STATE.name( ), "TokenQueryResponse", eventProperties );
			}
			break;

			default :
			{
				logger.error( "TokenAppState - " + currentState );
				
				changeState( currentState.name( ), TokenAppState.ERROR_STATE.name( ), "Error", eventProperties );
			}
			break;
		}
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class ProcessMover implements EdtEventAction<EdtTLSClientSocketEvent>
	{

		@Override
		public Class<EdtTLSClientSocketEvent> getEventClass()
		{
			return EdtTLSClientSocketEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + SocketEventType.INCOMING_DATA.name( ) + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}

		@Override
		public void onInvokeEvent(EdtTLSClientSocketEvent event)
		{
			switch(event.getEnumType( TLSSocketEventType.class ))
			{
				case INCOMING_DATA:
				{
					SasStaxXmlImpl myParser = new SasStaxXmlImpl( );
					SasMessage message;
					try
					{
						stopStateTimer();
						
						message = (SasMessage) myParser.unmarshall( new File( "" ), new ByteArrayInputStream( event.getData( ) ) );
						switch(message.getContent( ).getCommand( ).getType( ))
						{
							case ERRORCODE :
							{
								SasErrorCodeType errorCode = SasErrorCodeType.fromInt( Integer.parseInt( ((SasXmlErrorCodeMsg) message).getErrorCode( ).getValue( ) ) );
								String errorDescription = ((SasXmlErrorCodeMsg) message).getDescription( ).getValue( );
								processErrorCodeMessage( errorCode, errorDescription );
							}
							break;

							case RESPONSE :
							{
								SasXmlResponseMsg messageResponse = (SasXmlResponseMsg) message;
								Map<String, Object> eventProperties = new HashMap<String, Object>( );

								switch(messageResponse.getSasResponseType( ))
								{
									case PUBLISH_CERTIFICATE_RESPONSE :
										eventProperties.put( SasElementTags.VALIDATION_CODE.name( ), ((SasPublishCertificateResponse) messageResponse).getValidationCode( ) );
										changeState( TokenAppState.PUBLISH_CERTIFICATE_REQUEST_STATE.name( ), TokenAppState.PUBLISH_CERTIFICATE_RESPONSE_STATE.name( ), "PublishCertificateSuccesfully", eventProperties );
									break;

									case TOKEN_RESPONSE :
										eventProperties.put( SasResponseType.TOKEN_RESPONSE.name( ), event.getData( ) );
										changeState( TokenAppState.TOKEN_REQUEST_STATE.name( ), TokenAppState.TOKEN_RESPONSE_STATE.name( ), "TokenRecievedSuccesfully", eventProperties );
									break;

									case TOKEN_QUERY_RESPONSE :
										eventProperties.put( SasResponseType.TOKEN_QUERY_RESPONSE.name( ), event.getData( ) );
										changeState( TokenAppState.TOKEN_QUERY_REQUEST_STATE.name( ), TokenAppState.TOKEN_QUERY_RESPONSE_STATE.name( ), "TokenQueryRecievedSuccesfully", eventProperties );
									break;

									default:
										logger.error( "Invalid response type received from a connection with the SAS server." );
									break;
								}
							}
							break;

							default :
								logger.error( "Invalid command received from a connection with the SAS server." );
							break;
						}
					}
					catch (XMLStreamException | IOException e)
					{
						logger.error( e.getMessage( ) );
						processErrorCodeMessage( SasErrorCodeType.BAD_SERVER_SIGNATURE, e.getMessage( ) );
					}
				}
				break;

				default:
					logger.error( "Invalid event received from a connection with the SAS server." );
				break;
			}
		}
	}

	public class StateTimeOut implements EdtEventAction<EdtTimerEvent>
	{
		@Override
		public Class<EdtTimerEvent> getEventClass()
		{
			return EdtTimerEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.ISSUER_FILTER + "='" + issuer + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}

		@Override
		public void onInvokeEvent(EdtTimerEvent event)
		{
			TokenAppState currentState;
			try
			{
				currentState = TokenAppState.valueOf( PROCESS_ADAPTER.getCurrentState( ) );
			}
			catch (EdtServiceException e)
			{
				currentState = TokenAppState.UNKNOW_STATE;
			}
			
			switch(currentState)
			{
				case IDLE_STATE :
				{
					logger.info( "received a timeout in IDLE_STATE" );
				}
				break;				

				default :
				{
					try
					{
						logger.info( "Timeout received. Stopping connection..." );
						TokenManagerMessageBroker.getInstance( ).stopConection( );
					}
					catch (EdtServiceException e)
					{
						logger.error( "Stopping connection." );
					}
					
					logger.error( "Timeout sending a message. Check ip address configured." );
					
					processErrorCodeMessage( SasErrorCodeType.SECURE_AUTH_SERVER_INTERNAL_ERROR, "Timeout sending a message. Check ip address configured." );
				}
				break;
			}			
		}
	}
}

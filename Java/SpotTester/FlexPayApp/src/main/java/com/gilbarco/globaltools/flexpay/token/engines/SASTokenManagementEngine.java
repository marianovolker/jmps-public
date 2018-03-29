/*
* © 2012 Gilbarco Inc.
* Confidential and Proprietary
*
*/

package com.gilbarco.globaltools.flexpay.token.engines;

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.util.Calendar;
import java.util.Map;
import java.util.Observable;

import javax.xml.stream.XMLStreamException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;
import com.gilbarco.globaltools.flexpay.token.state.TokenAppState;
import com.gilbarco.globaltools.flexpay.token.state.TokenManagerMessageBroker;
import com.gilbarco.globaltools.flexpay.token.state.TokenStateManager;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement.SasElementTags;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg.SasErrorCodeType;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlResponseMsg.SasResponseType;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryResponse;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenResponse;
import com.gilbarco.globaltools.sasprotocol.xml.services.SasStaxXmlImpl;

/* TODO: TOKEN_COMMENTED_CODE
This class is never used as token management was removed.
*/
public class SASTokenManagementEngine extends Observable
{
	// ******************************************************************
	// PRIVATE FIELDS
	// ******************************************************************
	private static volatile SASTokenManagementEngine	instance				= null;
	private static final transient Log					logger					= LogFactory.getLog( SASTokenManagementEngine.class );
	private volatile boolean							receivedResponse		= false;
	private volatile boolean							errorInResponse			= false;
	private SasErrorCodeType							responseCode			= SasErrorCodeType.NO_ERROR;
	private SasTokenQueryResponse						tokenQueryResponse;
	private SasTokenResponse							tokenResponse;
	private String										strTargetUnit;
	private String										tokenCode;
	private String										validationCode;
	private String										errorDescription;
	private TokenStateManager							stateManager;
	private TokenManagerMessageBroker					messageBroker;
	private final long									ENGINE_SENDING_TIMEOUT	= 90000L; 													 

	// ******************************************************************
	// PRIVATE METHODS
	// ******************************************************************
	private static synchronized SASTokenManagementEngine createInstance() throws EdtServiceException
	{
		if (instance == null)
		{
			instance = new SASTokenManagementEngine( );
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
		logger.error( "technicianID [" + technicianID + "], " + str );
	}

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private SASTokenManagementEngine() throws EdtServiceException
	{
		super( );

		messageBroker = TokenManagerMessageBroker.getInstance( );
		stateManager = TokenStateManager.getInstance( );

		subscribeToEvents( );
	}

	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************
	
	// Getters...
	// -----------------------------------------------------------------------------------------------------
	public static SASTokenManagementEngine getInstance() throws EdtServiceException
	{
		if (instance != null)
		{
			return instance;
		}

		return createInstance( );
	}

	public boolean hasErrors()
	{
		return errorInResponse;
	}

	public SasTokenQueryResponse getTokenQueryResponse()
	{
		return tokenQueryResponse;
	}
	
	public SasTokenResponse getTokenResponse()
	{
		return tokenResponse;
	}	

	public SasErrorCodeType getResponseCode()
	{
		return responseCode;
	}
	
	public String getErrorDescription()
	{
		return errorDescription;
	}	
	
	public String getValidationCode()
	{
		return validationCode;
	}	

	public String getCurrentIPAddress()
	{
		return messageBroker.getCurrentIPAddress( );
	}

	public int getCurrentPortAddress()
	{
		return messageBroker.getCurrentPortAddress( );
	}
	
	public void reconfigure()
	{
		messageBroker.resetTLSServicesParameters( );
	}
	// -----------------------------------------------------------------------------------------------------

	// Setters
	// -----------------------------------------------------------------------------------------------------
	public void setTargetUnit(String strTargetUnit)
	{
		this.strTargetUnit = strTargetUnit;
	}
	
	public void setTokenCode(String tokenCode)
	{
		this.tokenCode = tokenCode;
	}
	// -----------------------------------------------------------------------------------------------------
	
	// Actions of this engine...
	// -----------------------------------------------------------------------------------------------------
	public void doTokenRequest() throws InterruptedException
	{
		errorInResponse = false;
		receivedResponse = false;
		responseCode = SasErrorCodeType.UNKNOWN_ERROR;

		logMsjInAllLogs( "The message [TOKEN_REQUEST] is being sent now...");
		logMsjInAllLogs( "TokenCode:" + tokenCode);
		
		requestToken();

		doActionAndWaitResponse();

		if( !hasErrors( ) )
		{
			logMsjInAllLogs( "The message [TOKEN_REQUEST] has been sent successfully!" );
		}		
	}	

	public void doTokenQueryRequest() throws InterruptedException
	{
		errorInResponse = false;
		receivedResponse = false;
		responseCode = SasErrorCodeType.UNKNOWN_ERROR;

		logMsjInAllLogs( "The message [TOKEN_QUERY_REQUEST] is being sent now..." );
		
		requestTokenQuery();

		doActionAndWaitResponse();

		if( !hasErrors( ) )
		{
			logMsjInAllLogs( "The message [TOKEN_QUERY_REQUEST] has been sent successfully!" );
		}		
	}
	
	public void doPublishCertificate() throws InterruptedException
	{
		errorInResponse = false;
		receivedResponse = false;
		responseCode = SasErrorCodeType.UNKNOWN_ERROR;

		logMsjInAllLogs( "The message [PUBLISH_CERTIFICATE] is being sent now..." );
		
		publishCertificate();

		doActionAndWaitResponse();

		if( !hasErrors( ) )
		{
			logMsjInAllLogs( "The message [PUBLISH_CERTIFICATE] has been sent successfully!" );
		}
	}
	// -----------------------------------------------------------------------------------------------------
	

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	protected synchronized void wakeup()
	{
		this.notify( );
	}
	
	protected void requestTokenQuery()
	{
		stateManager.requestTokenQueryMessageEvent( strTargetUnit );
	}	
	
	protected void requestToken()
	{
		stateManager.requestTokenMessageEvent( tokenCode );
	}	

	protected void publishCertificate()
	{
		stateManager.publishCertificate( );
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
					messageBroker.stopConection( );
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
				responseCode = SasErrorCodeType.NO_ERROR;
			}
		}		
	}	

	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new StateChangedAction( ) );
	}
	
	protected void getErrorCode( Map<String, Object> msgproperties )
	{
		if (msgproperties.containsKey( SasElementTags.CODE.name( ) ))
		{
			responseCode = (SasErrorCodeType) msgproperties.get( SasElementTags.CODE.name( ) );
		}
		else if (msgproperties.containsKey( SasElementTags.DESCRIPTION.name( ) ))
		{
			errorDescription = (String)msgproperties.get( SasElementTags.DESCRIPTION.name( ) );
			responseCode = SasErrorCodeType.UNKNOWN_ERROR;
		}
		else
		{
			errorDescription = SasErrorCodeType.UNKNOWN_ERROR.getDescription( );
			responseCode = SasErrorCodeType.UNKNOWN_ERROR;
		}
	}	

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************
	public class StateChangedAction implements EdtEventAction<EdtProcessEvent>
	{
		@Override
		public void onInvokeEvent(EdtProcessEvent event)
		{
			receivedResponse = false;
			errorInResponse  = true;
			
			TokenAppState appState = TokenAppState.IDLE_STATE;
			try
			{
				appState = TokenAppState.valueOf( PROCESS_ADAPTER.getCurrentState( ) );
			}
			catch (EdtServiceException e)
			{
				e.printStackTrace( );
			}

			switch(appState)
			{
				case IDLE_STATE :
				break;

				case ERROR_STATE :
				{
					receivedResponse = true;
					errorInResponse  = true;
					logErrorInAllLogs( "ERROR: [" + event.getDescription( ) + "]" );
				}
				break;
				
				// Response of TOKEN_QUERY_RESPONSE from SAS Server. 
				// -----------------------------------------------------------------------------------------------------				
				case TOKEN_QUERY_RESPONSE_STATE :
				{
					Map<String, Object> msgproperties = event.getEventProperties( );

					if (!msgproperties.isEmpty( ))
					{
						logger.debug( "Token Query Response received!" );

						receivedResponse = true;
						errorInResponse  = true;

						if (msgproperties.containsKey( SasResponseType.TOKEN_QUERY_RESPONSE.name( ) ))
						{
							SasStaxXmlImpl myParser = new SasStaxXmlImpl( );
							try
							{
								tokenQueryResponse = (SasTokenQueryResponse) myParser.unmarshall( new File( "" ), new ByteArrayInputStream( (byte[]) msgproperties.get( SasResponseType.TOKEN_QUERY_RESPONSE.name( ) ) ) );
								errorInResponse    = false;
							}
							catch (XMLStreamException|IOException e)
							{
								logErrorInAllLogs( "ERROR sending Token Query Request with description [" + e.getMessage( ) + "]" );
							}
						}
						else
						{
							getErrorCode(msgproperties);
							String errorMsj = ( (responseCode == SasErrorCodeType.UNKNOWN_ERROR)? (errorDescription):(responseCode.getDescription( )) );
							logErrorInAllLogs( "ERROR sending Token Query Request with description [" + errorMsj + "]" );
						}

						wakeup( );
					}
				}
				break;
				// -----------------------------------------------------------------------------------------------------
				
				// Response of TOKEN_RESPONSE from SAS Server.
				// -----------------------------------------------------------------------------------------------------
				case TOKEN_RESPONSE_STATE :
				{
					Map<String, Object> msgproperties = event.getEventProperties( );
					
					if (!msgproperties.isEmpty( ))
					{
						logger.debug( "Token Response received!" );

						receivedResponse = true;
						errorInResponse  = true;

						if (msgproperties.containsKey( SasResponseType.TOKEN_RESPONSE.name( ) ))
						{
							SasStaxXmlImpl myParser = new SasStaxXmlImpl( );
							try
							{
								tokenResponse   = (SasTokenResponse) myParser.unmarshall( new File( "" ), new ByteArrayInputStream( (byte[]) msgproperties.get( SasResponseType.TOKEN_RESPONSE.name( ) ) ) );
								errorInResponse = false;
							}
							catch (XMLStreamException|IOException e)
							{
								logErrorInAllLogs( "ERROR sending Request Token Message with description [" + e.getMessage( ) + "]" );
							}
						}
						else
						{
							getErrorCode(msgproperties);
							String errorMsj = ( (responseCode == SasErrorCodeType.UNKNOWN_ERROR)? (errorDescription):(responseCode.getDescription( )) );
							logErrorInAllLogs( "ERROR sending Request Token Message with description [" + errorMsj + "]" );							
						}

						wakeup( );
					}					
				}
				break;
				// -----------------------------------------------------------------------------------------------------
				
				// Response of PUBLISH_CERTIFICATE_RESPONSE from SAS Server.
				// -----------------------------------------------------------------------------------------------------
				case PUBLISH_CERTIFICATE_RESPONSE_STATE :
				{
					Map<String, Object> msgproperties = event.getEventProperties( );
					
					if (!msgproperties.isEmpty( ))
					{
						logger.debug( "Publish Certificate Received!" );

						receivedResponse = true;
						errorInResponse  = true;

						if (msgproperties.containsKey( SasElementTags.VALIDATION_CODE.name( ) ))
						{
							validationCode  = (String) msgproperties.get( SasElementTags.VALIDATION_CODE.name( ) );
							errorInResponse = false;
						}
						else
						{
							getErrorCode(msgproperties);
							String errorMsj = ( (responseCode == SasErrorCodeType.UNKNOWN_ERROR)? (errorDescription):(responseCode.getDescription( )) );
							logErrorInAllLogs( "ERROR sending Publish Certificate Message with description [" + errorMsj + "]" );							
						}

						wakeup( );
					}
				}
				break;
				// -----------------------------------------------------------------------------------------------------

				// -----------------------------------------------------------------------------------------------------
				default :
				{
					logger.debug( "FlexPayAppState - " + appState );
				}
				break;
				// -----------------------------------------------------------------------------------------------------
			}
		}

		@Override
		public Class<EdtProcessEvent> getEventClass()
		{
			return EdtProcessEvent.class;
		}

		@Override
		public String getSubscriptionFilter()
		{
			return EdtEvent.EVENT_TYPE_FILTER + "='" + ProcessEventType.STATE_CHANGED.name( ) + "'";
		}

		@Override
		public boolean justOnce()
		{
			return false;
		}
	}
}

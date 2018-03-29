/*
 * © 2012 Gilbarco Inc.
 * Confidential and Proprietary
 *
 */

package com.gilbarco.globaltools.flexpay.token.state;

/**
 * @author stomasini
 * 
 * @Description Message Broker to send and receive messages to terminal unit and to SAS server. 
 * 
 * @see TokenManagerMessageBroker
 * 
 * @version 1.0.0
 */

import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.PROCESS_ADAPTER;
import static com.gilbarco.globaltools.flexpay.model.FlexPayContext.SERVICE_MANAGER;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import javax.xml.stream.XMLStreamException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtEvent;
import com.gilbarco.globaltools.edt.services.EdtEventAction;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent;
import com.gilbarco.globaltools.edt.services.process.EdtProcessEvent.ProcessEventType;
import com.gilbarco.globaltools.flexpay.engines.TLSSocketTransportLayer;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasElement.SasElementTags;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasPublishCertificateRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenQueryRequest;
import com.gilbarco.globaltools.sasprotocol.xml.sasprotocol.SasTokenRequest;
import com.gilbarco.globaltools.sasprotocol.xml.services.SasXmlService;
import com.gilbarco.globaltools.sasprotocol.xml.services.SasXmlServiceFactory;

public class TokenManagerMessageBroker
{
	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log					logger								= LogFactory.getLog( TokenManagerMessageBroker.class );
	private static final int							SENDING_MESSAGE_TIMEOUT				= 5000;
	private static final int							CONNECTION_TIMEOUT					= 2000;
	private static volatile TokenManagerMessageBroker	instance							= null;
	private static final int							QUANTITY_RETRIES_TO_SEND_A_MESSAGE	= 3;	

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
	private TLSSocketTransportLayer						tlsServer;
	private TLSSocketTransportLayer						tlsSecondaryServer;
	private String										currentIPAddress;
	private int											currentPortAddress;
	private int											counterToUseSecondaryConexion;
	private TLSSocketTransportLayer 					currentConnection = null;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	private TokenManagerMessageBroker() throws EdtServiceException
	{
		super( );

		createSASPrimaryDataTransportLayer( );
		createSASSecondaryDataTransportLayer( );

		subscribeToEvents( );

		currentIPAddress = tlsServer.getServerUri( );
		currentPortAddress = tlsServer.getServerPort( );

		counterToUseSecondaryConexion = 0;
	}

	// ******************************************************************
	// PUBLIC METHODS
	// ******************************************************************
	public static TokenManagerMessageBroker getInstance() throws EdtServiceException
	{
		if (instance != null)
		{
			return instance;
		}

		return createInstance( );
	}

	public String getCurrentIPAddress()
	{
		return currentIPAddress;
	}

	public int getCurrentPortAddress()
	{
		return currentPortAddress;
	}
	
	public void stopConection()
	{
		if( currentConnection != null )
		{
			currentConnection.close( );
		}
		else
		{
			logger.error( "currentConection is null" );
		}
	}	

	public void sendTokenRequest(String tokencode)
	{
		try
		{
			SasXmlService xmlService = SasXmlServiceFactory.getInstance( SasXmlServiceFactory.Parsers.StAX );
			SasTokenRequest sastcr = new SasTokenRequest( );

			String strTechnicianID = TokenManagerModel.getTechnicianID( );
			File schema = null; // TODO: use schema validation when ready
			InputStream xmlOutputBuffer = null;

			sastcr.setTechnician( strTechnicianID );
			sastcr.setTokenCode( tokencode );
			try
			{
				xmlOutputBuffer = xmlService.marshall( sastcr, schema );
				xmlOutputBuffer.close( );
			}
			catch (XMLStreamException e)
			{
				String sErr = "TokenManagerMessageBroker.sendTokenRequest : Error marshaling message [" + e.getMessage( ) + "]";
				logger.error( sErr );
				reportApplicationError( "Error building sendTokenRequest message - Check your certificate" );
				return;
			}
			catch (IOException e)
			{
				logger.error( e.getMessage( ) );
				if (xmlOutputBuffer == null)
				{
					reportApplicationError( "Error building Token Request Message - Check your certificate" );
					return;
				}
			}

			byte buffer[] = new byte[2048];
			int len = xmlOutputBuffer.read( buffer );
			byte abData[] = Arrays.copyOf( buffer, len );
			send( abData, false );
		}
		catch (IOException e)
		{
			logger.error( "TokenManagerMessageBroker.sendTokenRequest() : ERROR - [" + e.getMessage( ) + "]" );
			reportApplicationError( "Error building Token Request Message" );
		}
	}

	/**
	 * @param cert
	 *            the certificate to be published on sas server
	 */
	public void sendPublishCertificateRequest(String cert)
	{
		try
		{
			SasXmlService xmlService = SasXmlServiceFactory.getInstance( SasXmlServiceFactory.Parsers.StAX );
			SasPublishCertificateRequest sastcr = new SasPublishCertificateRequest( );

			String strTechnicianID = TokenManagerModel.getTechnicianID( );
			File schema = null; // TODO: use schema validation when ready
			InputStream xmlOutputBuffer = null;

			sastcr.setTechnician( strTechnicianID );
			sastcr.setCertificate( cert );
			try
			{
				xmlOutputBuffer = xmlService.marshall( sastcr, schema );
				xmlOutputBuffer.close( );
			}
			catch (XMLStreamException e)
			{
				String sErr = "TokenManagerMessageBroker.sendPublishCertificateRequest : Error marshaling message [" + e.getMessage( ) + "]";
				logger.error( sErr );
				reportApplicationError( "Error building Publish Certificate Request Message - Check your certificate" );
				return;
			}
			catch (IOException e)
			{
				logger.error( e.getMessage( ) );
				if (xmlOutputBuffer == null)
				{
					reportApplicationError( "Error building Publish Certificate Request Message - Check your certificate" );
					return;
				}
			}

			byte buffer[] = new byte[2048];
			int len = xmlOutputBuffer.read( buffer );
			byte abData[] = Arrays.copyOf( buffer, len );
			send( abData, false );
		}
		catch (IOException e)
		{
			logger.error( "TokenManagerMessageBroker.sendPublishCertificateRequest() : ERROR - [" + e.getMessage( ) + "]" );
			reportApplicationError( "Error building Publish Certificate Request Message" );
		}
	}

	public void sendTokenQueryRequest(String targetUnit)
	{
		try
		{
			SasXmlService xmlService = SasXmlServiceFactory.getInstance( SasXmlServiceFactory.Parsers.StAX );
			SasTokenQueryRequest sastqr = new SasTokenQueryRequest( );

			File schema = null; // TODO: use schema validation when ready
			InputStream xmlOutputBuffer = null;

			sastqr.setTechnician( TokenManagerModel.getTechnicianID( ) );
			sastqr.setTargetUnit( targetUnit );
			try
			{
				xmlOutputBuffer = xmlService.marshall( sastqr, schema );
				xmlOutputBuffer.close( );
			}
			catch (XMLStreamException e)
			{
				String sErr = "TokenManagerMessageBroker.sendTokenQueryRequest() : Error marshaling message [" + e.getMessage( ) + "]";
				logger.error( sErr );
				reportApplicationError( "Error building Token Query Request Message - Check your certificate" );
				return;
			}
			catch (IOException e)
			{
				logger.error( e.getMessage( ) );
				if (xmlOutputBuffer == null)
				{
					reportApplicationError( "Error building Token Query Request Message - Check your certificate" );
					return;
				}
			}

			byte buffer[] = new byte[2048];
			int len = xmlOutputBuffer.read( buffer );
			byte abData[] = Arrays.copyOf( buffer, len );
			send( abData, false );
		}
		catch (IOException e)
		{
			logger.error( "TokenManagerMessageBroker.sendTokenQueryRequest() : ERROR - [" + e.getMessage( ) + "]" );
			reportApplicationError( "Error building Token Query Request Message" );
		}
	}

	public void resetTLSServicesParameters()
	{
		String primaryServerIPAddressNewValue = TokenManagerModel.getTokenServerIp1( );
		String secondaryServerIPAddressNewValue = TokenManagerModel.getTokenServerIp2( );
		
		String primaryServerIPAddressConfigured = tlsServer.getServerUri();
		String secondaryServerIPAddressConfigured = tlsSecondaryServer.getServerUri();
		
		logger.info( "[Primary Server IP Address New Value = " + primaryServerIPAddressNewValue + "] [Primary Server IP Address Configured = " + primaryServerIPAddressConfigured + "]" );
		logger.info( "[Secondary Server IP Address New Value = " + secondaryServerIPAddressNewValue + "] [Secondary Server IP Address Configured = " + secondaryServerIPAddressConfigured + "]" );
		
		// Primary server parameters
		// Check if there are changes in the configuration, in the case that is true set the new values to socket layer for the primary server connection.
		// ---------------------------------------------------------------------------------------------------------------
		if ( primaryServerIPAddressNewValue.equalsIgnoreCase(primaryServerIPAddressConfigured) == false )
		{
			tlsServer.setServerUri( primaryServerIPAddressNewValue );
			logger.info( "Saved new value for primary connection." );
		}
		if ( TokenManagerModel.getTokenServerPort( ) != tlsServer.getServerPort( ) )
		{
			tlsServer.setServerPort( TokenManagerModel.getTokenServerPort( ) );
		}
		// ---------------------------------------------------------------------------------------------------------------

		// Secondary server parameters
		// Check if there are changes in the configuration, in the case that is true set the new values to socket layer for the secondary server connection.
		// ---------------------------------------------------------------------------------------------------------------
		if ( secondaryServerIPAddressNewValue.equalsIgnoreCase(secondaryServerIPAddressConfigured) == false )
		{
			tlsSecondaryServer.setServerUri( secondaryServerIPAddressNewValue );
			logger.info( "Saved new value for secondary connection." );
		}		
		if ( TokenManagerModel.getTokenServerPort( ) != tlsSecondaryServer.getServerPort( ) )
		{
			tlsSecondaryServer.setServerPort( TokenManagerModel.getTokenServerPort( ) );
		}		
		// ---------------------------------------------------------------------------------------------------------------
		
		// Reset the attemps to use the secondary connection.
		// ---------------------------------------------------------------------------------------------------------------
		counterToUseSecondaryConexion = 0;
		// ---------------------------------------------------------------------------------------------------------------
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************
	/**
	 * Send a message to the SAS server using a dual connection. 
	 * If the first connection fail, this algorithm permits to do a retry to a secondary connection.
	 * 
	 * @param data
	 *            the data to send.
	 * @param retry
	 *            to do a second retry.
	 */
	protected void send( byte[] data, boolean retry )
	{
		if( counterToUseSecondaryConexion <= 0 )
		{
			currentConnection = tlsServer;
		}
		else
		{ 
			counterToUseSecondaryConexion--;
			currentConnection = tlsSecondaryServer;
			logger.info( "The current retry using secondary connection is [" + (QUANTITY_RETRIES_TO_SEND_A_MESSAGE - counterToUseSecondaryConexion) + "] of [" + QUANTITY_RETRIES_TO_SEND_A_MESSAGE + "]." );
		}
		
		try
		{
			currentConnection.send( data, CONNECTION_TIMEOUT );
			
			TokenStateManager.getInstance( ).startStateTimer( SENDING_MESSAGE_TIMEOUT );
			
			return;
		}
		catch (EdtServiceException e)
		{
			logger.error( "ERROR sending a message to ip address [" + currentConnection.getServerUri( ) + "] - [" + e.getMessage( ) + "]" );
		}

		if( retry )
		{
			reportApplicationError( "Error Sending a Message to SAS Server. Check ip address configured." );
		}
		else
		{
			counterToUseSecondaryConexion = (counterToUseSecondaryConexion <= 0)? (QUANTITY_RETRIES_TO_SEND_A_MESSAGE):(0);
			send(data, true);
		}
	}

	protected void subscribeToEvents() throws EdtServiceException
	{
		SERVICE_MANAGER.subscribe( new StateChangedAction( ) );
	}

	protected void createSASPrimaryDataTransportLayer()
	{
		tlsServer = new TLSSocketTransportLayer( TokenManagerModel.getTokenServerIp1( ), TokenManagerModel.getTokenServerPort( ) );
	}

	protected void createSASSecondaryDataTransportLayer()
	{
		tlsSecondaryServer = new TLSSocketTransportLayer( TokenManagerModel.getTokenServerIp2( ), TokenManagerModel.getTokenServerPort( ) );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private static synchronized TokenManagerMessageBroker createInstance() throws EdtServiceException
	{
		if (instance == null)
		{
			instance = new TokenManagerMessageBroker( );
		}

		return instance;
	}

	private void reportApplicationError(String errorDescription)
	{
		Map<String, Object> eventProperties = new HashMap<String, Object>( );
		eventProperties.put( SasElementTags.DESCRIPTION.name( ), errorDescription );
		
		TokenAppState currentState;
		try
		{
			currentState = TokenAppState.valueOf( PROCESS_ADAPTER.getCurrentState( ) );
		}
		catch (EdtServiceException e)
		{
			currentState = TokenAppState.UNKNOW_STATE;
		}		
		
		try
		{
			switch(currentState)
			{
				case TOKEN_REQUEST_STATE :
				{
					PROCESS_ADAPTER.callChangeToState( TokenAppState.TOKEN_REQUEST_STATE.name( ), TokenAppState.TOKEN_RESPONSE_STATE.name( ), "TokenResponse", eventProperties );
				}
				break;
	
				case PUBLISH_CERTIFICATE_REQUEST_STATE :
				{
					PROCESS_ADAPTER.callChangeToState( TokenAppState.PUBLISH_CERTIFICATE_REQUEST_STATE.name( ), TokenAppState.PUBLISH_CERTIFICATE_RESPONSE_STATE.name( ), "PublishCertificateResponse", eventProperties );
				}
				break;
	
				case TOKEN_QUERY_REQUEST_STATE :
				{
					PROCESS_ADAPTER.callChangeToState( TokenAppState.TOKEN_QUERY_REQUEST_STATE.name( ), TokenAppState.TOKEN_QUERY_RESPONSE_STATE.name( ), "TokenQueryResponse", eventProperties );
				}
				break;
	
				default :
				{
					logger.error( "reportError - invalid app state" );
				}
				break;
			}
		}
		catch (EdtServiceException e)
		{
			logger.error( "reportError - invalid app state - [" + e.getMessage( ) + "]" );
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
			TokenAppState destination = TokenAppState.valueOf( event.getDestination( ) );
			Map<String, Object> props = event.getEventProperties( );
			switch(destination)
			{
				case TOKEN_REQUEST_STATE :
				{
					if (props.containsKey( SasElementTags.TOKEN_CODE.name( ) ))
					{
						sendTokenRequest( (String) props.get( SasElementTags.TOKEN_CODE.name( ) ) );
					}
					else
					{
						logger.error( "TokenManagerMessage.onInvokeEvent - case TOKEN_REQUEST - No valid property arguments - message is DISCARDED" );
						reportApplicationError( "Error building Token Request Message" );
					}
				}
				break;

				case PUBLISH_CERTIFICATE_REQUEST_STATE :
				{
					if (props.containsKey( SasElementTags.CERTIFICATE.name( ) ))
					{
						sendPublishCertificateRequest( (String) props.get( SasElementTags.CERTIFICATE.name( ) ) );
					}
					else
					{
						logger.error( "TokenManagerMessage.onInvokeEvent - case PUBLISH_CERTIFICATE_REQUEST_STATE - No valid property arguments - message is DISCARDED" );
						reportApplicationError( "Error building Publish Certificate Request Message" );
					}
				}
				break;

				case TOKEN_QUERY_REQUEST_STATE :
				{
					if (props.containsKey( SasElementTags.TARGET_UNIT.name( ) ))
					{
						sendTokenQueryRequest( (String) props.get( SasElementTags.TARGET_UNIT.name( ) ) );
					}
					else
					{
						logger.error( "TokenManagerMessage.onInvokeEvent - case TOKEN_QUERY_REQUEST - No valid property arguments - message is DISCARDED" );
						reportApplicationError( "Error building Token Query Request Message" );
					}
				}
				break;

				default :
				{
					logger.debug( "FlexPayAppState - " + destination );
				}
				break;
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
